// CGameCameraManager::ReleaseScriptedControl() -- stop the scripted camera: reset the removal timer, drop the current script module, fall back to automatic mode (or re-push the manual mode when the camera is used manually), and latch the preload flags (fast mode only when the scripted mode came from the game)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00698950, length 124 bytes (+ int3 padding).
// Callees: the count block's destroy function (`mov ecx,[eax+8] ; call [eax+4]`), operator delete @ 0x00BFE9BC,
// CGameCameraManager::PushCurrentManualMode @ 0x006988B0.
// Layout: retail CGameCameraManager with PDB names (see PushCurrentManualMode 0x006988B0): NewAutoMode +0xa8, PCurrentScriptModule
// +0xd4 (object) / +0xd8 (count block), ScriptedCameraRemoveTimer +0xe4, CameraPreload +0xe8, CameraPreloadUseFastMode +0xe9,
// UsingCameraManually +0x12c, ScriptedCameraModeFromGame +0x144.
// Shape: `PCurrentScriptModule = 0` is the inlined CCountedPointer reset (block decremented through the member and re-read, destroyed
// and freed on zero, then block and object zero stores); the flag byte read into cl before the 1 store.
struct SCountBlock {
    long m_references;                      // +0
    void (__fastcall* m_pfnDestroy)(void*); // +4
    void* m_pObject;                        // +8
};
template <class T>
struct CCountedPointer {
    T* m_pObject;                           // +0
    SCountBlock* m_pCount;                  // +4
    void Reset()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
        m_pCount = 0;
        m_pObject = 0;
    }
};
namespace NCameraModes { struct CScriptModuleBase; }

struct CGameCameraManager {
    void*         __vftable;                                            // +0x00
    unsigned char _pad04[0xa4];                                         // +0x04
    bool          NewAutoMode;                                          // +0xa8
    unsigned char _pada9[0x2b];                                         // +0xa9
    CCountedPointer<NCameraModes::CScriptModuleBase> PCurrentScriptModule; // +0xd4
    CCountedPointer<NCameraModes::CScriptModuleBase> PPendingScriptModule; // +0xdc
    long          ScriptedCameraRemoveTimer;                            // +0xe4
    bool          CameraPreload;                                        // +0xe8
    bool          CameraPreloadUseFastMode;                             // +0xe9
    unsigned char _padea[0x42];                                         // +0xea
    bool          UsingCameraManually;                                  // +0x12c
    bool          BlockModeChange;                                      // +0x12d
    unsigned char _pad12e[0x16];                                        // +0x12e
    bool          ScriptedCameraModeFromGame;                           // +0x144
    void PushCurrentManualMode();                                       // 0x006988B0
    void ReleaseScriptedControl();
};

void CGameCameraManager::ReleaseScriptedControl()
{
    ScriptedCameraRemoveTimer = -1;
    PCurrentScriptModule.Reset();
    if (!UsingCameraManually)
        NewAutoMode = true;
    else
        PushCurrentManualMode();
    CameraPreload = true;
    CameraPreloadUseFastMode = ScriptedCameraModeFromGame;
}