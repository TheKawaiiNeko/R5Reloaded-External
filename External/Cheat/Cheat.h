#pragma once
#include "..\ImGui\imgui.h"
#include "..\ImGui\imgui_impl_win32.h"
#include "..\ImGui\imgui_impl_dx11.h"
#include "..\Utils\Config\Config.h"
#include "SDK\SDK.h"
#include <vector>

class Cheat
{
public:
    // Render
	void RenderInfo();
	void RenderMenu();
	void RenderESP();

    // Cheat
    bool AimBot(CBaseEntity target);
    void UpdateList();
private:
    CBaseEntity local{};
    std::vector<CBaseEntity> EntityList;

    // Colors
    ImColor FOV_User    = { 1.f, 1.f, 1.f, 1.f };
    ImColor ESP_Normal  = { 1.f, 0.f, 0.f, 1.f };
    ImColor ESP_Visible = { 0.f, 1.f, 0.f, 1.f };

    void DrawLine(ImVec2 a, ImVec2 b, ImColor color, float width)
    {
        ImGui::GetBackgroundDrawList()->AddLine(a, b, color, width);
    }
    void Text(ImVec2 pos, ImColor color, const char* text_begin, const char* text_end, float wrap_width, const ImVec4* cpu_fine_clip_rect)
    {
        ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), pos, color, text_begin, text_end, wrap_width, cpu_fine_clip_rect);
    }
    void String(ImVec2 pos, ImColor color, const char* text)
    {
        Text(pos, color, text, text + strlen(text), 200, 0);
    }
    void RectFilled(float x0, float y0, float x1, float y1, ImColor color, float rounding, int rounding_corners_flags)
    {
        ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(x0, y0), ImVec2(x1, y1), color, rounding, rounding_corners_flags);
    }
    void HealthBar(float x, float y, float w, float h, int value, int v_max, bool Background)
    {
        RectFilled(x - 1, y + 1, x + w + 1, y + h - 1, ImColor(0.f, 0.f, 0.f, 0.6f), 0.f, 0);
        RectFilled(x, y, x + w, y + ((h / float(v_max)) * (float)value), ImColor(min(510 * (v_max - value) / 100, 255), min(510 * value / 100, 255), 25, 255), 0.0f, 0);
    }
    void ShieldBar(float x, float y, float w, float h, int value, int v_max, bool Background)
    {
        ImColor barColor = ImColor(1.f, 1.f, 1.f, 1.f);

        switch (v_max)
        {
        case 50:
            barColor = ImColor(1.f, 1.f, 1.f, 1.f);
            break;
        case 75:
            barColor = ImColor(0.f, 0.85f, 1.f, 1.f);
            break;
        case 100:
            barColor = ImColor(0.7f, 0.f, 1.f, 1.f);
            break;
        case 125:
            barColor = ImColor(1.f, 0.01f, 0.f, 1.f);
            break;
        default:
            barColor = ImColor(0.f, 0.f, 0.f, 0.f);
            break;
        }

        RectFilled(x - 1, y + 1, x + w + 1, y + h - 1, ImColor(0.f, 0.f, 0.f, 0.6f), 0.f, 0);
        RectFilled(x, y, x + w, y + ((h / float(v_max)) * (float)value), barColor, 0.0f, 0);
    }
    void Circle(float x, float y, float fov_size, ImColor color, float size)
    {
        ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(x / 2.f, y / 2.f), fov_size, color, 100, size);
    }
};

// Key names
static const char* KeyNames[] =
{
    "NONE",
    "Mouse Left",
    "Mouse Right",
    "Cancel",
    "Middle Center",
    "MouseSide1",
    "MouseSide2",
    "",
    "Backspace",
    "Tab",
    "",
    "",
    "Clear",
    "Enter",
    "",
    "",
    "Shift",
    "Ctrl",
    "Alt",
    "Pause",
    "CapsLock",
    "",
    "",
    "",
    "",
    "",
    "",
    "Escape",
    "",
    "",
    "",
    "",
    "Space",
    "Page Up",
    "Page Down",
    "End",
    "Home",
    "Left",
    "Up",
    "Right",
    "Down",
    "",
    "",
    "",
    "Print",
    "Insert",
    "Delete",
    "",
    "0",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "A",
    "B",
    "C",
    "D",
    "E",
    "F",
    "G",
    "H",
    "I",
    "J",
    "K",
    "L",
    "M",
    "N",
    "O",
    "P",
    "Q",
    "R",
    "S",
    "T",
    "U",
    "V",
    "W",
    "X",
    "Y",
    "Z",
    "",
    "",
    "",
    "",
    "",
    "Numpad 0",
    "Numpad 1",
    "Numpad 2",
    "Numpad 3",
    "Numpad 4",
    "Numpad 5",
    "Numpad 6",
    "Numpad 7",
    "Numpad 8",
    "Numpad 9",
    "Multiply",
    "Add",
    "",
    "Subtract",
    "Decimal",
    "Divide",
    "F1",
    "F2",
    "F3",
    "F4",
    "F5",
    "F6",
    "F7",
    "F8",
    "F9",
    "F10",
    "F11",
    "F12",
};