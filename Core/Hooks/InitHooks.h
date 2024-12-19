#pragma once

void DetourInitilization();
void DetourUninitialization();

// Unity.Debug
void dDebug_Log(app::Object* message, MethodInfo* method);
void dDebug_LogError(app::Object* message, MethodInfo* method);
void dDebug_LogException(app::Exception* exception, MethodInfo* method);
void dDebug_LogWarning(app::Object* message, MethodInfo* method);

// Cursor
bool dCursor_get_visible(MethodInfo* method);
void dCursor_set_visible(bool value, MethodInfo* method);
app::CursorLockMode__Enum dCursor_get_lockState(MethodInfo* method);
void dCursor_set_lockState(app::CursorLockMode__Enum value, MethodInfo* method);

// Misc
bool dLockedInteractable_CanInteract(app::LockedInteractable* __this, app::GameObject* character, MethodInfo* method);
void dNolanBehaviour_OnAttributeUpdateValue(app::NolanBehaviour* __this, app::Attribute_1* attribute, MethodInfo* method);
bool dDevourInput_GetLongPress(app::DevourInput* __this, app::String* name, float duration, bool waitForRelease, MethodInfo* method);
void dNolanBehaviour_Update(app::NolanBehaviour* __this, MethodInfo* method);
void dNolanBehaviour_FixedUpdate(app::NolanBehaviour* __this, MethodInfo* method);
bool dOptionsHelpers_IsRobeUnlocked(app::OptionsHelpers* __this, app::String* robe, app::String* character, MethodInfo* method);
bool dOptionsHelpers_IsCharacterUnlocked(app::OptionsHelpers* __this, app::String* prefab, MethodInfo* method);
app::RankHelpers_ExpGainInfo* dRankHelpers_CalculateExpGain(app::RankHelpers* __this, int32_t mapProgress, int32_t numAwards, app::GameConfigToken* gameConfigToken, MethodInfo* method);
bool dSteamInventoryManager_UserHasItem(app::SteamInventoryManager* __this, int32_t steamItemDefID, MethodInfo* method);
bool dSteamInventoryManager_HasRetrievedUserInventoryItems(app::SteamInventoryManager* __this, MethodInfo* method);
bool dSurvivalLobbyController_UnlockedCharacterSelected(app::SurvivalLobbyController* __this, MethodInfo* method);
void dKnockoutBox_OnTriggerEnter(app::KnockoutBox* __this, app::Collider* collider, MethodInfo* method);
app::BoltConfig* dBoltRuntimeSettings_GetConfigCopy(app::BoltRuntimeSettings* __this, MethodInfo* method);

app::BoltEntity* dBoltNetwork_Instantiate_3(app::GameObject* prefab, app::IProtocolToken* token, app::Vector3 position, app::Quaternion rotation, MethodInfo* method);
