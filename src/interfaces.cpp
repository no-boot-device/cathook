/*
 * interfaces.cpp
 *
 *  Created on: Oct 3, 2016
 *      Author: nullifiedcat
 */

#include "common.h"
#include "sharedobj.h"
#include "copypasted/CSignature.h"
#include "sdk.h"

#include <unistd.h>

#include "beforecheaders.h"
#include <string>
#include <sstream>
#include "aftercheaders.h"

#include <steam/isteamclient.h>

//class ISteamFriends002;

ISteamClient* g_ISteamClient = nullptr;
ISteamFriends* g_ISteamFriends = nullptr;
IVEngineClient013* g_IEngine = nullptr;
vgui::ISurface* g_ISurface = nullptr;
vgui::IPanel* g_IPanel = nullptr;
IClientEntityList* g_IEntityList = nullptr;
ICenterPrint* g_ICenterPrint = nullptr;
ICvar* g_ICvar = nullptr;
IGameEventManager2* g_IEventManager2 = nullptr;
IBaseClientDLL* g_IBaseClient = nullptr;
IEngineTrace* g_ITrace = nullptr;
IVModelInfoClient* g_IModelInfo = nullptr;
IInputSystem* g_IInputSystem = nullptr;
CGlobalVarsBase* g_GlobalVars = nullptr;
IPrediction* g_IPrediction = nullptr;
IGameMovement* g_IGameMovement = nullptr;
IInput* g_IInput = nullptr;
IMatSystemSurface* g_IMatSystemSurface = nullptr;
ISteamUser* g_ISteamUser = nullptr;
IAchievementMgr* g_IAchievementMgr = nullptr;
ISteamUserStats* g_ISteamUserStats = nullptr;
IStudioRender* g_IStudioRender = nullptr;
IVDebugOverlay* g_IVDebugOverlay = nullptr;

template<typename T>
T* BruteforceInterface(std::string name, sharedobj::SharedObject* object, int start) {
	T* result = nullptr;
	std::stringstream stream;
	for (int i = start; i < 100; i++) {
		stream.str("");
		stream << name;
		int zeros = 0;
		if (i < 10) zeros = 2;
		else if (i < 100) zeros = 1;
		for (int j = 0; j < zeros; j++) stream << '0';
		stream << i;
		result = reinterpret_cast<T*>(object->CreateInterface(stream.str()));
		if (result) return result;
	}
	logging::Info("RIP Software: can't create interface %s!", name.c_str());
	exit(0);
	return nullptr;
}

void CreateInterfaces() {
	g_ICenterPrint = BruteforceInterface<ICenterPrint>("VCENTERPRINT", sharedobj::client);
	g_ICvar = BruteforceInterface<ICvar>("VEngineCvar", sharedobj::vstdlib);
	g_IEngine = BruteforceInterface<IVEngineClient013>("VEngineClient", sharedobj::engine);
	g_AppID = g_IEngine->GetAppID();
	g_IEntityList = BruteforceInterface<IClientEntityList>("VClientEntityList", sharedobj::client);
	g_IPanel = BruteforceInterface<vgui::IPanel>("VGUI_Panel", sharedobj::vgui2);
	g_ISteamClient = BruteforceInterface<ISteamClient>("SteamClient", sharedobj::steamclient, 17);
	g_ISurface = BruteforceInterface<vgui::ISurface>("VGUI_Surface", sharedobj::vguimatsurface);
	g_IEventManager2 = BruteforceInterface<IGameEventManager2>("GAMEEVENTSMANAGER", sharedobj::engine, 2);
	g_IBaseClient = BruteforceInterface<IBaseClientDLL>("VClient", sharedobj::client);
	g_ITrace = BruteforceInterface<IEngineTrace>("EngineTraceClient", sharedobj::engine);
	g_IModelInfo = BruteforceInterface<IVModelInfoClient>("VModelInfoClient", sharedobj::engine);
	g_IInputSystem = BruteforceInterface<IInputSystem>("InputSystemVersion", sharedobj::inputsystem);
	g_IStudioRender = BruteforceInterface<IStudioRender>("VStudioRender", sharedobj::studiorender);
	g_IVDebugOverlay = BruteforceInterface<IVDebugOverlay>("VDebugOverlay", sharedobj::engine);
	HSteamPipe sp = g_ISteamClient->CreateSteamPipe();
	HSteamUser su = g_ISteamClient->ConnectToGlobalUser(sp);
	g_ISteamFriends = g_ISteamClient->GetISteamFriends(su, sp, "SteamFriends002");
	g_GlobalVars = **(reinterpret_cast<CGlobalVarsBase***>((uintptr_t)11 + gSignatures.GetClientSignature("55 89 E5 83 EC ? 8B 45 08 8B 15 ? ? ? ? F3 0F 10")));
	g_IPrediction = BruteforceInterface<IPrediction>("VClientPrediction", sharedobj::client);
	g_IGameMovement = BruteforceInterface<IGameMovement>("GameMovement", sharedobj::client);
	if (TF2) g_IInput = **(reinterpret_cast<IInput***>((uintptr_t)1 + gSignatures.GetClientSignature("A1 ? ? ? ? C6 05 ? ? ? ? 01 8B 10 89 04 24 FF 92 B4 00 00 00 A1 ? ? ? ? 8B 10")));
	else if (TF2C) g_IInput = **(reinterpret_cast<IInput***>((uintptr_t)1 + gSignatures.GetClientSignature("A1 ? ? ? ? C6 05 ? ? ? ? 01 8B 10 89 04 24 FF 92 A8 00 00 00 A1 ? ? ? ? 8B 10")));
	else if (HL2DM)  g_IInput = **(reinterpret_cast<IInput***>((uintptr_t)1 + gSignatures.GetClientSignature("A1 ? ? ? ? 8B 10 89 04 24 FF 52 78 A1 ? ? ? ? 8B 10")));
	if (TF2 || HL2DM)
		g_IMatSystemSurface = **reinterpret_cast<IMatSystemSurface***>((uintptr_t)19 + gSignatures.GetClientSignature("FF 92 94 02 00 00 8B 8D C4 FE FF FF 89 85 B0 FE FF FF A1 ? ? ? ? 8B 10 89 4C 24 0C"));
	else if (TF2C)
		g_IMatSystemSurface = **reinterpret_cast<IMatSystemSurface***>((uintptr_t)53 + gSignatures.GetClientSignature("C7 44 24 1C FF 00 00 00 C7 44 24 18 FF 00 00 00 C7 44 24 14 FF 00 00 00 C7 44 24 10 00 00 00 00 89 74 24 08 8B 83 D0 E1 00 00 89 95 A4 FE FF FF 89 44 24 04 A1 ? ? ? ? 89 04 24 FF 91 88 02 00 00 8B 95 A4 FE FF FF A1 ? ? ? ? 29 FA 8B 08 89 54 24 0C"));
	g_ISteamUser = g_ISteamClient->GetISteamUser(su, sp, "SteamUser018");
	g_IAchievementMgr = g_IEngine->GetAchievementMgr();
	g_ISteamUserStats = g_ISteamClient->GetISteamUserStats(su, sp, "STEAMUSERSTATS_INTERFACE_VERSION011");
}
