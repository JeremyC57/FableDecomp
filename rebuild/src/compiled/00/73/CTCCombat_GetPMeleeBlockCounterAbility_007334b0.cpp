// CTCCombat::GetPMeleeBlockCounterAbility() const -- the block-counter melee ability (PBlockCounterAbility +0x48), copied out as a counted def pointer
// Leaf: ?GetPMeleeBlockCounterAbility@CTCCombat@@QBE?AV?$CDefPointer@$$CBVCMeleeCombatAbilityDef@@@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007334B0, 19 bytes (`ret 4`, hidden result pointer). No callees.
// Identity: Ego_r.pdb 0x0074B630 / FableWin.pdb 0x02500790 ?GetPMeleeBlockCounterAbility@CTCCombat@@QBE?AV?$CDefPointer@$$CBVCMeleeCombatAbilityDef@@@@XZ.
//   Ego_r order GetPMeleeFlourishAbility 0x0074B600 / GetPMeleeBlockCounterAbility 0x0074B630 / GetPMeleeFlourish360Ability 0x0074B650 =
//   retail 0x00733480 / 0x007334B0 (same 0x30 spacing); the body returns the member at +0x48, which is the retail CTCCombat's
//   PBlockCounterAbility (engine/CTCCombat.h, retail branch: STLport 12-byte vectors); the only caller, NPlayerGui::
//   CDrawMeleeActionButtonEffects::Update 0x0064BE45, calls it on its TCI_COMBAT component after IsMeleeBlockCounterValid 0x00733290.
//   The catalog label CThing::GetPBaseDef was propagated over the byte-identical "return a CDefPointer member" body.
// Shape: CDefPointer<T>'s copy constructor stores the pointer into the hidden result first and then bumps the def's count at +4 when
//   it is non-null (`mov [eax],ecx ; je ; inc dword ptr [ecx+4]`).
// Naming (NF138, 2026-11-20): moved off the local CThing / CDefPointer / CDefPointee stand-ins onto engine/CTCCombat.h
//   (FABLE_RETAIL_CTCCOMBAT_GETPMELEEBLOCKCOUNTERABILITY) and engine/CDefPointer.h; own symbol
//   ?GetPBaseDef@CThing@@QBE?AV?$CDefPointer@UCDefPointee@@@@XZ -> the PDB spelling above.
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
#define FABLE_RETAIL_CTCCOMBAT_GETPMELEEBLOCKCOUNTERABILITY
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCCombat.h"

CDefPointer<const CMeleeCombatAbilityDef> CTCCombat::GetPMeleeBlockCounterAbility() const
{
    return PBlockCounterAbility;
}
