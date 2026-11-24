// 008101f0 CTCDExplosiveTrail::FrameUpdate
// Leaf: ?FrameUpdate@CTCDExplosiveTrail@@UAEXXZ
// Flags:
#define FABLE_RETAIL_EXPLOSIVE_TRAIL_UPDATE_PASS28
#define FABLE_RETAIL_C3DVECTOR_ISNONZERO_ORIGINAL_YXZ
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_STLP_LIST_REVERSE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_STLP_LIST_PUSHBACK_NOINLINE
#define FABLE_RETAIL_STLP_LIST_INSERT_OUTOFLINE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_CROSSWITHUP
#define FABLE_RETAIL_C3DVECTOR_NORMALISE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_LASTPOSITION
#define FABLE_RETAIL_CTHINGOBJECT
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGOBJECT_CREATE
#define FABLE_RETAIL_CTHINGMAPMANAGER_PLACE_PASS27
#define FABLE_RETAIL_CTHING_CONSTRUCTFROMLEVELSCRIPT
#define FABLE_RETAIL_CTCTRAPBASE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_HEIGHTZ
#define FABLE_RETAIL_GFANGLES_SIGNED_ROUNDING
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_LIST
#include "rebuild_abi.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCDExplosiveTrail.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CExplosiveTrailDef.h"
#include "engine/CTCTrapBase.h"
#include "engine/CThingObject.h"
#include "engine/CWorld.h"
#include "engine/CWorldMap.h"
#include "engine/GFAngles.h"
#include "engine/GFTanh.h"
#include "engine/ThingAlive.h"
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#include "engine/CMainGameComponent.h"
#include "engine/GFFloatRandom.h"
namespace NExplosiveTrailHelpers { void FABLE_FASTCALL RandomiseThingOrientationAroundDirection(CThing&,const C3DVector&); }
extern const float DELTA_NOTIONAL_ZERO;
typedef std::list<std::pair<CIntelligentPointer<CThing>,long> > TrapList;
#if defined(_MSC_VER)
// Inline-budget: the pointer copy is out of line in the selected STLport pair copy at 00788D88.
#pragma inline_depth(0)
#endif
namespace std {
template<> inline pair<CIntelligentPointer<CThing>,long>::pair(const pair<CIntelligentPointer<CThing>,long>& source)
    : first(source.first), second(source.second) {}
}
#if defined(_MSC_VER)
#pragma inline_depth()
#endif

template<class T, class U>
inline void FABLE_FASTCALL GFLimitUpper(T& value, const U& limit)
{
    if (value > limit)
        value = limit;
}

