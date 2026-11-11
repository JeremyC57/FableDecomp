// CAIStateGroup_JackDragon::IntoGroundSwoop() -- the Jack-dragon's swoop into the ground and back out: fly from the creature through the side's two into-ground points, the third and a point behind the landed marker (rotated by its heading), then burninate along the marker's facing (flame from the marker to 20 units ahead, leaving through a point 10 ahead and 8 up and the first out-of-ground point), then fly out on a random side (out-of-ground points 2, 3 and 4 of that side) and choose the hover / strafe state; terminate when the enemy is gone or the group stops
// Leaf: ?IntoGroundSwoop@CAIStateGroup_JackDragon@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1 RTM) @ 0x00951430, 1765 bytes, plain `ret`, frame 0x1A4. Ego_r.pdb ?IntoGroundSwoop@CAIStateGroup_JackDragon@@AAEXXZ
//   (private; donor 0x00975F70 between IntoGround 0x00975BE0 and FlyOutOfGround 0x009764A0 = retail .. / 0x00951430 / 0x00951B20 landed).
// Callees (all checked against retail with rel.py): GetPItem 0x00A01B50, CThing::GetPos 0x004C73D0, C3DVector::RotateXY 0x00A14040,
//   CTCPhysicsBase::GetFacingAngleXY (virtual +0xFC) / GetFacingVector (virtual +0x118), vector<C3DVector>::_M_insert_overflow 0x0066E370
//   (the expanded push_backs) / push_back 0x0066E630 (the three called after VC7.1's caller-size budget runs out), __copy<C3DVector*>
//   0x0074CA20 (the expanded clear()), StartFlying 0x00950C30, IsStopping 0x008FD010 / Terminate 0x008FCF40 (the expanded same-TU
//   IsStillRunnable 0x0094FDF0), CCreatureAction_DragonBurninateInto ctor 0x009537B0 + its implicit destructor (free 0x00BFEA14 of the
//   route copy, ~CCombatActionBase 0x0062E900), AIAction_PerformActionAndWait 0x0097DB70, GetWorld 0x008FCFC0 / CWorld::GetWorldSeed
//   0x0049D810 / GFROR13 0x00497890 (the inline GFRandom(2, seed)), ChooseHoverOrStrafeState 0x0094FFF0.
// Constants: -37.93019f / 15.0f / -80.0f / 30.0f (the two marker offsets, stored at entry), 20.0f 0x0122F160, 10.0f 0x01237F0C,
//   8.0f 0x0125916C (VC7.1 literal __real@41000000), BurnStart 0.1f / BurnStop 0.6f pushed as immediates.
// Shape: (1) the second marker offset (0, -80, 30) is rotated with the first but never used (the call keeps it). (2) The 8-up exit point
//   is `... + C3DVector(0.0f, 0.0f, 8.0f)`: VC7.1 folds the +0 terms and stores the intermediate Z to the frame (0x00951821) before
//   adding 8; `out_pos.Z += 8.0f` keeps Z on the x87 stack; the extra temporary node also puts the facing results into one shared frame
//   slot (frame 0x1B0 -> 0x1A4). (3) The DragonBurninateInto constructor (same TU, landed 0x009537B0) is defined here, visible but
//   noinline: seeing that it only reads its C3DVector& arguments, VC7.1 stores the flame end point component by component and
//   schedules the flame start copy as retail (0x00951734..0x0095179B); declaration-only exposes both locals (Z-first sums).
// Layout: overlay opt-ins (hdr/README.md): CAIStateGroup_JackDragon retail class (+ _INTOGROUND: StartFlying / ChooseHoverOrStrafeState /
//   IntoGroundSwoop), CCreatureAction_DragonBurninateInto retail class (new opt-in); retail STLport vector<C3DVector> (12 bytes).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CAISTATEGROUP_JACKDRAGON
#define FABLE_RETAIL_CAISTATEGROUP_JACKDRAGON_INTOGROUND
#define FABLE_RETAIL_CCREATUREACTION_DRAGONBURNINATEINTO
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
#define FABLE_RETAIL_STLP_VECTOR_COPY_OUTOFLINE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CSoundPair.h"
#include "engine/CCombatActionBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CCreatureAction_DragonBurninateInto.h"
#include "engine/CAIStateGroup_JackDragon.h"
#include "engine/CWorld.h"
#include "engine/GFRandom.h"
#include "engine/ThingAlive.h"

