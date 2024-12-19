#pragma once
#include <string>

namespace XGlobalUtility {
	extern bool bUnhook;
}

namespace XUtility {
	bool IsHost();
	bool IsNull(app::Object_1* obj);
	bool IsInGame();
	std::string SceneName();
	bool IsLocalPlayer(app::NolanBehaviour* player);
	std::string ObjectToString(app::Object* object);
	std::string GameObjectObjectToString(app::GameObject* gameObject);
	bool IsSteamRunning();
	void ForceQuitApp();
	void WarnAndQuit(std::string_view msg);
	app::GameObject* GetAzazel(app::Survival* survival);
	bool IsPlayerCrawling(app::GameObject* gameObject);
	bool IsSequencePlaying();
}

namespace app {
	Vector3 operator+(const Vector3& lhs, const Vector3& rhs);
	Vector3 operator*(const Vector3& vec, float scalar);
	Vector3 operator-(const Vector3& lhs, const Vector3& rhs);
}
