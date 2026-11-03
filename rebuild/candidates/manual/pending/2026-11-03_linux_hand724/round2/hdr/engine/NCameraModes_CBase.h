#ifndef FABLE_ENGINE_NCAMERAMODES_CBASE_H
#define FABLE_ENGINE_NCAMERAMODES_CBASE_H
// NCameraModes::CBase -- root of the game-camera modes (new header; chunk 725 slots 0-2, completed in chunk 724b).
// RTTI .?AVCBase@NCameraModes@@, vtable 0x0124DD74, 24 slots: 0 the deleting destructor 0x0056E1F0, 1 Update / 2 GetModeType
// _purecall, 3..20 base bodies, 21 OnInit / 22 OnDie _purecall, 23 OnReset 0x0056E1E0 (CProjectileWeapon 0x01277804 overrides
// 1 = 0x00879900, 2 = 0x0087B5F0, 21 = 0x00878F50, 22 = 0x00878B40). Names: FableWin.pdb ?<name>@CBase@NCameraModes@@ members
// (U virtual, M protected virtual), in slot order; the catalog agrees at 5 PreparePrimitivesForRendering 0x0084D090, 15
// GetObservedThingCurrentMeshHeight 0x0084D8B0, 17 UpdateSubComponents 0x0084D2C0, 18 GetDefaultFOV 0x0084D790, 23 OnReset.
//   1 Update()                    -- CProjectileWeapon slot 1 0x00879900 (FableWin ?Update@CProjectileWeapon@NCameraModes@@UAEXXZ;
//                                    OnInit 0x00878F50 tail-jumps through [vtbl+4]).
//   2 GetModeType() const         -- 21 overrides ?GetModeType@<Mode>@NCameraModes@@UBE?AW4EMode@2@XZ in FableWin.pdb; CProjectileWeapon's
//                                    0x0087B5F0 is `mov eax,0Fh ; ret`; CGameCameraManager::GetCameraModeType 0x00697AB0 /
//                                    HasCameraMode 0x00697AC0 call it on CurrentMode.
// EMode is forward-declared (MS extension, as CNavigatorBase.h's ENavigatorReturn): no row yet needs its enumerators.
// Members (chunk 724b, from the Codex pass-12 model, native size 0xA4; Ego_r's 0xC8 carries Dinkumware containers):
// DefaultFOV +4, DefaultRollAngle +8, DefaultObservedPositionZOffset +0xC, LookAt (CPhysicsState) +0x10, CameraManager +0x40
// (GetObservedThing 0x0084F1B0: `mov ecx,[ecx+40h] ; add ecx,100h ; jmp CBaseIntelligentPointer::GetPItem`), PCameraModeDef
// +0x44, SubComponents (STLport vector) +0x48, GoTo (CPhysicsState) +0x54, CameraFOV +0x84, RollAngle +0x88,
// ObservedPositionZOffset +0x8C, RightStickVector +0x90, LeftStickVector +0x98, ViewOccluded +0xA0.
// Needs FABLE_RETAIL_DEF_CLASS_BASE (engine/CDefPointer.h).
#include "rebuild_abi.h"
#include "engine/CDefPointer.h"
#include "engine/CCameraModeDef.h"
class CEnginePrimitivePrepareInfo;
class CThingGameObject;
class CGameCameraManager;
namespace NCameraModes {
enum EMode;
class CBase {
public:
    CBase(CGameCameraManager&, const CDefPointer<const CCameraModeDef>&);
    virtual ~CBase();                                    // slot 0
    virtual void Update() = 0;                           // slot 1
    virtual EMode GetModeType() const = 0;               // slot 2
    virtual void OnCameraInsideObservedThing();          // 3 0x0056E150
    virtual bool IsUseRightStickLookAround() const;      // 4 0x0056E160
    virtual void PreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&) const;   // 5 0x0084D090
    virtual bool IsShortenOnOcclusion() const;           // 6 0x0056E170
    virtual bool IsRaiseUpOnHeroNearby() const;          // 7 0x0084D7C0
    virtual bool IsStretchAroundHero() const;            // 8 0x0084D7D0
    virtual bool IsAvoidLandscape() const;               // 9 0x0056E180
    virtual bool IsFOVChange() const;                    // 10 0x0056E190
    virtual bool IsTrackMeleeOpponents() const;          // 11 0x0056E1A0
    virtual bool IsTrackMeleeOpponentsAutoFOVChange() const;   // 12 0x0084CDE0
    virtual bool IsFinished() const;                     // 13 0x0056E1B0
    virtual bool IsStringAndCage() const;                // 14 0x0056E1C0
    virtual float GetObservedThingCurrentMeshHeight() const;   // 15 0x0084D8B0
    virtual void Recentre();                             // 16 0x0056E1D0
    virtual void UpdateSubComponents();                  // 17 0x0084D2C0
    virtual float GetDefaultFOV() const;                 // 18 0x0084D790
    virtual float GetDefaultRollAngle() const;           // 19 0x0084D7A0
    virtual float GetDefaultObservedPositionZOffset() const;   // 20 0x0084D7B0
protected:
    virtual void OnInit() = 0;                           // 21
    virtual void OnDie() = 0;                            // 22
    virtual void OnReset();                              // 23 0x0056E1E0
public:
    const CThingGameObject* GetObservedThing() const;    // 0x0084F1B0 ?GetObservedThing@CBase@NCameraModes@@QBEPBVCThingGameObject@@XZ
    const CDefPointer<const CCameraModeDef>& GetCameraModeDef() const { return PCameraModeDef; }   // FableWin QBE, inline
protected:
    float DefaultFOV, DefaultRollAngle, DefaultObservedPositionZOffset;   // +0x04
    unsigned char LookAt[0x30];                          // CPhysicsState +0x10
private:
    CGameCameraManager& CameraManager;                   // +0x40
    CDefPointer<const CCameraModeDef> PCameraModeDef;    // +0x44
    unsigned char SubComponents[0xc];                    // vector<CCountedPointer<CSubComponentBase> > +0x48
    unsigned char GoTo[0x30];                            // CPhysicsState +0x54
    float CameraFOV, RollAngle, ObservedPositionZOffset; // +0x84
    unsigned char RightStickVector[8];                   // C2DVector +0x90
    unsigned char LeftStickVector[8];                    // C2DVector +0x98
    bool ViewOccluded;                                   // +0xA0
};
} // namespace NCameraModes
FABLE_STATIC_ASSERT(sizeof(NCameraModes::CBase) == 0xa4);
#endif // FABLE_ENGINE_NCAMERAMODES_CBASE_H