// The same-TU IsStillRunnable (retail 0x0094FDF0: alive enemy and not stopping), visible so VC7.1 can expand it.
bool CAIStateGroup_JackDragon::IsStillRunnable()
{
    return IS_THING_ALIVE(PEnemy) && !IsStopping();
}

// The same-TU constructor (retail 0x009537B0, landed), visible but kept out of line as in the retail TU.
#ifdef _MSC_VER
__declspec(noinline)
#endif
CCreatureAction_DragonBurninateInto::CCreatureAction_DragonBurninateInto(CThingCreatureBase& creature, std::vector<C3DVector>& movement_points,
    C3DVector& previous_point, float burn_start, float burn_stop, C3DVector& flame_start_pos, C3DVector& flame_end_pos)
    : CCombatActionBase(creature, 250, GetSoundPair(creature, "SWORD_SWING"))
    , PreviousPoint(previous_point)
    , MovementPoints(movement_points)
    , BurnStart(burn_start)
    , BurnStop(burn_stop)
    , FlameStartPos(flame_start_pos)
    , FlameEndPos(flame_end_pos)
{
}

void CAIStateGroup_JackDragon::IntoGroundSwoop()
{
    C3DVector swoop_offset(0.0f, -37.93019f, 15.0f);
    C3DVector far_offset(0.0f, -80.0f, 30.0f);
    if (IS_THING_ALIVE(PLandedMarker) && PLandedMarker->GetPhysicsTC()) {
        swoop_offset.RotateXY(PLandedMarker->GetPhysicsTC()->GetFacingAngleXY());
        far_offset.RotateXY(PLandedMarker->GetPhysicsTC()->GetFacingAngleXY());
    }
    std::vector<C3DVector> route;
    route.push_back(GetCreature().GetPos());
    if (GoLeft) {
        route.push_back(DragonPosIntoGround1Left);
        route.push_back(DragonPosIntoGround2Left);
    } else {
        route.push_back(DragonPosIntoGround1Right);
        route.push_back(DragonPosIntoGround2Right);
    }
    route.push_back(DragonPosIntoGround3);
    route.push_back(swoop_offset + PLandedMarker->GetPos());
    StartFlying(GetCreature(), route, PreviousFlightPos, CCountedPointer<CCreatureActionBase>());
    if (!IsStillRunnable()) {
        Terminate();
        return;
    }
    C3DVector flame_start = PLandedMarker->GetPos();
    C3DVector flame_end = PLandedMarker->GetPhysicsTC()->GetFacingVector() * 20.0f + PLandedMarker->GetPos();
    C3DVector out_pos = PLandedMarker->GetPhysicsTC()->GetFacingVector() * 10.0f + PLandedMarker->GetPos() + C3DVector(0.0f, 0.0f, 8.0f);
    PreviousFlightPos = DragonPosIntoGround3;
    route.clear();
    route.push_back(GetCreature().GetPos());
    route.push_back(out_pos);
    route.push_back(DragonPosOutOfGround1);
    AIAction_PerformActionAndWait(CCreatureAction_DragonBurninateInto(GetCreature(), route, PreviousFlightPos, 0.1f, 0.6f, flame_start, flame_end), false);
    route.clear();
    route.push_back(GetCreature().GetPos());
    GoLeft = GFRandom(2, GetWorld().GetWorldSeed());
    if (GoLeft) {
        route.push_back(DragonPosOutOfGround2);
        route.push_back(DragonPosOutOfGround3Left);
        route.push_back(DragonPosOutOfGround4Left);
        PreviousFlightPos = DragonPosOutOfGround3Left;
    } else {
        route.push_back(DragonPosOutOfGround2);
        route.push_back(DragonPosOutOfGround3Right);
        route.push_back(DragonPosOutOfGround4Right);
        PreviousFlightPos = DragonPosOutOfGround3Right;
    }
    StartFlying(GetCreature(), route, PreviousFlightPos, CCountedPointer<CCreatureActionBase>());
    if (!IsStillRunnable()) {
        Terminate();
        return;
    }
    CurrentState = ChooseHoverOrStrafeState();
}
