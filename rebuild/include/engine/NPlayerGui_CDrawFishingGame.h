#ifndef FABLE_ENGINE_NPLAYERGUI_CDRAWFISHINGGAME_H
#define FABLE_ENGINE_NPLAYERGUI_CDRAWFISHINGGAME_H
// NPlayerGui::CDrawFishingGame (new, NF432): RTTI .?AVCDrawFishingGame@NPlayerGui@@ (retail COL at vftable 0x0125A580 - 4; one base, NPlayerGui::CDrawBase).
// Members: FableWin.pdb 0x3E12F / Ego_r.pdb (struct_layouts_egor.tsv: 44 bytes, every byte the CDrawBase part, no own data member). Retail sizeof 0x28: the
// CDrawBase part (its handle vector is STLport's 12 bytes, not the PDB's 16). FableWin.pdb field list: the constructor
// ??0CDrawFishingGame@NPlayerGui@@QAE@AAVCPlayerGui@@AAV?$vector@PAVCDrawBase@NPlayerGui@@V?$allocator@PAVCDrawBase@NPlayerGui@@@std@@@std@@@Z
// (0x0064D351; Ego_r.pdb names it too), ?Draw@CDrawFishingGame@NPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@@Z (0x00649E8B), and a
// compiler-generated destructor (so none is declared here).
// The vftable 0x0125A580 holds CDrawBase's two virtuals only (0x00644627, 0x00442760): no override, so none is declared.
// The constructor's mem-initialiser needs CDrawBase's constructor: define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_CTOR (and _VSLOTS for the
// retail slot order) before including this header.
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
#include "engine/NPlayerGui_CDrawBase.h"
class CPlayerGui; class CEnginePrimitivePrepareInfo;
namespace NPlayerGui {
class CDrawFishingGame : public CDrawBase {
public:
    CDrawFishingGame(CPlayerGui& playerGui, std::vector<CDrawBase*>& illustrators);   // 0x0064D351
    void Draw(const CEnginePrimitivePrepareInfo& info);                  // 0x00649E8B
};
}
FABLE_STATIC_ASSERT(sizeof(NPlayerGui::CDrawFishingGame) == 0x28);
#endif
