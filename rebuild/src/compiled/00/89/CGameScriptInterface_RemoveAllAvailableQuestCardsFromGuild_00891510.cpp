struct CGuild;
// Name fix (agentNF7 2026-11-10): 0x013B89FC bound as the PDB's CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass through the shared engine/CTBaseSingletonNoDefaultAlloc.h (was a private stand-in global).
#include "engine/CTBaseSingletonNoDefaultAlloc.h"
class CQuestManager;
// 0x013B89FC = CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass (private static; reached through the public inline Get())
extern void __fastcall Guild_RemoveAllAvailableQuestCards(CGuild* self);

void __fastcall CGameScriptInterface_RemoveAllAvailableQuestCardsFromGuild(void* self)
{
    (void)self;
    Guild_RemoveAllAvailableQuestCards(reinterpret_cast<CGuild*>(&CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get()));
}