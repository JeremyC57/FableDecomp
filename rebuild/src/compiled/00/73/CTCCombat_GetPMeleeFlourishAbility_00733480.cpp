// CTCCombat::GetPMeleeFlourishAbility() const -- the usable melee ability at CurrentAbilityIndex (wrapped modulo the list length), copied out as a counted def pointer
// Leaf: ?GetPMeleeFlourishAbility@CTCCombat@@QBE?AV?$CDefPointer@$$CBVCMeleeCombatAbilityDef@@@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00733480, oracle length 41 bytes (`ret 4`, hidden result pointer).
// Identity: Ego_r.pdb 0x0074B600 / FableWin.pdb 0x025005A0 ?GetPMeleeFlourishAbility@CTCCombat@@QBE?AV?$CDefPointer@$$CBVCMeleeCombatAbilityDef@@@@XZ
//   (Ego_r order GetPMeleeFlourishAbility 0x0074B600 / GetPMeleeBlockCounterAbility 0x0074B630 = retail 0x00733480 / 0x007334B0).
// Retail bytes:  mov eax,[ecx+38h] ; push esi ; mov esi,[ecx+3Ch] ; sub esi,eax ; mov eax,[ecx+44h] ; cdq ; sar esi,2 ; idiv esi ;
//   mov eax,[ecx+38h] ; pop esi ; mov edx,[eax+edx*4] ; test edx,edx ; mov eax,[esp+4] ; mov [eax],edx ; je R ; inc dword ptr [edx+4] ; R: ret 4
// Layout: engine/CTCCombat.h (retail branch): UsableAbilityList +0x38 (a 12-byte STLport vector), CurrentAbilityIndex +0x44.
// Shape: signed modulo (`cdq ; idiv`): the index is a long and the list size is taken as a long.
// Naming (NF138, 2026-11-20): moved off the local CTCCombat_Retail / CRetailVector / CDefPointer_MeleeCombatAbilityDef stand-ins onto the
//   shared engine/CTCCombat.h (FABLE_RETAIL_CTCCOMBAT_GETPMELEEFLOURISHABILITY) and engine/CDefPointer.h; own symbol
//   ?GetPMeleeFlourishAbility@CTCCombat_Retail@@QAE?AUCDefPointer_MeleeCombatAbilityDef@@XZ -> the PDB spelling above (q4747).
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
#define FABLE_RETAIL_CTCCOMBAT_GETPMELEEFLOURISHABILITY
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCCombat.h"

CDefPointer<const CMeleeCombatAbilityDef> CTCCombat::GetPMeleeFlourishAbility() const
{
    return UsableAbilityList[CurrentAbilityIndex % (long)UsableAbilityList.size()];
}
