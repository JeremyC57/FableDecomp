#ifndef FABLE_ENGINE_NCAMERAMODES_CMOVEONCYLINDER_H
#define FABLE_ENGINE_NCAMERAMODES_CMOVEONCYLINDER_H
#include "engine/NCameraModes_CBase.h"
namespace NCameraModes {
class CMoveOnCylinder : public CBase {
public:
 static NCameraModes::CBaseInit * FABLE_FASTCALL StatAlloc();
 CMoveOnCylinder(const NCameraModes::CMoveOnCylinder & p0);
 CMoveOnCylinder(CGameCameraManager & p0, const CDefPointer<CCameraModeDef const > & p1);
 static NCameraModes::EMode FABLE_FASTCALL StatGetModeType();
 virtual NCameraModes::EMode GetModeType() const;
 virtual void OnInit();
 virtual void Update();
 virtual void OnDie();
 virtual bool IsRaiseUpOnHeroNearby() const;
 virtual bool IsStretchAroundHero() const;
private:
 C3DVector AxisX;
 C3DVector AxisY;
 C3DVector AxisZ;
 float Theta;
 float HeightParam;
public:
 virtual ~CMoveOnCylinder();
};
FABLE_STATIC_ASSERT(sizeof(CMoveOnCylinder)==0xd0);
}
#endif
