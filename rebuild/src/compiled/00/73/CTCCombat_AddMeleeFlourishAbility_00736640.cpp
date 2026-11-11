// CTCCombat::AddMeleeFlourishAbility(long defIndex) -- looks up a melee combat ability def, appends it to AbilityList and recomputes the usable abilities
// Leaf: ?AddMeleeFlourishAbility@CTCCombat@@QAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00736640, length 128 bytes (default /O2 TU); Ego_r.pdb / FableWin.pdb
// ?AddMeleeFlourishAbility@CTCCombat@@QAEXJ@Z (it sits between UpdateUsableMeleeCombatAbilities 0x00736590 and
// RemoveMeleeFlourishAbility 0x007366C0).
// Callees: CTCBase::GetDefinitionManager() const @ 0x006869D0, CDefinitionManager::GetDef<CMeleeCombatAbilityDef> @ 0x00737F80
// (FableWin.pdb), the STLport vector<CDefPointer<const CMeleeCombatAbilityDef> >::_M_insert_overflow @ 0x00739BC0 (push_back's full
// path), CTCCombat::UpdateUsableMeleeCombatAbilities @ 0x00736590 (private).
// Shape: plain member code over the shared headers: the header's noinline GetDef body keeps the handle's address unescaped, so its
// object is read once after GetDef (edi) for the inline push_back copy and the release; push_back's __false_type tag sits in the dead
// def_index slot.
// Layout: engine/CTCCombat.h (AbilityList +0x2C, a 12-byte STLport vector).
// Name fix (agentNF10 2026-11-11): moved off the local CTCBase / CDefinitionManager / CDefPointer / CDefPointerVector stand-ins onto the
// shared headers (engine/CTCCombat.h FABLE_RETAIL_CTCCOMBAT_FLOURISH / _ABILITYLIST). 0x006869D0 binds the PDB const GetDefinitionManager,
// 0x00737F80 ??$GetDef@VCMeleeCombatAbilityDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCMeleeCombatAbilityDef@@@@@Z (was a
// GetDef_00737f80 stand-in), 0x00739BC0 the STLport vector member (was ?_M_insert_overflow@CDefPointerVector@@..), 0x00736590 the
// private AAE spelling (was QAE).
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCOMBATABILITY
#define FABLE_RETAIL_CCOMBATABILITYDATA
#define FABLE_RETAIL_CTCCOMBAT
#define FABLE_RETAIL_CTCCOMBAT_FLOURISH
#define FABLE_RETAIL_CTCCOMBAT_ABILITYLIST
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CDefClassBase.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CTCCombat.h"

void CTCCombat::AddMeleeFlourishAbility(long def_index)
{
    CDefPointer<const CMeleeCombatAbilityDef> def;
    GetDefinitionManager().GetDef(def_index, &def);
    AbilityList.push_back(def);
    UpdateUsableMeleeCombatAbilities();
}