namespace std {
// The list insertion body is owned by 00810C20.
template<> list<pair<CIntelligentPointer<CThing>,long> >::iterator
list<pair<CIntelligentPointer<CThing>,long> >::insert(iterator, const pair<CIntelligentPointer<CThing>,long>&);
}
void CTCDExplosiveTrail::FrameUpdate()
{
    const float NUM_SECS_BETWEEN_TRAPS = PDef->SecondsBetweenTraps;
    const float TRAP_SPACING = PDef->DistanceBetweenTraps;
    const float NUM_SECS_FOR_TRAP_PERSIST = PDef->SecondsForTrapPersistance;
    const float NUM_SECS_FOR_WHOLE_TRAIL_TO_LAST = PDef->SecondsTrailLifetime;
    const float TRAP_DEVIATION_AMOUNT = PDef->DistanceForLateralDeviation;
    ++Timer;
    if (Timer > CMainGameComponent::GetConstantFPS() * NUM_SECS_FOR_WHOLE_TRAIL_TO_LAST || Terminated) {
        if (ComponentTraps.empty())
            GetParentThing().Kill(true);
    } else {
        long trap_create_period = (long)(CMainGameComponent::GetConstantFPS() * NUM_SECS_BETWEEN_TRAPS);
        if (Timer % trap_create_period == 0) {
            long trap_index = Timer / trap_create_period;
            C3DVector base_pos = GetParentThing().GetPos();
            C3DVector fire_line = GetParentThing().GetPhysicsTC()->GetFacingVector();
            long num_traps = ComponentTraps.size();
            if (num_traps > 0) {
                TrapList::reverse_iterator it = ComponentTraps.rbegin();
                CThing* plast_trap = it->first.Get();
                if (IS_THING_ALIVE(plast_trap))
                    base_pos = plast_trap->GetPos();
                if (num_traps > 1) {
                    CThing* pbefore_last_trap = (++it)->first.Get();
                    if (IS_THING_ALIVE(pbefore_last_trap)) {
                        fire_line = base_pos - pbefore_last_trap->GetPos();
                    }
                }
            }
            fire_line.Z = 0.0f;
            if (fire_line.IsNonZero()) fire_line.AccurateNormalise();
            const float FRACTION_OF_LIFE_TO_CHASE = PDef->FractionOfLifetimeToTurnTowardsTarget;
            const float DEGREES_MAX_TURN_PER_TRAP = PDef->DegreesMaxTurnTowardsTargetPerTrap;
            const float SECONDS_TARGET_POS_PREDICT = PDef->SecondsIcicleAttackTargetPositionPredictionTime;
            if (IS_THING_ALIVE(PTarget) && Timer <= CMainGameComponent::GetConstantFPS() * NUM_SECS_FOR_WHOLE_TRAIL_TO_LAST * FRACTION_OF_LIFE_TO_CHASE) {
                const C3DVector& cur_target_pos = PTarget->GetPos();
                C3DVector target_vel = cur_target_pos - PTarget->GetPhysicsTC()->GetLastPosition();
                C3DVector target_pos = cur_target_pos + target_vel * SECONDS_TARGET_POS_PREDICT;
                C3DVector line_to_target_pos = target_pos - base_pos;
                line_to_target_pos.Z = 0.0f;
                if (line_to_target_pos.IsNonZero())
                    line_to_target_pos.AccurateNormalise();
                C3DVector start_to_target_pos = target_pos - GetParentThing().GetPos();
                start_to_target_pos.Z = 0.0f;
                if (start_to_target_pos.IsNonZero()) start_to_target_pos.AccurateNormalise();
                if (start_to_target_pos.Dot(line_to_target_pos) > 0.0f) {
                    float cos_thet = line_to_target_pos.Dot(fire_line);
                    if (cos_thet < -1.0f) cos_thet = -1.0f;
                    else if (cos_thet > 1.0f)
                    cos_thet = 1.0f;
                    float theta = GFArcCos(cos_thet);
                    if (line_to_target_pos.Cross(fire_line).Z < 0.0f) theta *= -1.0f;
                    GFLimitUpper(theta, DEGREES_MAX_TURN_PER_TRAP / 360.0f);
                    fire_line.Rotate(C3DVector(0.0f, 0.0f, 1.0f), theta);
                }
            }
            C3DVector perp_line = fire_line.CrossWithUp();
            float trap_deviation_mag_max = TRAP_DEVIATION_AMOUNT * GFTanh(trap_index / (TRAP_SPACING * 5.0f));
            float trap_dev_mag = (GFFloatRandom(trap_deviation_mag_max, GetWorld().GetWorldSeed()) * 2.0f) - trap_deviation_mag_max;
            C3DVector create_pos = base_pos + fire_line * TRAP_SPACING + perp_line * trap_dev_mag;
            CWorldMap& wmap = GetWorld().GetWorldMap();
            float height = wmap.GetHeightZBelowPos(create_pos + C3DVector(0.0f,0.0f,10.0f));
            float ground_height = wmap.GetGroundSizeZAt(create_pos + C3DVector(0.0f,0.0f,10.0f));
            if (height - ground_height > DELTA_NOTIONAL_ZERO)
                Terminated = true;
            else if (!wmap.IsPosPassable(create_pos))
            Terminated = true;
            if (!Terminated) {
                create_pos.Z = ground_height;
                if (PDef->TrapDefIndex > 0) {
                    CThing* pthing = CThingObject::Create(PDef->TrapDefIndex, create_pos, GetParentThing().GetPlayerNumber(), 0, 0, "");
                    if (pthing) {
                        NExplosiveTrailHelpers::RandomiseThingOrientationAroundDirection(*pthing, fire_line);
                        CTCTrapBase* ptc_trap;
                        if (pthing->GetTC(TCI_TRAP, &ptc_trap)) {
                            for (std::vector<CIntelligentPointer<CThing> >::iterator it = ThingsToIgnore.begin(); it != ThingsToIgnore.end(); ++it) {
                                if (IS_THING_ALIVE(*it))
                                    ptc_trap->SetThingAsImmune(*it->Get());
                            }
                            ptc_trap->ManualTrigger();
                        }
                        ComponentTraps.push_back(std::make_pair(CIntelligentPointer<CThing>(pthing), -1));
                    }
                }
            }
        }
    }
    TrapList::iterator it = ComponentTraps.begin();
    while (it != ComponentTraps.end()) {
        CThing* pthing = it->first.Get();
        long& timer = it->second;
        CTCTrapBase* ptc_trap;
        if (IS_THING_ALIVE(pthing) && pthing->GetTC(TCI_TRAP, &ptc_trap)) {
            if (ptc_trap->IsClosed()) {
                pthing->Kill(true);
                it = ComponentTraps.erase(it);
            } else {
                if (ptc_trap->IsOpen()) {
                    if (timer < 0)
                        timer = (long)(CMainGameComponent::GetConstantFPS() * NUM_SECS_FOR_TRAP_PERSIST);
                    else if (timer == 0)
                    ptc_trap->ManualReset();
                    else
                        --timer;
                }
                ++it;
            }
        } else {
            it = ComponentTraps.erase(it);
        }
    }
}