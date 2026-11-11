// CGameScriptInterface::EntityResetCombatTypeToDefault(const CScriptThing& thing) const -- for a live creature, set its combat type back to the one its creature def names (through EntitySetCombatType)
// Leaf: ?EntityResetCombatTypeToDefault@CGameScriptInterface@@UBEXABVCScriptThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00890F40, 113 bytes, `ret 4`. CGameScriptInterface vtable slot 462; Ego_r.pdb / FableWin.pdb
//   ?EntityResetCombatTypeToDefault@CGameScriptInterface@@UBEXABVCScriptThing@@@Z.
// Callees (PDB spellings): CScriptThing::GetPThing (virtual slot 11, +0x2C), CThingCreatureBase::GetCombatType 0x00662170 (Ego_r.pdb
//   ?GetCombatType@CThingCreatureBase@@QBEJXZ: Ego_r donor order GetEyeGraphic 0x0053CFE0 / GetCombatType 0x0053CFF0 / GetDamageMultiplier
//   0x0053D000 = retail 0x00662160 / 0x00662170 / 0x00662190; the body reads PDef->CombatTypeDef), CThing::PeekDefinitionManager 0x004C7990,
//   CDefinitionManager::GetDefNameFromGlobalIndex 0x009ACCC0, CDefStringTable::GetString 0x009D49B0 (the inline CDefString conversion on
//   GDefStringTable 0x013CA828), ~CCharString 0x0099EAE0, this->EntitySetCombatType (virtual slot 461, +0x734).
// Layout: CThing Dead bit +0x91, type flags +0x6C (bit 8: a creature).
// Name fix (agentNF11 2026-11-11): rewritten off the local Entity / ArgObj / DefObj stand-ins (the NF10 "unsalvageable" local model) onto
// the shared headers. 0x004C7990 binds ?PeekDefinitionManager@CThing@@QBEABVCGameDefinitionManager@@XZ, 0x00662170 the PDB GetCombatType
// (was ?Helper_ffdd1230@Entity@@QAEHXZ), 0x009ACCC0 the PDB const GetDefNameFromGlobalIndex (was ?Helper_11bd80@DefObj@@..), 0x009D49B0
// CDefStringTable::GetString (was ?Helper_143a70@GObj@@..); the row's own symbol is the PDB virtual const member.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_COMBATTYPE
#define FABLE_RETAIL_NSCRIPT
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefString.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::EntityResetCombatTypeToDefault(const CScriptThing& thing) const
{
    CThing* pthing = thing.GetPThing();
    if (pthing && !pthing->IsDead() && (pthing->GetTypeFlags() & 8)) {
        CThingCreatureBase* creature = static_cast<CThingCreatureBase*>(pthing);
        EntitySetCombatType(thing, pthing->PeekDefinitionManager().GetDefNameFromGlobalIndex(creature->GetCombatType()));
    }
}
