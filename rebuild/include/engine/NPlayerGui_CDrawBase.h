#ifndef FABLE_ENGINE_NPLAYERGUI_CDRAWBASE_H
#define FABLE_ENGINE_NPLAYERGUI_CDRAWBASE_H
// NPlayerGui::CDrawBase (new, chunk 765): RTTI .?AVCDrawBase@NPlayerGui@@ (no bases, polymorphic). Ego_r.pdb members (struct_layouts_egor.tsv,
// 44 bytes): vptr +0, PlayerGui +0x04, PlayerManager +0x08, World +0x0C, Component +0x10, Bank +0x14, Drawing +0x18, PrimitiveHandles
// (vector<CEnginePrimitiveHandle>, PDB 16 bytes, retail STLport 12) +0x1C: retail sizeof 0x28 (CDrawNPCName's Name sits at +0x28).
// Name set: ??1CDrawBase@NPlayerGui@@QAE@XZ (non-virtual), the virtuals ?ClearPrimitives@..UAEXXZ / ?ClearPrimitivesIfNecessary@..UAEXXZ
// (slot order unproven, so they are declared for the vptr only), ?IsDrawing@..QBE_NXZ, ?SetAsDrawing@..QAEX_N@Z.
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
#ifdef FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_HANDLES
#include "engine/CEnginePrimitiveHandle.h"
#endif
class CPlayerGui; class CPlayerManager; class CWorld; class CMainGameComponent; class CGraphicDataBank; class CEnginePrimitiveHandle;
namespace NPlayerGui {
class CDrawBase {
public:
#ifdef FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_CTOR
    // Retail opt-in (chunk 863, row 0x0043B570 CPlayerGui::CPlayerGui): Ego_r.pdb / FableWin.pdb
    // ??0CDrawBase@NPlayerGui@@QAE@AAVCPlayerGui@@AAV?$vector@PAVCDrawBase@NPlayerGui@@V?$allocator@PAVCDrawBase@NPlayerGui@@@std@@@std@@@Z,
    // out of line at 0x0064CC65 (the inline CDrawNPCName ctor calls it: `push illustrators ; push gui ; call`). Declaration only.
    CDrawBase(CPlayerGui& playerGui, std::vector<CDrawBase*>& illustrators);   // 0x0064CC65
#endif
#ifdef FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_IMPLICIT_DTOR
    // Retail opt-in (agentFAM10, 2026-11-12, the CCountedPointer<NPlayerGui::CDraw*>::DeleteData rows 0x00443580..0x00443A60): retail
    // expands the destruction inside each deleter with no vptr store -- only the PrimitiveHandles vector's teardown is left (every
    // handle's virtual destructor, slot 0 flag 0, then free of the block) before operator delete: the implicit destructor, so none is
    // declared here. Default unchanged.
#else
    ~CDrawBase();
#endif
    virtual void ClearPrimitives();
    virtual void ClearPrimitivesIfNecessary();
    bool IsDrawing() const { return Drawing; }
#ifdef FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_SETASDRAWING
    // Retail opt-in (chunk Q22, row 0x00649E8B CDrawFishingGame::Draw): FableWin.pdb ?SetAsDrawing@CDrawBase@NPlayerGui@@QAEX_N@Z
    // (named in this header's comment above; no out-of-line copy in Ego_r.pdb or Fable.exe: the row stores Drawing +0x18 in place,
    // `mov byte ptr [edi+18h],1` at 0x00649EFB). No layout change.
    void SetAsDrawing(bool drawing) { Drawing = drawing; }
#endif
#ifdef FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_HANDLES
    // Retail opt-in (chunk 867, row 0x0064BFBA CDrawHighlightingTargetingCursor::Draw): Ego_r.pdb ?ScaleAlpha@CDrawBase@NPlayerGui@@QBEEE@Z
    // (out of line at 0x00643D5D; the landed DrawIcon 0x00645292 calls it as the sprite alpha), declaration only; and PrimitiveHandles as
    // the vector of CEnginePrimitiveHandle objects it is (8-byte elements: the row passes `&PrimitiveHandles[1]` as `[this+1Ch]+8` to
    // CIEngine::SetPersistentPrimitive and for_each's Clear over begin()..end()).
    unsigned char ScaleAlpha(unsigned char alpha) const;                 // 0x00643D5D
#endif
protected:
    CPlayerGui* PlayerGui;                                               // +0x04
    CPlayerManager* PlayerManager;                                       // +0x08
    CWorld* World;                                                       // +0x0c
    CMainGameComponent* Component;                                       // +0x10
    CGraphicDataBank* Bank;                                              // +0x14
    bool Drawing;                                                        // +0x18
#ifdef FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_HANDLES
    std::vector<CEnginePrimitiveHandle> PrimitiveHandles;                // +0x1c vector<CEnginePrimitiveHandle> (12 bytes, 8-byte elements)
#else
    std::vector<CEnginePrimitiveHandle*> PrimitiveHandles;               // +0x1c vector<CEnginePrimitiveHandle> (12 bytes; element type opaque here)
#endif
};
}
FABLE_STATIC_ASSERT(sizeof(NPlayerGui::CDrawBase) == 0x28);
#endif
