// Leaf: ?RemoveAllAvailableQuestCardsFromGuild@CGameScriptInterface@@UBEXXZ
// Switch route (NF318, 2026-11-23): defined on the real class of engine/CGameScriptInterface.h under its class-model switch FABLE_RETAIL_NSCRIPT (was the flattened
//   free stand-in ?CGameScriptInterface_RemoveAllAvailableQuestCardsFromGuild@@YIXPAX@Z); the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member here.
#define FABLE_RETAIL_NSCRIPT
struct CGuild;
// Name fix (agentNF7 2026-11-10): 0x013B89FC bound as the PDB's CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass through the shared engine/CTBaseSingletonNoDefaultAlloc.h (was a private stand-in global).
#include "engine/CTBaseSingletonNoDefaultAlloc.h"
#include "engine/CGameScriptInterface.h"   // switch route: the real class model
class CQuestManager;
// 0x013B89FC = CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass (private static; reached through the public inline Get())
extern void __fastcall Guild_RemoveAllAvailableQuestCards(CGuild* self);

void CGameScriptInterface::RemoveAllAvailableQuestCardsFromGuild() const
{
    Guild_RemoveAllAvailableQuestCards(reinterpret_cast<CGuild*>(&CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get()));
}