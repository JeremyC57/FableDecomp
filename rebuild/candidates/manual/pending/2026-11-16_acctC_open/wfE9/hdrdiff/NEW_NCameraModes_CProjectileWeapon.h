#ifndef FABLE_ENGINE_NCAMERAMODES_CPROJECTILEWEAPON_H
#define FABLE_ENGINE_NCAMERAMODES_CPROJECTILEWEAPON_H
// NEW header (wfE9 overlay, 2026-11-15): NCameraModes::CProjectileWeapon (vtable 0x01277804), native sizeof 0x108.
// Member functions: Ego_r.pdb / FableWin.pdb manglings (?Update@CProjectileWeapon@NCameraModes@@UAEXXZ etc.; access and const-ness
// from the mangling letters). Data members after CBase (+0xA4): Ego_r names / types (struct_layouts_egor.tsv), retail offsets from
// pending hand-back 2026-11-03_linux_hand725 round2 (chunk 724b): the axis vectors sit at the unaligned +0xA9/+0xB5/+0xC1 (retail
// C3DVector members are byte-aligned there), kept as opaque storage.
#include "engine/NCameraModes_CBase.h"
#include "engine/C3DVector.h"
class CCameraFixture;
class CThing;
namespace NCameraModes {
class CProjectileWeapon : public CBase {
public:
 CProjectileWeapon(CGameCameraManager & p0, const CDefPointer<CCameraModeDef const > & p1, float p2, float p3);
 virtual ~CProjectileWeapon();
 static void FABLE_FASTCALL StatRegisterMode();
 static NCameraModes::EMode FABLE_FASTCALL StatGetModeType();
 virtual NCameraModes::EMode GetModeType() const;
 virtual void Update();
 virtual void OnInit();
 virtual void OnDie();
 virtual bool IsFinished() const;
 virtual bool IsRaiseUpOnHeroNearby() const;
 virtual bool IsShortenOnOcclusion() const;
 virtual float GetDefaultFOV() const;
 void SetShotFired();
 void SetShotAiming();
 void AddAttractor(const C3DVector & p0);
 void ClearAttractors();
private:
#ifdef FABLE_CAMERA_TEST_ACCESS
 friend class ::CCameraFixture;
#endif
 void SetUpParams();
 C3DVector GetBaseOffset() const;
 C3DVector GetCamPosOffsetXY(float p0) const;
 C3DVector GetPosFromParams(float p0, float p1) const;
 C3DVector GetLookDirectionFromParams(float p0, float p1) const;
 C3DVector GetLookDirectionXYFromParams(float p0, float p1) const;
 float GetPhiForPosition(const C3DVector & p0) const;
 float GetHeightParamForPostionWithPhi(const C3DVector & p0, float p1) const;
 float GetPhiForTargetedThing(const CThing & p0) const;
 float GetHeightParamForTargetedThingWithPhi(const CThing & p0, float p1) const;
 long ShotFiredTimer;            // +0xA4
 bool Finished;                  // +0xA8
 unsigned char XAxis[12];        // +0xA9 C3DVector (byte-aligned in retail)
 unsigned char YAxis[12];        // +0xB5 C3DVector
 unsigned char ZAxis[12];        // +0xC1 C3DVector
 float Phi;                      // +0xD0
 float HeightParam;              // +0xD4
 float DeltaPhi;                 // +0xD8
 float DeltaHeightParam;         // +0xDC
 float LockDeltaPhi;             // +0xE0
 float LockDeltaHeightParam;     // +0xE4
 float DeltaLockDeltaPhi;        // +0xE8
 float DeltaLockDeltaHeightParam;// +0xEC
 unsigned char Attractors[12];   // +0xF0 vector<C3DVector>
 const float MaxZoomInFOV;       // +0xFC
 const float MinZoomInFOV;       // +0x100
 float ZoomInFOV;                // +0x104
};
}
FABLE_STATIC_ASSERT(sizeof(NCameraModes::CProjectileWeapon)==0x108);
#endif
