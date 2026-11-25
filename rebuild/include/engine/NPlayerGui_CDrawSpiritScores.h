#ifndef FABLE_ENGINE_NPLAYERGUI_CDRAWSPIRITSCORES_H
#define FABLE_ENGINE_NPLAYERGUI_CDRAWSPIRITSCORES_H
// NPlayerGui::CDrawSpiritScores (new, NF432): RTTI .?AVCDrawSpiritScores@NPlayerGui@@ (retail COL at vftable 0x0125A598 - 4; one base, NPlayerGui::CDrawBase).
// Members: FableWin.pdb 0x3E1A3 / Ego_r.pdb (struct_layouts_egor.tsv: 44 bytes, every byte the CDrawBase part, no own data member). Retail sizeof 0x28: the
// CDrawBase part (its handle vector is STLport's 12 bytes, not the PDB's 16). FableWin.pdb field list: the constructor
// ??0CDrawSpiritScores@NPlayerGui@@QAE@AAVCPlayerGui@@AAV?$vector@PAVCDrawBase@NPlayerGui@@V?$allocator@PAVCDrawBase@NPlayerGui@@@std@@@std@@@Z
// (0x0064D389; Ego_r.pdb names it too), ?Draw@CDrawSpiritScores@NPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@@Z (0x00644424), and a
// compiler-generated destructor (so none is declared here).
// The vftable 0x0125A598 holds CDrawBase's two virtuals only (0x00644627, 0x00442760): no override, so none is declared.
// The constructor's mem-initialiser needs CDrawBase's constructor: define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_CTOR (and _VSLOTS for the
// retail slot order) before including this header.
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
#include "engine/NPlayerGui_CDrawBase.h"
class CPlayerGui; class CEnginePrimitivePrepareInfo;
namespace NPlayerGui {
class CDrawSpiritScores : public CDrawBase {
public:
    CDrawSpiritScores(CPlayerGui& playerGui, std::vector<CDrawBase*>& illustrators);   // 0x0064D389
    void Draw(const CEnginePrimitivePrepareInfo& info);                  // 0x00644424
};
}
FABLE_STATIC_ASSERT(sizeof(NPlayerGui::CDrawSpiritScores) == 0x28);
#endif
