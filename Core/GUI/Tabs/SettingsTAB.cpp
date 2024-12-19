#include "pch-il2cpp.h"
#include "SettingsTAB.h"
#include <imgui/imgui.h>
#include "GUI/GUITheme.h" 
#include "Settings/Settings.h"
#include "Utilities/Utilities.h"
#include <iostream>

void SettingsTAB::Render()
{
    if (ImGui::BeginTabItem("Settings")) {

        ImGui::Spacing();
        ImGui::Text("Version: 3.0.2");
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::PushTextWrapPos(550.0f);
        ImGui::Text("DevourX is currently under development. It is normal for some bugs to occur. If you have any feature requests, feel free to contact me. My Discord ID: Jadis0x");
        ImGui::PushTextWrapPos();
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Checkbox("Show Unity Logs", &settings.bEnableUnityLogs);
        ImGui::Spacing();

        if (ImGui::Button("Unhook")) {
            XGlobalUtility::bUnhook = true;
        }

        if (ImGui::Button("Donate")) {
            app::Application_OpenURL(reinterpret_cast<app::String*>(il2cpp_string_new("https://www.buymeacoffee.com/jadis0x")), nullptr);
        }

        ImGui::EndTabItem(); 
    }
}
