/************************************************************************************

	AstroMenace
	Hardcore 3D space scroll-shooter with spaceship upgrade possibilities.
	Copyright (c) 2006-2018 Mikhail Kurinnoi, Viewizard


	AstroMenace is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	AstroMenace is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with AstroMenace. If not, see <https://www.gnu.org/licenses/>.


	Web Site: https://www.viewizard.com/
	Project: https://github.com/viewizard/astromenace
	E-mail: viewizard@viewizard.com

*************************************************************************************/

#include "../game.h"
#include "../config/config.h"
#include "../gfx/shadow_map.h"
#include "../object3d/weapon/weapon.h"
#include "../object3d/space_ship/earth_space_fighter/earth_space_fighter.h"


//------------------------------------------------------------------------------------
// переменные
//------------------------------------------------------------------------------------
cEarthSpaceFighter *WorkshopFighterGame = nullptr;
cEarthSpaceFighter *WorkshopNewFighter = nullptr;
cWeapon *WorkshopNewWeapon = nullptr;
int	CurrentWorkshopNewFighter = 1;
int	CurrentWorkshopNewWeapon = 1;


sVECTOR3D WorkShopPointCamera;
void WorkshopDrawShip(cEarthSpaceFighter *WorkshopFighter, int Mode);
void WorkshopDrawWeapon(cWeapon *Weapon);
int CurrentWorkshop = 3;


// небольшая девиация оружия в слотах
float CurentDeviation = 0.0f;
float NeedDeviation = vw_Randf0*5.0f;
float CurentDeviationSum = 0.0f;
float CurentTime = 0.0f;

// для индикации нужных надписей
float CurrentAlert2 = 1.0f;
float CurrentAlert3 = 1.0f;

// можем рисовать
bool CanDrawWorkshop = false;


void Workshop_Shipyard();
void Workshop_Workshop();
void Workshop_Weaponry();


extern bool DragWeapon;
extern int DragWeaponNum;
extern int DragWeaponLevel;
extern int WeaponSetupSlot;



//------------------------------------------------------------------------------------
// создание корабля игрока, в меню (не при покупке нового корпуса!)
//------------------------------------------------------------------------------------
void WorkshopCreateShip(int Num)
{
	// создаем объект
	if (WorkshopFighterGame != nullptr) {
		delete WorkshopFighterGame;
		WorkshopFighterGame = nullptr;
	}

	int TMPGameEnemyArmorPenalty = GameEnemyArmorPenalty;
	GameEnemyArmorPenalty = 1;

	WorkshopFighterGame = new cEarthSpaceFighter;
	WorkshopFighterGame->Create(GameConfig().Profile[CurrentProfile].Ship);
	WorkshopFighterGame->DeviationOn = true;

	WorkshopFighterGame->ObjectStatus = eObjectStatus::none;
	WorkshopFighterGame->EngineDestroyType = true;
	WorkshopFighterGame->ShowStrength = false;

	WorkshopFighterGame->StrengthStart *= GameConfig().Profile[CurrentProfile].ShipHullUpgrade;
	WorkshopFighterGame->Strength = GameConfig().Profile[CurrentProfile].ShipHullCurrentStrength;


	// создаем оружие
	for (int i = 0; i < WorkshopFighterGame->WeaponQuantity; i++) {
		if (GameConfig().Profile[CurrentProfile].Weapon[i] &&
		    SetEarthSpaceFighterWeapon(WorkshopFighterGame, i + 1, GameConfig().Profile[CurrentProfile].Weapon[i])) {
			// убираем источник света
			if (auto sharedFire = WorkshopFighterGame->Weapon[i]->Fire.lock())
				vw_ReleaseLight(sharedFire->Light);

			WorkshopFighterGame->Weapon[i]->Ammo = GameConfig().Profile[CurrentProfile].WeaponAmmo[i];
			WorkshopFighterGame->WeaponYAngle[i] = -GameConfig().Profile[CurrentProfile].WeaponSlotYAngle[i];

			sVECTOR3D NeedAngle = WorkshopFighterGame->Rotation;
			NeedAngle.y += WorkshopFighterGame->WeaponYAngle[i];
			WorkshopFighterGame->Weapon[i]->SetRotation(NeedAngle);
		}
	}


	// создаем системы (визуальные)
	SetEarthSpaceFighterEngine(WorkshopFighterGame, GameConfig().Profile[CurrentProfile].EngineSystem);
	if (GameConfig().Profile[CurrentProfile].AdvancedProtectionSystem == 2)
		SetEarthSpaceFighterArmour(WorkshopFighterGame, 7);
	else
		SetEarthSpaceFighterArmour(WorkshopFighterGame, GameConfig().Profile[CurrentProfile].ShipHullUpgrade-1);

	GameEnemyArmorPenalty = TMPGameEnemyArmorPenalty;
	WorkshopFighterGame->SetLocation(sVECTOR3D(1000,-1000-(WorkshopFighterGame->Height/2.0f + WorkshopFighterGame->AABB[6].y), -(WorkshopFighterGame->Length/2.0f + WorkshopFighterGame->AABB[6].z)));

	if (Num == 1)
		WorkshopFighterGame->SetRotation(sVECTOR3D(0.0f,150.0f,0.0f));
	if (Num == 2)
		WorkshopFighterGame->SetRotation(sVECTOR3D(0.0f,170.0f,0.0f));
	if (Num == 3)
		WorkshopFighterGame->SetRotation(sVECTOR3D(0.0f,180.0f,0.0f));
}






