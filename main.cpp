#include <pylon/PylonUtilityIncludes.h>
#include <pylon/PylonIncludes.h>
#include <pylon/InstantCamera.h>
#include <pylon/DeviceClass.h>
#include <pylon/usb/BaslerUsbInstantCamera.h>
#include <pylon/usb/BaslerUsbInstantCameraArray.h>
#include <pylon/AcquireSingleFrameConfiguration.h>
#include <pylon/ConfigurationEventHandler.h>
#include <pylon/AcquireContinuousConfiguration.h>
#include <pylon/SoftwareTriggerConfiguration.h>
#include <pylon/ImagePersistence.h>


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

// ===============================  For only one and the first camera =============================================
	/*
	CInstantCamera camera( CTlFactory::GetInstance().CreateFirstDevice());	
	std::cout<<" Camera connected"<< std::endl;
	//get infos about each
	std::cout<<" - "<<camera.GetDeviceInfo().GetModelName()<< std::endl;

	//Lets take a pict
	CGrabResultPtr ptrGrabresult; // Pointer to receive the grabResult
	camera.Open(); // Open the camera
	std::cout<<" Camera is set to OPEN"<< std::endl;
	==============================================================================================================*/
	
	
	//For multiples cameras
	CTlFactory& tlFactory = CTlFactory::GetInstance();
	DeviceInfoList_t devices;
	if ( tlFactory.EnumerateDevices(devices) == 0 )
	{
		throw RUNTIME_EXCEPTION( "No camera present.");
	}
	// Create an array of instant cameras for the found devices and avoid exceeding a maximum number of devices.
	CBaslerUsbInstantCameraArray cameras(2);
	// Create and attach all Pylon Devices.
	std::cout << "Here are our cameras " << std::endl;
	for ( size_t i = 0; i < cameras.GetSize(); ++i)
	{
		cameras[ i ].Attach( tlFactory.CreateDevice( devices[ i ]));
		std::cout<<" - "<<cameras[ i ].GetDeviceInfo().GetModelName()<<" "<<cameras[ i ].GetDeviceInfo().GetDeviceGUID()<<std::endl;
	}


	//Set acquisition mode to continuous
	bool run = true;
	//Number of images to be grabbed.
	int numImagesToGrab = 1;

	//use software trigger confifguration to take a picture on all cameras
	for ( size_t i = 0; i < cameras.GetSize(); ++i)
	{
		cameras[ i ].Open();
		cameras[ i ].AcquisitionMode.SetValue(Basler_UsbCameraParams::AcquisitionMode_Continuous);
		cameras[ i ].TriggerSelector.SetValue(Basler_UsbCameraParams::TriggerSelector_FrameStart);
		cameras[ i ].TriggerMode.SetValue(Basler_UsbCameraParams::TriggerMode_On);
		//cameras[ i ].TriggerSource.SetValue(Basler_UsbCameraParams::TriggerSource_Software);
		//If we trigger the camera with hardware source
		cameras[ i ].TriggerSource.SetValue(Basler_UsbCameraParams::TriggerSource_Line1);
		cameras[ i ].ExposureTime.SetValue(130000);
		cameras[ i ].StartGrabbing(GrabStrategy_LatestImageOnly);

		//send software trigger
		
	}
	auto a = chrono::high_resolution_clock::now();
	for ( size_t i = 0; i < cameras.GetSize(); ++i)
	{
		cameras[ i ].ExecuteSoftwareTrigger();
		//auto b = chrono::high_resolution_clock::now();
		//std::cout<<"execution time : "<< chrono::duration_cast<std::chrono::milliseconds>(a-b).count() <<std::endl;
	}
	
	//Get images trigger by software trigger
	for ( size_t i = 0; i < cameras.GetSize(); ++i)
	{
		CGrabResultPtr ptrGrabresult;
		String_t name = i + "image.png";
		cameras[ i ].RetrieveResult(5000, ptrGrabresult, TimeoutHandling_ThrowException);
		if(ptrGrabresult->GrabSucceeded()){
			std::cout<<" Camera is set to SUCCEEDED"<< std::endl;
			CImageFormatConverter formatConverter;
			formatConverter.OutputPixelFormat = PixelType_BGR8packed;
			CPylonImage pylonImage;
			formatConverter.Convert(pylonImage, ptrGrabresult);
			std::stringstream ss;
			ss << i << ".png";
			std::string s = ss.str();
			CImagePersistence::Save(ImageFileFormat_Png, name, pylonImage);
			std::cout<<" Camera is set to SAVED"<< std::endl;
			//run = false;
			cameras[ i ].Close();
		}
	}
	
	/*
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
	*/
	return 0;
}
