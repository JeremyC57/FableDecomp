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
namespace NCameraModes {
enum EMode;
class CBase {
public:
    virtual ~CBase();                                    // slot 0
    virtual void Update() = 0;                           // slot 1
    virtual EMode GetModeType() const = 0;               // slot 2
};
} // namespace NCameraModes
#endif // FABLE_ENGINE_NCAMERAMODES_CBASE_H
