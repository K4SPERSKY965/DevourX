#include "pch-il2cpp.h"
#include "Menu.h"
#include "imgui/imgui.h"

#include "Settings/Settings.h"
#include "GUI/Tabs/SettingsTAB.h"
#include "GUI/Tabs/GameplayTAB.h"
#include "GUI/Tabs/PlayerTAB.h"
#include "GUI/Tabs/EntitiesTAB.h"
#include "GUI/Tabs/HooksTAB.h"
#include "GUI/Tabs/ObjectPoolTAB.h"

namespace Menu {
	bool init = false;
	bool firstRender = true;

	void Init() {
		ImGui::SetNextWindowBgAlpha(0.7f);
		ImGui::SetNextWindowSize(ImVec2(800, 600));
		init = true;
	}

	void Render() {
		if (!init)
			Menu::Init();

		ImGui::Begin("DevouX 3.3 by Jadis0x", &settings.bShowMenu, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
		ImGui::BeginTabBar("Devour#TopBar", ImGuiTabBarFlags_NoTabListScrollingButtons);

		if (firstRender) {
			firstRender = false;
		}
		else {
			SettingsTAB::Render();
			GameplayTAB::Render();
			PlayerTAB::Render();
			EntitiesTAB::Render();
			HooksTAB::Render();
			ObjectPoolTAB::Render();
		}

		ImGui::EndTabBar();
		ImGui::End();
	}
}