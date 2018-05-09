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
	along with AstroMenace. If not, see <http://www.gnu.org/licenses/>.


	Web Site: http://www.viewizard.com/
	Project: https://github.com/viewizard/astromenace
	E-mail: viewizard@viewizard.com

*************************************************************************************/

// TODO translate comments
// TODO revise code in order to remove 'goto' statement
// TODO revise code in order to remove new/delete usage.

// NOTE in future, use make_unique() to make unique_ptr-s (since C++14)

#include "../game.h"
#include "config.h"

namespace {

sGameConfig Config;

} // unnamed namespace


/*
 * Get game configuration for read only.
 */
const sGameConfig &GameConfig()
{
	return Config;
}

/*
 * Get game configuration for read and write.
 */
sGameConfig &ChangeGameConfig()
{
	return Config;
}

/*
 * Save game configuration file.
 */
void SaveXMLConfigFile()
{
	std::unique_ptr<cXMLDocument> XMLdoc{new cXMLDocument};

	sXMLEntry *RootXMLEntry = XMLdoc->CreateRootEntry("AstroMenaceSettings");

	if (!RootXMLEntry) {
		std::cerr << __func__ << "(): " << "Can't create XML root element.\n";
		return;
	}

	XMLdoc->AddComment(*RootXMLEntry, " AstroMenace game Settings ");

	XMLdoc->AddEntryAttribute(XMLdoc->AddEntry(*RootXMLEntry, "MenuLanguage"), "value", vw_GetText("en", Config.MenuLanguage));
	XMLdoc->AddEntryAttribute(XMLdoc->AddEntry(*RootXMLEntry, "VoiceLanguage"), "value", vw_GetText("en", Config.VoiceLanguage));
	XMLdoc->AddEntryAttribute(XMLdoc->AddEntry(*RootXMLEntry, "FontNumber"), "value", Config.FontNumber);

	XMLdoc->AddEntryAttribute(XMLdoc->AddEntry(*RootXMLEntry, "Width"), "value", Config.Width);
	XMLdoc->AddEntryAttribute(XMLdoc->AddEntry(*RootXMLEntry, "Height"), "value", Config.Height);
	XMLdoc->AddComment(*RootXMLEntry, " Window (BPP = 0) or Full Screen (BPP = 16, 24 or 32) ");
	XMLdoc->AddEntryAttribute(XMLdoc->AddEntry(*RootXMLEntry, "BPP"), "value", Config.BPP);
	XMLdoc->AddComment(*RootXMLEntry, " Aspect Ratio must be 4:3 or 16:10 ");
	if (Config.InternalWidth == 1024.0f)
		XMLdoc->AddEntryAttribute(XMLdoc->AddEntry(*RootXMLEntry, "AspectRatio"), "value", "4:3");
	else
		XMLdoc->AddEntryAttribute(XMLdoc->AddEntry(*RootXMLEntry, "AspectRatio"), "value", "16:10");
	XMLdoc->AddEntryAttribute(XMLdoc->AddEntry(*RootXMLEntry, "CameraModeWithStandardAspectRatio"), "value", Config.CameraModeWithStandardAspectRatio);

	XMLdoc->AddComment(*RootXMLEntry, " Common settings ");
	XMLdoc->AddEntryAttribute(XMLdoc->AddEntry(*RootXMLEntry, "MSAA"), "value", Config.MSAA);
	XMLdoc->AddEntryAttribute(XMLdoc->AddEntry(*RootXMLEntry, "CSAA"), "value", Config.CSAA);
	XMLdoc->AddEntryAttribute(XMLdoc->AddEntry(*RootXMLEntry, "VisualEffectsQuality"), "value", Config.VisualEffectsQuality);
	XMLdoc->AddEntryAttribute(XMLdoc->AddEntry(*RootXMLEntry, "AnisotropyLevel"), "value", Config.AnisotropyLevel);
	XMLdoc->AddEntryAttribute(XMLdoc->AddEntry(*RootXMLEntry, "TexturesCompressionType"), "value", Config.TexturesCompressionType);
	XMLdoc->AddEntryAttribute(XMLdoc->AddEntry(*RootXMLEntry, "UseGLSL120"), "value", Config.UseGLSL120);
	XMLdoc->AddEntryAttribute(XMLdoc->AddEntry(*RootXMLEntry, "ShadowMap"), "value", Config.ShadowMap);
	XMLdoc->AddEntryAttribute(XMLdoc->AddEntry(*RootXMLEntry, "MaxPointLights"), "value", Config.MaxPointLights);
	XMLdoc->AddEntryAttribute(XMLdoc->AddEntry(*RootXMLEntry, "MusicVolume"), "value", Config.MusicVolume);
	XMLdoc->AddEntryAttribute(XMLdoc->AddEntry(*RootXMLEntry, "SoundVolume"), "value", Config.SoundVolume);
	XMLdoc->AddEntryAttribute(XMLdoc->AddEntry(*RootXMLEntry, "VoiceVolume"), "value", Config.VoiceVolume);
	XMLdoc->AddEntryAttribute(XMLdoc->AddEntry(*RootXMLEntry, "VSync"), "value", Config.VSync);
	XMLdoc->AddEntryAttribute(XMLdoc->AddEntry(*RootXMLEntry, "Brightness"), "value", Config.Brightness);
	XMLdoc->AddEntryAttribute(XMLdoc->AddEntry(*RootXMLEntry, "ShowFPS"), "value", Config.ShowFPS);
	XMLdoc->AddEntryAttribute(XMLdoc->AddEntry(*RootXMLEntry, "GameWeaponInfoType"), "value", Config.GameWeaponInfoType);
	XMLdoc->AddEntryAttribute(XMLdoc->AddEntry(*RootXMLEntry, "GameSpeed"), "value", Config.GameSpeed);

	XMLdoc->AddComment(*RootXMLEntry, " Control settings ");
	XMLdoc->AddEntryAttribute(XMLdoc->AddEntry(*RootXMLEntry, "KeyBoardLeft"), "value", SDL_GetKeyName(Config.KeyBoardLeft));
	XMLdoc->AddEntryAttribute(XMLdoc->AddEntry(*RootXMLEntry, "KeyBoardRight"), "value", SDL_GetKeyName(Config.KeyBoardRight));
	XMLdoc->AddEntryAttribute(XMLdoc->AddEntry(*RootXMLEntry, "KeyBoardUp"), "value", SDL_GetKeyName(Config.KeyBoardUp));
	XMLdoc->AddEntryAttribute(XMLdoc->AddEntry(*RootXMLEntry, "KeyBoardDown"), "value", SDL_GetKeyName(Config.KeyBoardDown));
	XMLdoc->AddEntryAttribute(XMLdoc->AddEntry(*RootXMLEntry, "KeyBoardPrimary"), "value", SDL_GetKeyName(Config.KeyBoardPrimary));
	XMLdoc->AddEntryAttribute(XMLdoc->AddEntry(*RootXMLEntry, "KeyBoardSecondary"), "value", SDL_GetKeyName(Config.KeyBoardSecondary));
	XMLdoc->AddEntryAttribute(XMLdoc->AddEntry(*RootXMLEntry, "MousePrimary"), "value", Config.MousePrimary);
	XMLdoc->AddEntryAttribute(XMLdoc->AddEntry(*RootXMLEntry, "MouseSecondary"), "value", Config.MouseSecondary);
	XMLdoc->AddEntryAttribute(XMLdoc->AddEntry(*RootXMLEntry, "JoystickPrimary"), "value", Config.JoystickPrimary);
	XMLdoc->AddEntryAttribute(XMLdoc->AddEntry(*RootXMLEntry, "JoystickSecondary"), "value", Config.JoystickSecondary);
	XMLdoc->AddEntryAttribute(XMLdoc->AddEntry(*RootXMLEntry, "JoystickNum"), "value", Config.JoystickNum);
	XMLdoc->AddEntryAttribute(XMLdoc->AddEntry(*RootXMLEntry, "JoystickDeadZone"), "value", Config.JoystickDeadZone);
	XMLdoc->AddEntryAttribute(XMLdoc->AddEntry(*RootXMLEntry, "ControlSensivity"), "value", Config.ControlSensivity);
	XMLdoc->AddEntryAttribute(XMLdoc->AddEntry(*RootXMLEntry, "MouseControl"), "value", Config.MouseControl);
	XMLdoc->AddEntryAttribute(XMLdoc->AddEntry(*RootXMLEntry, "LastProfile"), "value", Config.LastProfile);
	XMLdoc->AddEntryAttribute(XMLdoc->AddEntry(*RootXMLEntry, "MenuScript"), "value", Config.MenuScript);

	for(int i = 0; i < 10; i++) {
		std::string tmpString{"HintStatus" + std::to_string(i + 1)};
		XMLdoc->AddEntryAttribute(XMLdoc->AddEntry(*RootXMLEntry, tmpString.c_str()), "value", Config.NeedShowHint[i]);
	}

	//
	// всегда последние
	//

	// упаковка таблицы рекордов
	int TopScoresDataSize = sizeof(sTopScores) * 10;
	unsigned char *TopScoresData = new unsigned char[TopScoresDataSize];
	memcpy(TopScoresData, Config.TopScores, TopScoresDataSize);

	unsigned char *TopScoresDataXORCode = new unsigned char[TopScoresDataSize * 3];
	char *TopScoresResultString = new char[TopScoresDataSize * 4 + 1];

	// XOR
	for (int i=0; i < TopScoresDataSize; i++) {
		int k1 = i;
		int k2 = TopScoresDataSize + i * 2;
		TopScoresDataXORCode[k1] = 97 + (unsigned char)vw_iRandNum(25);
		TopScoresDataXORCode[k2] = 0;
		TopScoresDataXORCode[k2 + 1] = TopScoresData[i]^TopScoresDataXORCode[k1];
		// в первую - десятки, во вторую - еденицы
		TopScoresDataXORCode[k2] = 97 + (unsigned char)(TopScoresDataXORCode[k2 + 1] / 10.0f);
		TopScoresDataXORCode[k2 + 1] = 97 + (TopScoresDataXORCode[k2 + 1] - (TopScoresDataXORCode[k2] - 97) * 10);
	}

	// чтобы разбить на блоки вставляем пробел
	// тогда красиво отображаться будет (если врапинг выставлен в редакторе)
	int k = 0;
	int j = 0;
	for (int i = 0; i < (TopScoresDataSize * 3); i++) {
		TopScoresResultString[k] = TopScoresDataXORCode[i];
		k++;
		j++;
		if (j >= 125) {
			TopScoresResultString[k] = 0x20;
			k++;
			j=0;
		}
	}
	TopScoresResultString[k] = 0;


	XMLdoc->AddEntryContent(XMLdoc->AddEntry(*RootXMLEntry, "TopScores"), TopScoresResultString);

	if (TopScoresResultString != nullptr)
		delete [] TopScoresResultString;
	if (TopScoresData != nullptr)
		delete [] TopScoresData;
	if (TopScoresDataXORCode != nullptr)
		delete [] TopScoresDataXORCode;

	// упаковка профайлов

	int ProfileDataSize = sizeof(sPilotProfile) * 5;
	unsigned char *ProfileData = new unsigned char[ProfileDataSize];
	memcpy(ProfileData, Config.Profile, ProfileDataSize);

	unsigned char *ProfileDataXORCode = new unsigned char[ProfileDataSize * 3];
	char *ResultString = new char[ProfileDataSize * 4 + 1];

	// XOR
	for (int i=0; i < ProfileDataSize; i++) {
		int k1 = i;
		int k2 = ProfileDataSize + i * 2;
		ProfileDataXORCode[k1] = 97 + (unsigned char)vw_iRandNum(25);
		ProfileDataXORCode[k2] = 0;
		ProfileDataXORCode[k2 + 1] = ProfileData[i] ^ ProfileDataXORCode[k1];
		// в первую - десятки, во вторую - еденицы
		ProfileDataXORCode[k2] = 97 + (unsigned char)(ProfileDataXORCode[k2 + 1] / 10.0f);
		ProfileDataXORCode[k2 + 1] = 97 + (ProfileDataXORCode[k2 + 1] - (ProfileDataXORCode[k2] - 97) * 10);
	}

	// чтобы разбить на блоки вставляем пробел
	// тогда красиво отображаться будет (если врапинг выставлен в редакторе)
	k = 0;
	j = 0;
	for (int i = 0; i < ProfileDataSize * 3; i++) {
		ResultString[k] = ProfileDataXORCode[i];
		k++;
		j++;
		if (j >= 125) {
			ResultString[k] = 0x20;
			k++;
			j=0;
		}
	}
	ResultString[k] = 0;

	XMLdoc->AddEntryContent(XMLdoc->AddEntry(*RootXMLEntry, "PilotsProfiles"), ResultString);

	delete [] ResultString;
	if (ProfileData != nullptr)
		delete [] ProfileData;
	if (ProfileDataXORCode != nullptr)
		delete [] ProfileDataXORCode;

	XMLdoc->Save(ConfigFileName);
}

