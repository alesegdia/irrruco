//	IrrBull is a very simple engine integrating Irrlicht and Bullet.
//	Copyright (C) 2013 Alejandro Seguí Díaz <alesegdia at gmail dot com>
//
//	IrrBull is free software: you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published by
//	the Free Software Foundation, either version 3 of the License, or
//	(at your option) any later version.
//
//	IrrBull is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.
//
//	You should have received a copy of the GNU General Public License
//	along with IrrBull.  If not, see <http://www.gnu.org/licenses/>.

#include "common.h"
#include <thread>
#include "CEventReceiver.hpp"

using namespace irr;


IrrlichtDevice* device = NULL;
video::IVideoDriver* driver;
CEventReceiver receiver;
scene::ISceneManager* smgr;
scene::ICameraSceneNode* cam;
cv::VideoCapture videocapture;
cv::Mat camframe, camframeund;
bool is_running = true;
video::ITexture* tex;
int ww, wh;

void SpawnBall( float x, float y, float z, float radius ){
	scene::ISceneNode* node = smgr->addSphereSceneNode(radius);
	node->setPosition( core::vector3df( x, y, z ) );
	node->setMaterialFlag(video::EMF_LIGHTING,0);
	node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
}

void CameraLoop() {
	while( videocapture.grab() && is_running )
	{
		videocapture.retrieve (camframe);
		//cv::imshow("in", camframe);
		//cv::waitKey(1);
	}
}

std::string cvmattype2str(int type) {
	std::string r;

	uchar depth = type & CV_MAT_DEPTH_MASK;
	uchar chans = 1 + (type >> CV_CN_SHIFT);

	switch ( depth ) {
		case CV_8U:  r = "8U"; break;
		case CV_8S:  r = "8S"; break;
		case CV_16U: r = "16U"; break;
		case CV_16S: r = "16S"; break;
		case CV_32S: r = "32S"; break;
		case CV_32F: r = "32F"; break;
		case CV_64F: r = "64F"; break;
		default:     r = "User"; break;
	}

	r += "C";
	r += (chans+'0');

	return r;
}


std::string irrtextype2str( video::ECOLOR_FORMAT fmt ) {
	switch( fmt ) {
		case video::ECF_A1R5G5B5: return "ECF_A1R5G5B5";
		case video::ECF_R5G6B5: return "ECF_R5G6B5";
		case video::ECF_R8G8B8: return "ECF_R8G8B8";
		case video::ECF_A8R8G8B8: return "ECF_A8R8G8B8";
		case video::ECF_R16F: return "ECF_R16F";
		case video::ECF_G16R16F: return "ECF_G16R16F";
		case video::ECF_A16B16G16R16F: return "ECF_A16B16G16R16F";
		case video::ECF_R32F: return "ECF_R32F";
		case video::ECF_G32R32F: return "ECF_G32R32F";
		case video::ECF_A32B32G32R32F: return "ECF_A32B32G32R32F";
	}
}

void SetupScene() {
	SKeyMap keyMap[8];
	keyMap[0].Action = EKA_MOVE_FORWARD;
	keyMap[0].KeyCode = KEY_UP;
	keyMap[1].Action = EKA_MOVE_FORWARD;
	keyMap[1].KeyCode = KEY_KEY_W;

	keyMap[2].Action = EKA_MOVE_BACKWARD;
	keyMap[2].KeyCode = KEY_DOWN;
	keyMap[3].Action = EKA_MOVE_BACKWARD;
	keyMap[3].KeyCode = KEY_KEY_S;

	keyMap[4].Action = EKA_STRAFE_LEFT;
	keyMap[4].KeyCode = KEY_LEFT;
	keyMap[5].Action = EKA_STRAFE_LEFT;
	keyMap[5].KeyCode = KEY_KEY_A;

	keyMap[6].Action = EKA_STRAFE_RIGHT;
	keyMap[6].KeyCode = KEY_RIGHT;
	keyMap[7].Action = EKA_STRAFE_RIGHT;
	keyMap[7].KeyCode = KEY_KEY_D;
	cam = smgr->addCameraSceneNodeFPS(0, 50.f, 0.01f, -1, keyMap, 8);

	SpawnBall( 0.5, 0, 2, 1 );
	SpawnBall( -0.5, 0, 2, 1 );
}

int main ()
{
	// init camera
	videocapture.open(1);
	if( !videocapture.isOpened() )
	{
		printf("VIDEO CAPTURE ERROR!!\n");
		exit(EXIT_FAILURE);
	}

	// get test image to know size
	videocapture >> camframe;
	ww = camframe.cols;
	wh = camframe.rows;

	// init irrlicht stuff
	device = createDevice( video::EDT_OPENGL, core::dimension2d<u32>(ww, wh), 16, false, false, false, &receiver );
	if(!device) exit(EXIT_FAILURE);
	driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	receiver.SetIrrDevice(device);
	SetupScene();

	// init irrlicht texture
	video::IImage* img = driver->createImage(video::ECOLOR_FORMAT::ECF_R8G8B8, core::dimension2d<u32>(ww,wh));
	tex = driver->addTexture("asd", img);

	printf("OpenCV camera image: %s %dx%d \n", cvmattype2str( camframe.type() ).c_str(), camframe.cols, camframe.rows );
	printf("Irrlicht texture: %s, [%d,%d]\n", irrtextype2str(tex->getColorFormat()).c_str(), tex->getSize().Width, tex->getSize().Height );

	// launch cam grabbing loop thread
	std::thread camthread( CameraLoop );

	// main rendering loop
	while( device->run() )
	{
		if(device->isWindowActive())
		{
			/* RENDERING */
			//cv::Mat aux( camframe.rows, camframe.cols, CV_8UC4, camframe.data );
			//cv::Mat aux2;
			//aux.copyTo(aux2);

			cv::Mat RGBA(camframe.size(), CV_8UC4);
			cv::cvtColor(camframe, RGBA, CV_RGB2RGBA, 4);

			void* texdata = tex->lock();
			unsigned char* camdata = RGBA.data;
			memcpy(texdata, camdata, camframe.cols * camframe.rows * sizeof(uint8_t) * 4);
			tex->unlock();

			driver->beginScene(true, true, video::SColor(64, 67, 74, 255));
			driver->draw2DImage( tex, core::position2d<s32>(0,0) );
			smgr->drawAll();
			driver->endScene();
		}
		else
		{
			device->yield();
		}
	}

	// tell cam loop to finish and wait for it
	is_running = false;
	camthread.join();

	// release irrlicht
	if(device) device->drop();

	return 0;
}
