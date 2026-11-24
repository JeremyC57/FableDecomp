// CCreatureAction_ScreamerDrainAttack::FinishAction -- transition to drain-out action then finish combat state
// Leaf: ?FinishAction@CCreatureAction_ScreamerDrainAttack@@UAEXXZ
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail008B9010,242 bytes. Callees: pointer GetPItemA01B50; Combat constructors857F30/858030;
// creature SetCurrentAction6644F0; Combat destructor62E900 and FinishAction859A20.
// Native combat action projection: base70 genuine slots; combat adds11 =81 slots.
// BaseA8 and Combat114; PDB baseB4/Combat12C. See actual headers and raw vtable audit.
// Name fix (agentNF6 2026-11-10): CCombatActionBase::FinishAction 0x00859A20 is protected virtual (?FinishAction@CCombatActionBase@@MAEXXZ, Ego_r.pdb / FableWin.pdb).
// Port (NF93, q11261 / q11262): the renaming-macro includes (CCreatureActionBase / CCombatActionBase / CThing / CThingCreatureBase /
//   ScreamerDrain* `#define X XPdb`) and the local models of those classes, CBaseIntelligentPointer / CIntelligentPointer and CSoundPair replaced by
//   the shared headers (the 0x008C2980 recipe + IS_THING_ALIVE) with the wfE11 _PROTOTYPE class blocks and the NF93 opt-ins
//   CCREATUREACTION_SCREAMERDRAINOUTOF_CTORS (the FableWin.pdb inline ctors) / CCREATUREACTION_SCREAMERDRAINATTACK_FINISHACTION:
//   SetCurrentAction 0x006644F0 and ~CCombatActionBase 0x0062E900 now bind the PDB spellings. CCreatureAction_CastWillDrainLifeSpell (the RTTI
//   base of ScreamerDrainAttack) has no shared header: row-local on CCombatActionBase as in the landed 0x006F5C10.
#define FABLE_RETAIL_CCOMBATACTIONBASE_ALLOWSCOMBINATIONANIMS  // NF264 (q12750): CCombatActionBase::AllowsCombinationAnims override, so emitted tables bind slot 66 = 0x0062EA10 as retail
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_HASTC_FORCEINLINE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCREATUREACTIONBASE_ACCESSORS
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCOMBATACTIONBASE_FINISHACTION
#define FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CCOMBATACTIONBASE_TARGETCTOR
#define FABLE_RETAIL_CCOMBATACTIONBASE_PRIORITYCTOR
#define FABLE_RETAIL_CCREATUREACTION_SCREAMERDRAINOUTOF_PROTOTYPE
#define FABLE_RETAIL_CCREATUREACTION_SCREAMERDRAINOUTOF_CTORS
#define FABLE_RETAIL_CCREATUREACTION_SCREAMERDRAINATTACK_PROTOTYPE
#define FABLE_RETAIL_CCREATUREACTION_SCREAMERDRAINATTACK_FINISHACTION
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CSoundPair.h"
#include "engine/CCombatActionBase.h"
#include "engine/ThingAlive.h"
#include "engine/CCreatureAction_ScreamerDrainOutOf.h"

class CCreatureAction_CastWillDrainLifeSpell : public CCombatActionBase {   // RTTI base of ScreamerDrainAttack (no shared header)
public:
    CCreatureAction_CastWillDrainLifeSpell();                            // 0x008BBB10
protected:
    long Level;                                                          // +0x114 (Ego_r.pdb +0x12C)
    long DrainDefIndex;                                                  // +0x118
};
#include "engine/CCreatureAction_ScreamerDrainAttack.h"

void CCreatureAction_ScreamerDrainAttack::FinishAction()
{
    if (IS_THING_ALIVE(PTarget)) {
        CCreatureAction_ScreamerDrainOutOf next(*PCreature, *PTarget);
        PCreature->SetCurrentAction(next);
    } else {
        CCreatureAction_ScreamerDrainOutOf next(*PCreature);
        PCreature->SetCurrentAction(next);
    }
    CCombatActionBase::FinishAction();
}
