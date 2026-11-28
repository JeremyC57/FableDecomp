// 0092d1f0 CAIStateGroup_GoToBed::FindFreeBed
// Flags:
// Leaf: ?FindFreeBed@CAIStateGroup_GoToBed@@IAE_N_N@Z
#define FABLE_RETAIL_STLP_REMOVE_COPY_IF
#define FABLE_RETAIL_CAISTATEGROUP_GOTOBED
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CTCSLEEP
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTHINGAICREATURE_HOMEBUILDING
#define FABLE_RETAIL_CTHINGAICREATURE_SAVE
#define FABLE_RETAIL_CTHINGAICREATURE_WORKBUILDING
#define FABLE_RETAIL_CTHINGAICREATURE_BUILDING_GETTERS_PASS26
#define FABLE_RETAIL_CTHINGCREATUREBASE_CREATURETYPE
#define FABLE_RETAIL_CTHINGBUILDING
#define FABLE_RETAIL_MINION_CRIMINAL_UPDATE_PASS24
#define FABLE_RETAIL_CTCWIFE
#define FABLE_RETAIL_CTCTHINGOWNER
#define FABLE_RETAIL_CTCUSERACTIONPOINTBASE
#define FABLE_RETAIL_CTCSINGLEPERSONOCCUPIABLE_ISOCCUPIED
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_GETRIGHT_OUTOFLINE
#define FABLE_RETAIL_C3DVECTOR_ROTATE_ANGLES
#define FABLE_RETAIL_STLP_FIND_IF_INPUT
#define FABLE_RETAIL_STLP_FIND_IF_CALLED_PASS23
#define FABLE_RETAIL_FREE_BED_PREDICATE_PASS30
#define FABLE_RETAIL_C3DVECTOR_SQUARED_MAGNITUDE_ORIGINAL_EXPR_PASS23
#define FABLE_RETAIL_C3DVECTOR_INITIALISE_SUB
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CWorld.h"
#include "engine/ThingAlive.h"
#include "engine/CAIStateGroup_GoToBed.h"
#include "engine/CThingBuilding.h"
#include "engine/CTCWife.h"
#include "engine/CTCThingOwner.h"
#include "engine/CTCUserActionPointBase.h"
#include "engine/CTCSinglePersonOccupiable.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CIsFreeBed.h"
#include "stlport/fable_algorithm.h"
#include "fable_assert.h"
#include "engine/GFIsNonZero.h"
bool CAIStateGroup_GoToBed::FindFreeBed(bool during_auto_placement)
{
    IsBedSleepPointMarker = false;
    CThingBuilding* pbuilding = GetCreature().GetHomeBuilding();
    C3DVector near_to = GetCreature().GetPos();
    CTCWife* tc_wife;
    if (IS_THING_ALIVE(pbuilding) && (GetCreature().GetCreatureType() == HUMAN_CHILD
        || (GetCreature().GetTC(&tc_wife) && tc_wife->IsMarriedToThePlayer())
        || !pbuilding->IsLockable() || pbuilding->IsLocked()
        || during_auto_placement || pbuilding->HasTC(TCI_TAVERN))) {
        PBed = CIntelligentPointer<CThing>(0);
        FABLE_RETAIL_ASSERT(IS_THING_ALIVE(pbuilding));
        CTCThingOwner* ptc_thing_owner;
        if (pbuilding->GetTC(&ptc_thing_owner)) {
            float best = 0.0f;
            const std::list<CIntelligentPointer<CThing> >& house_objects = ptc_thing_owner->GetObjectList();
            std::list<CIntelligentPointer<CThing> >::const_iterator bed_it = house_objects.begin();
            while (bed_it != house_objects.end()) {
                bed_it = std::find_if(bed_it, house_objects.end(), CIsFreeBed());
                if (bed_it == house_objects.end())
                    break;
                if (PBed == 0 || (near_to - (*bed_it)->GetPos()).GetSquaredMagnitude() < best) {
                    PBed = *bed_it;
                    best = (near_to - (*bed_it)->GetPos()).GetSquaredMagnitude();
                }
                ++bed_it;
            }
        }
        if (IS_THING_ALIVE(PBed)) {
            CTCSingleUserActionPoints* ptc_action_point;
            if (PBed->GetTC(&ptc_action_point)) {
                if (ptc_action_point->GetFreeActionPoint(GetCreature(), BedPos, Rhset, ActionPointIndex)) {
                    BedToRightOfCreature = false;
                    return true;
                }
            } else {
                IsBedSleepPointMarker = true;
                BedPos = PBed->GetPos();
                Rhset = PBed->GetPhysicsTC()->GetRHSet();
                CTCSinglePersonOccupiable* ptc_occupiable;
                // FableWin checks this call for success. Retail evaluates the call once.
                PBed->GetTC(&ptc_occupiable);
                FABLE_RETAIL_ASSERT(ptc_occupiable->GetOccupier() == 0);
                ptc_occupiable->SetOccupier(&GetCreature());
                C3DVector required_facing_dir = Rhset.GetForward();
                C3DVector right_vector = required_facing_dir;
                right_vector.Rotate(0.25f, 0.0f);
                float dot_product = right_vector.Dot(Rhset.GetRight());
                FABLE_RETAIL_ASSERT(fabs(dot_product) > DELTA_NOTIONAL_ZERO);
                if (dot_product > 0.0f)
                    BedToRightOfCreature = true;
                else
                    BedToRightOfCreature = false;
                return true;
            }
        }
    }
    PBed = CIntelligentPointer<CThing>(0);
    return false;
}