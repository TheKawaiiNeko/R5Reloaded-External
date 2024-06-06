#include "Cheat.h"
#include "..\Overlay\Overlay.h"

float vInfo[128]{};
CBaseEntity target{};

void Cheat::RenderInfo()
{
    ImGui::GetBackgroundDrawList()->AddText(ImVec2(8.f, 8.f), ImColor(1.f, 1.f, 1.f, 1.f), "EastWare | R5Reloaded v0.0");

    if (cfg.AimBot)
        Circle((float)cfg.GameRect.right, (float)cfg.GameRect.bottom, cfg.AimFov + 1.f, ImColor(1.f, 1.f, 1.f, 1.f), 1.0f);
}

void Cheat::RenderMenu()
{
    static int Index = 0;

    ImGui::SetNextWindowSize(ImVec2(400.f,300.f));
    ImGui::Begin("R5Reloaded", &cfg.ShowMenu, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    if (ImGui::Button("Aim", ImVec2((ImGui::GetContentRegionAvail().x / 2.f) - 8.f, 30.f)))
        Index = 0;
    ImGui::SameLine();
    if (ImGui::Button("Visual", ImVec2(ImGui::GetContentRegionAvail().x - 8.f, 30.f)))
        Index = 1;

    switch (Index)
    {
    case 0:
        ImGui::Checkbox("AimBot", &cfg.AimBot);

        ImGui::Checkbox("Visibility Check", &cfg.VisCheck);
        ImGui::Checkbox("NoSway", &cfg.NoSway);
        break;
    case 1:
        ImGui::Checkbox("ESP", &cfg.PlayerESP);

        ImGui::Checkbox("Box", &cfg.ESP_Box);
        ImGui::Checkbox("Line", &cfg.ESP_Line);
        ImGui::Checkbox("Distance", &cfg.ESP_Distance);
        ImGui::Checkbox("HealthBar", &cfg.ESP_HealthBar);
        break;
    default:
        break;
    }

    ImGui::End();
}

void Cheat::RenderESP()
{
    CBaseEntity* pLocal = &local;
    if (!pLocal->Update())
        return;

    float FOV = 0.f;
    float MinFov = 999.f;
    float MinDistance = 9999.f;
    Vector2 ScreenMiddle = { (float)cfg.GameRect.right / 2.f, (float)cfg.GameRect.bottom / 2.f };

    uintptr_t ViewRenderer = m.Read<uintptr_t>(m.BaseAddress + offset::ViewRender);
    Matrix ViewMatrix = m.Read<Matrix>(m.Read<uintptr_t>(ViewRenderer + offset::ViewMatrix));

    for (int i = 0; i < EntityList.size(); i++)
    {
        CBaseEntity *pEntity = &EntityList[i];
        if (!pEntity->Update())
            continue;

        //if (pEntity->Team == pLocal->Team)
            //continue;

        Vector2 g_Screen{};
        if (!WorldToScreen(ViewMatrix, cfg.GameRect, pEntity->pos, g_Screen))
            continue;

        Vector2 pTop{};
        Vector3 HeadPosition = pEntity->GetEntityBonePosition(8) + Vector3(0.f, 0.f, 6.f);
        if (!WorldToScreen(ViewMatrix, cfg.GameRect, HeadPosition, pTop))
            continue;

        float Height = abs(abs(pTop.y) - abs(g_Screen.y));
        float Width = Height / 2.f; // Half

        float distance = ((pEntity->pos - pLocal->pos).Length() * 0.01905f);
        bool visible = pEntity->VisTime > 0.f && pEntity->VisTime > vInfo[i];
        vInfo[i] = pEntity->VisTime;

        float bScale = Width / 2.f;
        ImColor color = visible ? ESP_Visible : ESP_Normal;

        if (cfg.ESP_Line)
            DrawLine(ImVec2((float)cfg.GameRect.right / 2.f, (float)cfg.GameRect.bottom), ImVec2(g_Screen.x, g_Screen.y), color, 1.f);

        if (cfg.ESP_Box)
        {
            DrawLine(ImVec2(g_Screen.x - bScale, pTop.y), ImVec2(g_Screen.x + bScale, pTop.y), color, 1.f);
            DrawLine(ImVec2(g_Screen.x - bScale, pTop.y), ImVec2(g_Screen.x - bScale, g_Screen.y), color, 1.f);
            DrawLine(ImVec2(g_Screen.x + bScale, pTop.y), ImVec2(g_Screen.x + bScale, g_Screen.y), color, 1.f);
            DrawLine(ImVec2(g_Screen.x - bScale, g_Screen.y), ImVec2(g_Screen.x + bScale, g_Screen.y), color, 1.f);
        }

        if (cfg.ESP_HealthBar)
        {
            HealthBar((g_Screen.x - bScale - 4.f), g_Screen.y, 2, -Height, pEntity->Health, pEntity->MaxHealth, true);

            if (pEntity->Shield != 0)
                ShieldBar(g_Screen.x - bScale - 8.f, g_Screen.y, 2, -Height, pEntity->Shield, pEntity->MaxShield, true);
        }

        if (cfg.ESP_Distance)
        {
            std::string DistStr = std::to_string((int)distance) + "m";
            String(ImVec2(g_Screen.x - (ImGui::CalcTextSize(DistStr.c_str()).x) / 2.f, g_Screen.y), ImColor(1.f, 1.f, 1.f, 1.f), DistStr.c_str());
        }

        if (cfg.AimBot)
        {
            if (cfg.VisCheck && !visible)
                continue;

            Vector2 pBone{};
            Vector3 pTargetBone = pEntity->GetEntityBonePosition(3);

            if (pTargetBone == Vector3(0.f, 0.f, 0.f))
                continue;
            else if (!WorldToScreen(ViewMatrix, cfg.GameRect, pTargetBone, pBone))
                continue;

            FOV = abs((ScreenMiddle - pBone).Length());

            if (FOV < cfg.AimFov)
            {
                target = EntityList[i];
                break;
            }
        }
    }

    if (cfg.AimBot)
    {
        this->AimBot(target);
        ZeroMemory(&target, sizeof(CBaseEntity));
    }
}