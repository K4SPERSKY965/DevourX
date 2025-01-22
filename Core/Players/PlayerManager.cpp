#include "pch-il2cpp.h"

#include "Utilities/Utilities.h"
#include "Players/PlayerManager.h"
#include <UnityResolver.hpp>

namespace Players {

    std::vector<app::GameObject*>& PlayersManager::GetAllPlayers() {
        if (playersList.empty()) {
            RefreshPlayers();
        }

        // nullptr olan oyuncuları temizlemek için set'i kullanıyoruz
        for (auto it = playersList.begin(); it != playersList.end(); ) {
            if (XUtility::IsNull((app::Object_1*)(*it))) {
                playersSet.erase(*it);  // Set'ten çıkar
                it = playersList.erase(it);  // Vektörden çıkar
            }
            else {
                ++it;
            }
        }

        static UnityAPI::UObject uObj("Assembly-CSharp.dll");

        // Yeni oyuncuların eklenmesi
        app::GameObject__Array* currentGameObjects = uObj.FindGameObjectsWithTag("Player");
        if (currentGameObjects == nullptr) {
            return playersList;  // Boşsa mevcut listeyi döndür
        }

        for (int i = 0; i < currentGameObjects->max_length; ++i) {
            app::GameObject* currentPlayer = currentGameObjects->vector[i];
            if (currentPlayer != nullptr && playersSet.find(currentPlayer) == playersSet.end()) {
                playersSet.insert(currentPlayer);
                playersList.push_back(currentPlayer);
            }
        }

        return playersList;
    }

    void PlayersManager::RefreshPlayers() {
        playersSet.clear();
        playersList.clear();

        static UnityAPI::UObject uObj("Assembly-CSharp.dll");

        app::GameObject__Array* gameObjects = uObj.FindGameObjectsWithTag("Player");
        if (gameObjects && gameObjects->max_length > 0) {
            for (il2cpp_array_size_t i = 0; i < gameObjects->max_length; ++i) {
                app::GameObject* currentPlayer = gameObjects->vector[i];
                if (!XUtility::IsNull((app::Object_1*)currentPlayer)) {
                    playersSet.insert(currentPlayer);
                    playersList.push_back(currentPlayer);
                }
            }
        }
    }

    std::vector<app::GameObject*>& GetPlayers() {
        return PlayersManager::Instance().GetAllPlayers();
    }
}

namespace LocalPlayer {

    app::GameObject* LocalPlayerManager::GetLocalPlayer() {
        if (cachedLocalPlayer != nullptr && !XUtility::IsNull((app::Object_1*)cachedLocalPlayer)) {
            return cachedLocalPlayer;
        }

        // Eğer cachedLocalPlayer geçersizse, sıfırla ve yenile
        cachedLocalPlayer = nullptr;

        static UnityAPI::UObject uObj("Assembly-CSharp.dll");

        const std::vector<app::GameObject*>& playerList = Players::GetPlayers();
        for (auto& currentPlayer : playerList) {
            app::Component* nbComponent = uObj.GetComponentByName(currentPlayer, "NolanBehaviour");
            if (nbComponent) {
                app::NolanBehaviour* nb = reinterpret_cast<app::NolanBehaviour*>(nbComponent);
                if (nb && XUtility::IsLocalPlayer(nb)) {
                    cachedLocalPlayer = currentPlayer;
                    return cachedLocalPlayer;
                }
            }
        }

        return nullptr;
    }

    app::NolanBehaviour* LocalPlayerManager::GetNolan() {
        if (cachedNolan != nullptr && !XUtility::IsNull((app::Object_1*)cachedNolan)) {
            return cachedNolan;
        }

        app::GameObject* localPlayer = GetLocalPlayer();
        if (!localPlayer || XUtility::IsNull((app::Object_1*)localPlayer)) {
            return nullptr;
        }

        static UnityAPI::UObject uObj("Assembly-CSharp.dll");

        app::Component* nbComponent = uObj.GetComponentByName(localPlayer, "NolanBehaviour");
        if (nbComponent != nullptr && !XUtility::IsNull((app::Object_1*)nbComponent)) {
            cachedNolan = reinterpret_cast<app::NolanBehaviour*>(nbComponent);
        }
        else {
            cachedNolan = nullptr;
        }

        return cachedNolan;
    }

    app::GameObject* GetLocalPlayer() {
        return LocalPlayerManager::Instance().GetLocalPlayer();
    }

    app::NolanBehaviour* GetNolan() {
        return LocalPlayerManager::Instance().GetNolan();
    }
}
