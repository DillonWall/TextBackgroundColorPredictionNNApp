//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"

using namespace RGBNeuNetApp;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Microsoft::UI::Xaml::Controls;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

MainPage::MainPage()
{
	InitializeComponent();

	srand(time(NULL));
	float inputs[3];
	float output = 0;
	for (int i = 0; i < 3; ++i)
	{
		inputs[i] = ((double) rand()) / (RAND_MAX);
	}

	if ((inputs[0] * 255 * 0.299 + inputs[1] * 255 * 0.587 + inputs[2] * 255 * 0.114) > 186)
		output = 1.0; //black text

	int layers[4] = { 3, 5, 5, 1 };
	rgbBrain = Network(layers, 4);

	rgbBrain.setInput(inputs, 3);
	rgbBrain.calculateOutput();

	RBox->Text = "" + inputs[0];
	GBox->Text = "" + inputs[1];
	BBox->Text = "" + inputs[2];

	OutTBlock->Text = "" + rgbBrain.getOutputs()[0];

	ExpectedTBlock->Text = "" + output;

	OutColor->Fill = ref new SolidColorBrush(Windows::UI::ColorHelper::FromArgb(255, inputs[0] * 255, inputs[1] * 255, inputs[2] * 255));
}


void RGBNeuNetApp::MainPage::InputsUpdated()
{
	//convert text boxes to floats and store them, then update n.n. and calc output with forward prop
	float inputs[3] = { _wtof(RBox->Text->Data()) , _wtof(GBox->Text->Data()) , _wtof(BBox->Text->Data()) };
	rgbBrain.setInput(inputs, 3);
	rgbBrain.calculateOutput();

	//fix textbox consistency
	RBox->Text = "" + inputs[0];
	GBox->Text = "" + inputs[1];
	BBox->Text = "" + inputs[2];

	//set output text to n.n. output
	OutTBlock->Text = "" + rgbBrain.getOutputs()[0];

	//calculate expected output
	float output = 0;
	if ((inputs[0] * 255 * 0.299 + inputs[1] * 255 * 0.587 + inputs[2] * 255 * 0.114) > 186)
		output = 1.0; //black text
	ExpectedTBlock->Text = "" + output;

	//draw color from rgb
	OutColor->Fill = ref new SolidColorBrush(Windows::UI::ColorHelper::FromArgb(255, inputs[0] * 255, inputs[1] * 255, inputs[2] * 255));

	//back prop using expected output
	float desired[1] = { output };
	rgbBrain.backPropagate(desired);
}

void RGBNeuNetApp::MainPage::TrainByEquation(int iterations)
{
	float inputs[3];
	float output = 0;
	for (int i = 0; i < iterations; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			inputs[j] = ((double)rand()) / (RAND_MAX);
		}
		rgbBrain.setInput(inputs, 3);
		rgbBrain.calculateOutput();

		//calculate expected output
		output = 0;
		if ((inputs[0] * 255 * 0.299 + inputs[1] * 255 * 0.587 + inputs[2] * 255 * 0.114) > 186)
			output = 1.0; //black text

		//back prop using expected output
		float desired[1] = { output };
		rgbBrain.backPropagate(desired);
	}
	//fix textbox consistency
	RBox->Text = "" + inputs[0];
	GBox->Text = "" + inputs[1];
	BBox->Text = "" + inputs[2];

	//set output text to n.n. output
	OutTBlock->Text = "" + rgbBrain.getOutputs()[0];

	ExpectedTBlock->Text = "" + output;

	//draw color from rgb
	OutColor->Fill = ref new SolidColorBrush(Windows::UI::ColorHelper::FromArgb(255, inputs[0] * 255, inputs[1] * 255, inputs[2] * 255));
}

void RGBNeuNetApp::MainPage::TextBlock_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

}


void RGBNeuNetApp::MainPage::CalcButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	//InputsUpdated();
	int passes = _wtoi(NumPasses->Text->Data());
	TrainByEquation(passes);
}


void RGBNeuNetApp::MainPage::SingleCalcButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	InputsUpdated();
}
