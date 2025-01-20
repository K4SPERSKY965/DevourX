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

extern const LPCWSTR LOG_FILE = L"RiftLoader-DevourX.txt";

HMODULE hModule;
HANDLE hUnloadEvent;

DWORD WINAPI MonitorUnload(LPVOID) {
	while (true) {
		if (GetAsyncKeyState(VK_F12) & 0x8000) { // Check if F12 is pressed
			SetEvent(hUnloadEvent); // Trigger unload event
			break;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
	return 0;
}

// Custom injected code entry point
void Run(LPVOID lpParam)
{
	hModule = (HMODULE)lpParam;
	auto& configReader = RiftConfig::configReader;

	// If you would like to output to a new console window, use il2cppi_new_console() to open one and redirect stdout
	il2cppi_new_console();
	SetConsoleTitleA("RiftSystems::DevourX");

	if (!RiftSystem::GameVersionCheck()) {
		RiftSystem::HandleError(lpParam);
		return;
	}

	if (!IsWindows10OrGreater())
	{
		MessageBoxA(NULL, "You need at least Windows 10.", "[RiftSystems]::DevourX - Version Not Supported", MB_OK);
		RiftSystem::HandleError(lpParam);
		return;
	}

	init_il2cpp();

	hUnloadEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (hUnloadEvent == NULL) {
		std::cout << "[RiftSystems]: " << dye::red("Failed to create unload event! Error code: ") << GetLastError() << std::endl;
		RiftSystem::HandleError(lpParam);
		return;
	}

	// Unload izleyicisini başlat
	std::thread unloadMonitorThread(MonitorUnload, nullptr);
	unloadMonitorThread.detach();

	il2cpp_thread_attach(il2cpp_domain_get());

	configReader.setModVersion("3.0.4");

	static UnityAPI::UApplication uApp;
	static UnityAPI::UObject uMenu;

	std::string productName = uApp.ProductName();
	std::string gameVersion = uApp.GetVersion();
	std::string unityVersion = uApp.GetUnityVersion();

	std::cout << "\tProduct Name: " << dye::green(productName) << std::endl;
	std::cout << "\tGame Version: " << dye::green(gameVersion) << std::endl;
	std::cout << "\tUnity Version: " << dye::green(unityVersion) << "\n\n";

	std::string currentModVersion = configReader.getModVersion();
	configReader.setGameVersion(gameVersion);

	std::cout << "\tDevourX " << dye::aqua("v" + currentModVersion) << "\n";
	std::cout << "\t" << dye::grey(__TIME__) << "\n\n";

	std::cout << dye::red("\tDeveloped by Jadis0x\n\n");

	std::cout << dye::grey("[DevourX]: Github: https://github.com/jadis0x\n");

	if (!XUtility::IsSteamRunning()) {
		XUtility::WarnAndQuit("Steam is not running!");
		return;
	}

	std::cout << "[DevourX]: " << dye::green("Cheat initialized. Press F1 to open the menu, F12 to unload...\n");

	std::string scene = XUtility::SceneName();

	if (scene == std::string("Menu")) {
		std::cout << dye::red("\n\n******************************\n");
		std::cout << dye::red("Be cautious when using the Object Pool menu. I've added it as a preview feature. A single wrong click could cause the game to crash.") << std::endl;
		std::cout << dye::red("******************************\n\n");
	}

	DetourInitilization();

	while (true) {
		// Check shutdown request
		if (WaitForSingleObject(hUnloadEvent, 100) == WAIT_OBJECT_0) {
			std::cout << "[RiftSystems]: " << dye::red("Unload event detected, shutting down...\n") << std::endl;
			break; // End the loop
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Relax the CPU :D
	}

	// Cleanup resources
	DetourUninitialization();
	fclose(stdout);
	FreeConsole();
	CloseHandle(hUnloadEvent);
	FreeLibraryAndExitThread(hModule, 0);
}