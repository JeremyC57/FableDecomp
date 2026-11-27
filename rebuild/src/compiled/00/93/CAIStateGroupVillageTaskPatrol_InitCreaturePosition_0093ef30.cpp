// 0093ef30 CAIStateGroup_VillageTaskPatrol::InitCreaturePosition
#define FABLE_RETAIL_CVECTORMAP_COMPONENT_LOWERBOUND_OWNER_PASS26
#define FABLE_RETAIL_STLP_BITSET_TEST_OWNER_PASS27
#define FABLE_RETAIL_CTHINGFILTER_HASTC_OUTOFLINE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHING_SCANINCLUSION_ISALIVE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTTHING_BODY_NULLTEST
// Leaf: ?InitCreaturePosition@CAIStateGroup_VillageTaskPatrol@@UAE_NXZ
// Flags:
#define FABLE_RETAIL_VILLAGE_PATROL_POSITION_PASS29
#define FABLE_RETAIL_CTCVILLAGE_FINEAMOUNT
#define FABLE_RETAIL_CVILLAGETASKPATROL_GET_START
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_SPEECH
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK_PROTECTED
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTTHING_BODY
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_ITERATOR_TAGS
#define FABLE_RETAIL_CTHINGCREATUREBASE_AUTOPLACENEAR
#define FABLE_RETAIL_CRIGHTHANDEDSET_ROTATE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_CTCCREATURENAVIGATION_NAVIGABLE
#define FABLE_RETAIL_MINION_CRIMINAL_UPDATE_PASS24
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
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCCreatureNavigation.h"
#include "engine/CThingFilter_HasTC.h"
#include "engine/CThingFilter_In2DRange.h"
#include "engine/CThingCompare_Nearest.h"
#include "engine/CFilter_Bind2.h"
#include "engine/CThingSearchTools.h"
#include <math.h>

// Fidelity: the patrol TU owns the 32-byte converting constructor at 0093ED30.
// Its 28-byte deleting destructor is owned by the size-built pointer TU.
template<> inline CIntelligentPointer<CVillageTaskPatrol>::CIntelligentPointer(CVillageTaskPatrol* pointer)
{
    SetPointer(pointer);
}

// Same speed-compiled native comparison at 004A8D80.
template<> inline CThing* CThingCompare_Nearest::operator()<CThing>(CThing* first, CThing* second) const
{
    if (!first) return second;
    if (!second) return first;
    return (first->GetPos() - Pos).GetSquaredMagnitude() < (second->GetPos() - Pos).GetSquaredMagnitude() ? first : second;
}
// Inline-budget: this search is a distinct retail call and its body matches the native template instance.
template<> inline
#ifdef _MSC_VER
__declspec(noinline)
#endif
bool CThingSearchTools::GetPBestThing(const CThingFilter_In2DRange& filter_func, const CThingCompare_Nearest& compare_func, CThing** out) const
{
    CThing* pbest_thing = 0;
    unsigned long flags = CThing::StatGetTypeFlags();
    const std::list<CClass*>& class_list = PeekClassList();
    for (std::list<CClass*>::const_iterator it = class_list.begin(); it != class_list.end(); ++it) {
        CThing* temp_thing = static_cast<CThing*>(*it);
        if (temp_thing->IsIncludedInThingScans() && (temp_thing->GetTypeFlags() & flags) == flags) {
            CThing* tthing = static_cast<CThing*>(*it);
            if (tthing && filter_func(*tthing))
                pbest_thing = compare_func(pbest_thing, tthing);
        }
    }
    *out = pbest_thing;
    return pbest_thing != 0;
}
// Inline-budget: this search is a distinct retail call and its body matches the native template instance.
template<> inline
#ifdef _MSC_VER
__declspec(noinline)
#endif
bool CThingSearchTools::GetPBestThing(const CFilter_Bind2<CThingFilter_HasTC, CThingFilter_In2DRange>& filter_func, const CThingCompare_Nearest& compare_func, CThing** out) const
{
    CThing* pbest_thing = 0;
    unsigned long flags = CThing::StatGetTypeFlags();
    const std::list<CClass*>& class_list = PeekClassList();
    for (std::list<CClass*>::const_iterator it = class_list.begin(); it != class_list.end(); ++it) {
        CThing* temp_thing = static_cast<CThing*>(*it);
        if (temp_thing->IsIncludedInThingScans() && (temp_thing->GetTypeFlags() & flags) == flags) {
            CThing* tthing = static_cast<CThing*>(*it);
            if (tthing && filter_func(*tthing))
                pbest_thing = compare_func(pbest_thing, tthing);
        }
    }
    *out = pbest_thing;
    return pbest_thing != 0;
}