//------------------------------------------------------------------------------------
// создание корабля для покупки
//------------------------------------------------------------------------------------
void WorkshopCreateNewShip()
{
	// создаем объект
	if (WorkshopNewFighter != nullptr) {
		delete WorkshopNewFighter;
		WorkshopNewFighter = nullptr;
	}

	int TMPGameEnemyArmorPenalty = GameEnemyArmorPenalty;
	GameEnemyArmorPenalty = 1;

	WorkshopNewFighter = new cEarthSpaceFighter;
	WorkshopNewFighter->Create(CurrentWorkshopNewFighter);
	WorkshopNewFighter->DeviationOn = true;

	WorkshopNewFighter->ObjectStatus = eObjectStatus::none;
	WorkshopNewFighter->EngineDestroyType = true;

	GameEnemyArmorPenalty = TMPGameEnemyArmorPenalty;
	WorkshopNewFighter->SetLocation(sVECTOR3D(2000,-2000-(WorkshopNewFighter->Height/2.0f + WorkshopNewFighter->AABB[6].y), -(WorkshopNewFighter->Length/2.0f + WorkshopNewFighter->AABB[6].z)));

	WorkshopNewFighter->SetRotation(sVECTOR3D(0.0f,-45.0f,0.0f));
}




//------------------------------------------------------------------------------------
// создание пушки для покупки
//------------------------------------------------------------------------------------
void WorkshopCreateNewWeapon()
{
	// создаем объект
	if (WorkshopNewWeapon != nullptr) {
		delete WorkshopNewWeapon;
		WorkshopNewWeapon = nullptr;
	}

	int TMPGameEnemyArmorPenalty = GameEnemyArmorPenalty;
	GameEnemyArmorPenalty = 1;

	WorkshopNewWeapon = new cWeapon;
	WorkshopNewWeapon->Create(CurrentWorkshopNewWeapon);

	WorkshopNewWeapon->ObjectStatus = eObjectStatus::none;
	GameEnemyArmorPenalty = TMPGameEnemyArmorPenalty;

	sVECTOR3D Ptmp = sVECTOR3D(0,-(WorkshopNewWeapon->Height/2.0f + WorkshopNewWeapon->AABB[6].y), -(WorkshopNewWeapon->Length/2.0f + WorkshopNewWeapon->AABB[6].z)-0.5f);
	vw_RotatePoint(Ptmp, sVECTOR3D(0.0f, -45.0f, 0.0f));

	WorkshopNewWeapon->SetLocation(sVECTOR3D(3000+Ptmp.x, -3000+Ptmp.y, Ptmp.z));

	WorkshopNewWeapon->SetRotation(sVECTOR3D(0.0f,-45.0f,0.0f));

	// убираем источник света
	if (auto sharedFire = WorkshopNewWeapon->Fire.lock())
		vw_ReleaseLight(sharedFire->Light);
}







