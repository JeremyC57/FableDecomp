// Leaf: ?RemoveAllQuestInfoElements@CGameScriptInterface@@UBEXXZ
// Switch route (NF318, 2026-11-23): defined on the real class of engine/CGameScriptInterface.h under its class-model switch FABLE_RETAIL_NSCRIPT (was the flattened
//   free stand-in ?CGameScriptInterface_RemoveAllQuestInfoElements@@YIXPAX@Z); the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member here.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"   // switch route: the real class model
struct QuestMgr;
extern void __fastcall QuestMgr_RemoveAll(QuestMgr* self);

struct GameObj;
extern GameObj* g_gameObj;

void CGameScriptInterface::RemoveAllQuestInfoElements() const
{
    QuestMgr_RemoveAll(*(QuestMgr**)((char*)g_gameObj + 0x1c8));
}