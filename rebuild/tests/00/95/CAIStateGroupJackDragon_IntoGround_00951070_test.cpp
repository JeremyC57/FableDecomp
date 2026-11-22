#include <vector>
#include <math.h>
#include <stdio.h>
#include "engine/CCountedPointer.h"

struct C3DVector {
    C3DVector(float x=0.0f, float y=0.0f, float z=0.0f) : X(x), Y(y), Z(z) {}
    C3DVector operator+(const C3DVector& other) const {
        return C3DVector(X+other.X, Y+other.Y, Z+other.Z);
    }
    void RotateXY(float angle) {
        const float x=X, y=Y;
        X=x*(float)cos(angle)-y*(float)sin(angle);
        Y=x*(float)sin(angle)+y*(float)cos(angle);
    }
    float X, Y, Z;
};
struct CPhysics {
    CPhysics() : Angle(0.0f), FacingRequests(0) {}
    float GetFacingAngleXY() { ++FacingRequests; return Angle; }
    float Angle;
    int FacingRequests;
};
struct CThing {
    CThing() : Physics(0), Dead(false) {}
    CPhysics* GetPhysicsTC() { return Physics; }
    const C3DVector& GetPos() const { return Position; }
    C3DVector Position;
    CPhysics* Physics;
    bool Dead;
};
template<class T> bool IS_THING_ALIVE(T* value) { return value && !value->Dead; }
struct CJackDragonDef { float DragonGroundHeightOffsetBodge; };
int DefinitionHolders;
template<class T> struct CDefPointer {
    CDefPointer() : Ptr(0) { ++DefinitionHolders; }
    ~CDefPointer() { --DefinitionHolders; }
    T* operator->() const { return Ptr; }
    T* Ptr;
};
struct CTCBoss {
    CTCBoss() : Counter(39) {}
    void SetGenericCounter(long counter) { Counter = counter; }
    long Counter;
};
struct CThingCreatureBase : CThing {
    CThingCreatureBase() : BossLookups(0), DefLookups(0) {}
    CThingCreatureBase& PeekBaseDef() { return *this; }
    bool GetSubDef(CDefPointer<const CJackDragonDef>* out) { ++DefLookups; out->Ptr = &Definition; return true; }
    bool GetTC(CTCBoss** out) { ++BossLookups; *out = &Boss; return true; }
    CTCBoss Boss;
    CJackDragonDef Definition;
    int BossLookups, DefLookups;
};
int LiveActions;
struct CCreatureActionBase {
    CCreatureActionBase() { ++LiveActions; }
    virtual ~CCreatureActionBase() { --LiveActions; }
    CThingCreatureBase* Owner;
    CThing* Target;
};
struct CCreatureAction_DragonLand : CCreatureActionBase {
    CCreatureAction_DragonLand(CThingCreatureBase& creature, CThing& thing) { Owner = &creature; Target = &thing; }
};
struct CWorld {
    CWorld() : Frame(100) {}
    long GetFrame() { return ++Frame; }
    long Frame;
};
class CAIStateGroup_JackDragon {
public:
    enum { DRAGON_STATE_GROUND = 1 };
    CAIStateGroup_JackDragon() : PEnemy(0), PLandedMarker(0), GoLeft(0),
        FramesInSnapRange(84), FrameOfLastSlam(85), FrameWhenLastInRange(86), FrameWhenLanded(87),
        CurrentState(0), StopAfterStart(false), KillAfterStart(false), Stopping(false),
        Terminated(false), Created(false), ValidStart(false), Starts(0) {}
    void IntoGround();
    bool IsStillRunnable();
    void ResetTimers();
    CThingCreatureBase& GetCreature() { return Creature; }
    CWorld& GetWorld() { return World; }
    bool IsStopping() const { return Stopping; }
    void Terminate() { Terminated = true; }
    void CreateIBC() { Created = true; }
    void StartFlying(CThingCreatureBase& creature, std::vector<C3DVector>& points,
                     C3DVector& previous, CCountedPointer<CCreatureActionBase> action) {
        ++Starts;
        ValidStart = &creature == &Creature && &previous == &PreviousFlightPos && LiveActions == 1
            && DefinitionHolders == 1 && action.Get() && action->Owner == &Creature && action->Target == PEnemy;
        Route = points;
        Stopping = StopAfterStart;
        if (KillAfterStart)
            PEnemy->Dead = true;
    }
    CThingCreatureBase Creature, *PEnemy;
    CThing* PLandedMarker;
    CWorld World;
    long GoLeft;
    C3DVector DragonPosIntoGround2Left, DragonPosIntoGround2Right, DragonPosIntoGround3, PreviousFlightPos;
    long FramesInSnapRange, FrameOfLastSlam, FrameWhenLastInRange, FrameWhenLanded, CurrentState;
    bool StopAfterStart, KillAfterStart, Stopping, Terminated, Created, ValidStart;
    int Starts;
    std::vector<C3DVector> Route;
};

bool CAIStateGroup_JackDragon::IsStillRunnable()
{
    if (!IS_THING_ALIVE(PEnemy))
        return false;
    if (IsStopping())
        return false;
    return true;
}

