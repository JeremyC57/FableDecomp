// 008de080 behavior test. The production member body is reproduced below.
// Engine entry points record their arguments. The expected positions, yaw and blend
// are calculated independently in double precision.
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#endif
#include "fable_compiler.h"
#include "rebuild_abi.h"
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_SQUARE_ORIGINAL_FORMAL
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_GROUPED_SUM
#define FABLE_RETAIL_GFFASTONEOVERSQRT_ORIGINAL_UNION
#define FABLE_RETAIL_GFFASTONEOVERSQRT_PORTABLE_SEED_FIELDS_PASS26
#define FABLE_RETAIL_JACK_STAGE1_UPDATE
#include "engine/C3DVector.h"
unsigned long GFastOneOverSqrtTable[128];
static int scenario;
static void check(bool ok, int line) {
    if (!ok) { printf("failure on line %d in case %d\n",line,scenario); exit(1); }
}
#define CHECK(x) check((x),__LINE__)
static bool near(double a,double b) { return fabs(a-b) < .00008*(1.0+fabs(b)); }
static void vectorNear(const C3DVector& p,double x,double y,double z) {
    CHECK(near(p.X,x)); CHECK(near(p.Y,y)); CHECK(near(p.Z,z));
}
static void initTable() {
    for (unsigned int i=0; i<128; ++i) {
        unsigned int u=(i|0x1f80u)<<17; float x; memcpy(&x,&u,4);
        x=(float)(1.0/sqrt((double)x)); memcpy(&u,&x,4);
        GFastOneOverSqrtTable[i]=((u+0x2000)>>15)&255;
    }
    GFastOneOverSqrtTable[64]=255;
}
enum { MODE_DRAGON_STRAFE_BURNINATE=77 };
enum Event { Base, Start, Kill, Beam, Spline, ReadPosition, Move, Face, Turn,
             Delta, Legs, Alpha, Look, Remove, Action };
