#include "pch-il2cpp.h"

#include <Windows.h>
#include "detours/detours.h"
#include "InitHooks.h"
#include <iostream>
#include "DirectX.h"

#include <Utilities/Utilities.h>
#include <Settings/Settings.h>
#include <helpers.h>

#include "Misc/Misc.h"
#include <UnityResolver.hpp>
#include <Utilities/ObjectPool.hpp>

using app::Debug_2_Log;
using app::Debug_2_LogError;
using app::Debug_2_LogException;
using app::Debug_2_LogWarning;

using app::Object;
using app::Exception;


#pragma region UNITY_HOOKS
void dDebug_Log(Object* message, MethodInfo* method)
{
	if (settings.bEnableUnityLogs)
		std::cout << "[DevourX-uLog]: " << XUtility::ObjectToString(message) << std::endl;

	app::Debug_2_Log(message, method);
}

void dDebug_LogError(Object* message, MethodInfo* method)
{
	if (settings.bEnableUnityLogs)
		std::cout << "[DevourX-uLogError]: " << XUtility::ObjectToString(message) << std::endl;

	app::Debug_2_LogError(message, method);
}

void dDebug_LogException(Exception* exception, MethodInfo* method)
{
	if (settings.bEnableUnityLogs) {
		std::string excMsg = il2cppi_to_string(exception->fields._message);
		std::cout << "[DevourX-uLogException]: " << excMsg << std::endl;
	}

	app::Debug_2_LogException(exception, method);
}

void dDebug_LogWarning(app::Object* message, MethodInfo* method)
{
	if (settings.bEnableUnityLogs)
		std::cout << "[DevourX-uLogWarning]: " << XUtility::ObjectToString(message) << std::endl;

	app::Debug_2_LogWarning(message, method);
}

#pragma endregion UNITY_HOOKS

#pragma region CURSOR_HOOKS
bool dCursor_get_visible(MethodInfo* method)
{
	if (settings.bShowMenu)
		return true;

	return app::Cursor_1_get_visible(method);
}

void dCursor_set_visible(bool value, MethodInfo* method)
{
	if (settings.bShowMenu)
		value = true;

	return app::Cursor_1_set_visible(value, method);
}

app::CursorLockMode__Enum dCursor_get_lockState(MethodInfo* method)
{
	if (settings.bShowMenu)
		return app::CursorLockMode__Enum::None;

	return app::Cursor_1_get_lockState(method);
}

void dCursor_set_lockState(app::CursorLockMode__Enum value, MethodInfo* method)
{
	if (settings.bShowMenu)
		value = app::CursorLockMode__Enum::None;

	app::Cursor_1_set_lockState(value, method);
}


#pragma endregion CURSOR_HOOKS

#pragma region MISC_HOOKS
bool dLockedInteractable_CanInteract(app::LockedInteractable* __this, app::GameObject* character, MethodInfo* method)
{
	if (settings.unlockDoors) {
		__this->fields.isLocked = false;
		return true;
	}

	return app::LockedInteractable_CanInteract(__this, character, method);
}

void dNolanBehaviour_OnAttributeUpdateValue(app::NolanBehaviour* __this, app::Attribute_1* attribute, MethodInfo* method)
{
	if (settings.unlimitedUVLight && strcmp(il2cppi_to_string(attribute->fields.m_Name).c_str(), "Battery") == 0) {
		attribute->fields.m_Value = 100.0f;
	}

	return app::NolanBehaviour_OnAttributeUpdateValue(__this, attribute, method);
}

bool dDevourInput_GetLongPress(app::DevourInput* __this, app::String* name, float duration, bool waitForRelease, MethodInfo* method)
{
	if (settings.disableLongInteract) {
		duration = 0.0f;
	}

	return app::DevourInput_GetLongPress(__this, name, duration, waitForRelease, method);
}

void dNolanBehaviour_FixedUpdate(app::NolanBehaviour* __this, MethodInfo* method) {

	app::NolanBehaviour* nb = __this;

	if (settings.change_azazel_speed) {
		app::GameObject* goAzazel = nb->fields.m_Survival->fields.m_Azazel;

		if (goAzazel && XUtility::IsHost()) {
			auto component = app::GameObject_GetComponentByName(goAzazel, ConvertToSystemString("UltimateCharacterLocomotion"), nullptr);

			if (component) {
				app::UltimateCharacterLocomotion* locomotion = reinterpret_cast<app::UltimateCharacterLocomotion*>(component);

				if (locomotion) {
					// confused Azazel!! fix it
					app::UltimateCharacterLocomotion_set_TimeScale(locomotion, settings.new_azazel_speed, NULL);
				}
			}
		}
	}

	return app::NolanBehaviour_FixedUpdate(__this, method);
}

