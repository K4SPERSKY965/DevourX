#include "pch-il2cpp.h"

#include "Misc.h"
#include "Utilities/Utilities.h"
#include "UnityResolver.hpp"
#include "helpers.h"
#include <map>

void Misc::FullBright(app::NolanBehaviour* localPlayer_nolanBehaviour)
{
	if (localPlayer_nolanBehaviour != nullptr) {
		app::Light* _flashlight = localPlayer_nolanBehaviour->fields.flashlightSpot;

		app::Light_set_intensity(_flashlight, 1.3f, nullptr);
		app::Light_set_range(_flashlight, 370.f, nullptr);
		app::Light_set_spotAngle(_flashlight, 290.f, nullptr);

		app::Light_set_shadows(_flashlight, app::LightShadows__Enum::None, nullptr);
	}
}

void Misc::ForceLobbyStart()
{
	if (!XUtility::IsHost()) return;

	static UnityAPI::UObject menuObject;

	app::Menu* menuComponent = menuObject.FindObjectOfType<app::Menu>("Menu", "Horror");

	if (menuComponent) {
		if (app::Menu_OnLobbyStartButtonClick != nullptr) {
			app::Menu_OnLobbyStartButtonClick(menuComponent, nullptr);
		}
	}
}

void Misc::InstantWin()
{
	std::string _scene = XUtility::SceneName();
	if (_scene == std::string("Menu") || !XUtility::IsHost()) return;


	static UnityAPI::UObject mapControllerObject;
	static UnityAPI::UObject slaughterhouseAltarControllerObject;
	static UnityAPI::UObject survivalObjectBurnControllerObject;

	app::GameObject* _LocalPlayer = LocalPlayer::GetLocalPlayer();
	if (_LocalPlayer == nullptr) return;

	int32_t progress = 10;

	if (_scene == std::string("Inn") || _scene == std::string("Manor")) {
		app::MapController* _MapController = mapControllerObject.FindObjectOfType<app::MapController>("MapController");

		if (_MapController) {

			// DO_APP_FUNC(0x00930CD0, void, MapController_SetProgressTo, (MapController * __this, int32_t progress, MethodInfo * method));
			if (app::MapController_SetProgressTo != nullptr) {
				app::MapController_SetProgressTo(_MapController, progress, nullptr);
			}
		}
	}
	else if (_scene == std::string("Slaughterhouse")) {
		app::SlaughterhouseAltarController* _SlaughterhouseAltarController = slaughterhouseAltarControllerObject.FindObjectOfType<app::SlaughterhouseAltarController>("SlaughterhouseAltarController");

		if (_SlaughterhouseAltarController) {

			// DO_APP_FUNC(0x0050DEB0, void, SlaughterhouseAltarController_SkipToGoat, (SlaughterhouseAltarController * __this, int32_t number, MethodInfo * method));
			if (app::SlaughterhouseAltarController_SkipToGoat != nullptr) {
				app::SlaughterhouseAltarController_SkipToGoat(_SlaughterhouseAltarController, progress, nullptr);
			}
		}
	}
	else {
		//app::SurvivalObjectBurnController* _SurvivalObjectBurnController = UnityEngine::Object::FindObjectOfType("SurvivalObjectBurnController");
		app::SurvivalObjectBurnController* _SurvivalObjectBurnController = survivalObjectBurnControllerObject.FindObjectOfType<app::SurvivalObjectBurnController>("SurvivalObjectBurnController");

		if (_SurvivalObjectBurnController) {
			// DO_APP_FUNC(0x00562590, void, SurvivalObjectBurnController_SkipToGoat, (SurvivalObjectBurnController * __this, int32_t number, MethodInfo * method));
			if (app::SurvivalObjectBurnController_SkipToGoat != nullptr) {
				app::SurvivalObjectBurnController_SkipToGoat(_SurvivalObjectBurnController, progress, nullptr);
			}
		}
	}
}

