#include "pch.h"
#include "Network.h"
#include <time.h>

const float LEARNING_RATE = 0.5f;
const float BIAS_LEARNING_RATE = 0.5f;

Network::Network() //: m_inputs(nullptr), m_weights(nullptr), m_biases(nullptr), m_outputs(nullptr)
{
}

Network::Network(const int * neuronLayers, int numLayers) //: m_inputs(nullptr), m_weights(nullptr), m_biases(nullptr), m_outputs(nullptr)
{
	srand(time(NULL)); //seed for the random initialization

	m_numVLayers = numLayers; //hidden layers + 1 for input + 1 for output
	m_numWLayers = numLayers - 1;

	//create the value vectors
	m_values = new Eigen::VectorXf[m_numVLayers];
	for (int i = 0; i < m_numVLayers; ++i)
	{
		m_values[i] = Eigen::VectorXf(neuronLayers[i]);
	}

	//initialize inputs to random values (0 to 1)
	for (int i = 0; i < neuronLayers[0]; ++i)
	{
		m_values[0](i) = ((double)rand()) / (RAND_MAX);
	}

	//create weight matricies and initialize them to random values (0 to 1) as being created
	m_weights = new Eigen::MatrixXf[m_numWLayers];
	for (int i = 0; i < m_numWLayers; ++i)
	{
		m_weights[i] = Eigen::MatrixXf(neuronLayers[i+1], neuronLayers[i]); //store backwards so we can dot product
		for (int r = 0; r < neuronLayers[i+1]; ++r)
		{
			for (int c = 0; c < neuronLayers[i]; ++c)
			{
				m_weights[i](r,c) = ((double)rand()) / (RAND_MAX);
			}
		}
	}

	//create bias vector and initialize them to 1
	m_biases = Eigen::VectorXf(m_numVLayers - 1); //no bias for input layer
	for (int i = 0; i < m_numVLayers - 1; ++i)
	{
		m_biases(i) = 1;
	}
	//create bias weights and set them to rand 0 to 1
	m_biasWeights = new Eigen::VectorXf[m_numVLayers - 1];
	for (int layer = 0; layer < m_numVLayers - 1; ++layer)
	{
		m_biasWeights[layer] = Eigen::VectorXf(m_values[layer + 1].rows());
		for (int i = 0; i < m_values[layer + 1].rows(); ++i) //no input layer bias
		{
			m_biasWeights[layer](i) = ((double)rand()) / (RAND_MAX);
		}
	}
}


Network::~Network()
{
	//delete[] m_values;
	//delete[] m_weights;
	//delete[] m_biasWeights;
}

void Network::setInput(float * inputs, int len)
{
	for (int i = 0; i < len; ++i)
	{
		m_values[0](i) = inputs[i];
	}
}

void Network::calculateOutput()
{	
	//start with input values, calculate layer1Vals, then layer2Vals, ..., then outputVals
	Eigen::MatrixXf weiVal;
	Eigen::VectorXf weiValBias;
	for (int i = 1; i < m_numVLayers; ++i)
	{
		weiVal = (m_weights[i-1] * m_values[i-1]);
		weiValBias = Eigen::VectorXf(weiVal.rows());
		for (int r = 0; r < weiVal.rows(); ++r)
		{
			weiValBias(r) = 0;
			for (int c = 0; c < weiVal.cols(); ++c)
			{
				weiValBias(r) += weiVal(r, c);
			}
			weiValBias(r) += m_biases[i-1] * m_biasWeights[i-1](r); //row is the right layer, col is the left layer... bias points to right layer
			m_values[i](r) = getSigmoid(weiValBias(r));
		}
	}
}

