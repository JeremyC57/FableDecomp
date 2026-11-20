// CAIStateGroup_VillageTaskPatrol::Update() -- take the village's best patrol task for the creature, draw a weapon if it carries none, then walk the patrol track node by node (from the node nearest the creature; the track start after an end node), locking the task at each arrival, glancing at the next node when it lies ahead and idling 0.5..1.5 seconds there, for as long as the task stays assigned to the creature
// Inline-budget: no flag, no pragma, no device. Ablated header contracts that decide expanded vs called (README): FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN's inline operator= (GetPItem + SetPItem in place; without: the implicit copy, DIFFER(1150v1185)); the new FABLE_RETAIL_CVILLAGETASKPATROL_SETLOCKED_INLINE SetTaskAsLocked (the 0x0093F5D7 store; declaration only: DIFFER(1188v1185)); CAIStateGroupBase::GetCreature's body via FABLE_RETAIL_CAIBRAIN_GETCREATURE (without: DIFFER(1213v1185)); IS_THING_ALIVE (the explicit `== 0 || IsDead()` form: DIFFER(1176v1185)). Default plain-inline header members, not ablated: the CIntelligentPointer (T*) constructor, C3DVector operator- / Dot / Normalise with GFFastOneOverSqrt (0x0093F6B7..0x0093F71A), CThingTrackNode IsTrackEnd / PeekNextNode, CThing::GetPhysicsTC, CRightHandedSet::GetForward, CMainGameComponent::GetConstantFPS. Out of line as in retail: the calls listed under Callees.
// Leaf: ?Update@CAIStateGroup_VillageTaskPatrol@@UAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0093F3C0, 1185 bytes, thiscall, ebp frame aligned to 8 (`and esp,0FFFFFFF8h ; sub esp,150h`: the 0x118
//   CCreatureAction_UnsheatheWeapons temporary at body esp+0x48 and the double of the wait expression at esp+0x40), no EH frame. Vtable
//   0x0128BA64 (RTTI .?AVCAIStateGroup_VillageTaskPatrol@@) slot 12.
// Identity: Ego_r.pdb / FableWin.pdb ?Update@CAIStateGroup_VillageTaskPatrol@@UAEXXZ (FableWin 0x02B94790, Ego_r 0x00963250). Members
//   (Ego_r.pdb layout, new engine/CAIStateGroup_VillageTaskPatrol.h): PTCVillageMember +0x14, CIntelligentPointer<CVillageTaskPatrol> PTask +0x18.
// Ego_d.pdb (debug length 2114, lines 68..161, egod_lines/): locals pvillage; block { track_node_target (CIntelligentPointer<const
//   CThingTrackNode>, vtable 0x0125A894); block { ret, current_dir, wait_till (long), desired_dir; block { nav_route_calculated } } }.
//   Shapes read from the /Od sizes: `Terminate();` 11 B / `return;` 5 B at lines 75 / 76 (outside the block), 11 / 20 B (with the
//   track_node_target destructor) at 102/103, 117/118, 143/144, 154/155; 97 (7 B) the `ret` initialiser; 106 (4 B) nav_route_calculated = false; 109 (10 B) the do-while
//   test; 120 (21 B) PTask->SetTaskAsLocked(); 133 / 134 / 135 (45 / 5 / 8 B) desired_dir, its Z = 0, Normalise; 149 (14 B) the idle loop
//   test; 159 / 160 the block's destructor then Terminate().
// Callees: CTCVillageMember::GetPVillage 0x007EE540, CTCVillage::GetBestPatrolTaskForThing 0x00658E90, CTCVillage::SetPatrolTaskAsAcceptedByThing
//   0x0065852D, CTCCarrying::IsThingCarryingWeapon 0x006A1540, CCreatureAction_UnsheatheWeapons(CThingCreatureBase&) 0x008B8590 (`ret 4`; the
//   `push 0` at 0x0093F466 is AIAction_PerformActionAndWait's `false`), ~CCombatActionBase 0x0062E900, AIAction_PerformActionAndWait 0x0097DB70,
//   CVillageTaskPatrol::GetNearestNode 0x00860A50, CThing::GetPos 0x004C73D0, CVillageTaskPatrol::IsCreatureAssignedToTask (vtable 0x0125A900
//   slot 7 = 0x0065EEDD), AIAction_MoveToPosProximity 0x0097E890, CThingTrackNode::PeekTrackStartNode 0x00836DA0, CTCPhysicsBase::GetRHSet
//   (slot 72, +0x120), AIAction_LookAtPos 0x0097D580, CAIStateGroupBase::GetPWorldSeed 0x008FCFD0, GFFloatRandom 0x004978A0, CWorld::GetFrame
//   0x0049D870, AIAction_MinionIdle 0x0097FA00, CAIStateGroupBase::Terminate 0x008FCF40; CBaseIntelligentPointer 0x00A01B10 / 0x00A01B50 /
//   0x00A01B90 / 0x00A01C10.
// Bindings (two sources each): 0x00658E90 = GetBestPatrolTaskForThing: the PDB name and one-reference signature (`ret 4`), its body runs
//   GetBestTaskForThing<CVillageTaskPatrol> 0x0065F856 (manifest) over CTCVillage::PatrolTasks +0x54 (shared retail layout), and
//   GetBestCrateTaskForThing 0x00658EA0 follows it as in Ego_r (0x0069AFB9 / 0x0069AFF2). 0x00836DA0 = PeekTrackStartNode: PDB QBEPBV1@XZ
//   and its body (walks PreviousNode +0xD4 until IsStart +0xEC); landed 0x0093CB20 / 0x0093CAB0 bind the same spelling.
// Globals: CMainGameComponent::ConstantFPS 0x01375550, GFastOneOverSqrtTable 0x013CE558; 0.5f 0x0122F59C (proximity / half), 3.0f
//   0x0122DED4, 0.0f 0x0122DEDC, 0.5 (double) 0x012316F0, 0.0027777f.
// Overlay (add-only opt-ins): engine/CTCVillage.h FABLE_RETAIL_CTCVILLAGE_PATROLTASK; engine/CThingTrackNode.h
//   FABLE_RETAIL_CTHINGTRACKNODE_PEEK_START; engine/CVillageTaskPatrol.h FABLE_RETAIL_CVILLAGETASKPATROL_SETLOCKED_INLINE; NEW
//   engine/CAIStateGroup_VillageTaskPatrol.h (PDB layout).
// Inferred: the statement text (Ego_d gives locals, scopes and per-line sizes only); `ret`'s initial AI_ACTION_FAILED (Torch 0x0093EA90's text;
//   Ego_d line 97 is a 7-byte store; without it the bytes are the same); `desired_dir.Dot(current_dir)` (the swapped receiver
//   and the FPS-first wait expression grade the same).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCREATUREACTION_UNSHEATHEWEAPONS
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MOVETOPOSPROXIMITY
#define FABLE_RETAIL_CAISTATEGROUPBASE_MINIONIDLE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_PATROLTASK
#define FABLE_RETAIL_CVILLAGETASKPATROL
#define FABLE_RETAIL_CVILLAGETASKPATROL_SETLOCKED_INLINE
#define FABLE_RETAIL_CTHINGTRACKNODE
#define FABLE_RETAIL_CTHINGTRACKNODE_PEEK_START
#define FABLE_RETAIL_CTHINGTRACKNODE_PEEK_NEXT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CTCCARRYING
#define FABLE_RETAIL_C3DVECTOR
#include "fable_string.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingTrackNode.h"
#include "engine/CRightHandedSet.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/EUnsheatheWeaponType.h"
#include "engine/CCreatureAction_UnsheatheWeapons.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CVillageTaskPatrol.h"
#include "engine/CTCVillage.h"
#include "engine/CTCVillageMember.h"
#include "engine/CAIStateGroup_VillageTaskPatrol.h"
#include "engine/CTCCarrying.h"
#include "engine/ThingAlive.h"
#include "engine/GFFloatRandom.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
void CAIStateGroup_VillageTaskPatrol::Update()
{
    CTCVillage* pvillage = PTCVillageMember->GetPVillage();
    PTask = pvillage->GetBestPatrolTaskForThing(GetCreature());
    if (PTask == 0 || !pvillage->SetPatrolTaskAsAcceptedByThing(*PTask, GetCreature()))
    {
        Terminate();
        return;
    }
    if (!CTCCarrying::IsThingCarryingWeapon(GetCreature()))
        AIAction_PerformActionAndWait(CCreatureAction_UnsheatheWeapons(GetCreature()), false);
    {
        CIntelligentPointer<const CThingTrackNode> track_node_target(PTask->GetNearestNode(GetCreature().GetPos()));
        while (PTask != 0 && PTask->IsCreatureAssignedToTask(GetCreature()))
        {
            EAIActionReturn ret = AI_ACTION_FAILED;
            do
            {
                if (PTask == 0 || !PTask->IsCreatureAssignedToTask(GetCreature()) || !IS_THING_ALIVE(track_node_target))
                {
                    Terminate();
                    return;
                }
                bool nav_route_calculated = false;
                ret = AIAction_MoveToPosProximity(ACTION_REPEAT_CALLBACK, track_node_target->GetPos(), 0.5f, nav_route_calculated,
                    AI_MOVEMENT_WALKING, true, false, true, 0, true, 0.0f);
            } while (ret == AI_ACTION_IN_PROGRESS);
            if (ret != AI_ACTION_FINISHED || PTask == 0 || !PTask->IsCreatureAssignedToTask(GetCreature()))
            {
                Terminate();
                return;
            }
            PTask->SetTaskAsLocked();
            if (track_node_target->IsTrackEnd())
                track_node_target = track_node_target->PeekTrackStartNode();
            else
                track_node_target = track_node_target->PeekNextNode();
            C3DVector current_dir = GetCreature().GetPhysicsTC()->GetRHSet().GetForward();
            C3DVector desired_dir = track_node_target->GetPos() - GetCreature().GetPos();
            desired_dir.Z = 0.0f;
            desired_dir.Normalise();
            if (desired_dir.Dot(current_dir) > 0.0f)
            {
                if (AIAction_LookAtPos(ACTION_REPEAT_UNTIL_FINISHED, track_node_target->GetPos(), 9, 0.0027777f) == AI_ACTION_TERMINATE_PROCESS)
                {
                    Terminate();
                    return;
                }
            }
            long wait_till = CWorld::GetFrame() + (GFFloatRandom(1.0f, GetPWorldSeed()) + 0.5) * CMainGameComponent::GetConstantFPS();
            while (CWorld::GetFrame() < wait_till)
            {
                if (AIAction_MinionIdle() != AI_ACTION_FINISHED || PTask == 0 || !PTask->IsCreatureAssignedToTask(GetCreature()))
                {
                    Terminate();
                    return;
                }
            }
        }
    }
    Terminate();
}
