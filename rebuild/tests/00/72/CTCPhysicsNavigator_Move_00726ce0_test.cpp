// VC7.1 /Od fixture using the shared game vector arithmetic and observable world and physics boundaries.
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COMPONENT_CTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#define FABLE_RETAIL_C3DVECTOR_ISNONZERO_ORIGINAL_YXZ
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_LOCALS_XYZ
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_FROM3D
#define FABLE_RETAIL_C2DVECTOR_ORIGINAL_COPIES_PASS26
#include "fable_assert.h"
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "engine/GFSqr.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <vector>

extern const float DELTA_NOTIONAL_ZERO=0.0001f;
unsigned long GFastOneOverSqrtTable[128];

namespace
{
float DISTANCE_AWAY_TO_ALLOW_ONSURFACE_UPDATE_FOR_NAVIGATORS=10.0f;
}

namespace NGlobalConsole
{
bool GJossTestPhysics;
}

static char calls[32];
static unsigned call_count;
static void record(char id)
{
    assert(call_count+1 < sizeof(calls));
    calls[call_count++]=id;
    calls[call_count]=0;
}

static bool close_to(float value,float expected)
{
    return value >= expected-0.0001f && value <= expected+0.0001f;
}

struct CThing { int id; };
static CThing moving_thing={1};
static CThing stationary_thing={2};
struct C2DBoxI { int marker; };
struct CWorld {
    CWorld() : lod(0,0,0) {}
    C3DVector GetGameLODCentre() const { record('L'); return lod; }
    C3DVector lod;
};

class CTCPhysicsBase {
public:
    CTCPhysicsBase() : Position(2,0,0), Velocity(1,0,0) {}
    virtual void SetPosition(const C3DVector& pos) { record('B'); Position=pos; }
    const C3DVector& GetPosition() const { return Position; }
    CWorld& GetWorld() { return world; }
    CWorld world;
    C3DVector Position,Velocity;
};

class CTCPhysicsNavigator : public CTCPhysicsBase {
public:
    CTCPhysicsNavigator() : enabled(true), ForcedMoveToPosSet(false), ForcedMoveToPosUsePhysics(false),
      PositionToNavigateTo(3,0,0), AdditionalImpulse(0,0,0), ForcedMoveToPos(20,0,0),
      adjust_pos(false), inspected_impulse(0), collected_speed(0), finished(false) {}
    bool IsNavigationEnabled() const { record('N'); return enabled; }
    void Move();
    C2DBoxI GetPossibleCollidedWithArea() const { record('A'); C2DBoxI area={17}; return area; }
    C2DBoxI GetPossibleCollidedWithAreaWithImpulse(const C3DVector& impulse) {
        record('I');
        inspected_impulse=impulse.X;
        C2DBoxI area={22};
        return area;
    }
    void GetPossibleInteractedWithList(const C2DBoxI& area,std::vector<CThing*>& thing_list,float speed) {
        record('G');
        assert(thing_list.empty());
        assert(area.marker==17 ? speed==0 : area.marker==22);
        thing_list.push_back(&moving_thing);
        thing_list.push_back(&stationary_thing);
        collected_speed=speed;
    }
    void UpdateOnSurface(std::vector<CThing*>& thing_list,C3DVector&) {
        record('S');
        assert(!thing_list.empty() && thing_list.back()==&stationary_thing);
    }
    virtual void SetPosition(const C3DVector& pos) {
        record('V');
        Position=pos;
    }
    void PerformForceUpdates() {
        record('F');
        if (adjust_pos)
            Position.X+=1.0f;
    }
    void RemoveIntersectingCreaturesFromList(std::vector<CThing*>& thing_list) const {
        record('R');
        assert(thing_list.size()==2 && thing_list.front()==&moving_thing);
        thing_list.erase(thing_list.begin());
    }
    void UpdateVelocityForImpassability(const C3DVector& pos,C3DVector& impulse) {
        record('U');
        assert(&pos!=&Position);
        if (&impulse==&Velocity)
            impulse.X+=1.0f;
        else
            assert(pos.X==Position.X);
    }
    void PerformForcedMovement(std::vector<CThing*>& thing_list,const C3DVector& impulse,C3DVector& pos) {
        record('M');
        assert(thing_list.size()==(NGlobalConsole::GJossTestPhysics ? 2 : 1));
        pos.X+=impulse.X;
    }
    void PerformStepBasedMovement(std::vector<CThing*>&,C3DVector& pos) {
        record('P');
        pos.X+=Velocity.X;
    }
    void UpdateAdditionalImpulse() {
        record('Z');
        assert(!finished && !ForcedMoveToPosSet);
        finished=true;
    }

    bool enabled,ForcedMoveToPosSet,ForcedMoveToPosUsePhysics;
    C3DVector PositionToNavigateTo,AdditionalImpulse,ForcedMoveToPos;
    bool adjust_pos;
    float inspected_impulse,collected_speed;
    bool finished;
};

