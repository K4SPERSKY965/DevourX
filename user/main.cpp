// Generated C++ file by Il2CppInspector - http://www.djkaty.com - https://github.com/djkaty
// Custom injected code entry point

#include "pch-il2cpp.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <il2cpp-init.h>
#include "il2cpp-appdata.h"

#include <iostream>
#include <filesystem>
#include <thread>
#include <VersionHelpers.h>
#include "helpers.h"
#include <Core.h>
#include <Utilities/Utilities.h>
#include <UnityResolver.hpp>
#include <Hooks/InitHooks.h>
#include "json/json.hpp"
#include "color/color.hpp"

using json = nlohmann::json;

// Set the name of your log file here
extern const LPCWSTR LOG_FILE = L"RiftLoader-DevourX.txt";

HMODULE hModule;
HANDLE hUnloadEvent;

void MonitorUnloadKey()
{
	while (!XGlobalUtility::bUnhook) {
		if (GetAsyncKeyState(VK_F12) & 0x8000) {
			XGlobalUtility::bUnhook = true;
			break;
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}


bool GameVersionCheck() {
	auto modulePath = RiftSystem::getModulePath(NULL);
	auto gameAssembly = modulePath.parent_path() / "GameAssembly.dll";

	if (!std::filesystem::exists(gameAssembly)) {
		std::cout << "[DevourX] GameAssembly.dll was not found" << "\n";
		MessageBox(NULL, L"Unable to locate GameAssembly.dll", L"DevourX", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
		return false;
	}

	return true;
}

DWORD __stdcall EjectThread(LPVOID lpParameter) {
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	DetourUninitialization();
	fclose(stdout);
	FreeConsole();
	CloseHandle(hUnloadEvent);
	FreeLibraryAndExitThread(hModule, 0);
}

// Custom injected code entry point
void Run(LPVOID lpParam)
{
	hModule = (HMODULE)lpParam;
	init_il2cpp();

	if (!GameVersionCheck()) {
		fclose(stdout);
		FreeConsole();
		FreeLibraryAndExitThread((HMODULE)lpParam, 0);
		return;
	}

	if (!IsWindows10OrGreater())
	{
		MessageBoxA(NULL, "You need at least Windows 10.", "RiftLoader::DevourX - Version Not Supported", MB_OK);
	}

	hUnloadEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	il2cpp_thread_attach(il2cpp_domain_get());

	// If you would like to output to a new console window, use il2cppi_new_console() to open one and redirect stdout
	il2cppi_new_console();
	SetConsoleTitleA("RiftLoader::DevourX");

	RiftConfig::configReader.setModVersion("3.0.4");

	static UnityAPI::UApplication uApp;
	static UnityAPI::UObject uMenu;

	std::string productName = uApp.ProductName();
	std::string gameVersion = uApp.GetVersion();
	std::string unityVersion = uApp.GetUnityVersion();

	std::cout << "\tProduct Name: " << dye::green(productName) << std::endl;
	std::cout << "\tGame Version: " << dye::green(gameVersion) << std::endl;
	std::cout << "\tUnity Version: " << dye::green(unityVersion) << "\n\n";

	std::string currentModVersion = RiftConfig::configReader.getModVersion();
	RiftConfig::configReader.setGameVersion(gameVersion);

	std::cout << "\tDevourX " << dye::aqua("v" + currentModVersion) << "\n";
	std::cout << "\t" << dye::grey(__TIME__) << "\n\n";

	std::cout << dye::red("\tDeveloped by Jadis0x\n\n");

	std::cout << dye::grey("[DevourX]: Github: https://github.com/jadis0x\n");
	std::cout << dye::grey("[DevourX]: Web Site: https://www.luridlane.com\n\n");

	if (!XUtility::IsSteamRunning()) {
		XUtility::WarnAndQuit("Steam is not running!");
		return;
	}

	app::String* steamName = nullptr;

	
	if (app::SteamFriends_GetPersonaName != nullptr) {
		steamName = app::SteamFriends_GetPersonaName(nullptr);

		if (steamName == nullptr) {
			std::cout << "Steam name retrieval failed, using default value." << std::endl;
			steamName = ConvertToSystemString("unknown-x-user");
		}
	}
	else {
		std::cout << "SteamFriends_GetPersonaName function is not available." << std::endl;
		steamName = ConvertToSystemString("unknown-x-user");
	}

	app::CSteamID steamUserID = app::SteamUser_GetSteamID(nullptr);


	std::cout << "[DevourX]: Logged in as " << steamName << " (" << dye::green(steamUserID.m_SteamID) << ")\n\n";
	std::cout << "[DevourX]: " << dye::grey("Initializing..\n");


	std::string scene = XUtility::SceneName();

	if (scene == std::string("Menu")) {
		MessageBoxA(
			NULL,
			"Be cautious when using the Object Pool menu. I’ve added it as a preview feature. A single wrong click could cause the game to crash.\n\n\nDiscord: Jadis0x",
			"Message From DevourX",
			MB_OK | MB_ICONWARNING
		);
	}


	DetourInitilization();

	std::thread UnloadKeyThread(MonitorUnloadKey);
	UnloadKeyThread.join();

	CreateThread(0, 0, EjectThread, 0, 0, 0);
}