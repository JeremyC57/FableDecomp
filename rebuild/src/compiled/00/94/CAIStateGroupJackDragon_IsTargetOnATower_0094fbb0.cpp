// Leaf: ?IsTargetOnATower@CAIStateGroup_JackDragon@@AAE_NXZ
// CAIStateGroup_JackDragon::IsTargetOnATower() -- is the enemy standing higher than the dragon's landing marker (by more than 1.5)?
// Option S unit (owner ruling, advisor decision 2026-11-24; NF376 pilot; unit NF379): this file is ONE retail translation unit's slice,
//   ai_state_group_dragon.obj in Ego_r.pdb / FableWin.pdb, and serves three catalog addresses in retail address order, one body each:
//     0x0094FBB0 ?IsTargetOnATower@CAIStateGroup_JackDragon@@AAE_NXZ   65 B  (this row; Ego_r #4, FableWin #92)
//     0x0094FDF0 ?IsStillRunnable@CAIStateGroup_JackDragon@@AAE_NXZ    55 B  (row file includes this TU; Ego_r #6)
//     0x00951FB0 ?UpdateHover@CAIStateGroup_JackDragon@@AAEXXZ       2272 B  (row file includes this TU; Ego_r #22, FableWin #61)
//   Each address is graded on its own leaf and keeps its own behaviour test. The unit's flag set is UpdateHover's (/d2inlT59, its
//   inline budget); the two small bodies grade the same under it (both were EQ strong copies in the HEAD 0x00951FB0 object, pk_all).
//   Before the unit, 0x00951FB0 defined IsTargetOnATower (AAE) and IsStillRunnable strongly beside the 0x0094FBB0 (QAE stand-in) and
//   0x0094FDF0 rows: NF309 / NF370 held 0x0094FBB0's self fix QAE -> AAE on that duplicate (T3 nf309). Here the self is the PDB AAE
//   (private in both PDBs and engine/CAIStateGroup_JackDragon.h). Remaining debt (not this unit): 0x00951070, 0x00951430 and 0x00952890,
//   same retail TU, still define IsStillRunnable strongly (pre-existing LNK2005s; a later S extension of this unit).
// 0x0094FBB0 (was its own row, NF309 notes): retail 65 B; the enemy's position is evaluated first (left operand), the marker's Z is used
//   straight from the returned pointer; 1.5f is the pool literal 0x0125AADC; `mov al,1` / `xor al,al`. Callees
//   CBaseIntelligentPointer::GetPItem 0x00A01B50, CThing::GetPosition 0x004C73D0.
// 0x0094FDF0 (was its own row, agentNF31): retail 55 B; Ego_r.pdb 0x009747B0 / FableWin.pdb 0x02BD9900 ?IsStillRunnable@..@@AAE_NXZ;
//   two early `return false` guards on PEnemy then the IsStopping test (`neg al ; sbb al,al ; inc al`); callees GetPItem 0x00A01B50,
//   CAIStateGroupBase::IsStopping 0x008FD010.
// 0x00951FB0 CAIStateGroup_JackDragon::UpdateHover (the HEAD 0x00951FB0 text, unchanged below; the two small bodies were moved into
//   retail address order, IsTargetOnATower first).
// Flags: /d2inlT59
// Inline-budget: reproduce the observed vector append and action constructor sites.
#define FABLE_RETAIL_JACKDRAGON_NATIVE_ACTION_SLOTS_PASS27
#define FABLE_RETAIL_CCOMBATACTIONBASE_FRAMEUPDATE
#define FABLE_RETAIL_CCOMBATACTIONBASE_OWN_VIRTUALS
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY
#define FABLE_RETAIL_JACKDRAGON_HOT_CTORS_PASS27
#define FABLE_RETAIL_CTHING_HASTC_FORCEINLINE
#define FABLE_RETAIL_JACKDRAGON_HOVER_PASS27
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CTHING_GETTCDRIVER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_DUMMYOBJECT
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER_ADDMODEPOINTER
#define FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
#define FABLE_RETAIL_CTCDPHYSICALOBSTRUCTION_CREATE
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_GFFASTONEOVERSQRT_ORIGINAL_UNION
#define FABLE_RETAIL_UPDATE_NEAREST_INTERESTING
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CJACKDRAGONDEF
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CCREATUREACTION_DRAGONFIREBLASTBASE
#define FABLE_RETAIL_CAISTATEGROUP_JACKDRAGON
#define FABLE_RETAIL_CAISTATEGROUP_JACKDRAGON_UPDATEGROUND
#define FABLE_RETAIL_CAISTATEGROUP_JACKDRAGON_UPDATESTRAFE_PASS26
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CMODESTANDBASE
#define FABLE_RETAIL_STLP_UNINIT_COPY_FALSE_FASTCALL_PASS26
#define FABLE_RETAIL_STLP_UNINIT_FILL_N_FORCEINLINE
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#include "fable_assert.h"
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
#include "engine/CTCBase.h"
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CRightHandedSet.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CTCDPhysicalObstruction.h"
#include "engine/CThingBaseDef.h"
#include "engine/CSubDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"