void dNolanBehaviour_Update(app::NolanBehaviour* __this, MethodInfo* method)
{
	app::NolanBehaviour* nb = __this;

	if (settings.bFlashlightModifier) {
		if (XUtility::IsLocalPlayer(nb)) {
			Misc::FullBright(nb);
		}
	}

	if (settings.fastPlayer) {
		if (XUtility::IsLocalPlayer(nb)) {
			if (__this->fields.speedChangeAbility)
			{
				__this->fields.speedChangeAbility->fields.m_SpeedChangeMultiplier = (float)settings.new_player_speed;
				__this->fields.speedChangeAbility->fields.m_MaxSpeedChangeValue = (float)settings.new_player_speed;
			}
		}
	}

	if (settings.bFly) {
		if (XUtility::IsLocalPlayer(nb) && nb != nullptr) {
			float speed = settings.bFlySpeed;

			app::Transform* transform = app::Component_get_transform((app::Component*)nb, nullptr);

			if (transform != nullptr) {

				UnityAPI::UTransform uTransform(transform);
				UnityAPI::UTime uTime;
				UnityAPI::UObject uObject;

				app::Vector3 pos = uTransform.GetPosition();

				if (GetAsyncKeyState('W') & 0x8000) {
					pos = pos + (app::Transform_get_forward(transform, nullptr) * speed * uTime.deltaTime());
				}
				if (GetAsyncKeyState('S') & 0x8000) {
					pos = pos - (app::Transform_get_forward(transform, nullptr) * speed * uTime.deltaTime());
				}
				if (GetAsyncKeyState('D') & 0x8000) {
					pos = pos + (app::Transform_get_right(transform, nullptr) * speed * uTime.deltaTime());
				}
				if (GetAsyncKeyState('A') & 0x8000) {
					pos = pos - (app::Transform_get_right(transform, nullptr) * speed * uTime.deltaTime());
				}
				if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
					pos = pos + (app::Transform_get_up(transform, nullptr) * speed * uTime.deltaTime());
				}
				if (GetAsyncKeyState(VK_LCONTROL) & 0x8000) {
					pos = pos - (app::Transform_get_up(transform, nullptr) * speed * uTime.deltaTime());
				}

				app::GameObject* thisGameObject = app::Component_get_gameObject((app::Component*)nb, nullptr);

				if (thisGameObject != nullptr || !XUtility::IsNull((app::Object_1*)thisGameObject)) {
					app::Component* _UltimateCharacterLocomotion = uObject.GetComponentByName(thisGameObject, "UltimateCharacterLocomotion");

					if (_UltimateCharacterLocomotion != nullptr && !XUtility::IsNull((app::Object_1*)_UltimateCharacterLocomotion)) {
						if (app::UltimateCharacterLocomotion_SetPosition_1) {
							app::UltimateCharacterLocomotion_SetPosition_1((app::UltimateCharacterLocomotion*)_UltimateCharacterLocomotion, pos, false, nullptr);
						}
					}
				}
			}
		}
	}

	return app::NolanBehaviour_Update(__this, method);
}

bool dOptionsHelpers_IsRobeUnlocked(app::OptionsHelpers* __this, app::String* robe, app::String* character, MethodInfo* method)
{
	if (settings.bIsRobeUnlocked) {
		auto itemSize = __this->fields.outfits->fields._items;

		for (int i = 0; i < itemSize->max_length; i++) {
			auto currentItem = itemSize->vector[i];

			currentItem->fields.isSupporter = true;
			currentItem->fields.isOwned = true;
			currentItem->fields.isHidden = false;
		}

		return true;
	}

	return app::OptionsHelpers_IsRobeUnlocked(__this, robe, character, method);
}

bool dOptionsHelpers_IsCharacterUnlocked(app::OptionsHelpers* __this, app::String* prefab, MethodInfo* method)
{
	if (settings.bIsRobeUnlocked) {
		std::cout << il2cppi_to_string(prefab) << "\n";

		return true;
	}

	return app::OptionsHelpers_IsCharacterUnlocked(__this, prefab, method);
}

