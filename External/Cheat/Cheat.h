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
    // System
    bool Init();
    void RainbowGenerator();
    HWND GameWindow = nullptr;
    ImColor Rainbow = { 1.f, 1.f, 1.f, 1.f };

    // Render
	void RenderInfo();
	void RenderMenu();
	void RenderESP();
    void RenderSpectatorList();

    // Cheat
    bool AimBot(Vector2& TargetPosition);
    void SetDefaultFOV();
    void Misc();
private:
    float DefaultFov, DefaultViewModelFov;
    void GetFOV(float& Normal, float& ViewModel);
    void SetFOV(const float& v1, const float& v2);
    bool SetBPS(int value);
    void KeyBinder(int &target_key);

    int CurrentBPS = 0;
    std::vector<std::string> SpectatorNameList;

    // Colors
    ImColor FOV_User    = { 1.f, 1.f, 1.f, 1.f };
    ImColor ESP_Normal  = { 1.f, 1.f, 1.f, 1.f };
    ImColor ESP_Visible = { 1.f, 1.f, 0.f, 1.f };
    ImColor ESP_Team    = { 0.f, 0.75f, 1.f, 1.f };
    ImColor ESP_Vehicle = { 1.f, 0.f, 0.f, 1.f };
    ImColor ESP_Filled  = { 0.f, 0.f, 0.f, 0.3f };
    ImColor ESP_Skeleton = { 1.f, 1.f, 1.f, 0.9f };

    void DrawLine(ImVec2 a, ImVec2 b, ImColor color, float width)
    {
        ImGui::GetBackgroundDrawList()->AddLine(a, b, color, width);
    }
    void DrawBox(int x, int y, int w, int h, ImColor color)
    {
        DrawLine(ImVec2(x, y), ImVec2(x + w, y), color, 1.0f);
        DrawLine(ImVec2(x, y), ImVec2(x, y + h), color, 1.0f);
        DrawLine(ImVec2(x + w, y), ImVec2(x + w, y + h), color, 1.0f);
        DrawLine(ImVec2(x, y + h), ImVec2(x + w, y + h), color, 1.0f);
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
    void HealthBar(float x, float y, float w, float h, int value, int v_max)
    {
        if (value < 0)
            value = 0;

        RectFilled(x, y, x + w, y + h, ImColor(0.f, 0.f, 0.f, 0.725f), 0.f, 0);
        RectFilled(x, y, x + w, y + ((h / float(v_max)) * (float)value), ImColor(min(510 * (v_max - value) / 100, 255), min(510 * value / 100, 255), 25, 255), 0.0f, 0);
    }
    void Circle(float x, float y, float fov_size, ImColor color, float size)
    {
        ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(x / 2.f, y / 2.f), fov_size, color, 100, size);
    }
    void StringEx(std::string Text, ImVec2 Pos, ImColor Color, float FontSize, bool KeepCenter)
    {
        if (!KeepCenter)
        {
            ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), FontSize, ImVec2(Pos), Color, Text.c_str());
        }
        else
        {
            float TextWidth = ImGui::GetFont()->CalcTextSizeA(FontSize, FLT_MAX, 0.f, Text.c_str()).x;
            ImVec2 Pos_ = { Pos.x - TextWidth / 2,Pos.y };
            ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), FontSize, Pos_, Color, Text.c_str());
        }
    }
    void StrokeText(std::string Text, ImVec2 Pos, ImColor Color, float FontSize, bool KeepCenter)
    {
        StringEx(Text, ImVec2(Pos.x - 1, Pos.y + 1), ImColor(0, 0, 0), FontSize, KeepCenter);
        StringEx(Text, ImVec2(Pos.x - 1, Pos.y - 1), ImColor(0, 0, 0), FontSize, KeepCenter);
        StringEx(Text, ImVec2(Pos.x + 1, Pos.y + 1), ImColor(0, 0, 0), FontSize, KeepCenter);
        StringEx(Text, ImVec2(Pos.x + 1, Pos.y - 1), ImColor(0, 0, 0), FontSize, KeepCenter);
        StringEx(Text, Pos, Color, FontSize, KeepCenter);
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