void CAIStateGroup_JackDragon::ResetTimers()
{
    FramesInSnapRange = 0;
    const long frame = GetWorld().GetFrame();
    FrameOfLastSlam = frame;
    FrameWhenLastInRange = frame;
}

void CAIStateGroup_JackDragon::IntoGround()
{
    CDefPointer<const CJackDragonDef> p_jack_dragon_def;
    GetCreature().PeekBaseDef().GetSubDef(&p_jack_dragon_def);
    C3DVector land_offset(0.0f, -37.930191f, 0.0f + 30.148378f);
    C3DVector start_offset(0.0f, -80.0f, 30.0f);
    if (IS_THING_ALIVE(PLandedMarker) && PLandedMarker->GetPhysicsTC()) {
        land_offset.RotateXY(PLandedMarker->GetPhysicsTC()->GetFacingAngleXY());
        start_offset.RotateXY(PLandedMarker->GetPhysicsTC()->GetFacingAngleXY());
    }
    float fly_speed = 30.0f;
    std::vector<C3DVector> movement_points;
    movement_points.push_back(GetCreature().GetPos());
    if (GoLeft)
        movement_points.push_back(DragonPosIntoGround2Left);
    else
        movement_points.push_back(DragonPosIntoGround2Right);
    movement_points.push_back(DragonPosIntoGround3);
    movement_points.push_back(PLandedMarker->GetPos() + land_offset +
        C3DVector(0.0f, 0.0f, p_jack_dragon_def->DragonGroundHeightOffsetBodge));
    CTCBoss* ptc_boss;
    GetCreature().GetTC(&ptc_boss);
    {
        CCountedPointer<CCreatureActionBase> p_land_action(new CCreatureAction_DragonLand(GetCreature(), *PEnemy));
        StartFlying(GetCreature(), movement_points, PreviousFlightPos, p_land_action);
    }
    if (!IsStillRunnable()) {
        Terminate();
        return;
    }
    CreateIBC();
    ptc_boss->SetGenericCounter(0);
    ResetTimers();
    FrameWhenLanded = GetWorld().GetFrame();
    CurrentState = DRAGON_STATE_GROUND;
}

static bool close(float a, float b) { return fabs(a-b) < 0.001f; }
static bool samePoint(const C3DVector& a, const C3DVector& b) {
    return close(a.X,b.X) && close(a.Y,b.Y) && close(a.Z,b.Z);
}
int main() {
    int cases=0;
    for (int left=0; left!=2; ++left) {
        for (int markerMode=0; markerMode!=3; ++markerMode) {
            for (int result=0; result!=3; ++result) {
                CThingCreatureBase enemy;
                CThing marker;
                CPhysics physics;
                physics.Angle=1.57079632679f;
                marker.Position=C3DVector(100.0f, 200.0f, 300.0f);
                marker.Physics=markerMode ? &physics : 0;
                marker.Dead=markerMode==2;
                CAIStateGroup_JackDragon group;
                group.Creature.Position=C3DVector(1.0f, 2.0f, 3.0f);
                group.Creature.Definition.DragonGroundHeightOffsetBodge=15.0f;
                group.PEnemy=&enemy;
                group.PLandedMarker=&marker;
                group.GoLeft=left;
                group.DragonPosIntoGround2Left=C3DVector(-11.0f, -12.0f, -13.0f);
                group.DragonPosIntoGround2Right=C3DVector(11.0f, 12.0f, 13.0f);
                group.DragonPosIntoGround3=C3DVector(21.0f, 22.0f, 23.0f);
                group.StopAfterStart=result==1;
                group.KillAfterStart=result==2;
                group.IntoGround();
                const bool success=result==0;
                const C3DVector expected=markerMode==1 ? C3DVector(137.930191f, 200.0f, 345.148378f)
                                                     : C3DVector(100.0f, 162.069809f, 345.148378f);
                if (group.Route.size()!=4 || group.Starts!=1 || !group.ValidStart || LiveActions || DefinitionHolders
                    || group.Creature.BossLookups!=1 || group.Creature.DefLookups!=1 || physics.FacingRequests!=(markerMode==1?2:0)
                    || !samePoint(group.Route[0], group.Creature.Position) || !samePoint(group.Route[2], group.DragonPosIntoGround3)
                    || !samePoint(group.Route[1], left ? group.DragonPosIntoGround2Left : group.DragonPosIntoGround2Right)
                    || !samePoint(group.Route[3], expected) || group.Created!=success || group.Terminated==success
                    || group.CurrentState!=(success?1:0) || group.Creature.Boss.Counter!=(success?0:39)
                    || group.FramesInSnapRange!=(success?0:84) || group.FrameOfLastSlam!=(success?101:85)
                    || group.FrameWhenLastInRange!=(success?101:86) || group.FrameWhenLanded!=(success?102:87)) {
                    printf("FAIL left=%d marker=%d result=%d\n", left, markerMode, result);
                    return 1;
                }
                ++cases;
            }
        }
    }
    CAIStateGroup_JackDragon group;
    if (cases!=18 || group.IsStillRunnable())
        return 2;
    puts("00951070_TEST PASS");
    return 0;
}