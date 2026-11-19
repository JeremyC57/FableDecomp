// CCombatAction_GenericStrikeResponseBase::FinishAction() -- finish response, recover stun or enter alert below dazed threshold.
// Leaf: ?FinishAction@CCombatAction_GenericStrikeResponseBase@@UAEXXZ
// Native008663D0,285 bytes. CreatureAction nativeA8 vs donorB4; stunned adds FlipA8. Physical HealthB4/KillableBC bit1 vs donorCC/D4.
// Name fix (agentNF7 2026-11-10): the local CBaseIntelligentPointer stand-in replaced by the shared engine/CIntelligentPointer.h (Ego_r.pdb CBaseIntelligentPointer: GetPItem returns CBaseObject*, SetPItem takes CBaseObject*, virtual dtor), so the calls bind the PDB spellings.
// Port (NF93, q11261): the renaming-macro includes (`#define CCreatureActionBase CCreatureActionBase_PDB` ...) of the _quarantine headers and
//   the local CCreatureActionBase / CCreatureAction_BeStunned / CCreatureAction_AlertToPlayer / CTCCreatureModeManager / Creature (GetInterface) /
//   CCombatActionBase models replaced by the shared headers (the 0x008C2980 recipe + IS_THING_ALIVE + PeekTC, whose const LowerBound is 0x004365B0) with the existing NF74
//   GENERICSTRIKERESPONSEBASE_FINISHACTION opt-in and the NF93 opt-ins CTHINGCREATUREBASE_GETDAZEDHEALTHTHRESHOLD / CCREATUREACTION_BEDAZED_CLASS.
//   The second temporary is CCreatureAction_BeDazed, not AlertToPlayer: 0x00865A00 stores vtable 0x01276F54 (RTTI .?AVCCreatureAction_BeDazed@@)
//   and is Ego_r ??0CCreatureAction_BeDazed@@QAE@AAVCThingCreatureBase@@@Z. Own leaf QAE -> UAE (the slot-4 override the landed 0x008C2550
//   already binds). Flags: HasTC(TCI_DECAPITATION) (bit 209, +0x38) and CThingPhysical::IsKillable() (+0xbc bit 1); mode 29 = MODE_STUNNED.
//   CCreatureAction_BeStunned has no shared header: row-local on CCreatureActionBase (Ego_r.pdb Flip +0xb4 = retail +0xa8).
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
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTHINGCREATUREBASE_GETDAZEDHEALTHTHRESHOLD
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CCOMBATACTIONBASE_INITIALISE
#define FABLE_RETAIL_CCOMBATACTION_GENERICSTRIKERESPONSEBASE_TYPED
#define FABLE_RETAIL_CCOMBATACTION_GENERICSTRIKERESPONSEBASE_FINISHACTION
#define FABLE_RETAIL_CCREATUREACTION_BEDAZED_CLASS
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
#include "engine/CTCCreatureModeManager.h"
#include "engine/NCreatureMode.h"
#include "engine/CCombatAction_GenericStrikeResponseBase.h"
#include "engine/CCreatureAction_BeDazed.h"

class CCreatureAction_BeStunned : public CCreatureActionBase {        // RTTI : CCreatureActionBase, retail sizeof 0xac (no shared header)
public:
    CCreatureAction_BeStunned(CThingCreatureBase& creature, bool flip);  // 0x00865940
    virtual void PerformAction();                                        // FableWin.pdb ?PerformAction@CCreatureAction_BeStunned@@UAEXXZ
    virtual CCreatureActionBase* Clone() const;                          // Ego_r.pdb ?Clone@CCreatureAction_BeStunned@@UBEPAVCCreatureActionBase@@XZ
protected:
    bool Flip;                                                           // +0xa8 (Ego_r.pdb +0xb4)
};

void CCombatAction_GenericStrikeResponseBase::FinishAction()
{
    CCombatActionBase::FinishAction();
    if (IS_THING_ALIVE(PCreature)) {
        const CTCCreatureModeManager* pmodes;
        PCreature->PeekTC(&pmodes);
        if (pmodes->GetCurrentMode().GetType() == NCreatureMode::MODE_STUNNED) {
            CThingCreatureBase* powner = PCreature.Get();
            powner->SetCurrentAction(CCreatureAction_BeStunned(*PCreature, false));
        } else {
            CThingCreatureBase* pcreature = PCreature.Get();
            float dazed_threshold = pcreature->GetDazedHealthThreshold();
            if (dazed_threshold > pcreature->GetHealth() && pcreature->HasTC(TCI_DECAPITATION) && pcreature->IsKillable()) {
                CThingCreatureBase* powner = PCreature.Get();
                powner->SetCurrentAction(CCreatureAction_BeDazed(*PCreature));
            }
        }
    }
}
