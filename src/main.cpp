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
using namespace irr;
#include "CEventReceiver.hpp"


IrrlichtDevice* device = NULL;
video::IVideoDriver* driver;
CEventReceiver receiver;
scene::ISceneManager* smgr;
scene::ICameraSceneNode* cam;

void InitIrrlicht( int wWidth, int wHeight )
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

void Update()
{
	core::vector3df v = cam->getPosition();
	std::cout << v.X << ", " << v.Y << ", " << v.Z << std::endl;
	if(device->isWindowActive())
	{
		/* RENDERING */
		printf("isactive\n"); fflush(0);
		driver->beginScene(true, true, video::SColor(64, 67, 74, 255));
		smgr->drawAll();
		driver->endScene();
	}
	else
	{
		device->yield();
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
	//SpawnBall( 0, 0, 2, 1 );
}

int main ()
{
	InitIrrlicht( 800, 600 );
	SetupScene();

	bool is_running = true;

	while(device->run())
	{
		Update();
	}

	if(device) device->drop();
	return 0;
}
