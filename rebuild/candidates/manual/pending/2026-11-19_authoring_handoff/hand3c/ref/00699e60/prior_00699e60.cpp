// CGameCameraManager::SetScriptedCameraMode(CCountedPointer<NCameraModes::CScriptModuleBase> module) -- queue a script camera module: while a scripted preload is pending just preload it; otherwise make it the pending module, decide the preload flags and switch the camera into a new scripted mode wrapping it
// Grade: DIFFER(459v431) -- lane gap: retail builds the new-mode counted pointer in the dead by-value argument slots (no frame, Info cached in edi); ours opens an 8-byte frame and re-reads the address-taken local
// Leaf: SetScriptedCameraMode
// Retail: Fable.exe (TLC, VC7.1) @ 0x00699E60, 432 bytes, `ret 8` (the counted pointer is passed by value).
// Callees: PreloadScriptedCamera 0x00699B00 (by value), operator new 0x00BFEA1A / operator delete 0x00BFE9BC, the scripted
//   camera mode constructor 0x0069DFE0 (0xe8 bytes; manager, mode-defaults global 0x013BA8D8, &pending module),
//   CCountedPointer<NCameraModes::CBase>::operator=(CBase*) 0x0069C2F0, SetCameraMode 0x00697E20 (landed, by value),
//   the pending-module promotion 0x00697C60; the current mode's slot 2 (GetMode, 8 = the first-person/fixed mode).
// Layout: Ego_r.pdb CGameCameraManager with the retail container shrink of the landed SetCameraMode 0x00697E20:
//   CurrentMode +0x98, PPendingScriptModule +0xdc, ScriptedCameraRemoveTimer +0xe4, CameraPreload +0xe8,
//   CameraPreloadUseFastMode +0xe9, ScriptHasPreloadedCamera +0xea, ScriptedCameraModeFromGame +0x144,
//   ScriptedCameraPreloadFlag +0x145; the global flag 0x013BA8D4.
struct CModeOwner { unsigned References; void (__fastcall* Destroy)(void*); void* Data; };
template<class T> struct CCountedPointer {
    T* Data; CModeOwner* Info;
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(const CCountedPointer& p) : Data(p.Data), Info(p.Info) { if (Info) ++Info->References; }
    void Release() { if (Info) { --Info->References; if (!Info->References) { Info->Destroy(Info->Data); delete Info; } } }
    ~CCountedPointer() { if (Info) { if (--Info->References == 0) { Info->Destroy(Info->Data); delete Info; } } }
    CCountedPointer& operator=(const CCountedPointer& p) { if (Info != p.Info) { Release(); Data = p.Data; Info = p.Info; if (Info) ++Info->References; } return *this; }
    CCountedPointer& operator=(T* p);   // 0x0069C2F0 (CBase instantiation)
};
namespace NCameraModes {
struct CBase { virtual void v0(); virtual void v1(); virtual long GetMode() const; };
struct CScriptModuleBase;
struct CScriptModeDefaults;
}
struct CGameCameraManager;
struct CScriptedCameraMode : NCameraModes::CBase {   // 0xe8 bytes
    unsigned char _body[0xe4];
    CScriptedCameraMode(CGameCameraManager* manager, NCameraModes::CScriptModeDefaults* defaults,
                        CCountedPointer<NCameraModes::CScriptModuleBase>* module);   // 0x0069DFE0
};
extern bool g_ScriptedCameraForcePreload;                       // 0x013BA8D4
extern NCameraModes::CScriptModeDefaults g_ScriptModeDefaults;  // 0x013BA8D8

struct CGameCameraManager {
    unsigned char _pad00[0x98];
    CCountedPointer<NCameraModes::CBase> CurrentMode;              // +0x98
    unsigned char _padA0[0xdc - 0xa0];
    CCountedPointer<NCameraModes::CScriptModuleBase> PPendingScriptModule;   // +0xdc
    long ScriptedCameraRemoveTimer;                                // +0xe4
    bool CameraPreload;                                            // +0xe8
    bool CameraPreloadUseFastMode;                                 // +0xe9
    bool ScriptHasPreloadedCamera;                                 // +0xea
    unsigned char _padEB[0x144 - 0xeb];
    bool ScriptedCameraModeFromGame;                               // +0x144
    bool ScriptedCameraPreloadFlag;                                // +0x145
    void PreloadScriptedCamera(CCountedPointer<NCameraModes::CScriptModuleBase> module);   // 0x00699B00
    void SetCameraMode(CCountedPointer<NCameraModes::CBase> mode);                         // 0x00697E20
    void PromotePendingScriptModule();                                                     // 0x00697C60
    void SetScriptedCameraMode(CCountedPointer<NCameraModes::CScriptModuleBase> module);
};

void CGameCameraManager::SetScriptedCameraMode(CCountedPointer<NCameraModes::CScriptModuleBase> module)
{
    if (ScriptedCameraPreloadFlag) {
        PreloadScriptedCamera(module);
        ScriptHasPreloadedCamera = true;
        return;
    }
    PPendingScriptModule = module;
    ScriptedCameraModeFromGame = false;
    ScriptedCameraRemoveTimer = -1;
    if (g_ScriptedCameraForcePreload) {
        CameraPreload = true;
        CameraPreloadUseFastMode = false;
        if (CurrentMode.Data->GetMode() == 8)
            CameraPreloadUseFastMode = true;
    } else if (ScriptHasPreloadedCamera) {
        CameraPreload = false;
        ScriptHasPreloadedCamera = false;
    } else if (CurrentMode.Data->GetMode() != 8) {
        CameraPreload = true;
        CameraPreloadUseFastMode = false;
    }
    CCountedPointer<NCameraModes::CBase> mode;
    mode = new CScriptedCameraMode(this, &g_ScriptModeDefaults, &PPendingScriptModule);
    SetCameraMode(mode);
    PromotePendingScriptModule();
}
