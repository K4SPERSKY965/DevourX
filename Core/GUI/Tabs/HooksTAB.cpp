#include "pch-il2cpp.h"
#include "HooksTAB.h"
#include "GUI/GUITheme.h" 
#include "Settings/Settings.h"
#include <imgui/imgui.h>

void HooksTAB::Render()
{
    if (ImGui::BeginTabItem("Hooks")) {
        ImGui::Spacing();
        ImGui::Checkbox("Unlock Robes", &settings.bIsRobeUnlocked);
        ImGui::Checkbox("Disable Character Locks", &settings.bUnlockedCharacterSelected);

        ImGui::Checkbox("Enable EXP Modifier", &settings.exp_modifier);
        if (settings.exp_modifier) {
            ImGui::SliderFloat("Amount", &settings.new_exp, 0.f, 5000.0f, "%.1f");
        }
        else {
            ImGui::Text("EXP Modifier is disabled.");
        }

        ImGui::Checkbox("Server Connection Limit", &settings.bServerhook);
        if (settings.bServerhook) {
            ImGui::SliderInt("Limit:", &settings.bNewServerConnectionLimit, 0, 64, "%d");
        }
        else {
            ImGui::Text("Server Connection Limit Hook Disabled.");
        }

        ImGui::EndTabItem();
    }
}