void CTCPhysicsNavigator::Move()
{
    if (IsNavigationEnabled() && !ForcedMoveToPosSet && !AdditionalImpulse.IsNonZero()) {
        C3DVector lod_centre = GetWorld().GetGameLODCentre();
        C2DVector between_vector_2d(lod_centre - PositionToNavigateTo);
        if (between_vector_2d.GetSquaredMagnitude() < GFSqr(DISTANCE_AWAY_TO_ALLOW_ONSURFACE_UPDATE_FOR_NAVIGATORS)) {
            C2DBoxI area = GetPossibleCollidedWithArea();
            std::vector<CThing*> thing_list;
            GetPossibleInteractedWithList(area,thing_list,0.0f);
            UpdateOnSurface(thing_list,PositionToNavigateTo);
        }
        CTCPhysicsBase::SetPosition(PositionToNavigateTo);
    } else if (ForcedMoveToPosSet && !ForcedMoveToPosUsePhysics) {
        SetPosition(ForcedMoveToPos);
        ForcedMoveToPosSet = false;
    } else {
        C3DVector current_pos = GetPosition();
        float add_mag = AdditionalImpulse.GetMagnitude();
        if (add_mag > 0.2f) {
            AdditionalImpulse.Normalise();
            AdditionalImpulse *= 0.2f;
        }
        C3DVector impulse = AdditionalImpulse;
        if (ForcedMoveToPosSet) {
            FABLE_RETAIL_ASSERT(ForcedMoveToPosUsePhysics);
            impulse += ForcedMoveToPos - current_pos;
            ForcedMoveToPosSet = false;
        }
        PerformForceUpdates();
        C2DBoxI area = GetPossibleCollidedWithAreaWithImpulse(impulse);
        std::vector<CThing*> thing_list;
        float speed = impulse.GetMagnitude();
        GetPossibleInteractedWithList(area,thing_list,speed);
        if (!NGlobalConsole::GJossTestPhysics)
            RemoveIntersectingCreaturesFromList(thing_list);
        current_pos = Position;
        UpdateVelocityForImpassability(current_pos,impulse);
        PerformForcedMovement(thing_list,impulse,current_pos);
        UpdateVelocityForImpassability(current_pos,Velocity);
        PerformStepBasedMovement(thing_list,current_pos);
        UpdateOnSurface(thing_list,current_pos);
        SetPosition(current_pos);
    }
    UpdateAdditionalImpulse();
}

static void check(CTCPhysicsNavigator& navigator,const char* expected)
{
    call_count=0;
    navigator.Move();
    assert(strcmp(calls,expected)==0);
    assert(navigator.finished);
}

int main()
{
    {
        CTCPhysicsNavigator snap;
        snap.world.lod=C3DVector(0,0,200);
        check(snap,"NLAGSBZ");
        assert(snap.Position.X==3 && snap.collected_speed==0);
    }
    {
        CTCPhysicsNavigator at_limit;
        at_limit.PositionToNavigateTo=C3DVector(6,8,0);
        check(at_limit,"NLBZ");
        assert(at_limit.Position.X==6 && at_limit.Position.Y==8);
    }
    {
        CTCPhysicsNavigator forced;
        forced.ForcedMoveToPosSet=true;
        check(forced,"NVZ");
        assert(forced.Position.X==20 && !forced.ForcedMoveToPosSet);
    }
    {
        CTCPhysicsNavigator physics;
        physics.adjust_pos=true;
        physics.ForcedMoveToPos=C3DVector(10,0,0);
        physics.ForcedMoveToPosSet=true;
        physics.ForcedMoveToPosUsePhysics=true;
        physics.AdditionalImpulse=C3DVector(1,0,0);
        // Represent 1.0 in the reciprocal-root lookup for the tested squared length.
        GFastOneOverSqrtTable[64]=256;
        check(physics,"NFIGRUMUPSVZ");
        assert(close_to(physics.AdditionalImpulse.X,0.2f) && !physics.ForcedMoveToPosSet);
        assert(close_to(physics.inspected_impulse,8.2f) && close_to(physics.collected_speed,8.2f));
        assert(close_to(physics.Position.X,13.2f));
    }
    {
        CTCPhysicsNavigator diagnostic;
        diagnostic.enabled=false;
        diagnostic.AdditionalImpulse=C3DVector(0.125f,0,0);
        NGlobalConsole::GJossTestPhysics=true;
        check(diagnostic,"NFIGUMUPSVZ");
        assert(close_to(diagnostic.inspected_impulse,0.125f) && close_to(diagnostic.Position.X,4.125f));
        NGlobalConsole::GJossTestPhysics=false;
    }
    {
        CTCPhysicsNavigator nonzero;
        nonzero.AdditionalImpulse=C3DVector(0.125f,0,0);
        check(nonzero,"NFIGRUMUPSVZ");
        assert(close_to(nonzero.inspected_impulse,0.125f) && close_to(nonzero.Position.X,4.125f));
    }
    puts("00726ce0_TEST PASS");
    return 0;
}