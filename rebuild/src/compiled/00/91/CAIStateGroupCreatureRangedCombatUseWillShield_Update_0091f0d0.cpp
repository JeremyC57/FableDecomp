// CAIStateGroup_CreatureRangedCombatUseWillShield::Update() -- casts the physical shield spell (and waits for it), then terminates the state group
// Leaf: ?Update@CAIStateGroup_CreatureRangedCombatUseWillShield@@MAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0091F0D0, length 97 bytes (+ int3 padding).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0 on "" @ 0x0122D70E,
// CCreatureAction_CastPhysicalShieldSpell::CCreatureAction_CastPhysicalShieldSpell(CThingCreatureBase*, long, bool,
// const CCharString&, long, float) @ 0x008BB6F0 (`ret 18h`), CAIStateGroupBase::AIAction_PerformActionAndWait
// (CCreatureActionBase*, bool) @ 0x0097DB70, the action base destructor @ 0x0062E900, CAIStateGroupBase::Terminate
// @ 0x008FCF40 (Terminated = true).
// Shape: the empty name is a NAMED local in an inner scope (built before anything is pushed, destroyed after the
// action temporary and before Terminate), the creature a named local after it (eax chain; inline gives ecx/edx);
// the action is a temporary inside the call (the landed 0x00933BB0
// shape: `false` pushed first, then the constructor arguments right to left with 0.0f as `push 0`, the
// constructor's returned `this` pushed as the action, the base destructor after the call). Frame 0x128 = the
// string (+0) and the 0x124-byte retail action (+4).
// Model: local (engine/CAIStateGroup_CreatureRangedCombatUseWillShield.h is a flattened base blob).
// NF134 (2026-11-20, q5021): ported onto the shared headers (as 0x00920730 here, template landed 0x009208E0):
//   engine/CAIStateGroupBase.h (PERFORMACTION: ?AIAction_PerformActionAndWait@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@ABVCCreatureActionBase@@_N@Z,
//   Ego_r.pdb 0x00848940 + FableWin.pdb 0x02892570), engine/CCombatActionBase.h and engine/CCreatureAction_CastPhysicalShieldSpell.h
//   (_CTOR: ??0CCreatureAction_CastPhysicalShieldSpell@@QAE@AAVCThingCreatureBase@@J_NABVCCharString@@JM@Z, Ego_r.pdb 0x008F3C30 + FableWin.pdb
//   0x02A188D0, as the landed 0x008E0060 binds it; the temporary's implicit destructor calls ~CCombatActionBase 0x0062E900). Was the namespaced
//   N_0062e900 stand-in and a local CCharString / CAIStateGroupBase / CAIBrain model.
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
#define FABLE_RETAIL_CCREATUREACTION_CASTPHYSICALSHIELD
#define FABLE_RETAIL_CCREATUREACTION_CASTPHYSICALSHIELD_CTOR
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
#include "engine/CCreatureAction_CastPhysicalShieldSpell.h"

class CAIStateGroup_CreatureRangedCombatUseWillShield : public CAIStateGroupBase {   // RTTI : CAIStateGroupBase
protected:
    virtual void Update();                  // ?Update@CAIStateGroup_CreatureRangedCombatUseWillShield@@MAEXXZ (Ego_r.pdb 0x00942F40 + FableWin.pdb 0x02C193F0: protected)
};

void CAIStateGroup_CreatureRangedCombatUseWillShield::Update()
{
    {
        CCharString sound_name("", -1);
        CThingAICreature& creature = GetCreature();
        AIAction_PerformActionAndWait(CCreatureAction_CastPhysicalShieldSpell(creature, 2, true, sound_name, 5, 0.0f), false);
    }
    Terminate();
}
