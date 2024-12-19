#include "pch-il2cpp.h"
#include "GameplayTAB.h"
#include "GUI/GUITheme.h" 
#include "Settings/Settings.h"
#include <imgui/imgui.h>
#include <Misc/Misc.h>

void GameplayTAB::Render()
{
    if (ImGui::BeginTabItem("Gameplay")) {
        ImGui::Spacing();
        ImGui::Checkbox("Unlimited UV Light", &settings.unlimitedUVLight);
        ImGui::Checkbox("Unlock Doors", &settings.unlockDoors);
        ImGui::Checkbox("Disable Long Interact", &settings.disableLongInteract);


        ImGui::Checkbox("Azazel Speed", &settings.change_azazel_speed);
        ImGui::SliderFloat("Multiplier", &settings.new_azazel_speed, 0, 15);

        if (ImGui::Button("Force Lobby Start")) {
            Misc::ForceLobbyStart();
        }

        if (ImGui::Button("Instant Win")) {
            Misc::InstantWin();
        }

        ImGui::EndTabItem();
    }
}
