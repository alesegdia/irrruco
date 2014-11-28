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

#include "CGame.hpp"
#include "CEngine.hpp"
#include "CPhysics.hpp"
#include "CEventReceiver.hpp"
#include "CGameObjectManager.hpp"
#include "GOEntity.hpp"
#include "GOEntityController.hpp"
#include "GOCamera.hpp"
#include "GOIAController.hpp"

CGame::CGame()
{

}

CGame::~CGame()
{

}

void CGame::Init()
{	engine.Init();
	_goMgr = new CGameObjectManager();
}

btRigidBody* CGame::PushSphere(const btVector3& position, f32 radius, btScalar mass)
{
	scene::ISceneNode* _cube = engine.GetSMgr()->addSphereSceneNode(radius);
	_cube->setMaterialFlag(video::EMF_LIGHTING,1);
	_cube->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);

	btRigidBody* rb = engine.GetPhysics()->PushObject(
			position,
			core::vector3df(1,1,1),
			new btSphereShape(radius),
			mass);
	rb->setUserPointer((void*)_cube);
	return rb;
}

btRigidBody* CGame::PushCube(const btVector3& position, const core::vector3df& scale, btScalar mass)
{
	scene::ISceneNode* _cube = engine.GetSMgr()->addCubeSceneNode(1.f);
	_cube->setScale(scale);
	_cube->setMaterialFlag(video::EMF_LIGHTING,1);
	_cube->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);

	btRigidBody* rb = engine.GetPhysics()->PushObject(
			position,
			scale,
			new btBoxShape(btVector3(scale.X/2, scale.Y/2, scale.Z/2)),
			mass);
	rb->setUserPointer((void*)_cube);
	return rb;
}





void CGame::SetupScene()
{
	/* LIGHTS ON! **************************/
	_light1 =
		engine.GetSMgr()->addLightSceneNode(
				0, core::vector3df(1000,1000,0),
				video::SColorf(0.3f,0.3f,0.3f),
				1000.f, 1);
	if(_light1)
	{
		_light1->getLightData().Attenuation.set(0.f,1.f/4000.f, 0.f);
	}


	/* PLAYER ******************************/
	GOEntity* tmp = new GOEntity();
	btCollisionShape* capsuleShape = new btCapsuleShape(10.f, 20.f);
	btCompoundShape* compound = new btCompoundShape();
	btTransform localTrans;
	localTrans.setIdentity();
	localTrans.setOrigin(btVector3(0,14,0));
	compound->addChildShape(localTrans,capsuleShape);
	tmp->Load("../media/doom/Cyberdemon/Cyber.md2",
			"../media/doom/Cyberdemon/cyber.jpg",
			btVector3(0.f, 300.f, 100.f),
			core::vector3df(0.11f,0.11f,0.11f),
			compound, btScalar(10));
	tmp->GetNode()->setAnimationSpeed(70);
	tmp->GetNode()->setFrameLoop(22,22);
	tmp->GetNode()->setLoopMode(true);
	tmp->GetRigidBody()->setAngularFactor(btVector3(0,0,0));
	_goMgr->RegisterGameObject(tmp, "player");


	/* ENEMY ******************************/
	tmp = new GOEntity();
	btCollisionShape* capsuleShape2 = new btCapsuleShape(10.f, 20.f);
	btCompoundShape* compound2 = new btCompoundShape();
	btTransform localTrans2;
	localTrans2.setIdentity();
	localTrans2.setOrigin(btVector3(0,14,0));
	compound2->addChildShape(localTrans2,capsuleShape2);
	tmp->Load("../media/revenant/Revenant.md2",
			"../media/revenant/revenant.jpg",
			btVector3(0.f, 300.f, 0.f),
			core::vector3df(0.50f,0.50f,0.50f),
			compound2, btScalar(10));
	tmp->GetNode()->setAnimationSpeed(40);
	tmp->GetNode()->setFrameLoop(12,35);
	tmp->GetNode()->setLoopMode(true);
	tmp->GetRigidBody()->setAngularFactor(btVector3(0,0,0));
	_goMgr->RegisterGameObject(tmp, "enemy");

	/* IA CONTROLLER *************************/
    GOIAController* iacont = new GOIAController();
    _goMgr->RegisterGameObject(iacont, "iacontroller");


	/* PLAY OBJECTS **************************/
	PushSphere(
		btVector3(100,200,0),
		10,
		50);

	PushSphere(
		btVector3(100,300,0),
		10,
		50);

	PushSphere(
		btVector3(100,400,0),
		10,
		50);

	PushSphere(
		btVector3(100,500,0),
		10,
		50);

	PushCube(
		btVector3(0,100,0),
		core::vector3df(10,10,10),
		10);

	PushCube(
		btVector3(0,0,150),
		core::vector3df(500,5,500),
		0);

	/* PLAYER CONTROLLER *******************/
	IGameObject* entityController = new GOEntityController();
	_goMgr->RegisterGameObject(entityController, "playercontroller");


	/* CAMARA ******************************/
	IGameObject* camera = new GOCamera();
	_goMgr->RegisterGameObject(camera, "maincamera");
}

void CGame::ConnectSlots()
{
    GOCamera* cam =                  _goMgr->GetGameObjectByTag<GOCamera>          ("maincamera");
    GOEntityController* controller = _goMgr->GetGameObjectByTag<GOEntityController>("playercontroller");
    GOEntity* entity =               _goMgr->GetGameObjectByTag<GOEntity>          ("player");
    GOIAController* iacont =         _goMgr->GetGameObjectByTag<GOIAController>    ("iacontroller");
    GOEntity* enemy =                _goMgr->GetGameObjectByTag<GOEntity>          ("enemy");

    controller->AttachCamera(cam);
    controller->AttachEntity(entity);
    iacont->SetPlayer(entity);
    iacont->SetEntity(enemy);
}

void CGame::Run()
{
	Init();
	SetupScene();
	_goMgr->Awake();
	ConnectSlots();
	_goMgr->Start();
	//Start();


	irr::video::SMaterial debugMat;
	debugMat.Lighting = false;
	bool pause = false;

	// engine.Update()?
	u32 then = engine.GetIrrDevice()->getTimer()->getTime();
	engine.GetIrrDevice()->getCursorControl()->setVisible(false);
	while(engine.IsRunning())
	{
		// engine.DrawAll()??
		if(engine.GetIrrDevice()->isWindowActive())
		{
			/* RENDERING */
			engine.GetVideoDriver()->beginScene(true, true, video::SColor(64, 67, 74, 255));
			engine.GetSMgr()->drawAll();

			engine.GetVideoDriver()->setMaterial(debugMat);
			engine.GetVideoDriver()->setTransform(irr::video::ETS_WORLD, irr::core::IdentityMatrix);
			engine.GetPhysics()->DebugDrawWorld();
			engine.GetVideoDriver()->endScene();
		}
		else
		{
			engine.GetIrrDevice()->yield();
		}

		const u32 now = engine.GetIrrDevice()->getTimer()->getTime();
		u32 delta = (now-then);
		then = now;
		if(!pause) engine.GetPhysics()->UpdatePhysics(delta);

		if(engine.GetEventReceiver()->IsKeyDown(KEY_KEY_P))
		{
			pause = !pause;
			//engine.GetPhysics()->pause();
			//engine.GetPhysics()->DeleteEntity(_goMgr->GetGameObjectByTag<GOEntity>("map"));
		}

		_goMgr->Update();
	}

	_goMgr->Unload();
	_goMgr->FreeEverything();
	engine.CleanUp();
}