app::RankHelpers_ExpGainInfo* dRankHelpers_CalculateExpGain(app::RankHelpers* __this, int32_t mapProgress, int32_t numAwards, app::GameConfigToken* gameConfigToken, MethodInfo* method)
{
	if (settings.exp_modifier) {
		app::RankHelpers_ExpGainInfo* gain = app::RankHelpers_CalculateExpGain(__this, mapProgress, numAwards, gameConfigToken, method);

		int32_t bonus = (int32_t)settings.new_exp;

		gain->fields.awardsBonus = bonus;
		gain->fields.winBonus = bonus;
		gain->fields.totalExp = bonus;

		return gain;
	}

	return app::RankHelpers_CalculateExpGain(__this, mapProgress, numAwards, gameConfigToken, method);
}

bool dSteamInventoryManager_UserHasItem(app::SteamInventoryManager* __this, int32_t steamItemDefID, MethodInfo* method)
{
	if (settings.bIsRobeUnlocked) {

		std::cout << "def id: " << steamItemDefID << "\n";

		__this->fields.addPromoItemLock = false;

		return true;
	}

	return app::SteamInventoryManager_UserHasItem(__this, steamItemDefID, method);
}

bool dSteamInventoryManager_HasRetrievedUserInventoryItems(app::SteamInventoryManager* __this, MethodInfo* method)
{
	if (settings.bIsRobeUnlocked) {
		return true;
	}

	return app::SteamInventoryManager_HasRetrievedUserInventoryItems(__this, method);
}

bool dSurvivalLobbyController_UnlockedCharacterSelected(app::SurvivalLobbyController* __this, MethodInfo* method)
{
	if (settings.bUnlockedCharacterSelected) {
		return true;
	}

	return app::SurvivalLobbyController_UnlockedCharacterSelected(__this, method);
}

void dKnockoutBox_OnTriggerEnter(app::KnockoutBox* __this, app::Collider* collider, MethodInfo* method)
{
	return;
}

app::BoltConfig* dBoltRuntimeSettings_GetConfigCopy(app::BoltRuntimeSettings* __this, MethodInfo* method)
{
	if (settings.bServerhook) {
		__this->fields._config->fields.serverConnectionLimit = settings.bNewServerConnectionLimit;
	}

	//auto acceptMode = __this->fields._config->fields.serverConnectionAcceptMode;

	__this->fields.instantiateMode = app::BoltPrefabInstantiateMode__Enum::Everyone;


	return app::BoltRuntimeSettings_GetConfigCopy(__this, method);
}

app::BoltEntity* dBoltNetwork_Instantiate_3(app::GameObject* prefab, app::IProtocolToken* token, app::Vector3 position, app::Quaternion rotation, MethodInfo* method) {

	app::BoltEntity* entity = app::BoltNetwork_Instantiate_3(prefab, token, position, rotation, method);

	// Instantiate sonrası elde edilen entity’den GameObject al
	app::GameObject* spawnedGO = app::BoltEntity_op_Implicit(entity, nullptr);

	// Prefab ismini kullanabilirsin ama map’e instantiated GameObject’i ekle
	std::string objName = XUtility::GameObjectObjectToString(prefab);
	XObjectPool::ObjectPool::GetInstance()->AddGameObject(objName, spawnedGO, position, rotation);

	return entity;
}


#pragma endregion MISC_HOOKS

bool HookFunction(PVOID* ppPointer, PVOID pDetour, const char* functionName) {
	if (const auto error = DetourAttach(ppPointer, pDetour); error != NO_ERROR) {
		std::cout << "[DevourX]: Failed to hook " << functionName << ", error " << error << std::endl;
		return false;
	}
	// std::cout << "Hooked " << functionName << std::endl;
	return true;
}

#define HOOKFUNC(n) if (!HookFunction(&(PVOID&)n, d##n, #n)) return;

bool UnhookFunction(PVOID* ppPointer, PVOID pDetour, const char* functionName) {
	if (const auto error = DetourDetach(ppPointer, pDetour); error != NO_ERROR) {
		std::cout << "[DevourX]: Failed to unhook " << functionName << ", error " << error << std::endl;
		return false;
	}
	// std::cout << "Unhooked " << functionName << std::endl;
	return true;
}

#define UNHOOKFUNC(n) if (!UnhookFunction(&(PVOID&)n, d##n, #n)) return;

