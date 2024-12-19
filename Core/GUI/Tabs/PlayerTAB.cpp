#include "pch-il2cpp.h"
#include "PlayerTAB.h"
#include "GUI/GUITheme.h" 
#include "Settings/Settings.h"
#include <imgui/imgui.h>
#include <Players/PlayerManager.h>
#include <Utilities/Utilities.h>
#include <Misc/Misc.h>

void PlayerTAB::Render()
{
    if (ImGui::BeginTabItem("Players")) {

        ImGui::Spacing();

        ImGui::Text("Local Player Options:");
        ImGui::Checkbox("Fullbright", &settings.bFlashlightModifier);
        ImGui::Checkbox("Enable Player Speed Boost", &settings.fastPlayer);
        if (settings.fastPlayer) {
            ImGui::SliderFloat("Player Speed", &settings.new_player_speed, 2.0f, 10.0f, "%.1f");
        }
        else {
            ImGui::Text("Speed adjustment is disabled.");
        }

        ImGui::Checkbox("Fly", &settings.bFly);
        if (settings.bFly) {
            ImGui::SliderFloat("Fly Speed", &settings.bFlySpeed, 1.0f, 15.0f, "%.1f");
        }
        else {
            ImGui::Text("Fly Speed adjustment is disabled.");
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.2f, 1.0f), "Player List:");

        auto& players = Players::PlayersManager::Instance().GetAllPlayers();
        if (!players.empty()) {
            if (ImGui::BeginTable("PlayersTable", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
                ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed, 50.0f);
                ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableSetupColumn("Actions", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableHeadersRow();

                for (size_t i = 0; i < players.size(); ++i) {
                    ImGui::TableNextRow();

                    // ID Kolonu
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("%zu", i + 1); // Oyuncu ID'si

                    // Name Kolonu
                    ImGui::TableSetColumnIndex(1);
                    if (players[i]) {
                        ImGui::Text("%s", XUtility::GameObjectObjectToString(players[i]).c_str()); // Oyuncu ismi
                    }
                    else {
                        ImGui::Text("unknown_player_name");
                    }

                    // Actions Kolonu
                    ImGui::TableSetColumnIndex(2);
                    if (ImGui::Button((std::string("Kill##") + std::to_string(i)).c_str())) {
                        Misc::Kill(players[i]); // Kill butonu
                    }
                    ImGui::SameLine(0, 10); // Butonlar arasında boşluk
                    if (ImGui::Button((std::string("Revive##") + std::to_string(i)).c_str())) {
                        Misc::Revive(players[i]); // Revive butonu
                    }
                    ImGui::SameLine(0, 10); // Butonlar arasında boşluk
                    if (ImGui::Button((std::string("Jumpscare##") + std::to_string(i)).c_str())) {
                        Misc::Jumpscare(players[i]); // Jumpscare butonu
                    }
                }
                ImGui::EndTable();
            }
        }
        else {
            ImGui::TextColored(ImVec4(1.0f, 0.2f, 0.2f, 1.0f), "No players found.");
        }

        ImGui::EndTabItem();
    }
}
