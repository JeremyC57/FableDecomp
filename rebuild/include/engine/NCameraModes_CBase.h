#ifndef FABLE_ENGINE_NCAMERAMODES_CBASE_H
#define FABLE_ENGINE_NCAMERAMODES_CBASE_H
#ifdef FABLE_RETAIL_NCAMERAMODES_CBASE_CYLINDER
#include "rebuild_abi.h"
#include "engine/CDefPointer.h"
#include "engine/CCountedPointer.h"
#include "engine/NCameraModes_CPhysicsState.h"
#include "engine/C2DVector.h"
class CCameraModeDef; class CGameCameraManager; class CThingGameObject; class CWorld; class CCamera; class CEnginePrimitivePrepareInfo;
namespace NCameraModes {
class CSubComponentBase; class CBaseInit; class CTools; enum EMode; enum ESubComponentType;
class CBase {
public:
 CBase(const NCameraModes::CBase & p0);
 CBase(CGameCameraManager & p0, const CDefPointer<CCameraModeDef const > & p1);
 virtual ~CBase();
 virtual void Update() = 0;
 virtual NCameraModes::EMode GetModeType() const = 0;
 virtual void OnCameraInsideObservedThing();
 void Init();
 void Die();
 virtual bool IsUseRightStickLookAround() const;
 virtual void PreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo & p0) const;
 virtual bool IsShortenOnOcclusion() const;
 virtual bool IsRaiseUpOnHeroNearby() const;
 virtual bool IsStretchAroundHero() const;
 virtual bool IsAvoidLandscape() const;
 virtual bool IsFOVChange() const;
 virtual bool IsTrackMeleeOpponents() const;
 virtual bool IsTrackMeleeOpponentsAutoFOVChange() const;
 virtual bool IsFinished() const;
 virtual bool IsStringAndCage() const;
 virtual float GetObservedThingCurrentMeshHeight() const;
 virtual void Recentre();
 void SetRightStickLookAround(const C2DVector & p0);
 void SetLeftStickLookAround(const C2DVector & p0);
 void Reset();
 void UpdateTrackMeleeOpponents(const C3DVector & p0, C3DVector & p1) const;
 void FrameUpdate();
 void TransferPersistantBaseData(const NCameraModes::CBase & p0);
 bool HasSubComponent(NCameraModes::ESubComponentType p0);
 CCountedPointer<NCameraModes::CSubComponentBase> GetSubComponent(NCameraModes::ESubComponentType p0);
 void AddSubComponent(CCountedPointer<NCameraModes::CSubComponentBase> p0, bool p1);
 void RemoveSubComponent(NCameraModes::ESubComponentType p0);
 virtual void UpdateSubComponents();
 void TransferSubComponentsFrom(const NCameraModes::CBase & p0);
 const CThingGameObject * GetObservedThing() const;
 const NCameraModes::CPhysicsState & GetGoToPhase() const;
 NCameraModes::CPhysicsState & GetGoToPhase();
 void SetGoToPhase(const NCameraModes::CPhysicsState & p0);
 const NCameraModes::CPhysicsState & GetLookAtPhase() const;
 NCameraModes::CPhysicsState & GetLookAtPhase();
 void SetLookAtPhase(const NCameraModes::CPhysicsState & p0);
 const C3DVector & GetPosition() const;
 void SetPosition(const C3DVector & p0);
 const C3DVector & GetObservedPos() const;
 void SetObservedPos(const C3DVector & p0);
 void SetFOV(float p0);
#ifndef FABLE_RETAIL_NCAMERAMODES_CBASE_INLINE_ACCESSORS // agentFAM04 (2026-11-11)
 float GetFOV() const;
#else // FABLE_RETAIL_NCAMERAMODES_CBASE_INLINE_ACCESSORS
 // agentFAM04 (2026-11-11): Ego_r.pdb ?GetFOV@CBase@NCameraModes@@QBEMXZ, inline in retail (CRotateAround::GetDefaultFOV 0x0087BD40 reads CameraFOV +0x84 in place).
 float GetFOV() const { return CameraFOV; }
#endif // FABLE_RETAIL_NCAMERAMODES_CBASE_INLINE_ACCESSORS
 void SetRollAngle(float p0);
#ifndef FABLE_RETAIL_NCAMERAMODES_CBASE_INLINE_ACCESSORS // agentFAM04 (2026-11-11)
 float GetRollAngle() const;
#else // FABLE_RETAIL_NCAMERAMODES_CBASE_INLINE_ACCESSORS
 // agentFAM04 (2026-11-11): inline in retail (CRotateAround / CZTargetNew::GetDefaultRollAngle 0x0087BD50 / 0x00871E80 read RollAngle +0x88 in place).
 float GetRollAngle() const { return RollAngle; }
#endif // FABLE_RETAIL_NCAMERAMODES_CBASE_INLINE_ACCESSORS
 void SetObservedPositionZOffset(float p0);
#ifndef FABLE_RETAIL_NCAMERAMODES_CBASE_INLINE_ACCESSORS // agentFAM04 (2026-11-11)
 float GetObservedPositionZOffset() const;
#else // FABLE_RETAIL_NCAMERAMODES_CBASE_INLINE_ACCESSORS
 // agentFAM04 (2026-11-11): inline in retail (GetDefaultObservedPositionZOffset 0x0087BD60 / 0x00871E90 read +0x8c in place).
 float GetObservedPositionZOffset() const { return ObservedPositionZOffset; }
#endif // FABLE_RETAIL_NCAMERAMODES_CBASE_INLINE_ACCESSORS
#ifndef FABLE_RETAIL_NCAMERAMODES_CBASE_INLINE_ACCESSORS // agentFAM04 (2026-11-11)
 const CDefPointer<CCameraModeDef const > & GetCameraModeDef() const;
#else // FABLE_RETAIL_NCAMERAMODES_CBASE_INLINE_ACCESSORS
 // agentFAM04 (2026-11-11): inline in retail (CBase::GetDefaultRollAngle 0x0084D7A0 reads the def pointer +0x44 in place).
 const CDefPointer<CCameraModeDef const > & GetCameraModeDef() const { return PCameraModeDef; }
#endif // FABLE_RETAIL_NCAMERAMODES_CBASE_INLINE_ACCESSORS
 virtual float GetDefaultFOV() const;
 virtual float GetDefaultRollAngle() const;
 virtual float GetDefaultObservedPositionZOffset() const;
 void SetDefaultFOV(float p0);
 void SetDefaultRollAngle(float p0);
 void SetDefaultObservedPositionZOffset(float p0);
 unsigned long * GetPWorldSeed() const;
 bool IsCameraViewPosOccluded() const;
protected:
 void AddAttractorGoTo(const C3DVector & p0, float p1);
 void AddIncidentalAttractorGoTo(const C3DVector & p0, float p1);
 void AddAttractorLookAt(const C3DVector & p0, float p1);
 void AddIncidentalAttractorLookAt(const C3DVector & p0, float p1);
 virtual void OnInit() = 0;
 virtual void OnDie() = 0;
 virtual void OnReset();
 void UpdateCameraStretchAroundHero(C3DVector & p0, C3DVector & p1) const;
 void UpdateCameraOcclusion(C3DVector & p0);
 bool IsCameraOcclusionFromTo(const C3DVector & p0, const C3DVector & p1, C3DVector & p2) const;
 void UpdateFixLookDirection(const C3DVector & p0, const C3DVector & p1, C3DVector & p2, float p3) const;
 void UpdateRaiseUp(C3DVector & p0) const;
 void SetGoToCullPhysics(bool p0);
 CGameCameraManager & GetCameraManager() { return CameraManager; }
 const CGameCameraManager & GetCameraManager() const { return CameraManager; }
 const NCameraModes::CTools & GetCamTools() const;
 const CWorld & GetWorld() const;
 void SetCameraModeDef(const CDefPointer<CCameraModeDef const > & p0);
 const C2DVector & GetRightStickVector() const;
 const C2DVector & GetLeftStickVector() const;
 CCamera & GetCamera();
 const CCamera & GetCamera() const;
 float DefaultFOV;
 float DefaultRollAngle;
 float DefaultObservedPositionZOffset;
 NCameraModes::CPhysicsState LookAt;
private:
 CGameCameraManager & CameraManager;
 CDefPointer<CCameraModeDef const > PCameraModeDef;
 std::vector<CCountedPointer<NCameraModes::CSubComponentBase>,std::allocator<CCountedPointer<NCameraModes::CSubComponentBase> > > SubComponents;
 NCameraModes::CPhysicsState GoTo;
 float CameraFOV;
 float RollAngle;
 float ObservedPositionZOffset;
 C2DVector RightStickVector;
 C2DVector LeftStickVector;
 bool ViewOccluded;
#ifdef FABLE_RETAIL_NCAMERAMODES_CBASE_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals missing from this declaration, declaration only (no layout change).
    // Slots: the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots; spellings: Ego_r / FableWin name sets.
public:
    virtual void OnCameraInsideObservedThing();  // slot 3, 0x0056E150
public:
    virtual bool IsUseRightStickLookAround() const;  // slot 4, 0x0056E160
public:
    virtual bool IsShortenOnOcclusion() const;  // slot 6, 0x0056E170
public:
    virtual bool IsAvoidLandscape() const;  // slot 9, 0x0056E180
public:
    virtual bool IsFOVChange() const;  // slot 10, 0x0056E190
public:
    virtual bool IsTrackMeleeOpponents() const;  // slot 11, 0x0056E1A0
public:
    virtual bool IsFinished() const;  // slot 13, 0x0056E1B0
public:
    virtual bool IsStringAndCage() const;  // slot 14, 0x0056E1C0
public:
    virtual void Recentre();  // slot 16, 0x0056E1D0
public:
#endif
};
FABLE_STATIC_ASSERT(sizeof(CBase)==0xa4);
}

