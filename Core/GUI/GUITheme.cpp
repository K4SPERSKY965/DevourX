#include "pch-il2cpp.h"

#include "GUI/GUITheme.h"
#include "Settings/Settings.h"
#include <imgui/imgui.h>

// color defines
#define DARK_BG(v)          ImVec4(0.07f, 0.05f, 0.09f, v)
#define DARK_ACCENT(v)      ImVec4(0.15f, 0.10f, 0.20f, v)
#define PRIMARY_COLOR(v)    ImVec4(0.40f, 0.20f, 0.60f, v)
#define SECONDARY_COLOR(v)  ImVec4(0.30f, 0.15f, 0.50f, v)
#define TEXT_BRIGHT(v)      ImVec4(0.90f, 0.85f, 0.95f, v)
#define TEXT_DIM(v)         ImVec4(0.60f, 0.55f, 0.65f, v)
#define BUTTON_HOVER(v)     ImVec4(0.50f, 0.25f, 0.75f, v)
#define BUTTON_ACTIVE(v)    ImVec4(0.45f, 0.20f, 0.70f, v)
#define BORDER_COLOR(v)     ImVec4(0.35f, 0.20f, 0.50f, v)

void ApplyTheme()
{
    auto& style = ImGui::GetStyle();

    // Renk ayarları
    style.Colors[ImGuiCol_Text] = TEXT_BRIGHT(1.00f);
    style.Colors[ImGuiCol_TextDisabled] = TEXT_DIM(1.00f);
    style.Colors[ImGuiCol_WindowBg] = DARK_BG(1.00f);
    style.Colors[ImGuiCol_ChildBg] = DARK_BG(1.00f);
    style.Colors[ImGuiCol_PopupBg] = DARK_ACCENT(1.00f);
    style.Colors[ImGuiCol_Border] = BORDER_COLOR(0.80f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    style.Colors[ImGuiCol_FrameBg] = DARK_ACCENT(1.00f);
    style.Colors[ImGuiCol_FrameBgHovered] = BUTTON_HOVER(1.00f);
    style.Colors[ImGuiCol_FrameBgActive] = BUTTON_ACTIVE(1.00f);
    style.Colors[ImGuiCol_TitleBg] = PRIMARY_COLOR(1.00f);
    style.Colors[ImGuiCol_TitleBgActive] = PRIMARY_COLOR(1.00f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = PRIMARY_COLOR(0.75f);
    style.Colors[ImGuiCol_MenuBarBg] = DARK_ACCENT(1.00f);
    style.Colors[ImGuiCol_ScrollbarBg] = DARK_ACCENT(1.00f);
    style.Colors[ImGuiCol_ScrollbarGrab] = PRIMARY_COLOR(1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = BUTTON_HOVER(1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = BUTTON_ACTIVE(1.00f);
    style.Colors[ImGuiCol_CheckMark] = PRIMARY_COLOR(1.00f);
    style.Colors[ImGuiCol_SliderGrab] = PRIMARY_COLOR(1.00f);
    style.Colors[ImGuiCol_SliderGrabActive] = BUTTON_ACTIVE(1.00f);
    style.Colors[ImGuiCol_Button] = SECONDARY_COLOR(1.00f);
    style.Colors[ImGuiCol_ButtonHovered] = BUTTON_HOVER(1.00f);
    style.Colors[ImGuiCol_ButtonActive] = BUTTON_ACTIVE(1.00f);
    style.Colors[ImGuiCol_Header] = PRIMARY_COLOR(1.00f);
    style.Colors[ImGuiCol_HeaderHovered] = BUTTON_HOVER(1.00f);
    style.Colors[ImGuiCol_HeaderActive] = BUTTON_ACTIVE(1.00f);
    style.Colors[ImGuiCol_Separator] = BORDER_COLOR(1.00f);
    style.Colors[ImGuiCol_SeparatorHovered] = BUTTON_HOVER(1.00f);
    style.Colors[ImGuiCol_SeparatorActive] = BUTTON_ACTIVE(1.00f);
    style.Colors[ImGuiCol_ResizeGrip] = BUTTON_HOVER(0.25f);
    style.Colors[ImGuiCol_ResizeGripHovered] = BUTTON_HOVER(0.65f);
    style.Colors[ImGuiCol_ResizeGripActive] = BUTTON_ACTIVE(0.90f);
    style.Colors[ImGuiCol_Tab] = SECONDARY_COLOR(1.00f);
    style.Colors[ImGuiCol_TabHovered] = BUTTON_HOVER(1.00f);
    style.Colors[ImGuiCol_TabActive] = PRIMARY_COLOR(1.00f);
    style.Colors[ImGuiCol_TabUnfocused] = DARK_ACCENT(1.00f);
    style.Colors[ImGuiCol_TabUnfocusedActive] = DARK_ACCENT(1.00f);

    style.FrameRounding = 6.0f;
    style.WindowRounding = 10.0f;
    style.PopupRounding = 6.0f;
    style.ScrollbarRounding = 6.0f;
    style.GrabRounding = 4.0f;
    style.TabRounding = 6.0f;

    // Padding & spacing
    style.WindowPadding = ImVec2(10, 10);
    style.FramePadding = ImVec2(8, 6);
    style.ItemSpacing = ImVec2(8, 8);
    style.ItemInnerSpacing = ImVec2(6, 6);

    // border
    style.FrameBorderSize = 1.0f;
    style.WindowBorderSize = 1.0f;
    style.ChildBorderSize = 1.0f;
    style.PopupBorderSize = 1.0f;

    // title align
    style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
}