void Network::backPropagate(float * desiredResult)
{
	Eigen::MatrixXf * prev_dEdO_dOdN_W = new Eigen::MatrixXf[m_numWLayers - 1]; //need to store this in each weight layer calc except the last one
	Eigen::MatrixXf * costChange = new Eigen::MatrixXf[m_numWLayers]; //cost change for each weight
	Eigen::VectorXf * costBiasChange = new Eigen::VectorXf[m_numWLayers]; //cost change for each bias

	//go through each layer of weights
	for (int l = m_numWLayers - 1; l >= 0 ; --l) //layer starts on output layer and goes backwards to and including 0
	{
		costChange[l] = Eigen::MatrixXf(m_weights[l].rows(), m_weights[l].cols());
		costBiasChange[l] = Eigen::VectorXf(m_weights[l].rows());
		if(l > 0) prev_dEdO_dOdN_W[l-1] = Eigen::MatrixXf(m_weights[l].rows(), m_weights[l].cols());

		//go through each individual weight and back propagate
		for (int i = 0; i < m_weights[l].rows(); ++i) //also m_values[l+1].rows()
		{
			//go through all weights pointing to this right side node
			for (int j = 0; j < m_weights[l].cols(); ++j) //also m_values[l].rows()
			{
				if (l == m_numWLayers - 1) //last layer of weights, easier computation
				{
					float out_prev = m_values[l](j);
					float out_this = m_values[l + 1](i);
					prev_dEdO_dOdN_W[l - 1](i,j) = -(desiredResult[i] - out_this) * out_this *(1 - out_this); //desiredResult of which output it points to
					costChange[l](i,j) = prev_dEdO_dOdN_W[l-1](i,j) * out_prev;
					prev_dEdO_dOdN_W[l - 1] *= m_weights[l](i, j);
				}
				else
				{
					float temp_dEdO_dOdN_W = 0;
					for (int h2 = 0; h2 < m_values[l + 2].rows(); ++h2) //l+1 is current layer, l+2 is the next layer after
					{
						temp_dEdO_dOdN_W += prev_dEdO_dOdN_W[l](h2, i);
					}
					float out_prev = m_values[l](j);
					float out_this = m_values[l + 1](i);
					temp_dEdO_dOdN_W *= out_this * (1 - out_this);
					costChange[l](i, j) = temp_dEdO_dOdN_W * out_prev;
					if(l > 0) prev_dEdO_dOdN_W[l - 1](i, j) = temp_dEdO_dOdN_W * m_weights[l](i, j);
				}
			}

			//calculate for the bias pointing to this right side node
			if (l == m_numWLayers - 1) //last layer of weights, easier computation
			{
				float out_prev = m_biases[l];
				float out_this = m_values[l + 1](i);
				costBiasChange[l](i) = -(desiredResult[i] - out_this) * out_this *(1 - out_this);
			}
			else
			{
				float temp_dEdO_dOdN_W = 0;
				for (int h2 = 0; h2 < m_values[l + 2].rows(); ++h2) //l+1 is current layer, l+2 is the next layer after
				{
					temp_dEdO_dOdN_W += prev_dEdO_dOdN_W[l](h2, i);
				}
				float out_prev = m_biases[l];
				float out_this = m_values[l + 1](i);
				temp_dEdO_dOdN_W *= out_this * (1 - out_this);
				costBiasChange[l](i) = temp_dEdO_dOdN_W;
			}
		}
	}

	//go through each individual weight and apply change
	for (int layer = 0; layer < m_numWLayers; ++layer)
	{
		for (int i = 0; i < m_weights[layer].rows(); ++i)
		{
			for (int j = 0; j < m_weights[layer].cols(); ++j)
			{
				m_weights[layer](i, j) -= LEARNING_RATE * costChange[layer](i, j);
			}
		}
	}

	//go through each individual bias weight and apply change
	for (int layer = 0; layer < m_numWLayers; ++layer)
	{
		for (int i = 0; i < m_biasWeights[layer].rows(); ++i)
		{
			m_biasWeights[layer](i) -= BIAS_LEARNING_RATE * costBiasChange[layer](i);
		}
	}

	delete[] prev_dEdO_dOdN_W;
	delete[] costChange;
	delete[] costBiasChange;
}

Eigen::MatrixXf * Network::getWeights() const
{
	return m_weights;
}

Eigen::VectorXf * Network::getValues() const
{
	return m_values;
}

Eigen::VectorXf Network::getOutputs() const
{
	return m_values[m_numVLayers-1];
}

float Network::getSigmoid(float x)
{
	float eToX = exp(-x);
	return (1 / (eToX + 1));
}
