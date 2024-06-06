#include "SDK.h"

Vector3 CBaseEntity::GetEntityBonePosition(int BoneId)
{
	Vector3 vOut = this->pos;
	uint64_t pBoneArray = m.Read<uint64_t>(ptr + offset::BoneMatrix);
	Bone EntityBone = m.Read<Bone>(pBoneArray + BoneId * 0x30);

	vOut.x += EntityBone.x;
	vOut.y += EntityBone.y;
	vOut.z += EntityBone.z;

	return vOut;
}

bool WorldToScreen(Matrix& ViewMatrix, RECT Size, Vector3& vIn, Vector2& vOut)
{
	float w = ViewMatrix.m[3][0] * vIn.x + ViewMatrix.m[3][1] * vIn.y + ViewMatrix.m[3][2] * vIn.z + ViewMatrix.m[3][3];

	if (w < 0.01)
		return false;

	vOut.x = ViewMatrix.m[0][0] * vIn.x + ViewMatrix.m[0][1] * vIn.y + ViewMatrix.m[0][2] * vIn.z + ViewMatrix.m[0][3];
	vOut.y = ViewMatrix.m[1][0] * vIn.x + ViewMatrix.m[1][1] * vIn.y + ViewMatrix.m[1][2] * vIn.z + ViewMatrix.m[1][3];

	float invw = 1.0f / w;

	vOut.x *= invw;
	vOut.y *= invw;

	float x = Size.right / 2;
	float y = Size.bottom / 2;

	x += 0.5 * vOut.x * Size.right + 0.5;
	y -= 0.5 * vOut.y * Size.bottom + 0.5;

	vOut.x = x;
	vOut.y = y;

	return true;
}

void NormalizeAngles(Vector3& angle)
{
	while (angle.x > 89.0f)
		angle.x -= 180.f;

	while (angle.x < -89.0f)
		angle.x += 180.f;

	while (angle.y > 180.f)
		angle.y -= 360.f;

	while (angle.y < -180.f)
		angle.y += 360.f;
}

Vector3 CalcAngle(const Vector3& src, const Vector3& dst)
{
	Vector3 angle = Vector3();
	Vector3 delta = Vector3((src.x - dst.x), (src.y - dst.y), (src.z - dst.z));

	double hyp = sqrt(delta.x * delta.x + delta.y * delta.y);

	angle.x = atan(delta.z / hyp) * (180.0f / 3.1415926535);
	angle.y = atan(delta.y / delta.x) * (180.0f / 3.1415926535);
	angle.z = 0;
	if (delta.x >= 0.0) angle.y += 180.0f;

	return angle;
}

// 基本的に m_latestPrimaryWeapons からデータを取得できないと見た方がいいので、それっぽい値を入れてユーザーに調節させる
Vector3 GetPredict(uint64_t Entity, float dis, float BulletSpeed)
{
	Vector3 predict = Vector3(0.f, 0.f, 0.f);

	float bulletTime = dis / BulletSpeed;
	Vector3 pd = m.Read<Vector3>(Entity + offset::m_vecAbsVelocity);
	predict.x = pd.x * bulletTime;
	predict.y = pd.y * bulletTime;
	predict.z = (150.f * 0.5f * (bulletTime * bulletTime));

	return predict;
}