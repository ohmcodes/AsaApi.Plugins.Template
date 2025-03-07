#include "API/ARK/Ark.h"

#include "PluginTemplate.h"

#include "Utils.h"

#include "RepairItems.h"

#include "Hooks.h"

#include "Timers.h"

#include "Commands.h"

#include "Reload.h"


#pragma comment(lib, "AsaApi.lib")



void OnServerReady()
{
	Log::GetLog()->info("PluginTemplate Initialized");

	ReadConfig();
	LoadDatabase();
	AddOrRemoveCommands();
	AddReloadCommands();
	SetTimers();
	SetHooks();
}

DECLARE_HOOK(AShooterGameMode_BeginPlay, void, AShooterGameMode*);
void Hook_AShooterGameMode_BeginPlay(AShooterGameMode* _this)
{
	AShooterGameMode_BeginPlay_original(_this);

	OnServerReady();
}

extern "C" __declspec(dllexport) void Plugin_Init()
{
	Log::Get().Init(PROJECT_NAME);

	AsaApi::GetHooks().SetHook("AShooterGameMode.BeginPlay()", Hook_AShooterGameMode_BeginPlay, &AShooterGameMode_BeginPlay_original);

	if (AsaApi::GetApiUtils().GetStatus() == AsaApi::ServerStatus::Ready)
		OnServerReady();
}

extern "C" __declspec(dllexport) void Plugin_Unload()
{
	AsaApi::GetHooks().DisableHook("AShooterGameMode.BeginPlay()", Hook_AShooterGameMode_BeginPlay);

	AddOrRemoveCommands(false);
	AddReloadCommands(false);
	SetTimers(false);
	SetHooks(false);
}