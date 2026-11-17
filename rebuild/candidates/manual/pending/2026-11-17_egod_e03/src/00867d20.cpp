// CCombatAction_GenericResponseToExplosion::FinishAction() -- the explosion response has finished: run the base FinishAction, keep the facing flip only when the animation's stats have no rotation and the creature's current mode is not 29, drop mode 29, hand the creature its next action (die when it has no health left and its def leaves a dead creature, else killed outright; be stunned when the knockdown leads to a stun; else get up), then drop mode 66
// Leaf: ?FinishAction@CCombatAction_GenericResponseToExplosion@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) 0x00867D20, 507 bytes (three `add esp,128h ; ret` exits), thiscall, public virtual override.
// Names: Ego_r.pdb / FableWin.pdb ?FinishAction@CCombatAction_GenericResponseToExplosion@@UAEXXZ (overlay opt-in
//   FABLE_RETAIL_CCOMBATACTIONGENERICRESPONSETOEXPLOSION_FINISHACTION); Ego_d.pdb locals anim_index / flip / tc_modes, block { info,
//   stats }, block { creature_def } (the def reference in the no-health arm). Written from the landed twin
//   CCombatAction_GenericStrikeResponseKnockdown::FinishAction 0x00867B40 (same includes, the same local action classes and
//   IsKnockdownToStun declaration, same callees), with this row's differences: the mode-29 test is
//   tc_modes->GetCurrentMode().GetType() == 29 (0x0073A6E0, CModeBase slot 1) and the trailing RemoveMode(0x42).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_PCREATURE
#define FABLE_RETAIL_CCREATUREACTIONBASE_PEEKWORLD
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCOMBATACTIONBASE_FINISHACTION
#define FABLE_RETAIL_CCOMBATACTIONGENERICSTRIKERESPONSEKNOCKDOWN
#define FABLE_RETAIL_CCOMBATACTIONGENERICRESPONSETOEXPLOSION
#define FABLE_RETAIL_CCOMBATACTIONGENERICRESPONSETOEXPLOSION_FINISHACTION
#define FABLE_RETAIL_CCREATUREACTION_DIE
#define FABLE_RETAIL_CCOMBATACTION_KNOCKDOWNRECOVER
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_MESH_BANK
#define FABLE_RETAIL_CWORLD_PEEKMESHBANK
#define FABLE_RETAIL_CMESHDATABANK_PEEKANIMATION
#define FABLE_RETAIL_CMESHDATABANK
#define FABLE_RETAIL_CMESHDATABANK_BASES
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CRESOURCE_RESOURCELIST
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CMESHDATABANKENTRY
#define FABLE_RETAIL_C3DANIMATIONINFO
#define FABLE_RETAIL_C3DANIMATIONINFO_BUILD
#include "engine/CThingCreatureBase.h"
#include "engine/CThingCreatureDef.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CDefPointer.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CWorld.h"
#include "engine/CMeshDataBank.h"
#include "engine/C3DAnimationStats.h"
#include "engine/C3DAnimationInfo.h"
#include "engine/CCombatAction_GenericStrikeResponseKnockdown.h"
#include "engine/CCombatAction_GenericResponseToExplosion.h"
#include "engine/CCreatureAction_Die.h"
#include "engine/CCombatAction_GenericStrikeResponseKnockdownRecoverBase.h"
extern const float DELTA_NOTIONAL_ZERO;                                  // 0x0129BA3C

class CCreatureAction_BeStunned : public CCreatureActionBase {           // RTTI .?AVCCreatureAction_BeStunned@@ : CCreatureActionBase, 0xAC bytes
public:
    CCreatureAction_BeStunned(CThingCreatureBase& creature, bool flip);  // 0x00865940
    virtual void PerformAction();
protected:
    virtual CCreatureActionBase* Clone() const;
    bool Flip;                                                           // +0xA8
};

class CCombatAction_GenericStrikeResponseKnockdownRecover : public CCombatAction_GenericStrikeResponseKnockdownRecoverBase {   // adds no members (0x118)
public:
    CCombatAction_GenericStrikeResponseKnockdownRecover(CThingCreatureBase& creature, bool flip);   // 0x007F05C0
};

namespace {
bool FABLE_FASTCALL IsKnockdownToStun(const CThingCreatureBase& creature);   // 0x00865AC0
}

void CCombatAction_GenericResponseToExplosion::FinishAction()
{
    CCombatActionBase::FinishAction();
    long anim_index = GetAnimationIndex();
    CTCCreatureModeManager* tc_modes;
    PCreature->GetTC(&tc_modes);
    bool flip = FlipFacing;
    if (flip) {
        if (anim_index) {
            const C3DAnimationInfo* info = PeekWorld().PeekMeshBank()->PeekAnimation(anim_index);
            CCountedPointer<const C3DAnimationStats> stats = info->GetAnimationStats();
            if (stats->HasRotation())
                flip = false;
        }
        if (flip && tc_modes->GetCurrentMode().GetType() == 0x1d)
            flip = false;
    }
    tc_modes->RemoveMode(0x1d);
    if (PCreature->GetHealth() < DELTA_NOTIONAL_ZERO) {
        const CThingCreatureDef& creature_def = (const CThingCreatureDef&)PCreature->PeekBaseDef();
        if (creature_def.LeaveDeadCreature)
            PCreature->SetCurrentAction(CCreatureAction_Die(*PCreature.Get(), flip));
        else
            PCreature->Kill(true);
    } else if (IsKnockdownToStun(*PCreature.Get()))
        PCreature->SetCurrentAction(CCreatureAction_BeStunned(*PCreature.Get(), flip));
    else
        PCreature->SetCurrentAction(CCombatAction_GenericStrikeResponseKnockdownRecover(*PCreature.Get(), flip));
    tc_modes->RemoveMode(0x42);
}
