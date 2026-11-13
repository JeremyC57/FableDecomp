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
#ifdef FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_INLINE
// Opt-in (chunk 2048): the retail-era inline bodies (Ego_r.pdb has no out-of-line CRenderStateCaptureBlock function; FableWin.pdb
// debug COMDATs: ??0 10 instructions = `Open = false`, ??1 13 instructions = `if (Open) Restore();`). The scoped block derives from
// it (PDB: size 1, no members of its own) and captures in its constructor / restores in its destructor.
class CRenderStateCaptureBlock {
    friend class CRenderStateScopedCaptureBlock;
public:
    CRenderStateCaptureBlock() : Open(false) { }
    ~CRenderStateCaptureBlock() { if (Open) Restore(); }
    void Capture() { StateManager->StartCaptureBlock(this); Open = true; }
    void Restore() { StateManager->RestoreCaptureBlock(); Open = false; }
    static void __fastcall SetStateManager(CRenderStateManager* manager);
private:
    bool Open;
    static CRenderStateManager* StateManager;                            // 0x013CB508
};
class CRenderStateScopedCaptureBlock : public CRenderStateCaptureBlock {
public:
    CRenderStateScopedCaptureBlock() { Capture(); }
    ~CRenderStateScopedCaptureBlock() { Restore(); }
};
#else
class CRenderStateCaptureBlock {
    friend class CRenderStateScopedCaptureBlock;
public:
    CRenderStateCaptureBlock();
    ~CRenderStateCaptureBlock();
#ifdef FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_CAPTURE_INLINE
    // Retail opt-in (wf20 row 0x00F39730 CSurfaceCompositor::StartComposition): ?Capture@CRenderStateCaptureBlock@@QAEXXZ is FableWin-only
    // (no Ego_r / retail copy); 0x00F39739 stores Open = true and expands StartCaptureBlock on StateManager in place.
    void Capture() { Open = true; StateManager->StartCaptureBlock(this); }
#else
    void Capture();
#endif
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
#endif