/*
 * Load game configuration file.
 */
bool LoadXMLConfigFile(bool NeedSafeMode)
{
	std::unique_ptr<cXMLDocument> XMLdoc{new cXMLDocument(ConfigFileName)};

	// читаем данные
	if (!XMLdoc->GetRootEntry()) {
		SaveXMLConfigFile();
		return true;
	}

	// берем первый элемент в скрипте
	sXMLEntry *RootXMLEntry = XMLdoc->GetRootEntry();

	// дополнительная проверка на содержимое конфигурационного файла
	if (!RootXMLEntry) {
		std::cerr << __func__ << "(): " << "Game configuration file corrupted: " << ConfigFileName << "\n";
		// сохранить дефолтные настройки, перезаписав файл
		SaveXMLConfigFile();
		// и сказать игре что это "первый запуск"
		return true;
	}
	if ("AstroMenaceSettings" != RootXMLEntry->Name) {
		std::cerr << __func__ << "(): " << "Game configuration file corrupted: " << ConfigFileName << "\n";
		// сохранить дефолтные настройки, перезаписав файл
		SaveXMLConfigFile();
		// и сказать игре что это "первый запуск"
		return true;
	}

	// если установлен NeedSafeMode, не грузим часть данных
	if (NeedSafeMode)
		goto LoadProfiles;

	if (XMLdoc->FindEntryByName(*RootXMLEntry, "MenuLanguage")) {
		std::string tmpMenuLanguage{};
		if (XMLdoc->GetEntryAttribute(*XMLdoc->FindEntryByName(*RootXMLEntry, "MenuLanguage"), "value", tmpMenuLanguage)) {
			for (unsigned int i = 0; i < vw_GetLanguageListCount(); i++) {
				if (tmpMenuLanguage == vw_GetText("en", i)) {
					Config.MenuLanguage = i;
					break;
				}
			}
		}
	}
	if (XMLdoc->FindEntryByName(*RootXMLEntry, "VoiceLanguage")) {
		std::string tmpVoiceLanguage{};
		if (XMLdoc->GetEntryAttribute(*XMLdoc->FindEntryByName(*RootXMLEntry, "VoiceLanguage"), "value", tmpVoiceLanguage)) {
			for (unsigned int i = 0; i < vw_GetLanguageListCount(); i++) {
				if (tmpVoiceLanguage == vw_GetText("en", i)) {
					Config.VoiceLanguage = i;
					break;
				}
			}
		}
	}
	if (XMLdoc->FindEntryByName(*RootXMLEntry, "FontNumber"))
		XMLdoc->iGetEntryAttribute(*XMLdoc->FindEntryByName(*RootXMLEntry, "FontNumber"), "value", Config.FontNumber);

	if (XMLdoc->FindEntryByName(*RootXMLEntry, "Width"))
		XMLdoc->iGetEntryAttribute(*XMLdoc->FindEntryByName(*RootXMLEntry, "Width"), "value", Config.Width);
	if (XMLdoc->FindEntryByName(*RootXMLEntry, "Height"))
		XMLdoc->iGetEntryAttribute(*XMLdoc->FindEntryByName(*RootXMLEntry, "Height"), "value", Config.Height);
	if (XMLdoc->FindEntryByName(*RootXMLEntry, "BPP"))
		XMLdoc->iGetEntryAttribute(*XMLdoc->FindEntryByName(*RootXMLEntry, "BPP"), "value", Config.BPP);

	if (XMLdoc->FindEntryByName(*RootXMLEntry, "AspectRatio")) {
		std::string tmpAspectRatio{};
		if (XMLdoc->GetEntryAttribute(*XMLdoc->FindEntryByName(*RootXMLEntry, "AspectRatio"), "value", tmpAspectRatio)) {
			if (tmpAspectRatio == "16:10") {
				Config.InternalWidth = 1228.0f;
				Config.InternalHeight = 768.0f;
			} else {
				Config.InternalWidth = 1024.0f;
				Config.InternalHeight = 768.0f;
			}
		}
	}
	if (XMLdoc->FindEntryByName(*RootXMLEntry, "CameraModeWithStandardAspectRatio"))
		XMLdoc->iGetEntryAttribute(*XMLdoc->FindEntryByName(*RootXMLEntry, "CameraModeWithStandardAspectRatio"), "value", Config.CameraModeWithStandardAspectRatio);

	if (XMLdoc->FindEntryByName(*RootXMLEntry, "MSAA"))
		XMLdoc->iGetEntryAttribute(*XMLdoc->FindEntryByName(*RootXMLEntry, "MSAA"), "value", Config.MSAA);
	if (XMLdoc->FindEntryByName(*RootXMLEntry, "CSAA"))
		XMLdoc->iGetEntryAttribute(*XMLdoc->FindEntryByName(*RootXMLEntry, "CSAA"), "value", Config.CSAA);
	if (XMLdoc->FindEntryByName(*RootXMLEntry, "VisualEffectsQuality"))
		XMLdoc->iGetEntryAttribute(*XMLdoc->FindEntryByName(*RootXMLEntry, "VisualEffectsQuality"), "value", Config.VisualEffectsQuality);
	if (XMLdoc->FindEntryByName(*RootXMLEntry, "AnisotropyLevel"))
		XMLdoc->iGetEntryAttribute(*XMLdoc->FindEntryByName(*RootXMLEntry, "AnisotropyLevel"), "value", Config.AnisotropyLevel);
	if (XMLdoc->FindEntryByName(*RootXMLEntry, "TexturesCompressionType"))
		XMLdoc->iGetEntryAttribute(*XMLdoc->FindEntryByName(*RootXMLEntry, "TexturesCompressionType"), "value", Config.TexturesCompressionType);
	if (XMLdoc->FindEntryByName(*RootXMLEntry, "UseGLSL120"))
		XMLdoc->bGetEntryAttribute(*XMLdoc->FindEntryByName(*RootXMLEntry, "UseGLSL120"), "value", Config.UseGLSL120);
	if (XMLdoc->FindEntryByName(*RootXMLEntry, "ShadowMap"))
		XMLdoc->iGetEntryAttribute(*XMLdoc->FindEntryByName(*RootXMLEntry, "ShadowMap"), "value", Config.ShadowMap);
	if (XMLdoc->FindEntryByName(*RootXMLEntry, "MaxPointLights"))
		XMLdoc->iGetEntryAttribute(*XMLdoc->FindEntryByName(*RootXMLEntry, "MaxPointLights"), "value", Config.MaxPointLights);
	if (XMLdoc->FindEntryByName(*RootXMLEntry, "MusicVolume"))
		XMLdoc->iGetEntryAttribute(*XMLdoc->FindEntryByName(*RootXMLEntry, "MusicVolume"), "value", Config.MusicVolume);
	if (XMLdoc->FindEntryByName(*RootXMLEntry, "SoundVolume"))
		XMLdoc->iGetEntryAttribute(*XMLdoc->FindEntryByName(*RootXMLEntry, "SoundVolume"), "value", Config.SoundVolume);
	if (XMLdoc->FindEntryByName(*RootXMLEntry, "VoiceVolume"))
		XMLdoc->iGetEntryAttribute(*XMLdoc->FindEntryByName(*RootXMLEntry, "VoiceVolume"), "value", Config.VoiceVolume);
	if (XMLdoc->FindEntryByName(*RootXMLEntry, "VSync"))
		XMLdoc->iGetEntryAttribute(*XMLdoc->FindEntryByName(*RootXMLEntry, "VSync"), "value", Config.VSync);
	if (XMLdoc->FindEntryByName(*RootXMLEntry, "Brightness"))
		XMLdoc->iGetEntryAttribute(*XMLdoc->FindEntryByName(*RootXMLEntry, "Brightness"), "value", Config.Brightness);
	if (XMLdoc->FindEntryByName(*RootXMLEntry, "ShowFPS"))
		XMLdoc->bGetEntryAttribute(*XMLdoc->FindEntryByName(*RootXMLEntry, "ShowFPS"), "value", Config.ShowFPS);
	if (XMLdoc->FindEntryByName(*RootXMLEntry, "GameWeaponInfoType"))
		XMLdoc->iGetEntryAttribute(*XMLdoc->FindEntryByName(*RootXMLEntry, "GameWeaponInfoType"), "value", Config.GameWeaponInfoType);
	if (XMLdoc->FindEntryByName(*RootXMLEntry, "GameSpeed"))
		XMLdoc->fGetEntryAttribute(*XMLdoc->FindEntryByName(*RootXMLEntry, "GameSpeed"), "value", Config.GameSpeed);

	if (XMLdoc->FindEntryByName(*RootXMLEntry, "KeyBoardLeft")) {
		std::string tmpKeyBoardLeft{};
		if (XMLdoc->GetEntryAttribute(*XMLdoc->FindEntryByName(*RootXMLEntry, "KeyBoardLeft"), "value", tmpKeyBoardLeft))
			Config.KeyBoardLeft = SDL_GetKeyFromName(tmpKeyBoardLeft.c_str());
	}
	if (XMLdoc->FindEntryByName(*RootXMLEntry, "KeyBoardRight")) {
		std::string tmpKeyBoardRight{};
		if (XMLdoc->GetEntryAttribute(*XMLdoc->FindEntryByName(*RootXMLEntry, "KeyBoardRight"), "value", tmpKeyBoardRight))
			Config.KeyBoardRight = SDL_GetKeyFromName(tmpKeyBoardRight.c_str());
	}
	if (XMLdoc->FindEntryByName(*RootXMLEntry, "KeyBoardUp")) {
		std::string tmpKeyBoardUp{};
		if (XMLdoc->GetEntryAttribute(*XMLdoc->FindEntryByName(*RootXMLEntry, "KeyBoardUp"), "value", tmpKeyBoardUp))
			Config.KeyBoardUp = SDL_GetKeyFromName(tmpKeyBoardUp.c_str());
	}
	if (XMLdoc->FindEntryByName(*RootXMLEntry, "KeyBoardDown")) {
		std::string tmpKeyBoardDown{};
		if (XMLdoc->GetEntryAttribute(*XMLdoc->FindEntryByName(*RootXMLEntry, "KeyBoardDown"), "value", tmpKeyBoardDown))
			Config.KeyBoardDown = SDL_GetKeyFromName(tmpKeyBoardDown.c_str());
	}
	if (XMLdoc->FindEntryByName(*RootXMLEntry, "KeyBoardPrimary")) {
		std::string tmpKeyBoardPrimary{};
		if (XMLdoc->GetEntryAttribute(*XMLdoc->FindEntryByName(*RootXMLEntry, "KeyBoardPrimary"), "value", tmpKeyBoardPrimary))
			Config.KeyBoardPrimary = SDL_GetKeyFromName(tmpKeyBoardPrimary.c_str());
	}
	if (XMLdoc->FindEntryByName(*RootXMLEntry, "KeyBoardSecondary")) {
		std::string tmpKeyBoardSecondary{};
		if (XMLdoc->GetEntryAttribute(*XMLdoc->FindEntryByName(*RootXMLEntry, "KeyBoardSecondary"), "value", tmpKeyBoardSecondary))
			Config.KeyBoardSecondary = SDL_GetKeyFromName(tmpKeyBoardSecondary.c_str());
	}

	if (XMLdoc->FindEntryByName(*RootXMLEntry, "MousePrimary"))
		XMLdoc->iGetEntryAttribute(*XMLdoc->FindEntryByName(*RootXMLEntry, "MousePrimary"), "value", Config.MousePrimary);
	if (XMLdoc->FindEntryByName(*RootXMLEntry, "MouseSecondary"))
		XMLdoc->iGetEntryAttribute(*XMLdoc->FindEntryByName(*RootXMLEntry, "MouseSecondary"), "value", Config.MouseSecondary);
	if (XMLdoc->FindEntryByName(*RootXMLEntry, "JoystickPrimary"))
		XMLdoc->iGetEntryAttribute(*XMLdoc->FindEntryByName(*RootXMLEntry, "JoystickPrimary"), "value", Config.JoystickPrimary);
	if (XMLdoc->FindEntryByName(*RootXMLEntry, "JoystickSecondary"))
		XMLdoc->iGetEntryAttribute(*XMLdoc->FindEntryByName(*RootXMLEntry, "JoystickSecondary"), "value", Config.JoystickSecondary);
	if (XMLdoc->FindEntryByName(*RootXMLEntry, "JoystickNum"))
		XMLdoc->iGetEntryAttribute(*XMLdoc->FindEntryByName(*RootXMLEntry, "JoystickNum"), "value", Config.JoystickNum);
	if (XMLdoc->FindEntryByName(*RootXMLEntry, "JoystickDeadZone"))
		XMLdoc->iGetEntryAttribute(*XMLdoc->FindEntryByName(*RootXMLEntry, "JoystickDeadZone"), "value", Config.JoystickDeadZone);
	if (XMLdoc->FindEntryByName(*RootXMLEntry, "ControlSensivity"))
		XMLdoc->iGetEntryAttribute(*XMLdoc->FindEntryByName(*RootXMLEntry, "ControlSensivity"), "value", Config.ControlSensivity);

	if (XMLdoc->FindEntryByName(*RootXMLEntry, "MouseControl"))
		XMLdoc->bGetEntryAttribute(*XMLdoc->FindEntryByName(*RootXMLEntry, "MouseControl"), "value", Config.MouseControl);

	if (XMLdoc->FindEntryByName(*RootXMLEntry, "LastProfile"))
		XMLdoc->iGetEntryAttribute(*XMLdoc->FindEntryByName(*RootXMLEntry, "LastProfile"), "value", Config.LastProfile);
	if (XMLdoc->FindEntryByName(*RootXMLEntry, "MenuScript"))
		XMLdoc->iGetEntryAttribute(*XMLdoc->FindEntryByName(*RootXMLEntry, "MenuScript"), "value", Config.MenuScript);

	for(int i=0; i<10; i++) {
		std::string tmpString{"HintStatus" + std::to_string(i + 1)};
		if (XMLdoc->FindEntryByName(*RootXMLEntry, tmpString.c_str()))
			 XMLdoc->bGetEntryAttribute(*XMLdoc->FindEntryByName(*RootXMLEntry, tmpString.c_str()), "value", Config.NeedShowHint[i]);
	}

	//
	// заполняем таблицу рекордов
	//

	if ((XMLdoc->FindEntryByName(*RootXMLEntry, "TopScores")) &&
	    !XMLdoc->FindEntryByName(*RootXMLEntry, "TopScores")->Content.empty()) {

		int TopScoresDataSize = XMLdoc->FindEntryByName(*RootXMLEntry, "TopScores")->Content.size();
		unsigned char *TopScoresData = new unsigned char[TopScoresDataSize+1];
		unsigned char *TopScoresDataXORCode = new unsigned char[TopScoresDataSize+1];
		char *TopScoresResultString = new char[TopScoresDataSize+1];

		strcpy(TopScoresResultString, XMLdoc->FindEntryByName(*RootXMLEntry, "TopScores")->Content.c_str());

		// первый цикл, восстанавливаем правильную последовательность, убираем все лишние элементы
		int k = 0;
		for (int i = 0; i < TopScoresDataSize; i++) {
			// берем только нужные символы
			if ((TopScoresResultString[i] >= 97) &&
			    (TopScoresResultString[i] <= 97 + 26)) {
				TopScoresDataXORCode[k] = TopScoresResultString[i];
				k++;
			}
		}
		// находим правильное значение, т.к. загружали использую длину строки (!!!)
		TopScoresDataSize = k / 3;

		// второй цикл, восстанавливаем последовательность структуры
		for (int i = 0; i< TopScoresDataSize; i++) {
			int k1 = i;
			int k2 = TopScoresDataSize + i * 2;

			unsigned char XORhash = TopScoresDataXORCode[k1];
			unsigned char XORdata = ((TopScoresDataXORCode[k2] - 97) * 10) + (TopScoresDataXORCode[k2 + 1] - 97);

			TopScoresData[i] = XORdata ^ XORhash;
		}

		// переносим данные в структуру
		memcpy(Config.TopScores, TopScoresData, TopScoresDataSize);

		if (TopScoresResultString)
			delete [] TopScoresResultString;
		if (TopScoresData)
			delete [] TopScoresData;
		if (TopScoresDataXORCode)
			delete [] TopScoresDataXORCode;
	}

LoadProfiles:

	//
	// загрузка профайлов пилотов
	//

	if (XMLdoc->FindEntryByName(*RootXMLEntry, "PilotsProfiles") &&
	    !XMLdoc->FindEntryByName(*RootXMLEntry, "PilotsProfiles")->Content.empty()) {
		int ProfileDataSize = XMLdoc->FindEntryByName(*RootXMLEntry, "PilotsProfiles")->Content.size();
		unsigned char *ProfileData = new unsigned char[ProfileDataSize+1];
		unsigned char *ProfileDataXORCode = new unsigned char[ProfileDataSize+1];
		char *ResultString = new char[ProfileDataSize + 1];

		strcpy(ResultString, XMLdoc->FindEntryByName(*RootXMLEntry, "PilotsProfiles")->Content.c_str());

		// первый цикл, восстанавливаем правильную последовательность, убираем все лишние элементы
		int k = 0;
		for (unsigned int i = 0; i < strlen(ResultString); i++) {
			// берем только нужные символы
			if ((ResultString[i] >= 97) &&
			    (ResultString[i] <= 97 + 26)) {
				ProfileDataXORCode[k] = ResultString[i];
				k++;
			}
		}
		// находим правильное значение, т.к. загружали используя длину строки (!!!)
		ProfileDataSize = k / 3;

		// второй цикл, восстанавливаем последовательность структуры
		for (int i = 0; i < ProfileDataSize; i++) {
			int k1 = i;
			int k2 = ProfileDataSize + i * 2;

			unsigned char XORhash = ProfileDataXORCode[k1];
			unsigned char XORdata = ((ProfileDataXORCode[k2] - 97) * 10) + (ProfileDataXORCode[k2 + 1] - 97);

			ProfileData[i] = XORdata^XORhash;
		}

		// переносим данные в структуру
		memcpy(Config.Profile, ProfileData, ProfileDataSize);

		if (ResultString != nullptr)
			delete [] ResultString;
		if (ProfileData != nullptr)
			delete [] ProfileData;
		if (ProfileDataXORCode != nullptr)
			delete [] ProfileDataXORCode;
	}

	//
	// делаем изменения, проверки и т.п.
	//

	if (Config.KeyBoardUp == 0)
		Config.KeyBoardUp = SDLK_UP;
	if (Config.KeyBoardDown == 0)
		Config.KeyBoardDown = SDLK_DOWN;
	if (Config.KeyBoardLeft == 0)
		Config.KeyBoardLeft = SDLK_LEFT;
	if (Config.KeyBoardRight == 0)
		Config.KeyBoardRight = SDLK_RIGHT;
	if (Config.KeyBoardPrimary == 0)
		Config.KeyBoardPrimary = SDLK_LCTRL;
	if (Config.KeyBoardSecondary == 0)
		Config.KeyBoardSecondary = SDLK_SPACE;
	if (Config.MousePrimary == 0)
		Config.MousePrimary = SDL_BUTTON_LEFT;
	if (Config.MouseSecondary == 0)
		Config.MouseSecondary = SDL_BUTTON_RIGHT;
	if (Config.JoystickPrimary == -1)
		Config.JoystickPrimary = 0;
	if (Config.JoystickSecondary == -1)
		Config.JoystickSecondary = 1;

	if ((Config.LastProfile >= 0) &&
	    (Config.LastProfile <= 4) &&
	    Config.Profile[Config.LastProfile].Used)
		CurrentProfile = Config.LastProfile;
	if (CurrentProfile != -1)
		CurrentMission = Config.Profile[Config.LastProfile].LastMission;

	if ((Config.FontNumber > (FontQuantity - 1)) || (Config.FontNumber < 0))
		Config.FontNumber = 0;
	if (Config.ControlSensivity > 10)
		Config.ControlSensivity = 10;
	if (Config.Brightness > 10)
		Config.Brightness = 10;
	if (Config.JoystickDeadZone > 10)
		Config.JoystickDeadZone = 10;

	return false;
}
