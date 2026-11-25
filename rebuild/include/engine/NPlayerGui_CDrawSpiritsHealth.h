#ifndef FABLE_ENGINE_NPLAYERGUI_CDRAWSPIRITSHEALTH_H
#define FABLE_ENGINE_NPLAYERGUI_CDRAWSPIRITSHEALTH_H
// NPlayerGui::CDrawSpiritsHealth (new, NF432): RTTI .?AVCDrawSpiritsHealth@NPlayerGui@@ (retail COL at vftable 0x0125A538 - 4; one base, NPlayerGui::CDrawBase).
// Members: FableWin.pdb 0x3E0CF / Ego_r.pdb (struct_layouts_egor.tsv: 44 bytes, every byte the CDrawBase part, no own data member). Retail sizeof 0x28: the
// CDrawBase part (its handle vector is STLport's 12 bytes, not the PDB's 16). FableWin.pdb field list: the constructor
// ??0CDrawSpiritsHealth@NPlayerGui@@QAE@AAVCPlayerGui@@AAV?$vector@PAVCDrawBase@NPlayerGui@@V?$allocator@PAVCDrawBase@NPlayerGui@@@std@@@std@@@Z
// (0x0064D2A9; Ego_r.pdb names it too), ?Draw@CDrawSpiritsHealth@NPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@@Z (0x0064E2CC), and a
// compiler-generated destructor (so none is declared here). It also names an unnamed nested type (LF_NESTTYPE `<unnamed-tag>`) that no member uses; it is not modelled.
// The vftable 0x0125A538 holds CDrawBase's two virtuals only (0x00644627, 0x00442760): no override, so none is declared.
// The constructor's mem-initialiser needs CDrawBase's constructor: define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_CTOR (and _VSLOTS for the
// retail slot order) before including this header.
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
#include "engine/NPlayerGui_CDrawBase.h"
class CPlayerGui; class CEnginePrimitivePrepareInfo;
namespace NPlayerGui {
class CDrawSpiritsHealth : public CDrawBase {
public:
    CDrawSpiritsHealth(CPlayerGui& playerGui, std::vector<CDrawBase*>& illustrators);   // 0x0064D2A9
    void Draw(const CEnginePrimitivePrepareInfo& info);                  // 0x0064E2CC
};
}
FABLE_STATIC_ASSERT(sizeof(NPlayerGui::CDrawSpiritsHealth) == 0x28);
#endif
