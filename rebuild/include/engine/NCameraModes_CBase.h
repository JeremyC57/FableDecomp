#ifndef FABLE_ENGINE_NCAMERAMODES_CBASE_H
#define FABLE_ENGINE_NCAMERAMODES_CBASE_H
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
namespace NCameraModes {
enum EMode;
class CBase {
public:
    virtual ~CBase();                                    // slot 0
    virtual void Update() = 0;                           // slot 1
    virtual EMode GetModeType() const = 0;               // slot 2
#ifdef FABLE_RETAIL_NCAMERAMODES_CBASE_MEMBERS
    // Per-TU opt-in (chunk 767, row 0x0084D2B0): the leading retail data members, from Ego_r.pdb struct_layouts_egor.tsv "CBase"
    // (the 200-byte camera-mode record: DefaultFOV / DefaultRollAngle / DefaultObservedPositionZOffset / LookAt / CameraManager ...)
    // with retail STLport sizes: CPhysicsState is 0x30 bytes in retail (two 4-byte lists, donor 0x40), so CameraManager sits at +0x40
    // (GetPWorldSeed 0x0084D2B0 reads [ecx+40h]). Ego_r.pdb ?GetPWorldSeed@CBase@NCameraModes@@QBEPAKXZ.
    unsigned long* GetPWorldSeed() const;                // 0x0084D2B0
    float               DefaultFOV;                      // +0x04
    float               DefaultRollAngle;                // +0x08
    float               DefaultObservedPositionZOffset;  // +0x0c
    unsigned char       LookAt[0x30];                    // +0x10 CPhysicsState (retail 0x30)
    CGameCameraManager* CameraManager;                   // +0x40
#endif
};
} // namespace NCameraModes
#endif // FABLE_ENGINE_NCAMERAMODES_CBASE_H
