#pragma once

#include <filesystem>
#include <Windows.h>

#include "json/json.hpp"


namespace RiftSystem {
    std::filesystem::path getModulePath(HMODULE hModule);
    std::filesystem::path getApplicationPath();
	void ShowMessageBox(const char* msg);
    bool GameVersionCheck();
    void HandleError(LPVOID lpParam);
    bool IsValidPointer(void* ptr);
}

namespace RiftConfig {
    class ConfigReader {
    public:
        // Singleton 
        static ConfigReader& getInstance(const std::string& fileName = "config.json");

        std::string getGameVersion() const;
        std::string getModVersion() const;
        int getInjectDelay() const;

        void setGameVersion(const std::string& version);
        void setModVersion(const char* version);
        void setInjectDelay(int second);

    private:
        ConfigReader(const std::string& fileName);

        ConfigReader(const ConfigReader&) = delete;
        ConfigReader& operator=(const ConfigReader&) = delete;

        ConfigReader(ConfigReader&&) = delete;
        ConfigReader& operator=(ConfigReader&&) = delete;

        std::string configFileName;
        nlohmann::json config;

        void loadConfig();

        void createDefaultConfig();

        void saveConfig();
    };

    extern ConfigReader& configReader;
}

