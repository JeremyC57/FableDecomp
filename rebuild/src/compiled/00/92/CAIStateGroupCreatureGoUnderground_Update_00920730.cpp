// CAIStateGroup_CreatureGoUnderground::Update() -- perform the go-underground action, then terminate
// Leaf: ?Update@CAIStateGroup_CreatureGoUnderground@@MAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00920730, length 59 bytes (+ int3 padding).
// Callees: CCreatureAction_GoUnderground::CCreatureAction_GoUnderground(CThingCreatureBase*)
// @ 0x008EC6A0, CAIStateGroupBase::AIAction_PerformActionAndWait(CCreatureActionBase*, bool)
// @ 0x0097DB70, CCreatureActionBase::~CCreatureActionBase @ 0x0062E900, Terminate @ 0x008FCF40.
// The action is a temporary inside the call expression: `false` is pushed before the temporary
// is constructed (VC evaluates right to left), the constructor's returned `this` is pushed as
// the first argument, and the base destructor runs after the call (the derived class adds no
// destructor, so only CCreatureActionBase's out-of-line one is called). The frame is exactly
// the temporary (0x114 bytes); the creature is read into a named local first (that is what keeps
// it in eax rather than ecx for the push). Taking a temporary's address is the VC extension the retail
// code relied on (C4238).
// NF134 (2026-11-20, q5021): ported onto the shared headers (template: landed 0x009208E0 CAIStateGroup_CreatureIdleUnderground::OnActivate):
//   engine/CAIStateGroupBase.h (retail members + PERFORMACTION: ?AIAction_PerformActionAndWait@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@ABVCCreatureActionBase@@_N@Z,
//   Ego_r.pdb 0x00848940 + FableWin.pdb 0x02892570; GetCreature, Terminate 0x008FCF40), engine/CCombatActionBase.h and
//   engine/CCreatureAction_UndeadReturnToEarth.h: the action built at 0x008EC6A0 is ??0CCreatureAction_UndeadReturnToEarth@@QAE@AAVCThingCreatureBase@@@Z
//   (catalog + landed binder 0x0074ACE0), destroyed through ~CCombatActionBase 0x0062E900. Was the namespaced N_0062e900 / N_008ec6a0 stand-ins
//   and a local CAIStateGroupBase / CAIBrain model.
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCREATUREACTION_UNDEADRETURNTOEARTH_CLASS
#include "fable_string.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CThingAICreature.h"
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCreatureAction_UndeadReturnToEarth.h"

class CAIStateGroup_CreatureGoUnderground : public CAIStateGroupBase {   // RTTI : CAIStateGroupBase
protected:
    virtual void Update();                  // ?Update@CAIStateGroup_CreatureGoUnderground@@MAEXXZ (Ego_r.pdb 0x009435D0 + FableWin.pdb 0x02C23CD0: protected)
};

void CAIStateGroup_CreatureGoUnderground::Update()
{
    AIAction_PerformActionAndWait(CCreatureAction_UndeadReturnToEarth(GetCreature()), false);
    Terminate();
}