static Event events[48]; static int ne;
static void record(Event e) { CHECK(ne<48); events[ne++]=e; }
class CTCPhysicsBase {
public:
    C3DVector pos, facing; float angle;
    void SetPosition(const C3DVector& p) { record(Move); pos=p; }
    C3DVector GetFacingVector() const { record(Face); return facing; }
    void SetFacingAngleXY(float a) { record(Turn);angle=a; }
};
class CAnimSet {
public:
    float percentage, lastAlpha;
    float GetPercentageComplete() const { return percentage; }
    void SetInternalAlpha(long index,float a,bool immediate) {
        CHECK(index==0 && !immediate); record(Alpha);lastAlpha=a;
    }
};
class CAnimateThing {
public:
    CAnimSet main, legs; bool hasLegs;
    CAnimSet& GetMainAnim() { return main; }
    CAnimSet* GetFirstCombinationAnimFromType(long type) {
        record(Legs); CHECK(type==34);return hasLegs ? &legs : 0;
    }
};
class CTCAnimationComplex {
public:
    CAnimateThing anim;
    CAnimateThing& GetAnimation() { return anim; }
};
class CTCLook {
public:
    C3DVector target;
    void LookAtPos(const C3DVector& p,long priority) {
        CHECK(priority==10);record(Look);target=p;
    }
};
class CTCCreatureModeManager;
class CThing {
public:
    CTCPhysicsBase physics; CTCAnimationComplex animation;
    CTCLook* look; CTCCreatureModeManager* modes;
    const C3DVector& GetPos() const { record(ReadPosition);return physics.pos; }
    CTCPhysicsBase* GetPhysicsTC() { return &physics; }
    bool GetTC(CTCAnimationComplex** p) { *p=&animation;return true; }
    bool GetTC(CTCLook** p) { if (!look) return false; *p=look;return true; }
    bool GetTC(CTCCreatureModeManager** p) { *p=modes;return true; }
};
class CCreatureActionBase {};
template<class T> class CCountedPointer {
public:
    T* value; int* owners;
    CCountedPointer():value(0),owners(0) {}
    CCountedPointer(const CCountedPointer& p):value(p.value),owners(p.owners) { if(owners)++*owners; }
    ~CCountedPointer() { if(owners)--*owners; }
    T* Get() const { return value; }
};
class CThingCreatureBase : public CThing {
public:
    CCreatureActionBase* assigned;
    bool SetCurrentAction(const CCountedPointer<CCreatureActionBase>& a) {
        CHECK(a.owners && *a.owners==2);record(Action);assigned=a.Get();return true;
    }
};
template<class A,class B> B* GFDebugDynamicCast(A* a,B*) { return static_cast<B*>(a); }
class CTCCreatureModeManager {
public:
    class CModeBase {
    public:
        CThing& ParentThing; CModeBase(CThing& t):ParentThing(t) {}
        void FrameUpdate() { record(Base); }
        CThing& GetParentThing() { return ParentThing; }
    };
    void RemoveMode(unsigned long m) { CHECK(m==77);record(Remove); }
};
class CCardinalSpline {
public:
    int count;mutable float parameter;C3DVector result;
    long GetControlPointCount() const { return count; }
    void GetSplinePosition(C3DVector& p,float t) const { record(Spline);parameter=t;p=result; }
};
static float requestedAngleDelta, angleA, angleB;
float FABLE_FASTCALL GFGetSignedAngleDifference(float a,float b) {
    record(Delta);angleA=a;angleB=b;return requestedAngleDelta;
}
namespace NCreatureMode {
class CModeDragonStrafeBurninate : public CTCCreatureModeManager::CModeBase {
public:
    CModeDragonStrafeBurninate(CThing& t):CModeBase(t) {}
    float SplinePercentageComplete,LegAnimAlpha,FlameStartPercentage,FlameStopPercentage,beamTime;
    bool FlameActive; long NumAnimLoops, NumAnimLoopsToDo;
    CCountedPointer<CCreatureActionBase> PEndAction;
    CCardinalSpline FlightSpline; C3DVector PreviousPos;
    void FrameUpdate();
    void StartBeamEffect() { record(Start);FlameActive=true; }
    void KillBeamEffect() { record(Kill);FlameActive=false; }
    void UpdateBeam(float f) { record(Beam);beamTime=f; }
};
}
namespace NCreatureMode {
void CModeDragonStrafeBurninate::FrameUpdate()
{
    CTCCreatureModeManager::CModeBase::FrameUpdate();
    CThingCreatureBase& creature = *GFDebugDynamicCast(&ParentThing, (CThingCreatureBase*)0);
    CTCAnimationComplex* ptc_animation = 0;
    GetParentThing().GetTC(&ptc_animation);
    CAnimateThing& anim = ptc_animation->GetAnimation();
    float anim_percent_done = anim.GetMainAnim().GetPercentageComplete();
    float percent_of_spline_travelled = (NumAnimLoops + anim_percent_done) / NumAnimLoopsToDo;
    SplinePercentageComplete = percent_of_spline_travelled;
    if (percent_of_spline_travelled >= FlameStartPercentage && !FlameActive)
        StartBeamEffect();
    else if (percent_of_spline_travelled >= FlameStopPercentage && FlameActive)
        KillBeamEffect();
    UpdateBeam(percent_of_spline_travelled);

    percent_of_spline_travelled *= 1.0f - 1.0f / (FlightSpline.GetControlPointCount() - 1.0f);
    percent_of_spline_travelled += 1.0f / (FlightSpline.GetControlPointCount() - 1.0f);
    if (percent_of_spline_travelled > 1.0f)
        percent_of_spline_travelled = 1.0f;
    C3DVector new_pos;
    FlightSpline.GetSplinePosition(new_pos, percent_of_spline_travelled);
    PreviousPos = GetParentThing().GetPos();
    GetParentThing().GetPhysicsTC()->SetPosition(new_pos);
    C3DVector ideal_movement_direction = new_pos - PreviousPos;
    ideal_movement_direction = ideal_movement_direction.Normalise();
    C3DVector movement_direction = (ideal_movement_direction + GetParentThing().GetPhysicsTC()->GetFacingVector()) / 2.0f;
    GetParentThing().GetPhysicsTC()->SetFacingAngleXY(movement_direction.GetAngleXY());
    float angle_delta = GFGetSignedAngleDifference(movement_direction.GetAngleXY(), GetParentThing().GetPhysicsTC()->GetFacingVector().GetAngleXY());
    float roll_angle = angle_delta * 20.0f;
    if (roll_angle > 0.125f)
        roll_angle = 0.125f;
    if (roll_angle < -0.125f)
        roll_angle = -0.125f;
    CAnimSet* pleg_anim_set = anim.GetFirstCombinationAnimFromType(0x22);
    if (pleg_anim_set) {
        float desired_alpha = (float)fabs(roll_angle) / 0.125f;
        if (anim_percent_done > 0.75f && NumAnimLoops == NumAnimLoopsToDo - 1)
            desired_alpha = 0.0f;
        float new_alpha = (desired_alpha - LegAnimAlpha) * 0.1f + LegAnimAlpha;
        pleg_anim_set->SetInternalAlpha(0, new_alpha, false);
        LegAnimAlpha = new_alpha;
    }
    CTCLook* ptc_look = 0;
    if (GetParentThing().GetTC(&ptc_look)) {
        C3DVector look_pos = GetParentThing().GetPos() + GetParentThing().GetPhysicsTC()->GetFacingVector() * 50.0f;
        ptc_look->LookAtPos(look_pos, 10);
    }
    if (anim_percent_done > 0.999f) {
        ++NumAnimLoops;
        if (NumAnimLoops >= NumAnimLoopsToDo) {
            CCountedPointer<CCreatureActionBase> p_end_action = PEndAction;
            CTCCreatureModeManager* ptc_mode_manager;
            GetParentThing().GetTC(&ptc_mode_manager);
            ptc_mode_manager->RemoveMode(MODE_DRAGON_STRAFE_BURNINATE);
            if (p_end_action.Get())
                creature.SetCurrentAction(p_end_action);
        }
    }
}
}
static double heading(double x,double y) {
    double t=atan2(x,y)/(2.0*acos(-1.0));return t<0?t+1.0:t;
}
static void run(float progress,int loop,int flags,int turn,int count,int quadrant) {
    ++scenario;ne=0;
    CThingCreatureBase creature;CTCLook look;CTCCreatureModeManager modes;CCreatureActionBase action;
    int owners=1;NCreatureMode::CModeDragonStrafeBurninate m(creature);
    creature.physics.pos=C3DVector(4,-8,16);
    creature.physics.facing=quadrant ? C3DVector(-.5f,-.5f,.125f) : C3DVector(.25f,.75f,-.5f);
    creature.look=(flags&2)?&look:0;creature.modes=&modes;creature.assigned=0;
    CAnimateThing& anim=creature.animation.anim;
    anim.main.percentage=progress;anim.hasLegs=(flags&1)!=0;anim.legs.lastAlpha=-4;
    m.FlameActive=(flags&4)!=0;m.FlameStartPercentage=.25f;m.FlameStopPercentage=.75f;
    if(flags&8) {m.PEndAction.value=&action;m.PEndAction.owners=&owners;}
    m.PreviousPos=C3DVector(99,99,99);m.LegAnimAlpha=.375f;m.NumAnimLoops=loop;m.NumAnimLoopsToDo=2;
    m.FlightSpline.count=count;m.FlightSpline.result=C3DVector(7,-4,28);
    const float deltas[]={-.0625f,-.00625f,-.003125f,0,.003125f,.00625f,.0625f};
    requestedAngleDelta=deltas[turn];
    m.FrameUpdate();

    const double raw=(loop+(double)progress)/2;
    const double off=1.0/(count-1);
    double adjusted=raw*(1.0-off)+off;if(adjusted>1)adjusted=1;
    CHECK(near(m.SplinePercentageComplete,raw) && near(m.beamTime,raw));
    CHECK(near(m.FlightSpline.parameter,adjusted));
    vectorNear(m.PreviousPos,4,-8,16);vectorNear(creature.physics.pos,7,-4,28);
    double fx=creature.physics.facing.X, fy=creature.physics.facing.Y;
    double expectedHeading=heading(3.0/13+fx,4.0/13+fy);
    CHECK(near(creature.physics.angle,expectedHeading) && near(angleA,expectedHeading) && near(angleB,heading(fx,fy)));
    double roll=deltas[turn]*20; if(roll<-.125)roll=-.125;if(roll>.125)roll=.125;
    double target=(progress>.75f && loop==1)?0:fabs(roll)*8;
    double alpha=(flags&1)? .375+(target-.375)*.1:.375;
    CHECK(near(m.LegAnimAlpha,alpha));CHECK(near(anim.legs.lastAlpha, (flags&1)?alpha:-4));
    if(flags&2)vectorNear(look.target,7+50*fx,-4+50*fy,28+50*creature.physics.facing.Z);
    bool finish=progress>.999f;bool remove=finish && loop+1>=2;
    CHECK(m.NumAnimLoops==loop+(finish?1:0));CHECK(owners==1);
    CHECK(creature.assigned==((remove&&(flags&8))?&action:0));
    int n=0;
    CHECK(events[n++]==Base);
    if(raw>=.25 && !(flags&4)) {CHECK(events[n++]==Start); CHECK(m.FlameActive);}
    else if(raw>=.75 && (flags&4)) {CHECK(events[n++]==Kill);CHECK(!m.FlameActive);}
    else CHECK(m.FlameActive==bool(flags&4));
    CHECK(events[n++]==Beam);CHECK(events[n++]==Spline);CHECK(events[n++]==ReadPosition);
    CHECK(events[n++]==Move);CHECK(events[n++]==Face);CHECK(events[n++]==Turn);
    CHECK(events[n++]==Face);CHECK(events[n++]==Delta);CHECK(events[n++]==Legs);
    if(flags&1)CHECK(events[n++]==Alpha);
    if(flags&2) {
        CHECK((events[n]==Face&&events[n+1]==ReadPosition)||(events[n]==ReadPosition&&events[n+1]==Face));n+=2;
        CHECK(events[n++]==Look);
    }
    if(remove)CHECK(events[n++]==Remove);
    if(remove&&(flags&8))CHECK(events[n++]==Action);
    CHECK(n==ne);
}
int main() {
    initTable();
    const float progress[]={0,.5f,.75f,.875f,.999f,1.0f};
    for(int p=0;p<6;++p)for(int loop=0;loop<3;++loop)for(int flags=0;flags<16;++flags)
        for(int turn=0;turn<7;++turn)for(int count=2;count<=4;count+=2)for(int q=0;q<2;++q)
            run(progress[p],loop,flags,turn,count,q);
    puts("008de080_TEST PASS");return 0;
}