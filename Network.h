#pragma once

#include "pch.h"
#include <Eigen/Dense>

class Network
{
public:
	Network();
	Network(const int * neuronLayers, int numLayers);
	~Network();

	void setInput(float * inputs, int len);
	void calculateOutput();
	void backPropagate(float * desiredResult);
	Eigen::MatrixXf * getWeights() const;
	Eigen::VectorXf * getValues() const;
	Eigen::VectorXf getOutputs() const;

private:
	float getSigmoid(float x);

	//all value layers, 0 is input layer, last is output layer
	Eigen::VectorXf * m_values;

	//Weight matrix layer array
	Eigen::MatrixXf * m_weights;

	//bias vector
	Eigen::VectorXf m_biases;

	//bias weights
	Eigen::VectorXf * m_biasWeights;

	//number of value layers
	int m_numVLayers;

	//number of wight layers
	int m_numWLayers;
};

