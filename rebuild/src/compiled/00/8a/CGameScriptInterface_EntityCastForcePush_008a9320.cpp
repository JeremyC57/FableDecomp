// CGameScriptInterface::EntityCastForcePush(const CScriptThing& thing, bool) const -- if the script thing is alive and a creature, make it
// cast a force push: a CCreatureAction_ForcePushInto(creature) (OnlyHitEnemies set from the flag) as its current action; returns
// SetCurrentAction's result (false when the thing is not alive or not a creature)
// Leaf: ?EntityCastForcePush@CGameScriptInterface@@UBE_NABVCScriptThing@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008A9320, 121 bytes, `ret 8`; CGameScriptInterface vtable slot 538.
// Callees: CScriptThing::IsAlive (virtual slot 75, +0x12C), CScriptThing::GetPThing (virtual slot 11, +0x2C),
//   CCreatureAction_ForcePushInto::CCreatureAction_ForcePushInto(CThingCreatureBase&) @ 0x008ADFE0 (stores vtable 0x012783D4, RTTI
//   CCreatureAction_ForcePushInto; FableWin.pdb ??0CCreatureAction_ForcePushInto@@QAE@AAVCThingCreatureBase@@@Z; clears +0x114),
//   CThingCreatureBase::SetCurrentAction @ 0x006644F0, ~CCombatActionBase @ 0x0062E900 (the action's implicit destructor).
// Shape: the 0x118-byte action is a named local (OnlyHitEnemies +0x114 is set between the constructor and the call); the creature test is
//   the CThingCreatureBase type bit (GetTypeFlags() & 8, `test byte ptr [esi+6Ch],8`).
// Port (NF93, q11261): the old row modelled CScriptThing through a fastcall function-pointer table overlay, the action as an opaque
//   `CCreatureAction_SheatheWeapons_Overlay` with Construct / Destruct members, and CThingCreatureBase locally. Now on the shared headers
//   (the landed 0x008A05A0 recipe for IsAlive / GetPThing): SetCurrentAction 0x006644F0 and ~CCombatActionBase 0x0062E900 bind the PDB
//   spellings and the ctor binds the FableWin spelling. The action class is CCreatureAction_ForcePushInto (the ctor body's vtable RTTI +
//   FableWin.pdb; Ego_r.pdb member OnlyHitEnemies +0x12C = retail +0x114), not SheatheWeapons. It has no shared header: row-local on the
//   shared CCombatActionBase.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_NSCRIPT
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterface.h"

class CCreatureAction_ForcePushInto : public CCombatActionBase {        // retail sizeof 0x118, vtable 0x012783D4 (no shared header)
public:
    CCreatureAction_ForcePushInto(CThingCreatureBase& creature);         // 0x008ADFE0 (FableWin.pdb)
    bool OnlyHitEnemies;                                                 // +0x114 (Ego_r.pdb +0x12C)
};

bool CGameScriptInterface::EntityCastForcePush(const CScriptThing& thing, bool only_hit_enemies) const
{
    if (thing.IsAlive()) {
        CThing* pthing = thing.GetPThing();
        if (pthing->GetTypeFlags() & 8) {
            CThingCreatureBase* pcreature = static_cast<CThingCreatureBase*>(pthing);
            CCreatureAction_ForcePushInto action(*pcreature);
            if (only_hit_enemies)
                action.OnlyHitEnemies = true;
            return pcreature->SetCurrentAction(action);
        }
    }
    return false;
}
