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
#ifdef FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_INLINE_CTOR
    // Retail opt-in (chunk 2024, row 0x00B891B0 CEngineScreenEffectOutlineGlow::CEngineScreenEffectOutlineGlow): ??0CRenderStateCaptureBlock@@QAE@XZ
    // is FableWin.pdb-only (debug 0x02D55560; no Ego_r.pdb or Fable.exe copy): inline in retail, the member is constructed in place as
    // `mov byte ptr [esi+1E0h],bl` (0x00B89232; the same Open = false store in the landed 0x00B86C00).
    CRenderStateCaptureBlock() : Open(false) { }
#else
    CRenderStateCaptureBlock();
#endif
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
#endif
