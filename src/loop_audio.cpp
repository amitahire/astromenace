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

// TODO translate comments

#include "game.h"
#include "config/config.h"

namespace {

std::string CurrentPlayingMusicName;
std::string CurrentPlayingMusicLoopPart;

struct sSoundData {
	const char *FileName; // file name
	float VolumeCorrection; // sound volume correction
	bool AllowStop; // allow stop during vw_StopAllSoundsIfAllowed() call
};

// перечень имен файлов игровых звуков
static sSoundData GameSoundList[] = {

	{"sfx/weapon1probl.wav", 1.0f, true},		// оружие повреждено или нечем стрелять, механическое оружие (Kinetic)
	{"sfx/weapon2probl.wav", 1.0f, true},		// оружие повреждено или нечем стрелять, "слабое" энергетическое оружие (Ion, Plasma)
	{"sfx/weapon3probl.wav", 1.0f, true},		// оружие повреждено или нечем стрелять, "лучевое/среднее" энергетическое оружие (Maser, Laser)
	{"sfx/weapon4probl.wav", 1.0f, true},		// оружие повреждено или нечем стрелять, "мощьное" энергетическое оружие (Antimatter, Gauss)
	{"sfx/weapon5probl.wav", 1.0f, true},		// оружие повреждено или нечем стрелять, пусковые установки (ракеты, торпеды, бомбы)

	{"sfx/explosion1.wav", 1.0f, true},		// малый взрыв: ракеты
	{"sfx/explosion2.wav", 1.0f, true},		// взрыв (обычный): торпеды, истребители землян и пиратов
	{"sfx/explosion3.wav", 1.0f, true},		// взрыв (энергетический): ядерные бомбы, корабли пришельцев

	{"sfx/weaponfire1.wav", 0.7f, true},		// выстрел, Kinetic 1
	{"sfx/weaponfire2.wav", 0.65f, true},		// выстрел, Kinetic 2
	{"sfx/weaponfire3.wav", 0.7f, true},		// выстрел, Kinetic 3
	{"sfx/weaponfire4.wav", 1.0f, true},		// выстрел, Kinetic 4
	{"sfx/weaponfire5.wav", 1.0f, true},		// выстрел, Ion 1
	{"sfx/weaponfire6.wav", 1.0f, true},		// выстрел, Ion 2
	{"sfx/weaponfire7.wav", 0.7f, true},		// выстрел, Ion 3
	{"sfx/weaponfire8.wav", 0.85f, true},		// выстрел, Plasma 1
	{"sfx/weaponfire9.wav", 0.95f, true},		// выстрел, Plasma 2
	{"sfx/weaponfire10.wav", 0.9f, true},		// выстрел, Plasma 3
	{"sfx/weaponfire11.wav", 0.6f, true},		// выстрел, Maser 1 - продолжительный, пока есть луч; приоритет выше, чтобы не останавливали
	{"sfx/weaponfire12.wav", 0.55f, true},		// выстрел, Maser 2 - продолжительный, пока есть луч; приоритет выше, чтобы не останавливали
	{"sfx/weaponfire13.wav", 0.9f, true},		// выстрел, Antimatter
	{"sfx/weaponfire14.wav", 0.8f, true},		// выстрел, Laser - продолжительный, пока есть луч; приоритет выше, чтобы не останавливали
	{"sfx/weaponfire15.wav", 0.8f, true},		// выстрел, Gauss
	{"sfx/weaponfire16.wav", 1.0f, true},		// выстрел, одиночная ракета; приоритет выше, не красиво если снимаем "хвост" эффекта
	{"sfx/weaponfire17.wav", 1.0f, true},		// выстрел, одна ракета из группы; приоритет выше, не красиво если снимаем "хвост" эффекта
	{"sfx/weaponfire18.wav", 1.0f, true},		// выстрел, торпеда; приоритет выше, не красиво если снимаем "хвост" эффекта
	{"sfx/weaponfire19.wav", 1.0f, true},		// выстрел, бомба; приоритет выше, не красиво если снимаем "хвост" эффекта

	{"sfx/kinetichit.wav", 1.0f, true},		// попадание и "разваливание" снаряда о корпус объекта, для Kinetic снарядов
	{"sfx/ionhit.wav", 1.0f, true},			// попадание и "разваливание" снаряда о корпус объекта, для Ion снарядов
	{"sfx/plasmahit.wav", 1.0f, true},		// попадание и "разваливание" снаряда о корпус объекта, для Plasma снарядов
	{"sfx/antimaterhit.wav", 1.0f, true},		// попадание и "разваливание" снаряда о корпус объекта, для Antimatter снарядов
	{"sfx/gausshit.wav", 1.0f, true},		// попадание и "разваливание" снаряда о корпус объекта, для Gauss снарядов

	{"sfx/explosion4.wav", 1.0f, true},		// малый взрыв: астероиды
};
#define GameSoundQuantity sizeof(GameSoundList)/sizeof(GameSoundList[0])

// перечень имен файлов звуков для меню
static sSoundData MenuSoundNames[] = {
	{"sfx/menu_onbutton.wav", 0.4f, false},		// навели на кнопку
	{"sfx/menu_click.wav", 0.6f, false},		// нажали на кнопку
	{"sfx/menu_new.wav", 1.0f, true},		// меняем меню
	{"sfx/menu_taping.wav", 0.8f, true},		// набор текста-названия профайла
	{"sfx/menu_online.wav", 0.75f, true},		// стоим над профайлом-миссией
	{"sfx/menu_selectline.wav", 1.0f, true},	// выбрали профайл-миссию
	{"sfx/menu_nclick.wav", 1.0f, false},		// не можем нажать на кнопку, но жмем
	{"sfx/drag_error.wav", 1.0f, true},		// не можем установить в слот
	{"sfx/drag_offslot.wav", 0.65f, true},		// снимаем со слота оружие
	{"sfx/drag_onslot.wav", 0.82f, true},		// устанавливаем в слот оружие
	{"sfx/drag_release.wav", 0.6f, true},		// освобождаем курсор от оружия, если тянули
	{"sfx/game_showmenu.wav", 1.0f, false},		// проказываем меню в игре (звук во время его появления)
	{"sfx/game_hidemenu.wav", 1.0f, false},		// скрываем меню в игре (звук во время его исчезновения)
	{"sfx/lowlife.wav", 1.0f, true},		// сирена или что-то подобное, когда менее 10% жизни остается (зацикленный небольшой фрагмент)
	{"sfx/menu_onbutton2.wav", 0.15f, false},	// навели на малую кнопку
};
#define MenuSoundQuantity sizeof(MenuSoundNames)/sizeof(MenuSoundNames[0])

// перечень имен файлов звуков для меню
static sSoundData VoiceNames[] = {
	{"lang/en/voice/Attention.wav", 1.0f, true},
	{"lang/en/voice/EngineMalfunction.wav", 1.0f, true},
	{"lang/en/voice/MissileDetected.wav", 1.0f, true},//++
	{"lang/en/voice/PowerSupplyReestablished.wav", 1.0f, true},
	{"lang/en/voice/PrepareForAction.wav", 1.0f, true},//++
	{"lang/en/voice/ReactorMalfunction.wav", 1.0f, true},//++
	{"lang/en/voice/Warning.wav", 1.0f, true},//++
	{"lang/en/voice/WeaponDamaged.wav", 1.0f, true},//++
	{"lang/en/voice/WeaponDestroyed.wav", 1.0f, true},//++
	{"lang/en/voice/WeaponMalfunction.wav", 1.0f, true},//++
};
#define VoiceQuantity sizeof(VoiceNames)/sizeof(VoiceNames[0])

} // unnamed namespace


