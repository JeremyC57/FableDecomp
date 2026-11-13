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
#ifdef FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_INLINE
    // Opt-in (bwf2, row 0x00B783F0): no retail copy of these four (Ego_r none; FableWin ctor 20 b, dtor 29 b); the row expands
    // Capture (StartCaptureBlock; Open = true), Restore (Open = false; RestoreCaptureBlock 0x00A05840) and the dtor (if Open Restore).
    CRenderStateCaptureBlock() : Open(false) { }
    ~CRenderStateCaptureBlock() { if (Open) Restore(); }
    void Capture() { StateManager->StartCaptureBlock(this); Open = true; }
    void Restore() { Open = false; StateManager->RestoreCaptureBlock(); }
#else
    CRenderStateCaptureBlock();
    ~CRenderStateCaptureBlock();
    void Capture();
    void Restore();
#endif
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
