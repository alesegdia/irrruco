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
#include "IQuadSceneNode.h"

using namespace irr;


IrrlichtDevice* device = NULL;
video::IVideoDriver* driver;
CEventReceiver receiver;
scene::ISceneManager* smgr;
scene::ICameraSceneNode* cam;
cv::VideoCapture videocapture;
cv::Mat camimg, camimgund;
bool is_running = true;
video::ITexture* tex;
int ww, wh;

void InitIrrlicht( int wWidth, int wHeight)
{
	device = createDevice(
			video::EDT_OPENGL, core::dimension2d<u32>(wWidth, wHeight),
			16, false, false, false, &receiver);
	if(!device)
		exit(EXIT_FAILURE);

	driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	receiver.SetIrrDevice(device);
}


void SpawnBall( float x, float y, float z, float radius )
{
	scene::ISceneNode* node = smgr->addSphereSceneNode(radius);
	node->setPosition( core::vector3df( x, y, z ) );
	node->setMaterialFlag(video::EMF_LIGHTING,0);
	node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
}

void UpdateRenderer()
{
	if(device->isWindowActive())
	{
		/* RENDERING */
		driver->beginScene(true, true, video::SColor(64, 67, 74, 255));
		driver->draw2DImage( tex, core::position2d<s32>(ww/4,wh/4) );
		smgr->drawAll();
		driver->endScene();
	}
	else
	{
		device->yield();
	}
}

void CameraLoop()
{
	while( videocapture.grab() && is_running )
	{
		videocapture.retrieve (camimg);
		cv::imshow("in", camimg);
		cv::waitKey(1);
	}
}

void SetupScene()
{
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
	videocapture >> camimg;

	// set width and height
	ww = camimg.cols;
	wh = camimg.rows;

	InitIrrlicht(ww, wh);
	SetupScene();

	tex = driver->addTexture(core::dimension2d<u32>(ww/2, wh/2), "asd");

	// launch cam grabbing loop thread
	std::thread camthread( CameraLoop );

	// main rendering loop
	while( device->run() ) UpdateRenderer();

	// tell cam loop to finish and wait for it
	is_running = false;
	camthread.join();

	// release irrlicht
	if(device) device->drop();

	return 0;
}