//------------------------------------------------------------------------------------
// общая процедура создания
//------------------------------------------------------------------------------------
void WorkshopCreate()
{
	// все установки в исходные
	CurrentWorkshopNewFighter = 1;
	CurrentWorkshopNewWeapon = 1;
	CurentDeviation = 0.0f;
	NeedDeviation = vw_Randf0*5.0f;
	CurentDeviationSum = 0.0f;
	CurentTime = 0.0f;
	CurrentAlert2 = 1.0f;
	CurrentAlert3 = 1.0f;

	// чтобы только при старте не повоторялись!
	if (CurrentWorkshopNewFighter == GameConfig().Profile[CurrentProfile].Ship)
		CurrentWorkshopNewFighter++;
	if (CurrentWorkshopNewFighter > 22)
		CurrentWorkshopNewFighter = 1;

	WorkshopCreateShip(CurrentWorkshop);
	WorkshopCreateNewShip();
	WorkshopCreateNewWeapon();

	// нужно для получение кнопки в веапон
	NeedCheck = 0;

	CanDrawWorkshop = true;
}









//------------------------------------------------------------------------------------
// удаление всего...
//------------------------------------------------------------------------------------
void WorkshopDestroyData()
{
	if (WorkshopFighterGame != nullptr) {
		delete WorkshopFighterGame;
		WorkshopFighterGame = nullptr;
	}
	if (WorkshopNewFighter != nullptr) {
		delete WorkshopNewFighter;
		WorkshopNewFighter = nullptr;
	}
	if (WorkshopNewWeapon != nullptr) {
		delete WorkshopNewWeapon;
		WorkshopNewWeapon = nullptr;
	}
}