#include "engine/CTCPhysicsBase.h"
#include "engine/CSoundPair.h"
#include "engine/CDefinitionManager.h"
#include "engine/CJackDragonDef.h"
#include "engine/CCreatureAction_DragonFireBlastBase.h"
#include "engine/CCreatureAction_DragonFireBlastForward.h"
#include "engine/CCreatureAction_DragonFireBlastLeft.h"
#include "engine/CCreatureAction_DragonFireBeamInto.h"
#include "engine/CCreatureAction_DragonHoverOutOf.h"
#include "engine/NCreatureMode_CModeDragonHover.h"
#include "engine/CAIStateGroup_JackDragon.h"
#include "engine/ThingAlive.h"

// Fidelity: the size-optimized owner at 007890EA expands HasTC and calls bitset::test.
// FableWin 026701E5 names the checked pointer cast. The size scope ends before the script methods.
#ifdef _MSC_VER
#pragma optimize("s",on)
#endif
template<> inline bool CThing::GetTCDriver<CTCDPhysicalObstruction>(CTCDPhysicalObstruction** result) {
    if (HasTC(CTCDPhysicalObstruction::StatGetInterfaceType())) {
        *result=GFDebugPDynamicCast(PTCDriver,(CTCDPhysicalObstruction*)0);
        return true;
    }
    return false;
}
#ifdef _MSC_VER
#pragma optimize("",on)
#endif

// 0x0094FBB0
bool CAIStateGroup_JackDragon::IsTargetOnATower() {
    FABLE_RETAIL_ASSERT(IS_THING_ALIVE(PEnemy));
    if (PEnemy->GetPos().Z > PLandedMarker->GetPos().Z + 1.5f) return true;
    return false;
}

// 0x0094FDF0
bool CAIStateGroup_JackDragon::IsStillRunnable()
{
    if (!IS_THING_ALIVE(PEnemy)) return false;
    if (IsStopping()) return false;
    return true;
}

