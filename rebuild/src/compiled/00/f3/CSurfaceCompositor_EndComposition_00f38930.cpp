// CSurfaceCompositor::EndComposition() -- finish an off-screen composition: point rendering back at the back buffer, detach the compositor's render target, end the frame's rendering unless it was already in progress when the composition began, clear the composing flag, restore the captured render states and the saved render window
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F38930, length 83 bytes.
// Callees: GFGetSystemManager @ 0x009A4EC0 (twice), CDisplayManager::SetRenderTargetAsBackScreen @ 0x009BF220
// (result dropped), CRenderTarget::DetachTarget @ 0x00A10E10, CDisplayManager::EndRender @ 0x009BEF50,
// CRenderStateManager::RestoreCaptureBlock @ 0x00A05840 (on the global @ 0x013CB508),
// CRenderManagerCore::SetAWindow(const C2DBoxF&) @ 0x00A0AA80.
// Shape: the display manager (system +0x60) and its render manager core (+8) are both fetched up front through
// two GFGetSystemManager calls and kept in edi/ebx; `add esi,18h ; push esi` for the saved window, `ret`.
// Model: PDB CSystemManager (+0x60 PDisplayManager) / CDisplayManager (+8 PRenderManager); the compositor's own
// fields (+8 composing flag, +9 was-rendering flag, +0x10 render target, +0x18 saved window) are descriptive.
class C2DBoxF { public:
    float left, top, right, bottom;
};

struct CRenderManagerCore {
    void SetAWindow(const C2DBoxF& window);        // 0x00A0AA80
};

class CDisplayManager { public:                           // PDB: +4 SystemManager, +8 PRenderManager (scoped_ptr)
    void*               __vftable;                 // +0x00
    void*               SystemManager;             // +0x04
    CRenderManagerCore* PRenderManager;            // +0x08
    bool SetRenderTargetAsBackScreen();            // 0x009BF220
    void EndRender();                              // 0x009BEF50
};

class CSystemManager { public:                            // PDB: +0x60 CCountedPointer<CDisplayManager> PDisplayManager
    unsigned char    _pad00[0x60];
    CDisplayManager* PDisplayManager;              // +0x60 (the counted pointer's object word)
    void*            PDisplayManagerCount;         // +0x64
};
CSystemManager* __fastcall GFGetSystemManager();   // 0x009A4EC0

class CRenderTarget { public:
    void DetachTarget();                           // 0x00A10E10
};

class CRenderStateManager { public:
    void RestoreCaptureBlock();                    // 0x00A05840
};
extern CRenderStateManager* g_pRenderStateManager; // 0x013CB508

struct CSurfaceCompositor {
    unsigned char  _pad00[8];
    bool           m_composing;                    // +0x08
    bool           m_wasRendering;                 // +0x09
    unsigned char  _pad0a[0x10 - 0x0a];
    CRenderTarget* m_pRenderTarget;                // +0x10
    unsigned char  _pad14[0x18 - 0x14];
    C2DBoxF        m_savedWindow;                  // +0x18
    void EndComposition();
};

void CSurfaceCompositor::EndComposition()
{
    CDisplayManager* display = GFGetSystemManager()->PDisplayManager;
    CRenderManagerCore* core = GFGetSystemManager()->PDisplayManager->PRenderManager;
    display->SetRenderTargetAsBackScreen();
    m_pRenderTarget->DetachTarget();
    if (!m_wasRendering)
        display->EndRender();
    m_composing = false;
    g_pRenderStateManager->RestoreCaptureBlock();
    core->SetAWindow(m_savedWindow);
}