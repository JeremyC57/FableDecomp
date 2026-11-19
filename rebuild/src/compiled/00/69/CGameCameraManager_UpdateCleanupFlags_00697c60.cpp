// CGameCameraManager::UpdateCleanupFlags() -- when a scripted camera module is pending, make it the current one and release the pending
//   pointer; then clear the camera-preload flags
// Leaf: ?UpdateCleanupFlags@CGameCameraManager@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00697C60, length 96 bytes, `ret`.
// Identity (NF93 q11378): Ego_r.pdb 0x006BE940 / FableWin.pdb 0x021FE570 ?UpdateCleanupFlags@CGameCameraManager@@AAEXXZ (private); Ego_r's
//   SetScriptedCameraMode 0x006C1080 calls it at the statement where retail 0x00699E60 calls 0x00697C60 (0x00699FEC; the landed 0x00699E60
//   binds it through engine/CGameCameraManager.h SCRIPTEDCONTROL). Body as both PDB builds: PCurrentScriptModule (+0xd4) =
//   PPendingScriptModule (+0xdc), the pending one released and cleared, then CameraPreload / CameraPreloadUseFastMode (+0xe8 / +0xe9) false.
//   Was the address-named CMapInfo_00697c60::Clear stand-in (local CCountedPointer / count-block models; the CMapInfo label was propagated).
// Callees: CCountedPointer<NCameraModes::CScriptModuleBase>::operator= 0x0069C380 (out of line, FABLE_RETAIL_CCOUNTEDPOINTER_ASSIGN_DECL_ONLY),
//   operator delete 0x00BFE9BC (the inline release of the count block).
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
#define FABLE_RETAIL_CCOUNTEDPOINTER_ASSIGN_DECL_ONLY
#define FABLE_RETAIL_CCAMERA_CTORS
#define FABLE_RETAIL_NCAMERAMODES_CTOOLS_CTOR
#define FABLE_RETAIL_CGAMECAMERAMANAGER_FULL
#define FABLE_RETAIL_CGAMECAMERAMANAGER_SCRIPTEDCONTROL
#include "fable_string.h"
#include "engine/CGameCameraManager.h"

void CGameCameraManager::UpdateCleanupFlags()
{
    if (!PPendingScriptModule.IsNull()) {
        PCurrentScriptModule = PPendingScriptModule;
        PPendingScriptModule.Reset(0);
    }
    CameraPreload = false;
    CameraPreloadUseFastMode = false;
}
