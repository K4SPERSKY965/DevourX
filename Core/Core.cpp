#include "pch-il2cpp.h"

#include "Core.h"

#include "Utilities/Utilities.h"
#include <vector>
#include <iostream>
#include <fstream>

using json = nlohmann::json;

namespace RiftSystem {
	std::filesystem::path getModulePath(HMODULE hModule)
	{
		TCHAR buff[MAX_PATH];
		GetModuleFileName(hModule, buff, MAX_PATH);
		return std::filesystem::path(buff);
	}

	std::filesystem::path getApplicationPath() {
		TCHAR buff[MAX_PATH];
		GetModuleFileName(NULL, buff, MAX_PATH);
		return std::filesystem::path(buff);
	}

	void ShowMessageBox(const char* msg)
	{
		MessageBoxA(
			NULL,
			msg,
			"RiftLoader",
			MB_OK | MB_ICONWARNING
		);
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

	void HandleError(LPVOID lpParam) {
		fclose(stdout);
		FreeConsole();
		FreeLibraryAndExitThread((HMODULE)lpParam, 0);
	}

	bool IsValidPointer(void* ptr)
	{
		return (ptr != nullptr) && !IsBadReadPtr(ptr, sizeof(void*));
	}
}

namespace RiftConfig {
	// Singleton için erişim fonksiyonu
	ConfigReader& ConfigReader::getInstance(const std::string& fileName) {
		static ConfigReader instance(fileName);
		return instance;
	}

	// Private constructor to prevent direct instantiation
	ConfigReader::ConfigReader(const std::string& fileName) : configFileName(fileName) {
		loadConfig();
	}

	std::string ConfigReader::getGameVersion() const {
		return config["gameVersion"];
	}

	std::string ConfigReader::getModVersion() const {
		return config["modVersion"];
	}

	int ConfigReader::getInjectDelay() const {
		return config["injectDelay"];
	}

	void ConfigReader::setGameVersion(const std::string& version) {
		config["gameVersion"] = version;
		saveConfig();
	}

	void ConfigReader::setModVersion(const char* version) {
		config["modVersion"] = version;
		saveConfig();
	}

	void ConfigReader::setInjectDelay(int second) {
		config["injectDelay"] = second;
		saveConfig();
	}

	void ConfigReader::loadConfig() {
		std::ifstream configFile(configFileName);
		if (configFile.is_open()) {
			configFile >> config;
		}
		else {
			createDefaultConfig();
			loadConfig(); // load again after the file is created
		}
	}

	void ConfigReader::createDefaultConfig() {
		config["gameVersion"] = "1.0.0"; // default value
		config["modVersion"] = "1.0.0";  // default value
		config["injectDelay"] = 8;      // default value
		saveConfig();
		std::cout << "[RiftLoader]: " << configFileName << " created." << std::endl;
	}

	void ConfigReader::saveConfig() {
		std::ofstream configFile(configFileName);
		configFile << config.dump(4);  // JSON format
		configFile.close();
	}

	ConfigReader& configReader = RiftConfig::ConfigReader::getInstance("RiftLoader_config.json");
}