#else
// NCameraModes::CBase -- root of the game-camera modes (hand-written, chunk 725; new header).
// RTTI .?AVCBase@NCameraModes@@, vtable 0x0124DD74: slot 0 the deleting destructor 0x0056E1F0, slots 1 and 2 _purecall,
// slots 3.. base bodies (0x0056E150 ..), 21 OnInit, 22 OnDie (CProjectileWeapon 0x00878F50 / 0x00878B40). Only slots 0-2 are
// modelled here:
//   1 Update()                    -- CProjectileWeapon slot 1 0x00879900 (FableWin ?Update@CProjectileWeapon@NCameraModes@@UAEXXZ;
//                                    OnInit 0x00878F50 tail-jumps through [vtbl+4]).
//   2 GetModeType() const         -- 21 overrides ?GetModeType@<Mode>@NCameraModes@@UBE?AW4EMode@2@XZ in FableWin.pdb; CProjectileWeapon's
//                                    0x0087B5F0 is `mov eax,0Fh ; ret`; CGameCameraManager::GetCameraModeType 0x00697AB0 /
//                                    HasCameraMode 0x00697AC0 call it on CurrentMode.
// EMode is forward-declared (MS extension, as CNavigatorBase.h's ENavigatorReturn): no row yet needs its enumerators.
// Members are not modelled (Ego_r: CBase sizeof 0x4 as the TPI flattens it; the retail mode data sit in the derived modes).
#include "rebuild_abi.h"
#ifdef FABLE_RETAIL_NCAMERAMODES_CBASE_MEMBERS
class CGameCameraManager;
#endif
#ifdef FABLE_RETAIL_NCAMERAMODES_CBASE_STRING_CAGE
#include "engine/C2DVector.h"
#endif
#ifdef FABLE_RETAIL_NCAMERAMODES_CBASE_TYPED_COMPOSITES
#include "engine/NCameraModes_CPhysicsState.h"
#include "engine/CDefPointer.h"
#include "engine/CCountedPointer.h"
class CCameraModeDef;
#endif
namespace NCameraModes {
#ifdef FABLE_RETAIL_NCAMERAMODES_CBASE_TYPED_COMPOSITES
class CSubComponentBase;
#endif
#ifdef FABLE_RETAIL_NCAMERAMODES_CBASE_STRING_CAGE
class CTools;
#endif
enum EMode;
class CBase {
public:
#ifdef FABLE_RETAIL_NCAMERAMODES_CBASE_POSITION
    // Original FableWin021f6030 / Ego_r original const reference accessor.
    const C3DVector& GetPosition() const;
#endif
    virtual ~CBase();                                    // slot 0
    virtual void Update() = 0;                           // slot 1
#ifdef FABLE_RETAIL_NCAMERAMODES_CBASE_LOOKAROUND
    // Both original PDB complete vtables: original slots3 and4.
    virtual EMode GetModeType() const = 0;
    virtual void OnCameraInsideObservedThing();
    virtual bool IsUseRightStickLookAround()const;
#else
    virtual EMode GetModeType() const = 0;
#endif
               // slot 2
#ifdef FABLE_RETAIL_NCAMERAMODES_CBASE_MEMBERS
    // Per-TU opt-in (chunk 767, row 0x0084D2B0): the leading retail data members, from Ego_r.pdb struct_layouts_egor.tsv "CBase"
    // (the 200-byte camera-mode record: DefaultFOV / DefaultRollAngle / DefaultObservedPositionZOffset / LookAt / CameraManager ...)
    // with retail STLport sizes: CPhysicsState is 0x30 bytes in retail (two 4-byte lists, donor 0x40), so CameraManager sits at +0x40
    // (GetPWorldSeed 0x0084D2B0 reads [ecx+40h]). Ego_r.pdb ?GetPWorldSeed@CBase@NCameraModes@@QBEPAKXZ.
    unsigned long* GetPWorldSeed() const;                // 0x0084D2B0
#ifdef FABLE_RETAIL_NCAMERAMODES_CBASE_STRING_CAGE
protected:
#endif
    float               DefaultFOV;                      // +0x04
    float               DefaultRollAngle;                // +0x08
    float               DefaultObservedPositionZOffset;  // +0x0c
#ifdef FABLE_RETAIL_NCAMERAMODES_CBASE_TYPED_COMPOSITES
    CPhysicsState LookAt;
#else
    unsigned char       LookAt[0x30];                    // +0x10 CPhysicsState (retail 0x30)
#endif
#ifdef FABLE_RETAIL_NCAMERAMODES_CBASE_STRING_CAGE
    // Retail opt-in (Codex pass 17): full original suffix, container sizes adjusted to native STLport.
    const CTools& GetCamTools() const;
private:
    CGameCameraManager& CameraManager;                   // +0x40
#ifdef FABLE_RETAIL_NCAMERAMODES_CBASE_TYPED_COMPOSITES
    CDefPointer<const CCameraModeDef> PCameraModeDef;
#else
    unsigned char PCameraModeDef[4];                     // +0x44 CDefPointer<const CCameraModeDef>
#endif
#ifdef FABLE_RETAIL_NCAMERAMODES_CBASE_TYPED_COMPOSITES
    std::vector<CCountedPointer<CSubComponentBase> > SubComponents;
#else
    unsigned char SubComponents[12];                    // +0x48 vector<CCountedPointer<CSubComponentBase> >
#endif
#ifdef FABLE_RETAIL_NCAMERAMODES_CBASE_TYPED_COMPOSITES
    CPhysicsState GoTo;
#else
    unsigned char GoTo[48];                              // +0x54 CPhysicsState
#endif
    float CameraFOV,RollAngle,ObservedPositionZOffset;    // +0x84
    C2DVector RightStickVector,LeftStickVector;           // +0x90
    bool ViewOccluded;                                  // +0xa0
#else
    CGameCameraManager* CameraManager;                   // +0x40
#endif
#endif
#ifdef FABLE_RETAIL_NCAMERAMODES_CBASE_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals missing from this declaration, declaration only (no layout change).
    // Slots: the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots; spellings: Ego_r / FableWin name sets.
public:
    virtual void OnCameraInsideObservedThing();  // slot 3, 0x0056E150
public:
    virtual bool IsUseRightStickLookAround() const;  // slot 4, 0x0056E160
public:
    virtual bool IsShortenOnOcclusion() const;  // slot 6, 0x0056E170
public:
    virtual bool IsAvoidLandscape() const;  // slot 9, 0x0056E180
public:
    virtual bool IsFOVChange() const;  // slot 10, 0x0056E190
public:
    virtual bool IsTrackMeleeOpponents() const;  // slot 11, 0x0056E1A0
public:
    virtual bool IsFinished() const;  // slot 13, 0x0056E1B0
public:
    virtual bool IsStringAndCage() const;  // slot 14, 0x0056E1C0
public:
    virtual void Recentre();  // slot 16, 0x0056E1D0
public:
#endif
};
#ifdef FABLE_RETAIL_NCAMERAMODES_CBASE_TYPED_COMPOSITES
FABLE_STATIC_ASSERT(sizeof(CBase)==0xa4);
#endif
} // namespace NCameraModes
#endif // FABLE_RETAIL_NCAMERAMODES_CBASE_CYLINDER
#endif // FABLE_ENGINE_NCAMERAMODES_CBASE_H
