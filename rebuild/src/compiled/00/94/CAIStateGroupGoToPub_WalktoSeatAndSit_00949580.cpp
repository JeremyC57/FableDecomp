// CAIStateGroup_GoToPub::WalktoSeatAndSit(bool during_pub_time) -- walk to the seat and turn along the seat's forward vector while the pub and table live (and, when during_pub_time, it is tavern time), then sit down on a new tavern stool mode and mark the seat as taken; terminate on any failure
// Leaf: ?WalktoSeatAndSit@CAIStateGroup_GoToPub@@IAE_N_N@Z
// Retail: Fable.exe (TLC, VC7.1 RTM) @ 0x00949580, 806 bytes (+ int3 padding), `ret 4`. Ego_r.pdb / FableWin.pdb
//   ?WalktoSeatAndSit@CAIStateGroup_GoToPub@@IAE_N_N@Z; Ego_d.pdb (ai_state_group_go_to_pub.obj, after SetBarmaid 0x0094B400) gives the
//   parameter and every local name / type (required_facing_dir, ptc_village, pos_to_look_at, psit_mode, ret, anim_entry,
//   ptc_village_member, tc_tavern_table; nav_route_calculated in the walk loop).
// Callees: CVectorMap::LowerBound 0x0040F020 (expanded GetTC: 0x23 CTCVillageMember, 0xB8 CTCTavernTable), CTCVillageMember::GetPVillage
//   0x007EE540, CBaseIntelligentPointer::GetPItem 0x00A01B50, CTCVillage::IsTavernTime 0x00658C0A, CAIStateGroupBase::AIAction_MoveToPos
//   0x0097EC00 / AIAction_LookAtPos 0x0097D580 / AIAction_PerformActionAndWait 0x0097DB70 / Terminate 0x008FCF40, CThingPhysical::PeekPos
//   0x006A4D00, CCharString(const char*) 0x0099EBF0 / dtor 0x0099EAE0, CThingCreatureBase::GetAnimFromName 0x00662A00, operator new
//   0x00BFEA1A, NCreatureMode::CModeSitTavernStool ctor 0x0094B530, CCountedPointer<NCreatureMode::CModeSitBase> (T*) 0x0094B3A0 / copy
//   0x008AA6A0 / dtor 0x008AB320 (COMDATs called), CCreatureAction_SitDown ctor 0x008AB9D0 / dtor 0x008ABAA0, CTCTavernTable::SetSeatState
//   0x007B8530. Data: "SIT_AT_TABLE_INTO" 0x01293060.
// Shape: the village-member out pointer is not initialised (the miss path reads the frame word, 0x009495CB); ret starts at
//   AI_ACTION_FAILED (0x00949620) and the walk loop's guard also tests it against AI_ACTION_TERMINATE_PROCESS (0x0094968A); the look
//   loop's TERMINATE_PROCESS test follows the loop (0x009497AB). The by-value CCountedPointer argument's copy constructor is visible
//   (default header text) and VC7.1 still calls its COMDAT; a declaration-only copy constructor (COPY_DECL_ONLY) exposes psit_mode's
//   address and costs 4 frame bytes (psit_mode no longer shares pos_to_look_at.Z's slot) -- measured, 24 offset hunks. The (T*) ctor
//   and the dtor are plain header inlines here: VC7.1 calls their COMDATs (0x0094B3A0 / 0x008AB320) without any inline control.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUP_GOTOPUB
#define FABLE_RETAIL_CAISTATEGROUP_GOTOPUB_WALKTOSEATANDSIT
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMATION
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_SITDOWN
#define FABLE_RETAIL_NCREATUREMODE_CMODESITTAVERNSTOOL
#define FABLE_RETAIL_CTCTAVERNTABLE_SEATS
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_TAVERN
#define FABLE_RETAIL_IS_THING_ALIVE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CCountedPointer.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingObject.h"
#include "engine/CThingBuilding.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/NCreatureMode_CModeSitFloor.h"
#include "engine/CAnimationEntry.h"
#include "engine/CCreatureAction_SitDown.h"
#include "engine/CTCVillage.h"
#include "engine/CTCVillageMember.h"
#include "engine/CTCTavernTable.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIStateGroup_GoToPub.h"
#include "engine/CAIBrain.h"
#include "engine/ThingAlive.h"

bool CAIStateGroup_GoToPub::WalktoSeatAndSit(bool during_pub_time)
{
    CTCVillageMember* ptc_village_member;
    GetCreature().GetTC(&ptc_village_member);
    CTCVillage* ptc_village = ptc_village_member->GetPVillage();
    CTCTavernTable* tc_tavern_table;
    PTable->GetTC(&tc_tavern_table);
    EAIActionReturn ret = AI_ACTION_FAILED;
    do {
        if (!IS_THING_ALIVE(PPub) || (!ptc_village->IsTavernTime() && during_pub_time) || !IS_THING_ALIVE(PTable) ||
            ret == AI_ACTION_TERMINATE_PROCESS) {
            Terminate();
            return false;
        }
        bool nav_route_calculated;
        ret = AIAction_MoveToPos(ACTION_REPEAT_CALLBACK, Pos, nav_route_calculated, AI_MOVEMENT_WALKING, true, true, 0, true, 0.0f);
    } while (ret == AI_ACTION_IN_PROGRESS);
    if (ret != AI_ACTION_FINISHED) {
        Terminate();
        return false;
    }
    C3DVector required_facing_dir = RHSet.GetForward();
    C3DVector pos_to_look_at = required_facing_dir + GetCreature().PeekPos();
    do {
        if (!IS_THING_ALIVE(PPub) || (!ptc_village->IsTavernTime() && during_pub_time) || !IS_THING_ALIVE(PTable)) {
            Terminate();
            return false;
        }
        ret = AIAction_LookAtPos(ACTION_REPEAT_CALLBACK, pos_to_look_at, 0x11, 0.01f);
    } while (ret == AI_ACTION_IN_PROGRESS);
    if (ret == AI_ACTION_TERMINATE_PROCESS) {
        Terminate();
        return false;
    }
    const CAnimationEntry& anim_entry = GetCreature().GetAnimFromName("SIT_AT_TABLE_INTO", 0);
    CCountedPointer<NCreatureMode::CModeSitBase> psit_mode(new NCreatureMode::CModeSitTavernStool(GetCreature()));
    if (AIAction_PerformActionAndWait(CCreatureAction_SitDown(GetCreature(), anim_entry, psit_mode), false) != AI_ACTION_FINISHED) {
        Terminate();
        return false;
    }
    tc_tavern_table->SetSeatState(SeatIndex, true);
    Sitting = true;
    return true;
}