//------------------------------------------------------------------------------------
// Запуск нужной музыки
//------------------------------------------------------------------------------------
void StartMusicWithFade(eMusicTheme StartMusic, uint32_t FadeInTicks, uint32_t FadeOutTicks)
{
	CurrentPlayingMusicLoopPart.clear();
	float MusicCorrection = 1.0f;

	switch (StartMusic) {
	case eMusicTheme::NONE:
		CurrentPlayingMusicName.clear();
		break;
	case eMusicTheme::MENU:
		CurrentPlayingMusicName = "music/menu.ogg";
		break;
	case eMusicTheme::GAME:
		CurrentPlayingMusicName = "music/game.ogg";
		// during in-game mission restart we need "restart" music as well
		// so, if this music already playing - release it, and start it again (see code below)
		vw_ReleaseMusic(CurrentPlayingMusicName);
		break;
	case eMusicTheme::BOSS:
		CurrentPlayingMusicName = "music/boss-loop.ogg";
		break;
	case eMusicTheme::FAILED:
		CurrentPlayingMusicName = "music/missionfailed.ogg";
		MusicCorrection = 0.7f;
		break;
	case eMusicTheme::CREDITS:
		CurrentPlayingMusicName = "music/boss-intro.ogg";
		CurrentPlayingMusicLoopPart = "music/boss-loop.ogg";
		break;
	}

	if (vw_GetAudioStatus() && //если можно играть
	    GameConfig().MusicVolume && // и громкость не нулевая
	    !CurrentPlayingMusicName.empty()) {

		vw_FadeOutAllMusicWithException(CurrentPlayingMusicName, FadeOutTicks, 1.0f, FadeInTicks);

		if (vw_IsMusicPlaying(CurrentPlayingMusicName))
			return;

		// пытаемся загрузить и играть
		if (!vw_PlayMusic(CurrentPlayingMusicName, 0.0f, MusicCorrection * (GameConfig().MusicVolume / 10.0f),
				  CurrentPlayingMusicLoopPart.empty(), CurrentPlayingMusicLoopPart)) {
			vw_ReleaseMusic(CurrentPlayingMusicName);
			CurrentPlayingMusicName.clear();
		} else // we are playing new music theme, FadeIn it
			vw_SetMusicFadeIn(CurrentPlayingMusicName, 1.0f, FadeInTicks);
	}
}

