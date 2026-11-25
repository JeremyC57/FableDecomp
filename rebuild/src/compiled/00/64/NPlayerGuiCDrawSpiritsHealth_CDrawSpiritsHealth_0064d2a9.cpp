#pragma optimize("s",on)
// NPlayerGui::CDrawSpiritsHealth::CDrawSpiritsHealth(CPlayerGui&, std::vector<CDrawBase*>&) -- forwards both references to CDrawBase, then this class's vtable
// Leaf: ??0CDrawSpiritsHealth@NPlayerGui@@QAE@AAVCPlayerGui@@AAV?$vector@PAVCDrawBase@NPlayerGui@@V?$allocator@PAVCDrawBase@NPlayerGui@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0064D2A9; size-mode TU as before (the row's pragmas are HEAD's).
// Names (NF432, two sources each): the constructor is ??0CDrawSpiritsHealth@NPlayerGui@@ in Ego_r.pdb and FableWin.pdb publics, and the table already binds that
//   spelling here (witness 0x0043B570 CPlayerGui::CPlayerGui); the vftable 0x0125A538 is retail RTTI .?AVCDrawSpiritsHealth@NPlayerGui@@ (its COL at
//   0x0125A538 - 4) and FableWin.pdb ??_7CDrawSpiritsHealth@NPlayerGui@@6B@. Its two slots are CDrawBase's ClearPrimitivesIfNecessary (0x00644627) and
//   ClearPrimitives (0x00442760) in that order (FableWin vftable of CDrawBase; engine/NPlayerGui_CDrawBase.h VSLOTS).
// Base constructor: ??0CDrawBase@NPlayerGui@@QAE@AAVCPlayerGui@@AAV?$vector@PAVCDrawBase@NPlayerGui@@V?$allocator@PAVCDrawBase@NPlayerGui@@@std@@@std@@@Z 0x0064CC65 (Ego_r / FableWin; table witness 0x0064E1BE).
// Was a local model (struct CDrawBase / CDrawSpiritsHealth with a placeholder v00) emitting ??_7CVtable_0125a538@@6B@ / ??_7CDrawSpiritsHealth@@6B@.
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_CTOR
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_VSLOTS
#include "engine/NPlayerGui_CDrawSpiritsHealth.h"
namespace NPlayerGui {
CDrawSpiritsHealth::CDrawSpiritsHealth(CPlayerGui& playerGui, std::vector<CDrawBase*>& illustrators)
    : CDrawBase(playerGui, illustrators)
{
}
}
