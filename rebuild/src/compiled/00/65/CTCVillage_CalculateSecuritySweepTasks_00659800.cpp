// 00659800 CTCVillage::CalculateSecuritySweepTasks
// Leaf: ?CalculateSecuritySweepTasks@CTCVillage@@AAEXXZ
// Flags: none
#define FABLE_RETAIL_STLP_FIND_IF_GUARD_BODY_PASS23

#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_CTHING_GETTC_DONOR_NAMES_PASS23
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D_ORIGINAL_SUM_PASS23
#define FABLE_RETAIL_C3DVECTOR_SQUARED2D_ONE_EXPRESSION_PASS23
#define FABLE_RETAIL_STLP_BINARY_COMPOSE
#define FABLE_RETAIL_STLP_FIND_IF_INPUT
#define FABLE_RETAIL_STLP_COUNT_IF_CALLED_FASTCALL_PASS23
#define FABLE_RETAIL_STLP_FIND_IF_CALLED_PASS23
#define FABLE_RETAIL_SECURITY_SWEEP_LIFETIMES_PASS23
#define FABLE_RETAIL_KID_EVADE_LIFETIMES_PASS23
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ORIGINAL_SETPTR_PASS22
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_SCOPED_PTR_RESET
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CBASICSTRING_EQ_OUTOFLINE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING_GETSTRING
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_VILLAGE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR
#define FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_PROPERTY
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGBUILDING
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCTHINGOWNER
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_MEMBERS
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_TYPED_MEMBERS
#define FABLE_RETAIL_CWORLD_DEFINITION_REFERENCE_PASS23
#define FABLE_RETAIL_CWORLD_GAMETIME
#define FABLE_RETAIL_CTCBASE_GETWORLD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTTHING_BODY
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGAICREATURE_STATTYPEFLAGS
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_CTCCREATURENAVIGATION_NAVIGABLE
#define FABLE_RETAIL_C3DVECTOR_NORMALISE
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_SMART_POINTER_ACCESSORS
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETWORLD
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CVILLAGETASKPATROL_CTOR
#define FABLE_RETAIL_CVILLAGETASKPICKCROPS_CTOR
#define FABLE_RETAIL_CVILLAGETASKSECURITYSWEEP
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CTCBase.h"
#include "engine/CThing.h"
#include "engine/CDefClassBase.h"
#include "engine/CSubDefClassBase.h"
#include "engine/CDefString.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingBuilding.h"
#include "engine/CThingMarker.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCThingOwner.h"
#include "engine/CTCVillageMember.h"
#include "engine/CTCVillage.h"
#include "engine/CWorld.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_HasTC.h"
#include "engine/CTCCreatureNavigation.h"
#include "engine/CVillageTaskSecuritySweep.h"
#include "engine/CMainGameComponent.h"
#include <math.h>
#include "engine/CThingCompare_Nearest.h"
#include "engine/HasGuardProperty.h"
#include "stlport/fable_count_if.h"
#include "stlport/fable_algorithm.h"
void CTCVillage::CalculateSecuritySweepTasks()
{
    long num_guards;
    CThing* pguard;
    {
                num_guards = std::count_if(VillagePeople.begin(), VillagePeople.end(), HasGuardProperty());
        std::list<CIntelligentPointer<CThingAICreature> >::iterator guard_it =
            std::find_if(VillagePeople.begin(), VillagePeople.end(), HasGuardProperty());
        if (guard_it == VillagePeople.end())
            return;
        pguard = guard_it->Get();
    }
    CTCCreatureNavigation* ptc_navigation = 0;
    pguard->GetTC(&ptc_navigation);
    CThingPlayerCreature* pplayer = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
    if (!pplayer)
        return;
    float radius_of_catchment_circle;
    const C3DVector* exit_position;
    {
    const C3DVector& player_position = pplayer->GetPos();
    CThing* nearest_exit = 0;
    PositionPlayerWasInWhenSecuritySweepCalculated = player_position;
    CMainGameComponent::Get()->GetWorld().PeekMyPlayer()->GetPControlledCreature();
    GetWorld().PeekThingSearchTools().GetPBestThing(
        std::compose2(std::logical_or<bool>(), CThingFilter_HasTC(TCI_REGION_EXIT), CThingFilter_HasTC(TCI_TELEPORTER)),
        CThingCompare_Nearest(PositionPlayerWasInWhenSecuritySweepCalculated), &nearest_exit);
    if (!nearest_exit)
        return;
    radius_of_catchment_circle = InitialSecuritySweepRadius -
        (float)((PeekWorld().GetFrame() - FrameStartedSecuritySweep) / CMainGameComponent::GetConstantFPS()) * 0.4f;
    {
    CThingAICreature* nearest_guard;
    if (GetWorld().PeekThingSearchTools().GetPBestThing(CThingFilter_HasTC(TCI_GUARD),
            CThingCompare_Nearest(PositionPlayerWasInWhenSecuritySweepCalculated), &nearest_guard)) {
        float guard_dist = (PositionPlayerWasInWhenSecuritySweepCalculated - nearest_guard->GetPos()).GetMagnitude2D();
        if (radius_of_catchment_circle > guard_dist) {
            InitialSecuritySweepRadius -= radius_of_catchment_circle - guard_dist;
            radius_of_catchment_circle = guard_dist;
        }
    }
    }
    const float minimumRadius = 2.0f;
    if (SecuritySweepUrgent)
        radius_of_catchment_circle = minimumRadius;
    if (radius_of_catchment_circle < minimumRadius)
        radius_of_catchment_circle = minimumRadius;
    exit_position = &nearest_exit->GetPos();
    }
    C3DVector guard_vector = *exit_position - PositionPlayerWasInWhenSecuritySweepCalculated;
    float distance_to_exit = guard_vector.GetSquaredMagnitude2D();
    if (distance_to_exit < radius_of_catchment_circle * radius_of_catchment_circle)
        radius_of_catchment_circle = (float)sqrt(distance_to_exit) * 0.8f;
    guard_vector.Normalise();
    guard_vector *= radius_of_catchment_circle;
    float angle_per_turn = 1.0f / num_guards;
    for (long c1 = 0; c1 < num_guards; ++c1) {
        float current_angle_mod = 0.0f;
        C3DVector t_guard_vector = guard_vector;
        C3DVector pos_to_go_to;
        while (!ptc_navigation->GetNearestNavigablePosTo(t_guard_vector + PositionPlayerWasInWhenSecuritySweepCalculated, 5.0f, pos_to_go_to)) {
            t_guard_vector = guard_vector;
            if (current_angle_mod >= 0.0f) {
                current_angle_mod += angle_per_turn * 0.2f;
                if (current_angle_mod >= angle_per_turn)
                    goto nextGuard;
            }
            else
                current_angle_mod *= -1.0f;
            t_guard_vector.RotateXY(current_angle_mod);
        }
        SecuritySweepTasks.push_back(CCountedPointer<CVillageTaskSecuritySweep>(
            new CVillageTaskSecuritySweep(pos_to_go_to, 5.0f, PositionPlayerWasInWhenSecuritySweepCalculated, 0.25f)));
    nextGuard:
        guard_vector.RotateXY(angle_per_turn);
    }
}