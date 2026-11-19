// CGameCameraManager::SetScriptedCameraMode(CCountedPointer<NCameraModes::CScriptModuleBase> pscript_module) -- hand the camera to a script module: while scripted cameras are being preloaded only preload it; otherwise make it the pending module, decide the camera preload flags from the current mode, switch into a new scripted-control mode and update the cleanup flags
// Inline-budget: /d2inlT150 -- SwitchIntoScriptedControl expanded at its one site (Ego_r.pdb 0x006C1080 expands it too; default budget: called, DIFFER(290v431); smallest N that matches: 150, 149 does not)
// Leaf: ?SetScriptedCameraMode@CGameCameraManager@@QAEXV?$CCountedPointer@VCScriptModuleBase@NCameraModes@@@@@Z
// Flags: /d2inlT150
// Retail: Fable.exe (TLC, VC7.1) @ 0x00699E60, 431 bytes, thiscall, `ret 8` (the counted pointer by value).
// Identity: Ego_r.pdb 0x006C1080 / FableWin.pdb 0x021F9950 (same name; Ego_d.pdb parameter pscript_module).
// Shape: FableWin 0x021F9950 statement for statement: the preload branch (PreloadScriptedCamera by value, ScriptHasPreloadedCamera =
//   true); PPendingScriptModule = pscript_module; ScriptedCameraModeFromGame = false; ScriptedCameraRemoveTimer = -1; the three-way
//   preload decision on g_PreloadScriptedCameras / ScriptHasPreloadedCamera / GetCameraMode()->GetModeType() == 8; the
//   IsSwitchingIntoScriptedControl assert (debug only); SwitchIntoScriptedControl (FableWin 0x022002F0, expanded in retail and in
//   Ego_r 0x006C11AE..0x006C124F: CCountedPointer<CBase> local, Reset(new CScriptedControl(*this, DEFAULT_CAMERA_MODE_DEF,
//   PPendingScriptModule)), SetCameraMode(by value)); UpdateCleanupFlags. FableWin's camera-position GFOutputDebugString lines are
//   debug output, absent from both release builds.
// Reset: CCountedPointer<CBase>::Reset is the header body kept out of line (FABLE_RETAIL_CCOUNTEDPOINTER_RESET_NOINLINE, the NF62
//   device): retail calls the COMDAT 0x0069C2F0 (this source grades RELOCATION_MATCH there too) yet keeps the local's Info in edi
//   across SetCameraMode (0x00699FB4..0x00699FD7); a declaration-only Reset makes VC7.1 reload it from the frame (DIFFER 450v431).
// Callees: PreloadScriptedCamera 0x00699B00, operator new 0x00BFEA1A, CScriptedControl::CScriptedControl 0x0069DFE0,
//   CCountedPointer<NCameraModes::CBase>::Reset 0x0069C2F0, SetCameraMode 0x00697E20, UpdateCleanupFlags 0x00697C60, operator delete
//   0x00BFE9BC; the current mode's slot 2 GetModeType. Globals: g_PreloadScriptedCameras 0x013BA8D4 and DEFAULT_CAMERA_MODE_DEF
//   0x013BA8D8 (the landed 0x0069AE80 spellings).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CCOUNTEDPOINTER_RESET_NOINLINE
#define FABLE_RETAIL_CCAMERA_CTORS
#define FABLE_RETAIL_NCAMERAMODES_CTOOLS_CTOR
#define FABLE_RETAIL_CGAMECAMERAMANAGER_FULL
#define FABLE_RETAIL_CGAMECAMERAMANAGER_SCRIPTEDCONTROL
#define FABLE_RETAIL_NCAMERAMODES_CBASE_MEMBERS
#define FABLE_RETAIL_NCAMERAMODES_CBASE_STRING_CAGE
#define FABLE_RETAIL_NCAMERAMODES_CBASE_TYPED_COMPOSITES
#include "fable_string.h"
#include "engine/CGameCameraManager.h"
#include "engine/NCameraModes_CBase.h"
#include "engine/NCameraModes_CScriptedControl.h"

namespace { extern CDefPointer<const CCameraModeDef> DEFAULT_CAMERA_MODE_DEF; }   // 0x013BA8D8
extern bool g_PreloadScriptedCameras;                                    // 0x013BA8D4 (descriptive; landed 0x0069AE80 spelling)

inline void CGameCameraManager::SwitchIntoScriptedControl()
{
    CCountedPointer<NCameraModes::CBase> scripted_control;
    scripted_control.Reset(new NCameraModes::CScriptedControl(*this, DEFAULT_CAMERA_MODE_DEF, PPendingScriptModule));
    SetCameraMode(scripted_control);
}

void CGameCameraManager::SetScriptedCameraMode(CCountedPointer<NCameraModes::CScriptModuleBase> pscript_module)
{
    if (ScriptedCameraPreloadFlag) {
        PreloadScriptedCamera(pscript_module);
        ScriptHasPreloadedCamera = true;
        return;
    }
    PPendingScriptModule = pscript_module;
    ScriptedCameraModeFromGame = false;
    ScriptedCameraRemoveTimer = -1;
    if (g_PreloadScriptedCameras) {
        CameraPreload = true;
        CameraPreloadUseFastMode = false;
        if (GetCameraMode()->GetModeType() == 8)
            CameraPreloadUseFastMode = true;
    } else if (ScriptHasPreloadedCamera) {
        CameraPreload = false;
        ScriptHasPreloadedCamera = false;
    } else if (GetCameraMode()->GetModeType() != 8) {
        CameraPreload = true;
        CameraPreloadUseFastMode = false;
    }
    SwitchIntoScriptedControl();
    UpdateCleanupFlags();
}