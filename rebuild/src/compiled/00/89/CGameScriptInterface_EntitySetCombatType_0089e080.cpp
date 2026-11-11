// CGameScriptInterface::EntitySetCombatType(const CScriptThing& thing, const CCharString& combat_type) const -- for a live thing with a combat component, look the combat type def up by name and set it
// Leaf: ?EntitySetCombatType@CGameScriptInterface@@UBEXABVCScriptThing@@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0089E080, 164 bytes, `ret 8`. CGameScriptInterface vtable slot 461; Ego_r.pdb / FableWin.pdb
//   ?EntitySetCombatType@CGameScriptInterface@@UBEXABVCScriptThing@@ABVCCharString@@@Z.
// Callees (PDB spellings): CScriptThing::GetPThing (virtual slot 11, +0x2C), CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020
//   (the forced-inline GetTC<CTCCombat>, TCI_COMBAT 0x30), CThing::PeekDefinitionManager 0x004C7990,
//   CDefinitionManager::GetDef<CCombatTypeDef>(const CCharString&, CDefPointer<const CCombatTypeDef>*) const 0x00669770 (landed),
//   CTCCombat::SetCombatType(const CDefPointer<const CCombatTypeDef>&) 0x007355F0, the def's virtual release.
// Layout: CThing Dead bit +0x91, component bit 0x30 in +0x24; the def handle homed in the dead first-argument slot.
// Shape: the handle escapes into GetDef and SetCombatType, so its release re-reads the count through ecx again (the long form).
// Name fix (agentNF11 2026-11-11): moved off the local ScriptMethods / CThingNative / DefPointer / CTCCombatMethods stand-ins onto the shared
// headers. 0x004C7990 binds ?PeekDefinitionManager@CThing@@QBEABVCGameDefinitionManager@@XZ (was a local GetDefinitionManager), 0x00669770
// the PDB GetDef<CCombatTypeDef> by-name instantiation (was ?GetCombatType@CDefinitionManagerMethods@@..), 0x007355F0 the PDB SetCombatType
// (was CTCCombatMethods); the row's own symbol is the PDB virtual const member (was CGameScriptInterfaceMethods).
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCOMBATABILITY
#define FABLE_RETAIL_CCOMBATABILITYDATA
#define FABLE_RETAIL_CTCCOMBAT
#define FABLE_RETAIL_NSCRIPT
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CDefClassBase.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CTCCombat.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::EntitySetCombatType(const CScriptThing& thing, const CCharString& combat_type) const
{
    CThing* pthing = thing.GetPThing();
    CDefPointer<const CCombatTypeDef> type_def;
    CTCCombat* combat;
    if (pthing && !pthing->IsDead() && pthing->GetTC(&combat)) {
        if (pthing->PeekDefinitionManager().GetDef(combat_type, &type_def))
            combat->SetCombatType(type_def);
    }
}