//------------------------------------------------------------------------------------
// покупка-установка и ремонт систем корабля
//------------------------------------------------------------------------------------
void WorkshopMenu()
{

	// небольшое качение... девиация
	float TimeDelta = vw_GetTimeThread(0) - CurentTime;
	if (CurentTime==0.0f) {
		CurentTime = vw_GetTimeThread(0);
	} else {
		CurentTime = vw_GetTimeThread(0);
		float Sign = 1.0f;
		// нужно двигать
		if (NeedDeviation < 0.0f) Sign = -1.0f;
		if (Sign == 1.0f) {
			if (NeedDeviation < CurentDeviationSum) Sign = -1.0f;
		} else {
			if (NeedDeviation > CurentDeviationSum) Sign = 1.0f;
		}

		CurentDeviation = Sign*0.7f*TimeDelta;

		if (Sign == 1.0f) {
			if (NeedDeviation <= CurentDeviationSum+CurentDeviation) {
				CurentDeviation -= CurentDeviationSum+CurentDeviation-NeedDeviation;
				CurentDeviationSum += CurentDeviation;
				NeedDeviation = vw_Randf0*5.0f;
			} else CurentDeviationSum += CurentDeviation;
		} else {
			if (NeedDeviation >= CurentDeviationSum+CurentDeviation) {
				CurentDeviation += CurentDeviationSum+CurentDeviation-NeedDeviation;
				CurentDeviationSum += CurentDeviation;
				NeedDeviation = vw_Randf0*5.0f;
			} else CurentDeviationSum += CurentDeviation;
		}
	}



	// просчитываем индикацию
	CurrentAlert2 += 0.4f*TimeDelta;
	if (CurrentAlert2 > 1.0f) CurrentAlert2 = 0.3f;
	CurrentAlert3 -= 1.9f*TimeDelta;
	if (CurrentAlert3 < 0.1f) CurrentAlert3 = 1.0f;








	// прорисовка 3д части
	switch (CurrentWorkshop) {
// покупка - ремонт корабля
	case 1:
		Workshop_Shipyard();
		break;


// покупка - внутренних систем корабля
	case 2:
		Workshop_Workshop();
		break;

// покупка - оружия корабля
	case 3:
		Workshop_Weaponry();
		break;
	}






	// кнопки

	int X = GameConfig().InternalWidth / 2 - 482;
	int Y = 180+100*5;
	if (DrawButton128_2(X,Y, vw_GetText("BACK"), MenuContentTransp, false)) {
		ComBuffer = eCommand::SWITCH_TO_MISSION;
		CanDrawWorkshop = false;
		// ничего не тянем... только включили меню
		DragWeaponNum = 0;
		DragWeaponLevel = 0;
		DragWeapon = false;
		WeaponSetupSlot = -1;
	}



	X = GameConfig().InternalWidth / 2 - 320;
	bool Off = false;
	if (CurrentWorkshop == 1)
		Off = true;
	if (DrawButton200_2(X,Y, vw_GetText("Shipyard"), MenuContentTransp, Off)) {
		CurrentWorkshop = 1;
		// используем разные повороты объектов, нужно пересоздать объект
		WorkshopCreateShip(CurrentWorkshop);
		WorkshopCreateNewShip();
		// ничего не тянем... только включили меню
		DragWeaponNum = 0;
		DragWeaponLevel = 0;
		DragWeapon = false;
		WeaponSetupSlot = -1;

		if (GameConfig().NeedShowHint[1])
			SetCurrentDialogBox(eDialogBox::ShipyardTipsAndTricks);
	}


	X = GameConfig().InternalWidth / 2 - 100;
	Off = false;
	if (CurrentWorkshop == 2)
		Off = true;
	if (DrawButton200_2(X,Y, vw_GetText("Workshop"), MenuContentTransp, Off)) {
		CurrentWorkshop = 2;
		// используем разные повороты объектов, нужно пересоздать объект
		WorkshopCreateShip(CurrentWorkshop);
		// ничего не тянем... только включили меню
		DragWeaponNum = 0;
		DragWeaponLevel = 0;
		DragWeapon = false;
		WeaponSetupSlot = -1;

		if (GameConfig().NeedShowHint[2])
			SetCurrentDialogBox(eDialogBox::SystemsTipsAndTricks);
	}


	X = GameConfig().InternalWidth / 2 + 120;
	Off = false;
	if (CurrentWorkshop == 3)
		Off = true;
	if (DrawButton200_2(X,Y, vw_GetText("Weaponry"), MenuContentTransp, Off)) {
		CurrentWorkshop = 3;
		// используем разные повороты объектов, нужно пересоздать объект
		WorkshopCreateShip(CurrentWorkshop);
		WorkshopCreateNewWeapon();
		// ничего не тянем... только включили меню
		DragWeaponNum = 0;
		DragWeaponLevel = 0;
		DragWeapon = false;
		WeaponSetupSlot = -1;

		if (GameConfig().NeedShowHint[3])
			SetCurrentDialogBox(eDialogBox::WeaponryTipsAndTricks);
	}



	X = GameConfig().InternalWidth / 2 + 354;
	if (DrawButton128_2(X,Y, vw_GetText("START"), MenuContentTransp, false)) {
		if (GameConfig().NeedShowHint[4])
			SetCurrentDialogBox(eDialogBox::ShortkeyTipsAndTricks);
		else {
			MenuContentTransp = 0.98f; // небольшая "защелка" от быстрых двойных нажатий на кнопку
			// ничего не тянем... только включили меню
			DragWeaponNum = 0;
			DragWeaponLevel = 0;
			DragWeapon = false;
			WeaponSetupSlot = -1;
			//
			LastMenuOnOffUpdateTime = vw_GetTimeThread(0);
			ComBuffer = eCommand::TURN_OFF_WORKSHOP_MENU;
		}
	}
}










