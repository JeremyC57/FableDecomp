#ifndef FABLE_ENGINE_NCAMERAMODES_CSCRIPTEDCONTROL_H
#define FABLE_ENGINE_NCAMERAMODES_CSCRIPTEDCONTROL_H
// NCameraModes::CScriptedControl -- the camera mode that hands the camera to a script module (hand3c, new header; row 0x00699E60
// CGameCameraManager::SetScriptedCameraMode).
// Layout: retail 0xE8 bytes (operator new(0xE8) at 0x00699F77): the 0xA4-byte CBase (NCameraModes_CBase.h with
// FABLE_RETAIL_NCAMERAMODES_CBASE_MEMBERS + _STRING_CAGE + _TYPED_COMPOSITES), then the Ego_r.pdb members PScriptModule +0xA4
// (CCountedPointer, Data first), ObservedThingMeshHeightOnInit +0xAC (1.0f in the ctor) and InitCamera +0xB0 (CCamera, 0x38).
// The constructor (Ego_r.pdb / FableWin.pdb ??0CScriptedControl@NCameraModes@@QAE@AAVCGameCameraManager@@ABV?$CDefPointer@$$CBVCCameraModeDef@@@@
// AAV?$CCountedPointer@VCScriptModuleBase@NCameraModes@@@@@Z) is out of line in retail at 0x0069DFE0 (landed); Update (slot 1,
// 0x0087BBC0 landed) and GetModeType (slot 2) are declared so the class is concrete. Declarations only: no vtable is emitted.
// Needs engine/CCamera.h (FABLE_RETAIL_CCAMERA_CTORS) and engine/CGameCameraManager.h's forward names.
#include "engine/NCameraModes_CBase.h"
#include "engine/CCamera.h"
class CGameCameraManager;
class CCameraModeDef;
namespace NCameraModes {
class CScriptModuleBase;
class CScriptedControl : public CBase {
public:
    CScriptedControl(CGameCameraManager& cam_man, const CDefPointer<const CCameraModeDef>& cam_mode_def,
                     CCountedPointer<CScriptModuleBase>& pbase);   // 0x0069DFE0 (Ego_d.pdb parameter names)
    virtual void Update();                                                  // slot 1 0x0087BBC0
    virtual EMode GetModeType() const;                                      // slot 2
private:
    CCountedPointer<CScriptModuleBase> PScriptModule;                       // +0xa4
    float ObservedThingMeshHeightOnInit;                                    // +0xac
    CCamera InitCamera;                                                     // +0xb0
};
FABLE_STATIC_ASSERT(sizeof(CScriptedControl) == 0xe8);
} // namespace NCameraModes
#endif // FABLE_ENGINE_NCAMERAMODES_CSCRIPTEDCONTROL_H
