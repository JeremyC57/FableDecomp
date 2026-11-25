// CGameScriptInterface::DisplayMiniGameInfo(bool display, EMiniGameType type) const -- the player GUI's quest info DisplayMiniGameInfo(display, type, 0)
// Leaf: ?DisplayMiniGameInfo@CGameScriptInterface@@UBEX_NW4EMiniGameType@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00891C60. Identity (NF433, 2026-11-25): the retail RTTI vftable placement of
//   CGameScriptInterface (vtable 0x01260F0C, ghidra_out/labels_rtti_port.tsv) names this PDB member here, and Ego_r.pdb /
//   FableWin.pdb list the symbol (two sources); defined on the real class of engine/CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT).
// FableWin's /Od body: ?Get@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@SIAAVCPlayerGui@@XZ, the inline ?GetQuestInfo@CPlayerGui@@QAEAAVCDrawQuestInfo@NPlayerGui@@XZ
//   (PDrawQuestInfo +0x1C8; retail expands both, PClass 0x013B8790), then ?DisplayMiniGameInfo@CDrawQuestInfo@NPlayerGui@@QAEX_NW4EMiniGameType@@J@Z
//   (retail 0x00654593) with (display, type, 0).
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
#define FABLE_RETAIL_CPLAYERGUI_GETQUESTINFO
#define FABLE_RETAIL_CDRAWQUESTINFO_ORACLESYMBOLS
#include "engine/CGameScriptInterface.h"
#include "engine/CPlayerGui.h"
#include "engine/NPlayerGui_CDrawQuestInfo.h"

void CGameScriptInterface::DisplayMiniGameInfo(bool display, EMiniGameType type) const
{
    CPlayerGui::Get().GetQuestInfo().DisplayMiniGameInfo(display, type, 0);
}