//------------------------------------------------------------------------------------
// Прорисовка 3д части
//------------------------------------------------------------------------------------
void WorkshopDrawShip(cEarthSpaceFighter *SpaceFighter, int Mode)
{
	if (!CanDrawWorkshop || (SpaceFighter == nullptr))
		return;

	float tmpViewportX, tmpViewportY, tmpViewportWidth, tmpViewportHeight;
	vw_GetViewport(&tmpViewportX, &tmpViewportY, &tmpViewportWidth, &tmpViewportHeight);

	if (Mode == 1) {
		WorkShopPointCamera = sVECTOR3D(0.0f, 4.0f, -32.0f);
		SpaceFighter->SetRotation(sVECTOR3D(0.0f, 0.0f, CurentDeviation));
		SpaceFighter->SetRotation(sVECTOR3D(0.0f,CurentDeviation/2.0f,0.0f));

		vw_SetViewport((GLint)((GameConfig().InternalWidth / 2 - 512) * (tmpViewportWidth / GameConfig().InternalWidth)),
			       0,
			       (GLsizei)(1024 * (tmpViewportWidth / GameConfig().InternalWidth)),
			       (GLsizei)(768 * (tmpViewportHeight / GameConfig().InternalHeight)));
		vw_ResizeScene(45.0f, 1024.0f/768.0f, 1.0f, 2000.0f);
		vw_Clear(RI_DEPTH_BUFFER);

		vw_LoadIdentity();
		vw_SetCameraLocation(sVECTOR3D(1000+WorkShopPointCamera.x,-1000+WorkShopPointCamera.y,WorkShopPointCamera.z));
		vw_SetCameraMoveAroundPoint(sVECTOR3D(1000,-1000-6.0f,0), 0.0f, sVECTOR3D(0.0f, 0.0f, 0.0f));
		vw_CameraLookAt();


		bool ShadowMap = false;

		if (GameConfig().ShadowMap > 0) {
			float EffectiveDistance = 20.0f;
			ShadowMap_StartRenderToFBO(sVECTOR3D(0,5,0), EffectiveDistance, EffectiveDistance*2);

			SpaceFighter->Draw(true);
			if (SpaceFighter->Weapon != nullptr)
				for (int i=0; i<SpaceFighter->WeaponQuantity; i++) {
					if (SpaceFighter->Weapon[i] != nullptr)
						SpaceFighter->Weapon[i]->Draw(true);
				}

			ShadowMap_EndRenderToFBO();
			ShadowMap = true;
			ShadowMap_StartFinalRender();
		}

		SpaceFighter->Draw(false, ShadowMap);
		if (SpaceFighter->Weapon != nullptr)
			for (int i=0; i<SpaceFighter->WeaponQuantity; i++) {
				if (SpaceFighter->Weapon[i] != nullptr)
					SpaceFighter->Weapon[i]->Draw(false, ShadowMap);
			}

		if (GameConfig().ShadowMap > 0)
			ShadowMap_EndFinalRender();

		// рисуем эффекты двигателей только для этой модели
		vw_DrawParticleSystems(SpaceFighter->Engines);

		vw_SetCameraLocation(sVECTOR3D(-50,30,-50));
		vw_SetViewport(tmpViewportX, tmpViewportY, tmpViewportWidth, tmpViewportHeight);
		vw_ResizeScene(45.0f, GameConfig().InternalWidth / GameConfig().InternalHeight, 1.0f, 2000.0f);
		return;
	}



	if (Mode == 4) {
		WorkShopPointCamera = sVECTOR3D(0.0f, 35.0f, -0.01f);
		SpaceFighter->SetRotation(sVECTOR3D(0.0f, 0.0f, CurentDeviation));
		vw_SetViewport((GLint)((GameConfig().InternalWidth / 2) * (tmpViewportWidth / GameConfig().InternalWidth)),
			       (GLint)(30 * (tmpViewportHeight / GameConfig().InternalHeight)),
			       (GLsizei)(512 * (tmpViewportWidth / GameConfig().InternalWidth)),
			       (GLsizei)(638 * (tmpViewportHeight / GameConfig().InternalHeight)));
		vw_ResizeScene(45.0f, 512.0f/608.0f, 1.0f, 2000.0f);
		vw_Clear(RI_DEPTH_BUFFER);
		vw_LoadIdentity();
		vw_SetCameraLocation(sVECTOR3D(1000+WorkShopPointCamera.x,-1000+WorkShopPointCamera.y,WorkShopPointCamera.z));
		vw_SetCameraMoveAroundPoint(sVECTOR3D(1000,-1000, 0.0f), 0.0f, sVECTOR3D(0.0f, 180.0f, 0.0f));
		vw_CameraLookAt();


		bool ShadowMap = false;

		if (GameConfig().ShadowMap > 0) {
			float EffectiveDistance = 20.0f;
			ShadowMap_StartRenderToFBO(sVECTOR3D(0,0,0), EffectiveDistance, EffectiveDistance*2);

			SpaceFighter->Draw(true);
			if (SpaceFighter->Weapon != nullptr)
				for (int i=0; i<SpaceFighter->WeaponQuantity; i++) {
					if (SpaceFighter->Weapon[i] != nullptr)
						SpaceFighter->Weapon[i]->Draw(true);
				}

			ShadowMap_EndRenderToFBO();
			ShadowMap = true;
			ShadowMap_StartFinalRender();
		}

		SpaceFighter->Draw(false, ShadowMap);
		if (SpaceFighter->Weapon != nullptr)
			for (int i=0; i<SpaceFighter->WeaponQuantity; i++) {
				if (SpaceFighter->Weapon[i] != nullptr)
					SpaceFighter->Weapon[i]->Draw(false, ShadowMap);
			}

		if (GameConfig().ShadowMap > 0)
			ShadowMap_EndFinalRender();

		// рисуем эффекты двигателей только для этой модели
		vw_DrawParticleSystems(SpaceFighter->Engines);

		vw_SetCameraLocation(sVECTOR3D(-50,30,-50));
		vw_SetViewport(tmpViewportX, tmpViewportY, tmpViewportWidth, tmpViewportHeight);
		vw_ResizeScene(45.0f, GameConfig().InternalWidth / GameConfig().InternalHeight, 1.0f, 2000.0f);
		return;
	}






	if (Mode == 3) {
		WorkShopPointCamera = sVECTOR3D(0.0f, 10.0f, -34.0f);
		SpaceFighter->SetRotation(sVECTOR3D(0.0f,CurentDeviation/2.0f,0.0f));
		vw_SetViewport((GLint)((GameConfig().InternalWidth / 2) * (tmpViewportWidth / GameConfig().InternalWidth)),
			       0,
			       (GLsizei)(512 * (tmpViewportWidth / GameConfig().InternalWidth)),
			       (GLsizei)(512 * (tmpViewportHeight / GameConfig().InternalHeight)));
		vw_ResizeScene(45.0f, 512.0f/512.0f, 1.0f, 2000.0f);
		vw_Clear(RI_DEPTH_BUFFER);
		vw_LoadIdentity();
		vw_SetCameraLocation(sVECTOR3D(1000+WorkShopPointCamera.x/1.2f,-1000+WorkShopPointCamera.y/1.2f,WorkShopPointCamera.z/1.2f));
		vw_SetCameraMoveAroundPoint(sVECTOR3D(1000,-1000-SpaceFighter->AABB[6].y-SpaceFighter->Height/3,0), 0.0f, sVECTOR3D(0.0f, 0.0f, 0.0f));
	}
	if (Mode == 2) {
		WorkShopPointCamera = sVECTOR3D(0.0f, 10.0f, -34.0f);
		sVECTOR3D PointCameraTMP = WorkShopPointCamera;
		vw_RotatePoint(PointCameraTMP, sVECTOR3D(0.0f, -90.0f, 0.0f));
		SpaceFighter->SetRotation(sVECTOR3D(0.0f,CurentDeviation/2.0f,0.0f));
		vw_SetViewport((GLint)((GameConfig().InternalWidth / 2 - 512) * (tmpViewportWidth / GameConfig().InternalWidth)),
			       0,
			       (GLsizei)(512 * (tmpViewportWidth / GameConfig().InternalWidth)),
			       (GLsizei)(512 * (tmpViewportHeight / GameConfig().InternalHeight)));
		vw_ResizeScene(45.0f, 512.0f/512.0f, 1.0f, 2000.0f);
		vw_Clear(RI_DEPTH_BUFFER);
		vw_LoadIdentity();
		vw_SetCameraLocation(sVECTOR3D(2000+PointCameraTMP.x/1.2f,-2000+PointCameraTMP.y/1.2f,PointCameraTMP.z/1.2f));
		vw_SetCameraMoveAroundPoint(sVECTOR3D(2000,-2000-SpaceFighter->AABB[6].y-SpaceFighter->Height/3,0), 0.0f, sVECTOR3D(0.0f, 170.0f, 0.0f));
	}
	vw_CameraLookAt();


	bool ShadowMap = false;

	if (GameConfig().ShadowMap > 0) {
		float EffectiveDistance = 20.0f;
		ShadowMap_StartRenderToFBO(sVECTOR3D(0,-2,0), EffectiveDistance, EffectiveDistance*2);

		SpaceFighter->Draw(true);
		if (SpaceFighter->Weapon != nullptr)
			for (int i=0; i<SpaceFighter->WeaponQuantity; i++) {
				if (SpaceFighter->Weapon[i] != nullptr)
					SpaceFighter->Weapon[i]->Draw(true);
			}

		ShadowMap_EndRenderToFBO();
		ShadowMap = true;
		ShadowMap_StartFinalRender();
	}

	SpaceFighter->Draw(false, ShadowMap);

	if (SpaceFighter->Weapon != nullptr)
		for (int i=0; i<SpaceFighter->WeaponQuantity; i++) {
			if (SpaceFighter->Weapon[i] != nullptr)
				SpaceFighter->Weapon[i]->Draw(false, ShadowMap);
		}

	if (GameConfig().ShadowMap > 0)
		ShadowMap_EndFinalRender();

	// рисуем эффекты двигателей только для этой модели
	vw_DrawParticleSystems(SpaceFighter->Engines);

	vw_SetCameraLocation(sVECTOR3D(-50,30,-50));
	vw_SetViewport(tmpViewportX, tmpViewportY, tmpViewportWidth, tmpViewportHeight);
	vw_ResizeScene(45.0f, GameConfig().InternalWidth / GameConfig().InternalHeight, 1.0f, 2000.0f);

}


