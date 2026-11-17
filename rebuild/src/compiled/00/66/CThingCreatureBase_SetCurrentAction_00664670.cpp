// CThingCreatureBase::SetCurrentAction(const CCountedPointer<CCreatureActionBase>& action) -- the counted-pointer overload: forwards the pointee
//   to SetCurrentAction(const CCreatureActionBase&) 0x006644F0 (a tail jump that rewrites the argument slot with action.Data)
// Leaf: ?SetCurrentAction@CThingCreatureBase@@QAE_NABV?$CCountedPointer@VCCreatureActionBase@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00664670, 15 bytes: `mov eax,[esp+4] ; mov eax,[eax] ; mov [esp+4],eax ; jmp 0x006644F0`.
// Name (NF88, q11161): Ego_r.pdb 0x005429A0 / FableWin.pdb 0x01D94200 ?SetCurrentAction@CThingCreatureBase@@QAE_NABV?$CCountedPointer@VCCreatureActionBase@@@@@Z;
//   the landed callers 0x00712B60 (NTCScriptedControl::CActionDoCreatureAction::OnActivate) and 0x008D6D40 / 0x008D6C80 bind exactly this
//   spelling, and the jump target 0x006644F0 is the landed reference overload ?SetCurrentAction@CThingCreatureBase@@QAE_NABVCCreatureActionBase@@@Z.
//   Was catalogued as std::_Cons_val<allocator<pair<EHeroMorphType, CParticleMorphs::CEntry> >, ...> (a propagated label) with the stand-in
//   leaf ?Cons_val@@YIXPAUAlloc@@PAUPair@@PAPAX@Z over a free cons_helper.
// Port (NF88): on engine/CThingCreatureBase.h (NF83 opt-in FABLE_RETAIL_CTHINGCREATUREBASE_SETCURRENTACTION_COUNTED) and engine/CCountedPointer.h,
//   the 0x008D6C80 define set.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_SETCURRENTACTION_COUNTED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCountedPointer.h"

bool CThingCreatureBase::SetCurrentAction(const CCountedPointer<CCreatureActionBase>& action)
{
    return SetCurrentAction(*action);
}
