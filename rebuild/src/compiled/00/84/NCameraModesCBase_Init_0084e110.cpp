// NCameraModes::CBase::Init() -- (re)start a camera mode: OnInit, on a single cut take the default FOV / roll / observed-Z offset and set the
// GoTo phase's CullPhysics, then for mode type 8 or a single cut run one FrameUpdate and (still a single cut) roll the manager's camera.
// Leaf: ?Init@CBase@NCameraModes@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0084E110, 136 bytes (+8 int3), thiscall, no arguments.
// Callers: CGameCameraManager::SetCameraMode 0x00697E50 (in 0x00697E20), CGameCameraManager::PreloadScriptedCamera 0x00699B79 (in 0x00699B00);
//   both landed rows bind ?Init@CBase@NCameraModes@@QAEXXZ.
// Callees: virtual slots 21 OnInit (+0x54), 18 / 19 / 20 GetDefaultFOV / GetDefaultRollAngle / GetDefaultObservedPositionZOffset
//   (+0x48 / +0x4C / +0x50), 2 GetModeType (+8); CBase::FrameUpdate 0x0084DC10.
// Naming (NF157, 2026-11-20; PRIORITY q3987): was the stand-in ?Init@CCameraModeBase@@QAEXXZ over a local CCameraModeBase / CCameraTracking
//   model; now the shared engine/NCameraModes_CBase.h (CYLINDER) class with engine/CGameCameraManager.h (retail + SINGLECUT branch, the
//   landed 0x00878BE0 pairing), as Ego_r.pdb 0x00845E10 / FableWin.pdb 0x02886380 spell it. The FrameUpdate call binds Ego_r.pdb 0x008459B0 /
//   FableWin.pdb 0x02883CC0 ?FrameUpdate@CBase@NCameraModes@@QAEXXZ (was ?FrameUpdate@CCameraModeBase@@QAEXXZ). Layout (retail STLport sizes):
//   CameraManager +0x40, GoTo +0x54 (CullPhysics +0x80), CameraFOV / RollAngle / ObservedPositionZOffset +0x84..+0x8C; the manager's
//   SingleCut +0xB0, Camera +0x04, OldCamera +0x3C (SetSingleCut, FableWin.pdb ?SetSingleCut@CGameCameraManager@@QAEXXZ, inline:
//   `OldCamera = Camera; SingleCut = true`, a 14-dword `rep movsd`). Mode type 8 compared as an integer (the enumerator name is unknown;
//   the landed 0x00699E60 writes the same `GetModeType() == 8`).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_NCAMERAMODES_CBASE_CYLINDER
#define FABLE_RETAIL_CGAMECAMERAMANAGER
#define FABLE_RETAIL_CGAMECAMERAMANAGER_SINGLECUT
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_C3DVECTOR
#include "engine/C3DVector.h"
#include "engine/NCameraModes_CBase.h"
#include "engine/CGameCameraManager.h"

namespace NCameraModes {
void CBase::Init()
{
    OnInit();
    if (CameraManager.SingleCut) {
        CameraFOV = GetDefaultFOV();
        RollAngle = GetDefaultRollAngle();
        ObservedPositionZOffset = GetDefaultObservedPositionZOffset();
        GoTo.CullPhysics = true;
    }
    if (GetModeType() == 8 || CameraManager.SingleCut) {
        FrameUpdate();
        if (CameraManager.SingleCut)
            CameraManager.SetSingleCut();
    }
}
}
