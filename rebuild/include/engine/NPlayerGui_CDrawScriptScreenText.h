#ifndef FABLE_ENGINE_NPLAYERGUI_CDRAWSCRIPTSCREENTEXT_H
#define FABLE_ENGINE_NPLAYERGUI_CDRAWSCRIPTSCREENTEXT_H
// NPlayerGui::CDrawScriptScreenText (new, NF432): RTTI .?AVCDrawScriptScreenText@NPlayerGui@@ (retail COL at vftable 0x0125A5C4 - 4; one base, NPlayerGui::CDrawBase).
// Members: FableWin.pdb 0x3E159 / Ego_r.pdb (struct_layouts_egor.tsv: 44 bytes, every byte the CDrawBase part, no own data member). Retail sizeof 0x28: the
// CDrawBase part (its handle vector is STLport's 12 bytes, not the PDB's 16). FableWin.pdb field list: the constructor
// ??0CDrawScriptScreenText@NPlayerGui@@QAE@AAVCPlayerGui@@AAV?$vector@PAVCDrawBase@NPlayerGui@@V?$allocator@PAVCDrawBase@NPlayerGui@@@std@@@std@@@Z
// (0x0064DA6E; Ego_r.pdb names it too), ?Draw@CDrawScriptScreenText@NPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@@Z (0x0064560D), and a
// compiler-generated destructor (so none is declared here).
// The vftable 0x0125A5C4 holds CDrawBase's two virtuals only (0x00644627, 0x00442760): no override, so none is declared.
// The constructor's mem-initialiser needs CDrawBase's constructor: define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_CTOR (and _VSLOTS for the
// retail slot order) before including this header.
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
#include "engine/NPlayerGui_CDrawBase.h"
class CPlayerGui; class CEnginePrimitivePrepareInfo;
namespace NPlayerGui {
class CDrawScriptScreenText : public CDrawBase {
public:
    CDrawScriptScreenText(CPlayerGui& playerGui, std::vector<CDrawBase*>& illustrators);   // 0x0064DA6E
    void Draw(const CEnginePrimitivePrepareInfo& info);                  // 0x0064560D
};
}
FABLE_STATIC_ASSERT(sizeof(NPlayerGui::CDrawScriptScreenText) == 0x28);
#endif
