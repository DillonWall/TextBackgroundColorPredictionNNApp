# TextBackgroundColorPredictionNNApp

## Overview
This program is a C++ UWP application that trains a custom made Neural Network from scratch to be able to determine if
a background should have black or white text on it so that it contrasts the most.

## Details
The 3 text boxes on the left of the application correspond to the Red, Green, and Blue values of the background respectively.
These should be numbers between 0 and 1 and may be entered manually. 
If this is done, make sure to click "Calculate Single" to only train for one iteration on the current color.

The "Number of Passes" text box may contain any positive number, 
indicating the amount of random colors to train on per click of the "Calculate" button.

The decimal number in the middle of the screen indicates the output of the Neural Network, which is a number between 0 and 1.
This number can be thought of as a percentage scale from white to black, white being 0 and black being 1.

The expected result is similar to the actual result (white being 0 and black being 1), 
but instead indicating what the network should have predicted for the current color.

The top right colored rectangle is a preview of the current color.

## Findings
The neural network I created from scratch seems to become very accurate after about 100,000 iterations.
After this point, it may still occasionally get some predictions wrong; 
it seems to wrongly predict more often when the background is a lighter color and needs black text,
probably because it is more likely to have instances with a darker colored background when choosing random colors.

## Installation
Open and run with Visual Studio 2022

Ensure that you have the "Universal Windows Platform development" workload installed under "Tools" -> "Get Tools and Features..."
![image](https://user-images.githubusercontent.com/49173127/166116441-a49a6772-d96e-4c10-8ff3-663d0eab3d99.png)

Ensure you have the Windows SDK Version 10.0.19041.0, or alternatively try retargeting the project for your installed version by
right-clicking the project in the solution explorer and selecting "Retarget Projects"
![image](https://user-images.githubusercontent.com/49173127/166116471-caa10f52-239a-49e1-ad65-84049cd9de3b.png)


## Status
This project is finished, but I would have liked to add the option to save the neural network and reload it to retain training.

## Screenshots
![image](https://user-images.githubusercontent.com/49173127/166116548-9b7375b9-3000-428b-8a1c-21b6f48b1046.png)

![image](https://user-images.githubusercontent.com/49173127/166116370-f086b75c-d126-484e-a315-30e5f95f1524.png)
