#ifndef FABLE_ENGINE_NPLAYERGUI_CDRAWBASE_H
#define FABLE_ENGINE_NPLAYERGUI_CDRAWBASE_H
// NPlayerGui::CDrawBase (new, chunk 765): RTTI .?AVCDrawBase@NPlayerGui@@ (no bases, polymorphic). Ego_r.pdb members (struct_layouts_egor.tsv,
// 44 bytes): vptr +0, PlayerGui +0x04, PlayerManager +0x08, World +0x0C, Component +0x10, Bank +0x14, Drawing +0x18, PrimitiveHandles
// (vector<CEnginePrimitiveHandle>, PDB 16 bytes, retail STLport 12) +0x1C: retail sizeof 0x28 (CDrawNPCName's Name sits at +0x28).
// Name set: ??1CDrawBase@NPlayerGui@@QAE@XZ (non-virtual), the virtuals ?ClearPrimitives@..UAEXXZ / ?ClearPrimitivesIfNecessary@..UAEXXZ
// (slot order unproven, so they are declared for the vptr only), ?IsDrawing@..QBE_NXZ, ?SetAsDrawing@..QAEX_N@Z.
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
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
    ~CDrawBase();
    virtual void ClearPrimitives();
    virtual void ClearPrimitivesIfNecessary();
    bool IsDrawing() const { return Drawing; }
protected:
    CPlayerGui* PlayerGui;                                               // +0x04
    CPlayerManager* PlayerManager;                                       // +0x08
    CWorld* World;                                                       // +0x0c
    CMainGameComponent* Component;                                       // +0x10
    CGraphicDataBank* Bank;                                              // +0x14
    bool Drawing;                                                        // +0x18
    std::vector<CEnginePrimitiveHandle*> PrimitiveHandles;               // +0x1c vector<CEnginePrimitiveHandle> (12 bytes; element type opaque here)
};
}
FABLE_STATIC_ASSERT(sizeof(NPlayerGui::CDrawBase) == 0x28);
#endif
