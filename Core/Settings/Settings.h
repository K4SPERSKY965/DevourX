#pragma once
#include <Utilities/Keybinds.h>

class Settings {
public:
#pragma region MenuStatus
	KeyBinds::Config KeyBinds = {
		VK_F1 // toggle menu
	};

	bool ImGuiInitialized = false;
	bool bShowMenu = false;

	bool bEnableUnityLogs = true;
#pragma endregion MenuStatus

	bool unlockDoors = false;
	bool unlimitedUVLight = false;
	bool disableLongInteract = false;

	bool bFlashlightModifier = false;

	bool fastPlayer = false;
	float new_player_speed = 2;

	bool bFly = false;
	float bFlySpeed = 1;

#pragma region MiscHooks
	bool bIsRobeUnlocked = true;
	bool bUnlockedCharacterSelected = true;

	bool exp_modifier = false;
	float new_exp = 2000;

	bool change_azazel_speed;
	float new_azazel_speed = 0;

	bool bServerhook = false;
	int bNewServerConnectionLimit = 1;
#pragma endregion MiscHooks
};

extern Settings settings;