void Misc::CarryItem(const char* itemName)
{
	std::string _scene = XUtility::SceneName();

	if (_scene == std::string("Menu")) {
		return;
	}

	std::map<std::string, std::string> itemMap = {
		{"Hay", "SurvivalHay"},
		{"First aid", "SurvivalFirstAid"},
		{"Spade", "SurvivalSpade"},
		{"Cake", "SurvivalCake"},
		{"Bone", "SurvivalBone"},
		{"Battery", "SurvivalBattery"},
		{"Gasoline", "SurvivalGasoline"},
		{"Fuse", "SurvivalFuse"},
		{"Food", "SurvivalRottenFood"},
		{"Bleach", "SurvivalBleach"},
		{"Ritual Book (inactive)", "RitualBook-InActive-1"},
		{"Ritual Book (active)", "RitualBook-Active-1"},
		{"Matchbox", "Matchbox-3"},
		{"Egg 1", "Egg-Clean-1"},
		{"Egg 2", "Egg-Clean-2"},
		{"Egg 3", "Egg-Clean-3"},
		{"Egg 4", "Egg-Clean-4"},
		{"Egg 5", "Egg-Clean-5"},
		{"Egg 6", "Egg-Clean-6"},
		{"Egg 7", "Egg-Clean-7"},
		{"Egg 8", "Egg-Clean-8"},
		{"Egg 9", "Egg-Clean-9"},
		{"Egg 10", "Egg-Clean-10"},
		{"Head (Dirty/Gardener)", "Head-Dirty-Gardener"},
		{"Head (Clean/Gardener)", "Head-Clean-Gardener"},
		{"Head (Dirty/Butler)", "Head-Dirty-Butler"},
		{"Head (Clean/Butler)", "Head-Clean-Butler)"},
		{"Head (Dirty/Bridesmaid_01)", "Head-Dirty-Bridesmaid_01"},
		{"Head (Clean/Bridesmaid_01)", "Head-Clean-Bridesmaid_01"},
		{"Head (Dirty/Bridesmaid_02)", "Head-Dirty-Bridesmaid_02"},
		{"Head (Clean/Bridesmaid_02)", "Head-Clean-Bridesmaid_02"},
		{"Head (Dirty/Cook)", "Head-Dirty-Cook"},
		{"Head (Clean/Cook)", "Head-Clean-Cook"},
		{"Head (Dirty/Groomsman_01)", "Head-Dirty-Groomsman_01"},
		{"Head (Clean/Groomsman_01)", "Head-Clean-Groomsman_01"},
		{"Head (Dirty/Groomsman_02)", "Head-Dirty-Groomsman_02"},
		{"Head (Clean/Groomsman_02)", "Head-Clean-Groomsman_02"},
		{"Head (Dirty/Maid)", "Head-Dirty-Maid"},
		{"Head (Clean/Maid)", "Head-Clean-Maid"},
		{"Head (Dirty/Photographer)", "Head-Dirty-Photographer"},
		{"Head (Clean/Photographer)", "Head-Clean-Photographer"},
		{"Head (Dirty/Priest)", "Head-Dirty-Priest"},
		{"Head (Clean/Priest)", "Head-Clean-Priest"}
	};

	std::string carryItem = itemMap[itemName];

	if (!carryItem.empty() && LocalPlayer::GetLocalPlayer()) {
		app::NolanBehaviour_StartCarry(LocalPlayer::GetNolan(), ConvertToSystemString(carryItem.c_str()), nullptr);
	}
}

void Misc::CarryAnimal(const char* animalName)
{
	std::string _scene = XUtility::SceneName();

	if (_scene == std::string("Menu")) return;

	std::map<std::string, std::string> animalMap = {
		{"Rat", "SurvivalRat"},
		{"Goat", "SurvivalGoat"},
		{"Pig", "SurvivalPig"}
	};

	std::string carryAnimal = animalMap[animalName];

	if (!carryAnimal.empty() && LocalPlayer::GetLocalPlayer()) {
		app::NolanBehaviour_StartCarry(LocalPlayer::GetNolan(), ConvertToSystemString(carryAnimal.c_str()), nullptr);
	}
}

