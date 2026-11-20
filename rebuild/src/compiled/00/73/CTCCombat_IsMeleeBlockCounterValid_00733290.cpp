// CTCCombat::IsMeleeBlockCounterValid() const -- whether a block-counter melee move is available: always false in the retail build
// Leaf: ?IsMeleeBlockCounterValid@CTCCombat@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00733290, 3 bytes: `xor al,al ; ret` (thiscall; `this` unused).
// Identity: Ego_r.pdb 0x0074B420 / FableWin.pdb 0x02500780 ?IsMeleeBlockCounterValid@CTCCombat@@QBE_NXZ. Ego_r order
//   IsMeleeBlockCounterValid 0x0074B420 / SetPossibleQueuedAction 0x0074B430 = retail 0x00733290 / 0x007332A0 (landed
//   CTCCombat::SetPossibleQueuedAction). Its two callers use it as the block-counter gate: NPlayerGui::CDrawMeleeActionButtonEffects::
//   Update 0x0064BE45 calls it on the TCI_COMBAT component and, when true, CTCCombat::GetPMeleeBlockCounterAbility 0x007334B0;
//   CInputProcessCombat::ProcessArmedMeleeInput 0x0068ECA0 queues CREATURE_ABILITY_BREAK_BLOCK behind it.
// Naming (NF138, 2026-11-20): was the free stand-in `bool __fastcall fn_operator()` (?fn_operator@@YI_NXZ; catalog label
//   CTCCombat::IsBreakBlockValid, descriptive); now the member on engine/CTCCombat.h (FABLE_RETAIL_CTCCOMBAT_ISMELEEBLOCKCOUNTERVALID).
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCOMBATABILITY
#define FABLE_RETAIL_CCOMBATABILITYDATA
#define FABLE_RETAIL_CTCCOMBAT
#define FABLE_RETAIL_CTCCOMBAT_ISMELEEBLOCKCOUNTERVALID
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCCombat.h"

bool CTCCombat::IsMeleeBlockCounterValid() const
{
    return false;
}
