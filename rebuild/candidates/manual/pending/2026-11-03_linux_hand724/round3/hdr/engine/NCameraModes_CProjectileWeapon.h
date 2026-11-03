#ifndef FABLE_ENGINE_NCAMERAMODES_CPROJECTILEWEAPON_H
#define FABLE_ENGINE_NCAMERAMODES_CPROJECTILEWEAPON_H
// New header (chunk 724b, Codex pass-12 model): NCameraModes::CProjectileWeapon (vtable 0x01277804), native sizeof 0x108.
// Members after CBase (+0xA4): Ego_r names; the axis vectors sit at the unaligned +0xA9/+0xB5/+0xC1 (retail C3DVector members
// are byte-aligned), kept as opaque storage.
#include "engine/NCameraModes_CBase.h"
#include "engine/C3DVector.h"
class CCameraFixture;
class CThing;
namespace NCameraModes {
class CProjectileWeapon : public CBase {
public:
 CProjectileWeapon(CGameCameraManager&,const CDefPointer<const CCameraModeDef>&,float,float);
private:
#ifdef FABLE_CAMERA_TEST_ACCESS
 friend class ::CCameraFixture;
#endif
 C3DVector GetPosFromParams(float,float)const;
 C3DVector GetLookDirectionFromParams(float,float)const;
 float GetHeightParamForPostionWithPhi(const C3DVector&,float)const;
 // chunk 724c (FableWin.pdb ?<name>@CProjectileWeapon@NCameraModes@@ABEM..., private const; called by Update 0x00879900):
 float GetPhiForPosition(const C3DVector&)const;                          // 0x00879680
 float GetPhiForTargetedThing(const CThing&)const;                        // 0x008798C0
 float GetHeightParamForTargetedThingWithPhi(const CThing&,float)const;   // 0x00879560
public:
 virtual void Update();                                                   // slot 1 0x00879900 ?Update@CProjectileWeapon@NCameraModes@@UAEXXZ
private:
 long ShotFiredTimer; // +a4
 bool Finished; // +a8
 // Original C3DVector members have byte alignment (raw PDB +cd,+d9,+e5;
 // retail +a9,+b5,+c1). Opaque original storage avoids introducing packing.
 unsigned char XAxis[12];
 unsigned char YAxis[12];
 unsigned char ZAxis[12];
 float Phi,HeightParam,DeltaPhi,DeltaHeightParam,LockDeltaPhi,LockDeltaHeightParam,DeltaLockDeltaPhi,DeltaLockDeltaHeightParam;
 unsigned char Attractors[12]; // vector<C3DVector> +f0
 const float MaxZoomInFOV,MinZoomInFOV;
 float ZoomInFOV;
};
}
FABLE_STATIC_ASSERT(sizeof(NCameraModes::CProjectileWeapon)==0x108);
#endif