void Misc::Kill(app::GameObject* targetPlayer)
{
	if (targetPlayer == nullptr) return;

	std::string _scene = XUtility::SceneName();

	if (_scene == std::string("Menu")) return;

	if (!XUtility::IsHost()) return;

	static UnityAPI::UObject uSurvival;
	static UnityAPI::UObject uSurvivalAzazelBehaviour;

	app::Survival* survival = uSurvival.FindObjectOfType<app::Survival>("Survival");
	if (survival == nullptr) return;

	app::GameObject* azazel = XUtility::GetAzazel(survival);
	if (azazel == nullptr) return;

	app::SurvivalAzazelBehaviour* azazelBehaviour = uSurvivalAzazelBehaviour.FindObjectOfType<app::SurvivalAzazelBehaviour>("SurvivalAzazelBehaviour");
	if(azazelBehaviour != nullptr)

	if (app::SurvivalAzazelBehaviour_OnKnockout != nullptr) {
		app::SurvivalAzazelBehaviour_OnKnockout(azazelBehaviour, azazel, targetPlayer, nullptr);
	}
}

void Misc::Revive(app::GameObject* targetPlayer)
{
	if (targetPlayer == nullptr) return;

	std::string _scene = XUtility::SceneName();

	if (_scene == std::string("Menu")) return;

	static UnityAPI::UObject uSurvivalReviveInteractable;

	app::SurvivalReviveInteractable* revive = uSurvivalReviveInteractable.FindObjectOfType<app::SurvivalReviveInteractable>("SurvivalReviveInteractable");

	if (revive == nullptr) {
		UnityAPI::UObject uNolanObject;

		app::Component* nbComponent = uNolanObject.GetComponentByName(targetPlayer, "NolanBehaviour");
		if (nbComponent == nullptr) return;

		app::NolanBehaviour* nb = reinterpret_cast<app::NolanBehaviour*>(nbComponent);

		if (nb != nullptr && XUtility::IsPlayerCrawling(targetPlayer)) {
			app::Vector3 vec{ 0.f, -100.f, 0.f };

			if (app::Quaternion_get_identity != nullptr && app::NolanBehaviour_TeleportTo != nullptr) {
				app::NolanBehaviour_TeleportTo(nb, vec, app::Quaternion_get_identity(nullptr), false, nullptr);
			}
		}
	}
	else {
		if (XUtility::IsPlayerCrawling(targetPlayer)) {
			app::SurvivalReviveInteractable_Interact(revive, targetPlayer, nullptr);
		}
	}
}

void Misc::Jumpscare(app::GameObject* targetPlayer)
{
	if (targetPlayer == nullptr) return;

	std::string _scene = XUtility::SceneName();

	if (_scene == std::string("Menu")) return;

	if (!XUtility::IsHost()) return;

	static UnityAPI::UObject uSurvival;
	static UnityAPI::UObject uSurvivalAzazelBehaviour;
	static UnityAPI::UObject uNolanObject;

	app::Survival* survival = uSurvival.FindObjectOfType<app::Survival>("Survival");
	if (survival == nullptr) return;

	app::GameObject* azazel = XUtility::GetAzazel(survival);
	if (azazel == nullptr) return;

	app::SurvivalAzazelBehaviour* azazelBehaviour = uSurvivalAzazelBehaviour.FindObjectOfType<app::SurvivalAzazelBehaviour>("SurvivalAzazelBehaviour");

	if (azazelBehaviour) {
		app::Component* nbComponent = uNolanObject.GetComponentByName(targetPlayer, "NolanBehaviour");
		if (nbComponent == nullptr) return;

		app::NolanBehaviour* nb = reinterpret_cast<app::NolanBehaviour*>(nbComponent);
		if (nb == nullptr) return;

		if (app::SurvivalAzazelBehaviour_OnPickedUpPlayer != nullptr) {
			app::SurvivalAzazelBehaviour_OnPickedUpPlayer(azazelBehaviour, azazel, targetPlayer, true, nullptr);
		}
	}
}
