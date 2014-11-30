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


IrrlichtDevice* device;
video::IVideoDriver* driver;
CEventReceiver receiver;
scene::ISceneManager* smgr;

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

void CleanIrrlicht()
{
	if(device) device->drop();
	//delete driver;
	//delete smgr;
}

void Update()
{
	if(device->isWindowActive())
	{
		/* RENDERING */
		driver->beginScene(true, true, video::SColor(64, 67, 74, 255));
		smgr->drawAll();
		driver->endScene();
	}
	else
	{
		device->yield();
	}
}

int main ()
{
	InitIrrlicht( 800, 600 );
	bool is_running = true;
	
	while(device->run())
	{
		Update();
	}
	
	CleanIrrlicht();
	return 0;
}