// установка громкости на все 2д звуки
void Audio_SetSound2DGlobalVolume(float NewGlobalVolume)
{
	for (unsigned int i = 0; i < MenuSoundQuantity; i++) {
		vw_SetSoundGlobalVolume(MenuSoundNames[i].FileName, NewGlobalVolume);
	}
}

// установка громкости на все голосовые звуки
void Audio_SetVoiceGlobalVolume(float NewGlobalVolume)
{
	for (unsigned int i = 0; i < VoiceQuantity; i++) {
		vw_SetSoundGlobalVolume(vw_GetText(VoiceNames[i].FileName, GameConfig().VoiceLanguage), NewGlobalVolume);
	}
}

//------------------------------------------------------------------------------------
// Проигрываем звук в меню, или другие 2д звуки
//------------------------------------------------------------------------------------
unsigned int Audio_PlaySound2D(unsigned int SoundID, float LocalVolume)
{
	if (!vw_GetAudioStatus() ||
	    !GameConfig().SoundVolume ||
	    (SoundID > MenuSoundQuantity))
		return 0;

	// т.к. у нас со смещением же в 1 идет
	SoundID--;

	LocalVolume = LocalVolume * MenuSoundNames[SoundID].VolumeCorrection;

	// если это звук меню и мы его уже проигрываем, его надо перезапустить
	int ret = vw_ReplayFirstFoundSound(MenuSoundNames[SoundID].FileName);
	if (ret)
		return ret;

	// чтобы не было искажения по каналам, делаем установку относительно камеры...
	return vw_PlaySound(MenuSoundNames[SoundID].FileName,
			    LocalVolume, GameConfig().SoundVolume / 10.0f, 0.0f, 0.0f, 0.0f,
			    true, MenuSoundNames[SoundID].AllowStop, 1);
}

