#include "Cheat.h"

float vAInfo[15000];

void Cheat::UpdateList()
{
	while (cfg.Run)
	{
        std::vector<CBaseEntity> EntityTemp;
		local.ptr = m.Read<uintptr_t>(m.BaseAddress + offset::dwLocalPlayer);

		for (int i = 0; i < 128; i++)
		{
            CBaseEntity entity{};
			entity.ptr = m.Read<uintptr_t>(m.BaseAddress + offset::dwEntityList + (i * 0x20));

			if (!entity.IsValid() || entity.ptr == local.ptr)
				continue;

            EntityTemp.push_back(entity);
		}

        EntityList = EntityTemp;
        EntityTemp.clear();

        Sleep(500);
	}
}

// AimBot
bool Cheat::AimBot(CBaseEntity target)
{
    if (!IsKeyDown(cfg.AimKey))
        return false;

    if (target.ptr != NULL)
    {
        CBaseEntity* pLocal = &local;
        CBaseEntity* pTarget = &target;
        Vector2 ScreenMiddle = { (float)cfg.GameRect.right / 2.f, (float)cfg.GameRect.bottom / 2.f };
        Vector3 TargetBone = pTarget->GetEntityBonePosition(3); 

        if (TargetBone == Vector3(0.f, 0.f, 0.f))
            return false;

        // WorldToScreen
        Vector2 g_Screen{};
        uint64_t ViewRenderer = m.Read<uint64_t>(m.BaseAddress + offset::ViewRender);
        Matrix ViewMatrix = m.Read<Matrix>(m.Read<uint64_t>(ViewRenderer + offset::ViewMatrix));
        if (!WorldToScreen(ViewMatrix, cfg.GameRect, TargetBone, g_Screen))
            return false;

        // Angle
        Vector3 AimAngle = CalcAngle(pLocal->CameraPosition, TargetBone);
        Vector3 TmpViewAngle = pLocal->ViewAngle;
        Vector3 Delta = (AimAngle - TmpViewAngle);

        // NoRecoil, NoSway (Aiming Only
        if (cfg.NoSway)
        {
            Vector3 Breath = pLocal->SwayAngle - TmpViewAngle;

            if (Breath.x != 0.f || Breath.y != 0.f)
                Delta = (AimAngle - TmpViewAngle) - Breath;
        }

        NormalizeAngles(Delta);
        Vector3 newAngle = TmpViewAngle + Delta;
        NormalizeAngles(newAngle);

        if (newAngle.x != 0.f || newAngle.y != 0.f)
            m.Write<Vector3>(pLocal->ptr + 0x2188, newAngle);
    }
}