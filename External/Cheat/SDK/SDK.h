#pragma once
#include "..\..\Utils\Memory\Memory.h"
#include "..\..\Utils\SimpleMath\SimpleMath.h"
using namespace DirectX::SimpleMath;

namespace offset
{
	constexpr auto dwEntityList		= 0x1F33F78;
	constexpr auto dwLocalPlayer	= 0x22E93F8;
	constexpr auto ViewMatrix		= 0x1A93D0;
	constexpr auto ViewRender		= 0xD4138F0;

	constexpr auto m_vecAbsVelocity	= 0x140; // Vector3
	constexpr auto m_localOrigin	= 0x14C; // Vector3
	constexpr auto m_shieldHealth	= 0x170; // int
	constexpr auto m_shieldHealthMax= 0x174; // int
	constexpr auto m_iHealth		= 0x3E0; // int
	constexpr auto m_iTeamNum		= 0x3F0; // int
	constexpr auto m_localAngle		= 0x428; // Vector3
	constexpr auto m_iMaxHealth		= 0x510; // int
	constexpr auto m_lastvisibletime= 0x1754; // float
	constexpr auto camera_origin	= 0x1b68; // Vector3
	constexpr auto SwayAngle		= 0x2178; // Vector3
	constexpr auto ViewAngle		= 0x2188; // Vector3

	constexpr auto BoneMatrix		= 0xEE0; // Pointer
}

class CBaseEntity
{
public:
	uintptr_t ptr;

	int Team;
	int Health;
	int MaxHealth;
	int Shield;
	int MaxShield;
	float VisTime;
	Vector3 pos;
	Vector3 AbsVelocity;
	Vector3 CameraPosition;
	Vector3 SwayAngle;
	Vector3 ViewAngle;

	bool IsValid()
	{
		return ptr != NULL;
	}
	bool IsDead()
	{
		return pos == Vector3(0.f, 0.f, 0.f) || Health <= 0;
	}
	bool Update()
	{
		pos = m.Read<Vector3>(ptr + offset::m_localOrigin);
		Health = m.Read<int>(ptr + offset::m_iHealth);

		if (IsDead())
			return false;

		MaxHealth = m.Read<int>(ptr + offset::m_iMaxHealth);
		Shield = m.Read<int>(ptr + offset::m_shieldHealth);
		MaxShield = m.Read<int>(ptr + offset::m_shieldHealthMax);
		Team = m.Read<int>(ptr + offset::m_iTeamNum);
		VisTime = m.Read<float>(ptr + offset::m_lastvisibletime);
		AbsVelocity = m.Read<Vector3>(ptr + offset::m_vecAbsVelocity);
		CameraPosition = m.Read<Vector3>(ptr + offset::camera_origin);
		SwayAngle = m.Read<Vector3>(ptr + 0x2178);
		ViewAngle = m.Read<Vector3>(ptr + 0x2188);

		return true;
	}
	Vector3 GetEntityBonePosition(int BoneId);
};

struct GlowColor
{
	float R, G, B;
};

struct GlowMode
{
	int8_t GeneralGlowMode, BorderGlowMode, BorderSize, TransparentLevel;
};

struct ClientClass
{
	uint64_t pCreateFn;
	uint64_t pCreateEventFn;
	uint64_t pNetworkName;
	uint64_t pRecvTable;
	uint64_t pNext;
	uint32_t ClassID;
	uint32_t ClassSize;
};

struct Bone
{
	char pad_01[0xCC];
	float x;
	char pad_02[0xC];
	float y;
	char pad_03[0xC];
	float z;
};

extern bool WorldToScreen(Matrix& ViewMatrix, RECT Size, Vector3& vIn, Vector2& vOut);
extern void NormalizeAngles(Vector3& angle);
extern Vector3 CalcAngle(const Vector3& src, const Vector3& dst);
extern Vector3 GetPredict(uint64_t Entity, float dis, float BulletSpeed);