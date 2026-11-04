// CThingCreatureBase::ClearQueuedActions() -- drop every action queued behind the creature's current action
// Leaf: ?ClearQueuedActions@CThingCreatureBase@@QAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00663600, 11 bytes: `add ecx,108h ; jmp 0x00669B50` (PQueuedActions.clear(), tail call).
// Identity (two sources): Ego_r.pdb / FableWin.pdb ?ClearQueuedActions@CThingCreatureBase@@QAEXXZ, the only "clear" of the queued-action
//   list list<CCountedPointer<CCreatureActionBase> > PQueuedActions (+0x108, engine/CThingCreatureBase.h; Ego_r +0x124 less the
//   STLport shrinks), whose out-of-line _List_base clear 0x00669B50 is the jump target; its callers are the script commands that clear a
//   creature's actions (CGameScriptInterface::SheatheHeroWeapons 0x008916D2, CScriptGameResourceObjectScriptedThing::ClearAllActions
//   0x00903957 / ClearAllActionsIncludingCurrent 0x009039A7). The catalog's "_Cons_val<..CParticleMorphs..>" was a propagated label.
// Callees: ?clear@?$_List_base@V?$CCountedPointer@VCCreatureActionBase@@@@V?$allocator@V?$CCountedPointer@VCCreatureActionBase@@@@@std@@@std@@QAEXXZ
//   0x00669B50 (STLport 4.6 defines _List_base::clear out of class in _list.c; retail calls it out of line, so the TU sees only the
//   declaration of that instantiation).
// Header: engine/CThingCreatureBase.h (FABLE_RETAIL_CTHINGCREATUREBASE + FABLE_RETAIL_CTHINGCREATUREBASE_CLEARQUEUED).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_CLEARQUEUED
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CDefPointer.h"
#include "stlport/fable_containers.h"
#include "engine/CThingCreatureBase.h"
class CCreatureActionBase;
template <> void std::_List_base<CCountedPointer<CCreatureActionBase>, std::allocator<CCountedPointer<CCreatureActionBase> > >::clear();   // 0x00669B50
void CThingCreatureBase::ClearQueuedActions()
{
    PQueuedActions.clear();
}