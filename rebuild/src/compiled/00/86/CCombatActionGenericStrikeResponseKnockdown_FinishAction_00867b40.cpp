// CCombatAction_GenericStrikeResponseKnockdown::FinishAction() -- the knockdown response has finished: run the base FinishAction, decide whether
// the follow-up action keeps the facing flip, drop creature mode 29 and hand the creature its next action (die / be stunned / get up).
// Leaf: ?FinishAction@CCombatAction_GenericStrikeResponseKnockdown@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00867B40, 468 bytes (0x1D4: the last epilogue's `add esp,128h ; ret` ends at 0x00867D13, int3
// padding to 0x00867D20; the manifest / batch oracle length 465 cuts the final `00 00 C3`). thiscall, no arguments, no frame pointer,
// no EH frame. Public virtual override: vtable 0x01277344 slot 4 (Ego_r.pdb ?FinishAction@CCombatAction_GenericStrikeResponseKnockdown@@UAEXXZ).
// Callees: CCombatActionBase::FinishAction 0x00859A20; GetAnimationIndex (virtual slot 7, +0x1C); CBaseIntelligentPointer::GetPItem
//   0x00A01B50 (PCreature +0x08, eight times); CVectorMap::LowerBound 0x0040F020 (the expanded CThing::GetTC for
//   TCI_ENTITY_MODE_MANAGER 0x31); CCreatureActionBase::PeekWorld 0x00692950; CMeshDataBank::PeekAnimation 0x00A242C0;
//   C3DAnimationInfo::GetAnimationStats 0x00A269C0; C3DAnimationStats::HasRotation (virtual slot 4, +0x10); operator delete
//   0x00BFE9BC (the inline ~CCountedPointer); CTCCreatureModeManager::IsModeActive 0x0073A8A0 / RemoveMode 0x0073ABA0;
//   CCreatureAction_Die ctor 0x008ED650; CCreatureAction_BeStunned ctor 0x00865940; CCombatAction_GenericStrikeResponseKnockdownRecover
//   ctor 0x007F05C0; CThingCreatureBase::SetCurrentAction 0x006644F0; ~CCreatureActionBase 0x00693EF0; ~CCombatActionBase 0x0062E900;
//   CThing::Kill 0x004C9B80; the file-static IsKnockdownToStun 0x00865AC0 (__fastcall, anonymous namespace of this TU).
// Data: DELTA_NOTIONAL_ZERO 0x0129BA3C.
// Shape: the mode manager pointer is the uninitialised GetTC out-parameter (its frame word doubles as the lookup key); the animation
// index lives in edi across the lookup; the animation info is a NAMED local (PeekAnimation is called before the stats' hidden result
// slot is pushed); FlipFacing (+0x13A) is copied to a local that the stats' HasRotation and an active mode 29 clear; the creature's
// LeaveDeadCreature flag is read through the inline CThing::PeekBaseDef (PBaseDef +0x70, def +0x148 bit 0). The three temporaries
// share one 0x118-byte frame slot; Die / BeStunned are destroyed by ~CCreatureActionBase (tail merged), Recover by ~CCombatActionBase.
// Headers: overlay opt-ins FABLE_RETAIL_CCREATUREACTIONBASE_PEEKWORLD, FABLE_RETAIL_CCOMBATACTIONGENERICSTRIKERESPONSEKNOCKDOWN_FINISHACTION,
// FABLE_RETAIL_CCREATUREACTION_DIE (hdr/README.md). CCreatureAction_BeStunned / ...KnockdownRecover have no engine header (local views).
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
#define FABLE_RETAIL_CCOMBATACTIONGENERICSTRIKERESPONSEKNOCKDOWN_FINISHACTION
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
bool __fastcall IsKnockdownToStun(const CThingCreatureBase& creature);   // 0x00865AC0
}

void CCombatAction_GenericStrikeResponseKnockdown::FinishAction()
{
    CCombatActionBase::FinishAction();
    long animation_index = GetAnimationIndex();
    CTCCreatureModeManager* mode_manager;
    PCreature->GetTC(&mode_manager);
    bool flip_facing = FlipFacing;
    if (flip_facing) {
        if (animation_index) {
            const C3DAnimationInfo* animation_info = PeekWorld().PeekMeshBank()->PeekAnimation(animation_index);
            CCountedPointer<const C3DAnimationStats> stats = animation_info->GetAnimationStats();
            if (stats->HasRotation())
                flip_facing = false;
        }
        if (flip_facing && mode_manager->IsModeActive(0x1d))
            flip_facing = false;
    }
    mode_manager->RemoveMode(0x1d);
    if (PCreature->GetHealth() < DELTA_NOTIONAL_ZERO) {
        if (((const CThingCreatureDef&)PCreature->PeekBaseDef()).LeaveDeadCreature)
            PCreature->SetCurrentAction(CCreatureAction_Die(*PCreature.Get(), flip_facing));
        else
            PCreature->Kill(true);
    } else if (IsKnockdownToStun(*PCreature.Get()))
        PCreature->SetCurrentAction(CCreatureAction_BeStunned(*PCreature.Get(), flip_facing));
    else
        PCreature->SetCurrentAction(CCombatAction_GenericStrikeResponseKnockdownRecover(*PCreature.Get(), flip_facing));
}