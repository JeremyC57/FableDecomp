#ifndef FABLE_ENGINE_NCAMERAMODES_CPROJECTILEWEAPON_H
#define FABLE_ENGINE_NCAMERAMODES_CPROJECTILEWEAPON_H
#ifdef FABLE_RETAIL_NCAMERAMODES_CPROJECTILEWEAPON_HEIGHT
#include "engine/NCameraModes_CBase.h"
#include "engine/C3DVector.h"
#include "stlport/fable_containers.h"
class CGameCameraManager;
namespace NCameraModes {
class CProjectileWeapon : public CBase {
public:
 CProjectileWeapon(CGameCameraManager&,const CDefPointer<const CCameraModeDef>&,float,float);
#ifdef FABLE_RETAIL_PROJECTILE_UPDATE_PASS28
 virtual void Update();
private:
#ifdef FABLE_RETAIL_CPROJECTILEWEAPON_INVERTY_PASS28
 // Ego_d PUB32 and FableWin PUB32/GDATA32 at 04AC5B41; retail reads byte 013BAF98 before negating the vertical input.
 static bool InvertY;
#endif
 void SetUpParams();
 C3DVector GetBaseOffset() const;
 C3DVector GetCamPosOffsetXY(float p0) const;
 C3DVector GetLookDirectionXYFromParams(float p0, float p1) const;
 float GetPhiForPosition(const C3DVector & p0) const;
 float GetPhiForTargetedThing(const CThing & p0) const;
 float GetHeightParamForTargetedThingWithPhi(const CThing & p0, float p1) const;
#endif
private:
 C3DVector GetPosFromParams(float,float)const;
 C3DVector GetLookDirectionFromParams(float,float)const;
 float GetHeightParamForPostionWithPhi(const C3DVector&,float)const;
 long ShotFiredTimer; // +a4
 bool Finished; // +a8
 // Original C3DVector members have byte alignment (raw PDB +cd,+d9,+e5;
 // retail +a9,+b5,+c1). Opaque original storage avoids introducing packing.
 C3DVector XAxis;
 C3DVector YAxis;
 C3DVector ZAxis;
 float Phi,HeightParam,DeltaPhi,DeltaHeightParam,LockDeltaPhi,LockDeltaHeightParam,DeltaLockDeltaPhi,DeltaLockDeltaHeightParam;
 std::vector<C3DVector> Attractors; // vector<C3DVector> +f0
 const float MaxZoomInFOV,MinZoomInFOV;
 float ZoomInFOV;
};
}
FABLE_STATIC_ASSERT_AT(29,sizeof(NCameraModes::CProjectileWeapon)==0x108);
#endif // FABLE_RETAIL_NCAMERAMODES_CPROJECTILEWEAPON_HEIGHT
#endif
