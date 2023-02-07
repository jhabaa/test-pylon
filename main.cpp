#include <pylon/PylonUtilityIncludes.h>
#include <pylon/PylonIncludes.h>
#include <pylon/InstantCamera.h>
#include <pylon/usb/BaslerUsbInstantCamera.h>
#include <iomanip>
#include <iostream>
#include <string>
#include <cstdio>
#include <chrono>


using namespace Pylon;
using namespace std;
int main(int argc, char* argv[]) {
	//Pylon init
	PylonInitialize();
	std::cout <<"Program has started"<< std::endl;
	//Enumerate devices
	CInstantCamera camera( CTlFactory::GetInstance().CreateFirstDevice() );
	std::cout<<" Camera connected"<< std::endl;
	//get infos about each
	std::cout<<" - "<<camera.GetDeviceInfo().GetModelName()<< std::endl;

	//Lets take a pict
	CGrabResultPtr ptrGrabresult; // Pointer to receive the grabResult
	camera.Open(); // Open the camera
	std::cout<<" Camera is set to OPEN"<< std::endl;
	
	std::cout<<"press \"p\" to take a picture and \"q\" to quit"<< std::endl;
	//Set acquisition mode to continuous
	bool run = true;
	//Number of images to be grabbed.
	int numImagesToGrab = 1;
	while(run){
		//if key P is pressed
		std::cout<<"press \"p\" to take a picture and \"q\" to quit"<< std::endl;
		std::string input;
		std::cin>>input;
		if (input == "p" || input == "P"){
			std::cout<<" Camera is set to GRABBING"<< std::endl;
			camera.StartGrabbing(GrabStrategy_LatestImageOnly);
			std::cout<<" Camera is set to GRABBING"<< std::endl;
			camera.RetrieveResult(5000, ptrGrabresult, TimeoutHandling_ThrowException);
			std::cout<<" Camera is set to RETRIEVING"<< std::endl;
			if(ptrGrabresult->GrabSucceeded()){
				std::cout<<" Camera is set to SUCCEEDED"<< std::endl;
				CImageFormatConverter formatConverter;
				formatConverter.OutputPixelFormat = PixelType_BGR8packed;
				CPylonImage pylonImage;
				formatConverter.Convert(pylonImage, ptrGrabresult);
				CImagePersistence::Save(ImageFileFormat_Png, "test.png", pylonImage);
				std::cout<<" Camera is set to SAVED"<< std::endl;
				//run = false;
				camera.Close();
			}
		}
		if(input == "q" || input == "Q"){
			run = false;
		}
	}
	camera.StopGrabbing();
	return 0;
}
