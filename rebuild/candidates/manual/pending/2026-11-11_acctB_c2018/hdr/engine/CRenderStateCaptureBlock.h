#ifndef FABLE_ENGINE_CRENDERSTATECAPTUREBLOCK_H
#define FABLE_ENGINE_CRENDERSTATECAPTUREBLOCK_H
// CRenderStateCaptureBlock / CRenderStateScopedCaptureBlock (new 2026-11-03, Codex pass 13 row 0x009DA9F0; Ego_r.pdb / FableWin.pdb
// declarations: ??0/??1CRenderStateCaptureBlock@@QAE@XZ, ?Capture / ?Restore@CRenderStateCaptureBlock@@QAEXXZ,
// ?SetStateManager@CRenderStateCaptureBlock@@SIXPAVCRenderStateManager@@@Z, ??0/??1CRenderStateScopedCaptureBlock@@QAE@XZ).
// The scoped block is inline in retail: its constructor expands CRenderStateManager::StartCaptureBlock(void*) on the static
// StateManager (0x013CB508, cleared by ~CRenderStateManager 0x00A044B0) and its destructor calls RestoreCaptureBlock 0x00A05840.
#ifndef FABLE_RETAIL_CRENDERSTATEMANAGER
#define FABLE_RETAIL_CRENDERSTATEMANAGER
#endif
#include "rebuild_abi.h"
#include "engine/CRenderStateManager.h"
class CRenderStateScopedCaptureBlock;
class CRenderStateCaptureBlock {
    friend class CRenderStateScopedCaptureBlock;
public:
    CRenderStateCaptureBlock();
#ifdef FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_INLINE_DTOR
    // Retail opt-in (chunk 2018, row 0x00B2B180 CEngineSurfaceCompositionManager::RenderToTexture): ??1CRenderStateCaptureBlock@@QAE@XZ
    // (Ego_r.pdb / FableWin.pdb) is inline where the surface compositor's implicit destructor is expanded: `if (Open) { Open = false;
    // StateManager->RestoreCaptureBlock(); }` (0x00B2B622..0x00B2B649; the landed ~CSurfaceCompositor 0x00B2E6C0 has the same body).
    ~CRenderStateCaptureBlock()
    {
        if (Open) {
            Open = false;
            StateManager->RestoreCaptureBlock();
        }
    }
#else
    ~CRenderStateCaptureBlock();
#endif
    void Capture();
    void Restore();
    static void __fastcall SetStateManager(CRenderStateManager* manager);
private:
    bool Open;
    static CRenderStateManager* StateManager;                            // 0x013CB508
};
class CRenderStateScopedCaptureBlock {
public:
    CRenderStateScopedCaptureBlock() { CRenderStateCaptureBlock::StateManager->StartCaptureBlock(this); }
    ~CRenderStateScopedCaptureBlock() { CRenderStateCaptureBlock::StateManager->RestoreCaptureBlock(); }
};
#endif
