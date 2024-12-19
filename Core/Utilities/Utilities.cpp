#include "pch-il2cpp.h"

#include "Utilities.h"
#include <helpers.h>
#include <UnityResolver.hpp>
#include <Core.h>

#include "RiftHTTP/RiftHTTP.h"
#include "json/json.hpp"

namespace XGlobalUtility {
	bool bUnhook = FALSE;
}

namespace XUtility {
	bool IsHost() {
		Resolver::Bridge boltdll("BoltNetwork", "Photon.Bolt", "bolt.dll");

		return boltdll.InvokeMethod<bool>("get_IsServer");
	}

	bool IsNull(app::Object_1* obj)
	{
		return !app::Object_1_op_Implicit(obj, nullptr);
	}

	bool IsInGame()
	{
		app::OptionsHelpers* helper = app::OptionsHelpers_get_singleton(nullptr);
		if (helper == nullptr) return true;

		return app::OptionsHelpers_get_inGame(helper, nullptr);
	}

	std::string SceneName()
	{
		app::Scene scene = app::SceneManager_GetActiveScene(nullptr);

		return il2cppi_to_string(app::Scene_get_name(&scene, nullptr));
	}

	bool IsLocalPlayer(app::NolanBehaviour* player) {
		auto boltEntity = app::EntityBehaviour_get_entity((app::EntityBehaviour*)player, NULL);

		if (not boltEntity
			|| not app::BoltEntity_get_IsAttached(boltEntity, NULL)
			|| not app::BoltEntity_get_IsOwner(boltEntity, NULL))
			return false;

		return true;
	}

	std::string ObjectToString(app::Object* object)
	{
		std::string type = il2cppi_to_string(app::Object_ToString(object, NULL));
		if (type == "System.String") {
			return il2cppi_to_string((app::String*)object);
		}
		return type;
	}

	std::string GameObjectObjectToString(app::GameObject* gameObject)
	{
		if (gameObject == nullptr) return std::string("");

		return il2cppi_to_string(app::Object_1_GetName((app::Object_1*)gameObject, nullptr));
	}


	bool IsSteamRunning()
	{
		if (app::SteamAPI_IsSteamRunning != nullptr) {
			return app::SteamAPI_IsSteamRunning(nullptr);
		}

		return false;
	}

	void ForceQuitApp()
	{
		app::Application_Quit_1(nullptr);
	}

	void WarnAndQuit(std::string_view msg)
	{
		RiftSystem::ShowMessageBox(msg.data());
		app::Application_Quit_1(nullptr);
		return;
	}

	app::GameObject* GetAzazel(app::Survival* survival)
	{
		app::GameObject* azazel = app::Survival_GetAzazel(survival, nullptr);

		return azazel ? azazel : nullptr;
	}

	bool IsPlayerCrawling(app::GameObject* gameObject)
	{
		if (gameObject == nullptr) return false;

		UnityAPI::UObject nolanObj;

		app::Component* nbComponent = nolanObj.GetComponentByName(gameObject, "NolanBehaviour");
		if (nbComponent == nullptr) return false;

		app::NolanBehaviour* nolanBehaviour = reinterpret_cast<app::NolanBehaviour*>(nbComponent);

		if (nolanBehaviour) {
			return app::NolanBehaviour_IsCrawling(nolanBehaviour, nullptr);
		}
		else {
			return false;

		}
	}
	bool IsSequencePlaying()
	{
		app::InGameHelpers* inGameHelpersSingleton = app::InGameHelpers_get_singleton(nullptr);
		if (inGameHelpersSingleton == nullptr) return true;

		app::Survival* mSurvival = inGameHelpersSingleton->fields.m_Survival;
		if (mSurvival == nullptr) return true;

		return app::Survival_IsEndingPlaying(mSurvival, NULL)
			|| app::Survival_IsJumpScarePlaying(mSurvival, NULL)
			|| app::Survival_StartingToPlayFailEnding(mSurvival, NULL);
	}
}

namespace app {
	Vector3 operator+(const Vector3& lhs, const Vector3& rhs) {
		return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z };
	}

	Vector3 operator*(const Vector3& vec, float scalar) {
		return { vec.x * scalar, vec.y * scalar, vec.z * scalar };
	}

	Vector3 operator-(const Vector3& lhs, const Vector3& rhs) {
		return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z };
	}
}
