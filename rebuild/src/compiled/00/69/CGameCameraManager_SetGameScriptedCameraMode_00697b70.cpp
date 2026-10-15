// CGameCameraManager::SetGameScriptedCameraMode(CCountedPointer<NCameraModes::CScriptModuleBase> module, bool preload) -- flag the module as game-driven, make it the pending script module (counted-pointer assignment), mark the scripted mode as coming from the game, reset the removal timer and, when asked, latch the preload flags; the by-value module pointer is released on the way out
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00697B70, length 150 bytes (+ int3 padding). `ret 0Ch` (8-byte counted pointer + bool).
// Callees: the count block's destroy function (`mov ecx,[eax+8] ; call [eax+4]`), operator delete @ 0x00BFE9BC (twice).
// Layout: engine/CGameCameraManager.h is the PDB layout; retail is 0xc bytes narrower from AvailableCamPoints and 0x24 from
// RegisteredModes on (12-byte retail vectors/maps), so `this` is modelled locally with PDB names at retail offsets:
// PCurrentScriptModule +0xd4, PPendingScriptModule +0xdc (object) / +0xe0 (count block), ScriptedCameraRemoveTimer +0xe4, CameraPreload +0xe8,
// CameraPreloadUseFastMode +0xe9, ScriptedCameraModeFromGame +0x144 (PDB +0x160); the module object's byte at +0x41 is set.
// Shape: CCountedPointer::operator= inlined (compare the blocks, release the old block -- decremented through the member and
// re-read --, copy object and block, add-ref through the argument register), then the by-value parameter's destructor
// (`dec [edi] ; jne` without a re-read) at the end.
struct SCountBlock {
    long m_references;                              // +0
    void (__fastcall* m_pfnDestroy)(void*);         // +4 per-type deleter, object in ecx
    void* m_pObject;                                // +8
};

template <class T>
struct CCountedPointer {
    T* m_pObject;                                   // +0
    SCountBlock* m_pCount;                          // +4
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        if (m_pCount != other.m_pCount) {
            if (m_pCount) {
                --m_pCount->m_references;
                if (m_pCount->m_references == 0) {
                    m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                    delete m_pCount;                // operator delete @ 0x00BFE9BC
                }
            }
            m_pObject = other.m_pObject;
            m_pCount = other.m_pCount;
            if (m_pCount)
                ++m_pCount->m_references;
        }
        return *this;
    }
    ~CCountedPointer()
    {
        if (m_pCount) {
            if (--m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
    }
};

namespace NCameraModes {
struct CScriptModuleBase {
    unsigned char _pad[0x41];
    bool          m_bFromGame;                      // +0x41
};
}

struct CGameCameraManager {
    void*         __vftable;                                            // +0x00
    unsigned char _pad04[0xd8];                                         // +0x04
    CCountedPointer<NCameraModes::CScriptModuleBase> PPendingScriptModule; // +0xdc
    long          ScriptedCameraRemoveTimer;                            // +0xe4
    bool          CameraPreload;                                        // +0xe8
    bool          CameraPreloadUseFastMode;                             // +0xe9
    unsigned char _padea[0x5a];                                           // +0xea
    bool          ScriptedCameraModeFromGame;                           // +0x144
    void SetGameScriptedCameraMode(CCountedPointer<NCameraModes::CScriptModuleBase> module, bool preload);
};

void CGameCameraManager::SetGameScriptedCameraMode(CCountedPointer<NCameraModes::CScriptModuleBase> module, bool preload)
{
    module.m_pObject->m_bFromGame = true;
    PPendingScriptModule = module;
    ScriptedCameraModeFromGame = true;
    ScriptedCameraRemoveTimer = -1;
    if (preload) {
        CameraPreload = preload;
        CameraPreloadUseFastMode = true;
    }
}