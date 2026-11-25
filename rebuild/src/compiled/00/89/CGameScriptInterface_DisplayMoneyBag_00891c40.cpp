// CGameScriptInterface::DisplayMoneyBag(bool show) const -- the player GUI singleton's ShowMoneyBag (+0x29e) = show
// Leaf: ?DisplayMoneyBag@CGameScriptInterface@@UBEX_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00891C40. Identity (NF431, 2026-11-25): the retail RTTI vftable placement of
//   CGameScriptInterface (vtable 0x01260F0C, ghidra_out/labels_rtti_port.tsv) names this PDB member here, and Ego_r.pdb /
//   FableWin.pdb list the symbol (two sources); defined on the real class of engine/CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT).
// FableWin's /Od body: ?Get@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@SIAAVCPlayerGui@@XZ then the inline
//   ?SetShowMoneyBag@CPlayerGui@@QAEX_N@Z (one byte store, +0x29e); retail expands both (PClass 0x013B8790).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_MEMBERS
#define FABLE_RETAIL_CPLAYERGUI_SETSHOWMONEYBAG
#include "engine/CGameScriptInterface.h"
#include "engine/CPlayerGui.h"

void CGameScriptInterface::DisplayMoneyBag(bool show) const
{
    CPlayerGui::Get().SetShowMoneyBag(show);
}