// 0x00951FB0
void CAIStateGroup_JackDragon::UpdateHover()
{
    CTCCreatureModeManager* ptc_cmm;
    CDefPointer<const CJackDragonDef> p_jack_dragon_def;
    GetCreature().PeekBaseDef().GetSubDef(&p_jack_dragon_def);
    GetCreature().GetTC(&ptc_cmm);
    ptc_cmm->AddMode(CCountedPointer<CTCCreatureModeManager::CModeBase>(new NCreatureMode::CModeDragonHover(GetCreature(), *PEnemy)));
    CThing* pmarker;
    if (GoLeft)
        pmarker = PHoverMarker1.Get();
    else
        pmarker = PHoverMarker2.Get();
    StayIdleAndTurnToFace(1.5f, pmarker->GetPhysicsTC()->GetFacingVector());
    if (!IsStillRunnable()) {
        Terminate();
        return;
    }
    C3DVector mouth_pos;
    CRightHandedSet mouth_rhs;
    {
        GetCreature().GetTCGraphicAppearance()->GetDummyObject("DRAGON_MOUTH_DUMMY", 0, 0, mouth_pos, mouth_rhs, 1.0f);
    }
    mouth_pos.Z = GetCreature().GetPos().Z;
    long num_loops = p_jack_dragon_def->MaxHoverLoops;
    long loop;
    for (loop = 0; loop < num_loops; ++loop) {
        SetHeadTurning(true);
        C3DVector vector_to_target = PEnemy->GetPos() - GetCreature().GetPos();
        float dis_to_target = vector_to_target.GetMagnitude2D();
        if (IsTargetOnATower())
            break;
        if (dis_to_target < p_jack_dragon_def->MaxFirebeamDistance) {
            SetHeadTurning(false);
            WaitForAnimLoop(0.0f);
            if (!IsStillRunnable()) {
                Terminate();
                return;
            }
            CThing* p_obstruction_thing = CTCDPhysicalObstruction::Create(GetCreature().GetPos(), mouth_pos, p_jack_dragon_def->HoverObstructionRadius);
            if (p_obstruction_thing) {
                CTCDPhysicalObstruction* ptcd_obstruction = 0;
                p_obstruction_thing->GetTCDriver(&ptcd_obstruction);
                ptcd_obstruction->SetLifeTimeSeconds(60.0f);
                ptcd_obstruction->SetThingToIgnoreObstruction(&GetCreature());
            }
            AIAction_PerformActionAndWait(CCreatureAction_DragonFireBeamInto(GetCreature(), *PEnemy, 5.0f), false);
            // Inline-budget: the native test of this action result calls IsStillRunnable.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
            if (!IsStillRunnable())
#ifdef _MSC_VER
#pragma inline_depth()
#endif
            {
                Terminate();
                return;
            }
            if (p_obstruction_thing)
                p_obstruction_thing->Kill(true);
            SetHeadTurning(true);
            StayIdle(2.0f);
        } else {
            C3DVector dir_to_target = vector_to_target;
            dir_to_target.Z = 0.0f;
            dir_to_target = dir_to_target.Normalise();
            C3DVector dir_left = dir_to_target;
            dir_left.RotateXY(-0.25f);
            if (GoLeft || dir_to_target.Dot(GetCreature().GetPhysicsTC()->GetFacingVector()) > 0.866f)
                AIAction_PerformActionAndWait(CCreatureAction_DragonFireBlastForward(GetCreature(), *PEnemy), false);
            else if (dir_to_target.Dot(dir_left) > 0.0f)
                AIAction_PerformActionAndWait(CCreatureAction_DragonFireBlastLeft(GetCreature(), *PEnemy), false);
            else
                AIAction_PerformActionAndWait(CCreatureAction_DragonFireBlastForward(GetCreature(), *PEnemy), false);
            if (!IsStillRunnable()) {
                Terminate();
                return;
            }
            StayIdle(2.0f);
        }
        // Inline-budget: the native test of this action result calls IsStillRunnable.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
        if (!IsStillRunnable())
#ifdef _MSC_VER
#pragma inline_depth()
#endif
        {
            Terminate();
            return;
        }
    }
    SetHeadTurning(false);
    WaitForAnimLoop(0.0f);
    if (!IsStillRunnable()) {
        Terminate();
        return;
    }
    ptc_cmm->RemoveMode(71);
    AIAction_PerformActionAndWait(CCreatureAction_DragonHoverOutOf(GetCreature()), false);
    if (!IsStillRunnable()) {
        Terminate();
        return;
    }
    C3DVector target_pos_offset(0.0f, 100.0f, 50.0f);
    target_pos_offset.RotateXY(GetCreature().GetPhysicsTC()->GetFacingAngleXY());
    C3DVector target_pos = GetCreature().GetPos() + target_pos_offset;
    float fly_speed = 30.0f;
    std::vector<C3DVector> movement_points;
    movement_points.push_back(GetCreature().GetPos());
    C3DVector fly_out_pos = GetCreature().GetPos() + GetCreature().GetPhysicsTC()->GetFacingVector()*30.0f + C3DVector(0.0f,0.0f,15.0f);
    if (GoLeft) {
// Inline-budget: the six original exit-path points call the STLport append; the first point expands it.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
        movement_points.push_back(DragonPosOutOfGround3Right);
#ifdef _MSC_VER
#pragma inline_depth()
#endif
// Inline-budget: the six original exit-path points call the STLport append; the first point expands it.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
        movement_points.push_back(DragonPosOutOfGround3Left);
#ifdef _MSC_VER
#pragma inline_depth()
#endif
// Inline-budget: the six original exit-path points call the STLport append; the first point expands it.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
        movement_points.push_back(DragonPosOutOfGround4Left);
#ifdef _MSC_VER
#pragma inline_depth()
#endif
        PreviousFlightPos = DragonPosOutOfGround3Left;
    } else {
// Inline-budget: the six original exit-path points call the STLport append; the first point expands it.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
        movement_points.push_back(DragonPosOutOfGround3Left);
#ifdef _MSC_VER
#pragma inline_depth()
#endif
// Inline-budget: the six original exit-path points call the STLport append; the first point expands it.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
        movement_points.push_back(DragonPosOutOfGround3Right);
#ifdef _MSC_VER
#pragma inline_depth()
#endif
// Inline-budget: the six original exit-path points call the STLport append; the first point expands it.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
        movement_points.push_back(DragonPosOutOfGround4Right);
#ifdef _MSC_VER
#pragma inline_depth()
#endif
        PreviousFlightPos = DragonPosOutOfGround3Right;
    }
    C3DVector take_off_pos = pmarker->GetPos();
    StartFlying(GetCreature(), movement_points, take_off_pos, CCountedPointer<CCreatureActionBase>(0));
    if (!IsStillRunnable()) {
        Terminate();
        return;
    }
// Inline-budget: the exit at 0095284B calls the original tower predicate.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
    if (IsTargetOnATower())
#ifdef _MSC_VER
#pragma inline_depth()
#endif
        CurrentState = DRAGON_STATE_STRAFE;
    else
        CurrentState = ChooseIntoGroundState();
}