void DetourInitilization() {
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	dx11api d3d11 = dx11api();
	if (!d3d11.presentFunction) {
		std::cout << "[DevourX]: Unable to retrieve IDXGISwapChain::Present method" << std::endl;
		return;
	}

	oPresent = d3d11.presentFunction;
	if (!oPresent) {
		std::cout << "[DevourX]: oPresent is null!" << std::endl;
		return;
	}

	std::cout << "[DevourX]: Attempting to hook oPresent at address: " << oPresent << std::endl;

	if (!HookFunction(&(PVOID&)oPresent, dPresent, "D3D_PRESENT_FUNCTION")) {
		DetourTransactionAbort();
		return;
	}

	if (!HookFunction(&(PVOID&)Debug_2_Log, dDebug_Log, "Debug_2_Log")) {
		DetourTransactionAbort();
		return;
	}

	if (!HookFunction(&(PVOID&)Debug_2_LogError, dDebug_LogError, "Debug_2_LogError")) {
		DetourTransactionAbort();
		return;
	}

	if (!HookFunction(&(PVOID&)Debug_2_LogException, dDebug_LogException, "Debug_2_LogException")) {
		DetourTransactionAbort();
		return;
	}

	if (!HookFunction(&(PVOID&)Debug_2_LogWarning, dDebug_LogWarning, "Debug_2_LogWarning")) {
		DetourTransactionAbort();
		return;
	}

	if (!HookFunction(&(PVOID&)app::Cursor_1_get_visible, dCursor_get_visible, "Cursor_1_get_visible")) {
		DetourTransactionAbort();
		return;
	}

	if (!HookFunction(&(PVOID&)app::Cursor_1_set_visible, dCursor_set_visible, "Cursor_1_set_visible")) {
		DetourTransactionAbort();
		return;
	}

	if (!HookFunction(&(PVOID&)app::Cursor_1_get_lockState, dCursor_get_lockState, "Cursor_1_get_lockState")) {
		DetourTransactionAbort();
		return;
	}

	if (!HookFunction(&(PVOID&)app::Cursor_1_set_lockState, dCursor_set_lockState, "Cursor_1_set_lockState")) {
		DetourTransactionAbort();
		return;
	}

	if (!HookFunction(&(PVOID&)app::LockedInteractable_CanInteract, dLockedInteractable_CanInteract, "LockedInteractable_CanInteract")) {
		DetourTransactionAbort();
		return;
	}

	if (!HookFunction(&(PVOID&)app::NolanBehaviour_OnAttributeUpdateValue, dNolanBehaviour_OnAttributeUpdateValue, "dNolanBehaviour_OnAttributeUpdateValue")) {
		DetourTransactionAbort();
		return;
	}

	if (!HookFunction(&(PVOID&)app::DevourInput_GetLongPress, dDevourInput_GetLongPress, "DevourInput_GetLongPress")) {
		DetourTransactionAbort();
		return;
	}

	if (!HookFunction(&(PVOID&)app::NolanBehaviour_FixedUpdate, dNolanBehaviour_FixedUpdate, "NolanBehaviour_FixedUpdate")) {
		DetourTransactionAbort();
		return;
	}

	if (!HookFunction(&(PVOID&)app::NolanBehaviour_Update, dNolanBehaviour_Update, "NolanBehaviour_Update")) {
		DetourTransactionAbort();
		return;
	}

	if (!HookFunction(&(PVOID&)app::OptionsHelpers_IsRobeUnlocked, dOptionsHelpers_IsRobeUnlocked, "OptionsHelpers_IsRobeUnlocked")) {
		DetourTransactionAbort();
		return;
	}

	if (!HookFunction(&(PVOID&)app::OptionsHelpers_IsCharacterUnlocked, dOptionsHelpers_IsCharacterUnlocked, "OptionsHelpers_IsCharacterUnlocked")) {
		DetourTransactionAbort();
		return;
	}

	if (!HookFunction(&(PVOID&)app::RankHelpers_CalculateExpGain, dRankHelpers_CalculateExpGain, "RankHelpers_CalculateExpGain")) {
		DetourTransactionAbort();
		return;
	}

	if (!HookFunction(&(PVOID&)app::SteamInventoryManager_UserHasItem, dSteamInventoryManager_UserHasItem, "SteamInventoryManager_UserHasItem")) {
		DetourTransactionAbort();
		return;
	}

	if (!HookFunction(&(PVOID&)app::SteamInventoryManager_HasRetrievedUserInventoryItems, dSteamInventoryManager_HasRetrievedUserInventoryItems, "SteamInventoryManager_HasRetrievedUserInventoryItems")) {
		DetourTransactionAbort();
		return;
	}

	if (!HookFunction(&(PVOID&)app::SurvivalLobbyController_UnlockedCharacterSelected, dSurvivalLobbyController_UnlockedCharacterSelected, "SurvivalLobbyController_UnlockedCharacterSelected")) {
		DetourTransactionAbort();
		return;
	}

	if (!HookFunction(&(PVOID&)app::BoltRuntimeSettings_GetConfigCopy, dBoltRuntimeSettings_GetConfigCopy, "BoltRuntimeSettings_GetConfigCopy")) {
		DetourTransactionAbort();
		return;
	}

	if (!HookFunction(&(PVOID&)app::KnockoutBox_OnTriggerEnter, dKnockoutBox_OnTriggerEnter, "KnockoutBox_OnTriggerEnter")) {
		DetourTransactionAbort();
		return;
	}

	if (!HookFunction(&(PVOID&)app::BoltNetwork_Instantiate_3, dBoltNetwork_Instantiate_3, "BoltNetwork_Instantiate_3")) {
		DetourTransactionAbort();
		return;
	}

	DetourTransactionCommit();
}