void WorkshopDrawWeapon(cWeapon *Weapon)
{
	if (!CanDrawWorkshop || (Weapon == nullptr))
		return;

	float tmpViewportX, tmpViewportY, tmpViewportWidth, tmpViewportHeight;
	vw_GetViewport(&tmpViewportX, &tmpViewportY, &tmpViewportWidth, &tmpViewportHeight);

	// удаление относительно размеров оружия
	if (Weapon->InternalType == 18 || Weapon->InternalType == 19)
		WorkShopPointCamera = sVECTOR3D(0.0f, 1.25f, -5.0f);
	else
		WorkShopPointCamera = sVECTOR3D(0.0f, 1.0f, -4.0f);

	sVECTOR3D PointCameraTMP = WorkShopPointCamera;
	vw_RotatePoint(PointCameraTMP, sVECTOR3D(0.0f, -90.0f, 0.0f));

	Weapon->SetRotation(sVECTOR3D(0.0f,CurentDeviation/2.0f,0.0f));
	vw_SetViewport((GLint)((GameConfig().InternalWidth / 2 - 448) * (tmpViewportWidth / GameConfig().InternalWidth)),
		       (GLint)(105 * (tmpViewportHeight / GameConfig().InternalHeight)),
		       (GLsizei)(384 * (tmpViewportWidth / GameConfig().InternalWidth)),
		       (GLsizei)(350 * (tmpViewportHeight / GameConfig().InternalHeight)));
	vw_ResizeScene(45.0f, 384.0f/350.0f, 1.0f, 2000.0f);
	vw_Clear(RI_DEPTH_BUFFER);
	vw_LoadIdentity();
	vw_SetCameraLocation(sVECTOR3D(3000+PointCameraTMP.x,-3000+PointCameraTMP.y,PointCameraTMP.z));
	vw_SetCameraMoveAroundPoint(sVECTOR3D(3000,-3000,0), 0.0f, sVECTOR3D(0.0f, 170.0f, 0.0f));
	vw_CameraLookAt();

	Weapon->Draw(false);

	vw_SetCameraLocation(sVECTOR3D(-50,30,-50));
	vw_ResizeScene(45.0f, GameConfig().InternalWidth / GameConfig().InternalHeight, 1.0f, 2000.0f);
	vw_SetViewport(tmpViewportX, tmpViewportY, tmpViewportWidth, tmpViewportHeight);
}