#ifdef _MSC_VER
#pragma optimize("s",on)
#endif
// Inline-budget: the native size-built default constructor at 008FE9A7 selects the 28-byte virtual destructor at 008FEA79.
template CIntelligentPointer<CVillageTaskPatrol>::CIntelligentPointer();

#ifdef _MSC_VER
#pragma optimize("t",on)
#endif
bool CAIStateGroup_VillageTaskPatrol::InitCreaturePosition()
{
    PTask = 0;
    CTCVillage* pvillage = PTCVillageMember->GetPVillage();
    if (pvillage) {
        CThingPlayerCreature* player = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
        if (pvillage->GetFineAmountForCurrentCrimes() > 0 && IS_THING_ALIVE(player)) {
            CThing* thing;
            bool entered = false;
            if (PeekThingSearchTools().GetPBestThing(GFFilter_Bind2(CThingFilter_HasTC(TCI_REGION_ENTRANCE), CThingFilter_In2DRange(player->GetPos(), 0.4f)), CThingCompare_Nearest(player->GetPos()), &thing))
                entered = true;
            else if (PeekThingSearchTools().GetPBestThing(GFFilter_Bind2(CThingFilter_HasTC(TCI_TELEPORTER), CThingFilter_In2DRange(player->GetPos(), 0.4f)), CThingCompare_Nearest(player->GetPos()), &thing))
                entered = true;
            if (entered) {
                CTCPhysicsBase* tc_physics = player->GetPhysicsTC();
                if (false) {} else for (float angle = 0.0f; (float)fabs(angle) < 0.25; angle = angle > 0.0f ? -angle : -angle + 0.02f) {
                    C3DVector offset = tc_physics->GetRHSet().GetForward();
                    offset.RotateXY(angle);
                    C3DVector guard_pos = tc_physics->GetPosition() + offset * 8.0f;
                    guard_pos.Z += 1.5;
                    CRightHandedSet guard_rhset = tc_physics->GetRHSet();
                    guard_rhset.Rotate(guard_rhset.GetUp(), 0.5f);
                    CTCCreatureNavigation* tc_creature_navigation;
                    if (GetCreature().GetTC(&tc_creature_navigation) && tc_creature_navigation->IsPosNavigable(guard_pos)) {
                        if (!PeekThingSearchTools().GetPBestThing(CThingFilter_In2DRange(guard_pos, 0.4f), CThingCompare_Nearest(guard_pos), &thing)) {
                            CTCCreatureNavigation* nav;
                            GetCreature().GetTC(&nav);
                            nav->InterruptNavigation();
                            guard_pos.Z = tc_creature_navigation->FindPosZAt(guard_pos);
                            GetCreature().GetPhysicsTC()->ResetPosition(guard_pos);
                            GetCreature().GetPhysicsTC()->ResetRHSet(guard_rhset);
                            nav->InterruptNavigation();
                            return true;
                        }
                    }
                }
            }
        }
        PTask = pvillage->GetFirstUnAssignedPatrolTask();
        if (PTask != 0 && PTask->GetPatrolStart()) {
            pvillage->SetPatrolTaskAsAcceptedByThing(*PTask, GetCreature());
            GetCreature().AutoPlaceNear(PTask->GetPatrolStart()->GetPos(), 3.0f);
            return true;
        }
    }
    return false;
}

// Fidelity: scaffolding for the native 008FEA79 size-compiled implicit deleting destructor.
#ifdef _MSC_VER
#pragma optimize("s",on)
#endif