void DetourUninitialization() {
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	if (DetourDetach(&(PVOID&)oPresent, dPresent) != 0) return;

	if (DetourDetach(&(PVOID&)Debug_2_Log, dDebug_Log) != 0) return;
	if (DetourDetach(&(PVOID&)Debug_2_LogError, dDebug_LogError) != 0) return;
	if (DetourDetach(&(PVOID&)Debug_2_LogException, dDebug_LogException) != 0) return;
	if (DetourDetach(&(PVOID&)Debug_2_LogWarning, dDebug_LogWarning) != 0) return;

	if (DetourDetach(&(PVOID&)app::Cursor_1_get_visible, dCursor_get_visible) != 0) return;
	if (DetourDetach(&(PVOID&)app::Cursor_1_set_visible, dCursor_set_visible) != 0) return;
	if (DetourDetach(&(PVOID&)app::Cursor_1_get_lockState, dCursor_get_lockState) != 0) return;
	if (DetourDetach(&(PVOID&)app::Cursor_1_set_lockState, dCursor_set_lockState) != 0) return;

	if (DetourDetach(&(PVOID&)app::LockedInteractable_CanInteract, dLockedInteractable_CanInteract) != 0) return;
	if (DetourDetach(&(PVOID&)app::NolanBehaviour_OnAttributeUpdateValue, dNolanBehaviour_OnAttributeUpdateValue) != 0) return;
	if (DetourDetach(&(PVOID&)app::DevourInput_GetLongPress, dDevourInput_GetLongPress) != 0) return;

	if (DetourDetach(&(PVOID&)app::NolanBehaviour_FixedUpdate, dNolanBehaviour_FixedUpdate) != 0) return;
	if (DetourDetach(&(PVOID&)app::NolanBehaviour_Update, dNolanBehaviour_Update) != 0) return;
	if (DetourDetach(&(PVOID&)app::OptionsHelpers_IsRobeUnlocked, dOptionsHelpers_IsRobeUnlocked) != 0) return;
	if (DetourDetach(&(PVOID&)app::OptionsHelpers_IsCharacterUnlocked, dOptionsHelpers_IsCharacterUnlocked) != 0) return;
	if (DetourDetach(&(PVOID&)app::RankHelpers_CalculateExpGain, dRankHelpers_CalculateExpGain) != 0) return;
	if (DetourDetach(&(PVOID&)app::SteamInventoryManager_UserHasItem, dSteamInventoryManager_UserHasItem) != 0) return;
	if (DetourDetach(&(PVOID&)app::SteamInventoryManager_HasRetrievedUserInventoryItems, dSteamInventoryManager_HasRetrievedUserInventoryItems) != 0) return;
	if (DetourDetach(&(PVOID&)app::SurvivalLobbyController_UnlockedCharacterSelected, dSurvivalLobbyController_UnlockedCharacterSelected) != 0) return;
	if (DetourDetach(&(PVOID&)app::KnockoutBox_OnTriggerEnter, dKnockoutBox_OnTriggerEnter) != 0) return;
	if (DetourDetach(&(PVOID&)app::BoltRuntimeSettings_GetConfigCopy, dBoltRuntimeSettings_GetConfigCopy) != 0) return;

	if (DetourDetach(&(PVOID&)app::BoltNetwork_Instantiate_3, dBoltNetwork_Instantiate_3) != 0) return;



	if (DetourTransactionCommit() == NO_ERROR) {
		DirectX::Shutdown();
	}
}