//------------------------------------------------------------------------------------
// Проигрываем голос
//------------------------------------------------------------------------------------
unsigned int Audio_PlayVoice(unsigned int VoiceID, float LocalVolume)
{
	if (!vw_GetAudioStatus() ||
	    !GameConfig().VoiceVolume ||
	    (VoiceID > VoiceQuantity))
		return 0;

	// т.к. у нас со смещением же в 1 идет
	VoiceID--;

	// FIXME should be connected to language code, not column number, that could be changed
	// русский голос делаем немного тише
	if (GameConfig().VoiceLanguage == 3)
		VoiceNames[VoiceID].VolumeCorrection = 0.6f;

	LocalVolume = LocalVolume * VoiceNames[VoiceID].VolumeCorrection;

	// чтобы не было искажения по каналам, делаем установку относительно камеры...
	return vw_PlaySound(vw_GetText(VoiceNames[VoiceID].FileName, GameConfig().VoiceLanguage),
			    LocalVolume, GameConfig().VoiceVolume / 10.0f, 0.0f, 0.0f, 0.0f,
			    true, VoiceNames[VoiceID].AllowStop, 1);
}

//------------------------------------------------------------------------------------
// Проигрываем 3д звуки
//------------------------------------------------------------------------------------
unsigned int Audio_PlaySound3D(int SoundID, float LocalVolume, sVECTOR3D Location, int AtType)
{
	if (!vw_GetAudioStatus() ||
	    !GameConfig().SoundVolume)
		return 0;

	LocalVolume = LocalVolume * GameSoundList[SoundID-1].VolumeCorrection;

	// т.к. у нас со смещением же в 1 идет
	SoundID--;

	return vw_PlaySound(GameSoundList[SoundID].FileName,
			    LocalVolume, GameConfig().SoundVolume / 10.0f, Location.x, Location.y, Location.z,
			    false, GameSoundList[SoundID].AllowStop, AtType);
}

//------------------------------------------------------------------------------------
// Цикл звука
//------------------------------------------------------------------------------------
void Audio_LoopProc()
{
	// делаем установку слушателя (он в точке камеры)
	// получаем текущее положение камеры
	sVECTOR3D CurrentCameraLocation;
	vw_GetCameraLocation(&CurrentCameraLocation);
	sVECTOR3D CurrentCameraRotation;
	vw_GetCameraRotation(&CurrentCameraRotation);

	sVECTOR3D ListenerOriV1(0.0f, 0.0f, -1.0f);
	vw_RotatePoint(ListenerOriV1, CurrentCameraRotation);
	sVECTOR3D ListenerOriV2(0.0f, 1.0f, 0.0f);
	vw_RotatePoint(ListenerOriV2, CurrentCameraRotation);

	// Position of the Listener.
	float ListenerPos[3] = {CurrentCameraLocation.x, CurrentCameraLocation.y, CurrentCameraLocation.z};
	// Velocity of the Listener.
	float ListenerVel[3] = {0.0f, 0.0f, 0.0f};
	// Orientation of the Listener. (first 3 elements are "at", second 3 are "up")
	// Also note that these should be units of '1'.
	float ListenerOri[6] = {ListenerOriV1.x, ListenerOriV1.y, ListenerOriV1.z,
				ListenerOriV2.x, ListenerOriV2.y, ListenerOriV2.z};

	vw_Listener(ListenerPos, ListenerVel, ListenerOri);

	// передаем управление, чтобы внутри ядра все сделали
	vw_UpdateSound(SDL_GetTicks());
	vw_UpdateMusic(SDL_GetTicks());

	// запускаем нужную музыку... только включили громкость или выключили
	if (!vw_IsAnyMusicPlaying()) {
		if (vw_GetAudioStatus() && // если можно вообще играть
		    GameConfig().MusicVolume && // если громкость не нулевая
		    !CurrentPlayingMusicName.empty()) { // если установлен
			// пытаемся загрузить и проиграть
			if (!vw_PlayMusic(CurrentPlayingMusicName, 1.0f, GameConfig().MusicVolume / 10.0f,
					  CurrentPlayingMusicLoopPart.empty(), CurrentPlayingMusicLoopPart)) {
				vw_ReleaseMusic(CurrentPlayingMusicName);
				CurrentPlayingMusicName.clear();
			} else // we are playing new music theme, FadeIn it
				vw_SetMusicFadeIn(CurrentPlayingMusicName, 1.0f, 2000);
		}
	} else {
		// если что-то играем, но звук уже выключен, нужно все убрать...
		if (vw_GetAudioStatus() && // играть можно
		    !GameConfig().MusicVolume) { // но громкость нулевая
			vw_ReleaseAllMusic();
		}
	}
}
