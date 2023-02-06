#include <pylon/PylonUtilityIncludes.h>
#include <pylon/PylonIncludes.h>
#include <pylon/InstantCamera.h>
#include <pylon/usb/BaslerUsbInstantCamera.h>
#include <iostream>
#include <string>
#include <cstdio>

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
	//Set acquisition mode to continuous
	camera.StartGrabbing(GrabStrategy_OneByOne);
	//Take a single image
	camera.RetrieveResult(5000, ptrGrabresult, TimeoutHandling_ThrowException);
	//CImagePersistence::Save(ImageFileFormat_Png, "./results/hello.png", ptrGrabresult); //Save image to file 

	//camera.StopGrabbing();//Stop the camera
	//camera.Close();//close camera

	PylonTerminate();
	return 0;
}
