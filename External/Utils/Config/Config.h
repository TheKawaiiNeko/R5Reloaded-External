#pragma once
#include <Windows.h>

struct Config
{
    // Status
    bool Run = false;
    RECT GameRect;
    POINT GamePoint;
    bool ShowMenu = false;

    // AimBot
    bool AimBot = true;
    bool VisCheck = true;
    bool NoSway = true;
    float AimFov = 150.f;

    // Visual
    bool PlayerESP = true;

    bool ESP_Box = true;
    bool ESP_Line = false;
    bool ESP_Distance = true;
    bool ESP_HealthBar = true;
    float ESP_MaxDistance = 750.f;

    // Key
    int MenuKey = VK_END;
    int AimKey  = VK_RBUTTON;
};

extern Config cfg;
extern bool IsKeyDown(int VK);