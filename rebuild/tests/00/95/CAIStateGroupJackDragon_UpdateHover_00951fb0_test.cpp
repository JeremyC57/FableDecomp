#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_UPDATE_NEAREST_INTERESTING
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_GFFASTONEOVERSQRT_PORTABLE_BITS_PASS26
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D
#include "engine/C3DVector.h"
unsigned long GFastOneOverSqrtTable[128];
const float DELTA_NOTIONAL_ZERO=0.0001f;
static int turnSign=1;
void C3DVector::RotateXY(float angle){
    if(angle==0.0f)return;
    if(angle!=-0.25f)exit(10);
    float x=X;
    X=Y+x*(float)turnSign/16777216.0f;
    Y=-x;
}
namespace HoverFixture {
void require(bool b,int line){if(!b){fprintf(stderr,"fixture check failed at %d\n",line);exit(1);}}
#define check(b) require((b),__LINE__)
bool same(const C3DVector& a,const C3DVector& b){return a.X==b.X && a.Y==b.Y && a.Z==b.Z;}
template<class T>struct Ptr{T* p;T* Get()const{return p;}T* operator->()const{return p;}T& operator*()const{return *p;}};
template<class T>struct CDefPointer: public Ptr<T>{};
template<class T>struct CCountedPointer: public Ptr<T>{explicit CCountedPointer(T* q){this->p=q;}};
struct CJackDragonDef{long MaxHoverLoops;float MaxFirebeamDistance,HoverObstructionRadius;};
struct CRightHandedSet{};
struct Physics{C3DVector face;const C3DVector& GetFacingVector(){return face;}float GetFacingAngleXY(){return 0.0f;}};
struct CTCDPhysicalObstruction;
struct CThing {
    C3DVector pos; Physics phy; bool killed; CTCDPhysicalObstruction* obstruction;
    CThing():pos(0,0,0),killed(false),obstruction(0){phy.face=C3DVector(1,0,0);}
    const C3DVector& GetPos(){return pos;} Physics* GetPhysicsTC(){return &phy;}
    void GetTCDriver(CTCDPhysicalObstruction** out){*out=obstruction;}
    void Kill(bool value){check(value);killed=true;}
};
struct CCreatureActionBase{};
struct CTCCreatureModeManager{
    struct CModeBase{virtual ~CModeBase(){}};
    unsigned add,remove;
    CTCCreatureModeManager():add(0),remove(0){}
    void AddMode(CCountedPointer<CModeBase> p){check(p.Get()!=0);++add;delete p.Get();}
    void RemoveMode(long i){check(i==71);++remove;}
};
namespace NCreatureMode{
struct CModeDragonHover:public CTCCreatureModeManager::CModeBase{CModeDragonHover(CThing&,CThing&){}};
}
struct CDefClassBase{
    CJackDragonDef config;void GetSubDef(CDefPointer<const CJackDragonDef>* p){p->p=&config;}
};
struct Graphic{
    void GetDummyObject(const char* n,int x,int y,C3DVector& pos,CRightHandedSet&,float f){
        check(!strcmp(n,"DRAGON_MOUTH_DUMMY") && x==0 && y==0 && f==1.0f);pos=C3DVector(20,30,40);
    }
};
struct Creature:public CThing{
    CTCCreatureModeManager mode; CDefClassBase def; Graphic graphic;
    CDefClassBase& PeekBaseDef(){return def;}void GetTC(CTCCreatureModeManager** p){*p=&mode;}
    Graphic* GetTCGraphicAppearance(){return &graphic;}
};
struct CTCDPhysicalObstruction{
    static CThing* supplied; static int creates;float lifetime;CThing* ignore;
    CTCDPhysicalObstruction():lifetime(0),ignore(0){}
    static CThing* Create(const C3DVector& p,const C3DVector& mouth,float radius){
        check(same(p,C3DVector(0,0,2))&&same(mouth,C3DVector(20,30,2))&&radius==4);++creates;return supplied;
    }
    void SetLifeTimeSeconds(float x){lifetime=x;}void SetThingToIgnoreObstruction(CThing* x){ignore=x;}
};
CThing* CTCDPhysicalObstruction::supplied=0;int CTCDPhysicalObstruction::creates=0;
struct Action{int which;explicit Action(int n):which(n){}};
struct CCreatureAction_DragonFireBeamInto:public Action{CCreatureAction_DragonFireBeamInto(Creature&,CThing&,float t):Action(1){check(t==5);}};
struct CCreatureAction_DragonFireBlastForward:public Action{CCreatureAction_DragonFireBlastForward(Creature&,CThing&):Action(2){}};
struct CCreatureAction_DragonFireBlastLeft:public Action{CCreatureAction_DragonFireBlastLeft(Creature&,CThing&):Action(3){}};
struct CCreatureAction_DragonHoverOutOf:public Action{explicit CCreatureAction_DragonHoverOutOf(Creature&):Action(4){}};
struct CAIStateGroup_JackDragon{
    enum{DRAGON_STATE_STRAFE=4};
    Creature dragon;CThing enemy,m1,m2;Ptr<CThing>PEnemy,PHoverMarker1,PHoverMarker2;
    C3DVector DragonPosOutOfGround3Left,DragonPosOutOfGround3Right,DragonPosOutOfGround4Left,DragonPosOutOfGround4Right,PreviousFlightPos;
    int CurrentState,stop,checks,towerPolls,stopState;bool GoLeft,tower,terminated;
    std::vector<int> events; std::vector<C3DVector> route;C3DVector takeoff;
    CAIStateGroup_JackDragon():DragonPosOutOfGround3Left(1,1,1),DragonPosOutOfGround3Right(2,2,2),
       DragonPosOutOfGround4Left(3,3,3),DragonPosOutOfGround4Right(4,4,4),PreviousFlightPos(0,0,0),
       CurrentState(-1),stop(0),checks(0),towerPolls(0),stopState(0),GoLeft(false),tower(false),terminated(false){
        PEnemy.p=&enemy;PHoverMarker1.p=&m1;PHoverMarker2.p=&m2;dragon.pos=C3DVector(0,0,2);enemy.pos=C3DVector(8,0,2);
        m1.pos=C3DVector(-4,2,3);m2.pos=C3DVector(4,-2,3);m1.phy.face=C3DVector(0,1,0);m2.phy.face=C3DVector(0,-1,0);
    }
    Creature& GetCreature(){return dragon;}
    void StayIdleAndTurnToFace(float delay,const C3DVector& f){check(delay==1.5f && same(f,(GoLeft?m1:m2).phy.face));events.push_back(10);}
    bool IsStillRunnable(){++checks;return checks!=stop;}
    bool IsTargetOnATower(){++towerPolls;return tower;}
    void Terminate(){check(!terminated);terminated=true;events.push_back(11);}
    void SetHeadTurning(bool on){events.push_back(on?20:21);}
    void WaitForAnimLoop(float delay){check(delay==0);events.push_back(22);}
    void StayIdle(float delay){check(delay==2);events.push_back(23);}
    void AIAction_PerformActionAndWait(const Action& a,bool f){check(!f);events.push_back(a.which);}
    void StartFlying(Creature& who,const std::vector<C3DVector>& p,const C3DVector& end,CCountedPointer<CCreatureActionBase> final){
        check(&who==&dragon && !final.Get());route=p;takeoff=end;events.push_back(24);
    }
    int ChooseIntoGroundState(){events.push_back(25);return 5;}
    void UpdateHover();
};
void CAIStateGroup_JackDragon::UpdateHover()
{
    CTCCreatureModeManager* ptc_cmm;
    CDefPointer<const CJackDragonDef> p_jack_dragon_def;
    GetCreature().PeekBaseDef().GetSubDef(&p_jack_dragon_def);
    GetCreature().GetTC(&ptc_cmm);
    ptc_cmm->AddMode(CCountedPointer<CTCCreatureModeManager::CModeBase>(new NCreatureMode::CModeDragonHover(GetCreature(), *PEnemy)));
    CThing* pmarker;
    if (GoLeft)
        pmarker = PHoverMarker1.Get();
    else
        pmarker = PHoverMarker2.Get();
    StayIdleAndTurnToFace(1.5f, pmarker->GetPhysicsTC()->GetFacingVector());
    if (!IsStillRunnable()) {
        Terminate();
        return;
    }
    C3DVector mouth_pos;
    CRightHandedSet mouth_rhs;
    {
        GetCreature().GetTCGraphicAppearance()->GetDummyObject("DRAGON_MOUTH_DUMMY", 0, 0, mouth_pos, mouth_rhs, 1.0f);
    }
    mouth_pos.Z = GetCreature().GetPos().Z;
    long num_loops = p_jack_dragon_def->MaxHoverLoops;
    long loop;
    for (loop = 0; loop < num_loops; ++loop) {
        SetHeadTurning(true);
        C3DVector vector_to_target = PEnemy->GetPos() - GetCreature().GetPos();
        float dis_to_target = vector_to_target.GetMagnitude2D();
        if (IsTargetOnATower())
            break;
        if (dis_to_target < p_jack_dragon_def->MaxFirebeamDistance) {
            SetHeadTurning(false);
            WaitForAnimLoop(0.0f);
            if (!IsStillRunnable()) {
                Terminate();
                return;
            }
            CThing* p_obstruction_thing = CTCDPhysicalObstruction::Create(GetCreature().GetPos(), mouth_pos, p_jack_dragon_def->HoverObstructionRadius);
            if (p_obstruction_thing) {
                CTCDPhysicalObstruction* ptcd_obstruction = 0;
                p_obstruction_thing->GetTCDriver(&ptcd_obstruction);
                ptcd_obstruction->SetLifeTimeSeconds(60.0f);
                ptcd_obstruction->SetThingToIgnoreObstruction(&GetCreature());
            }
            AIAction_PerformActionAndWait(CCreatureAction_DragonFireBeamInto(GetCreature(), *PEnemy, 5.0f), false);
            // Inline-budget: the native test of this action result calls IsStillRunnable.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
            if (!IsStillRunnable())
#ifdef _MSC_VER
#pragma inline_depth()
#endif
            {
                Terminate();
                return;
            }
            if (p_obstruction_thing)
                p_obstruction_thing->Kill(true);
            SetHeadTurning(true);
            StayIdle(2.0f);
        } else {
            C3DVector dir_to_target = vector_to_target;
            dir_to_target.Z = 0.0f;
            dir_to_target = dir_to_target.Normalise();
            C3DVector dir_left = dir_to_target;
            dir_left.RotateXY(-0.25f);
            if (GoLeft || dir_to_target.Dot(GetCreature().GetPhysicsTC()->GetFacingVector()) > 0.866f)
                AIAction_PerformActionAndWait(CCreatureAction_DragonFireBlastForward(GetCreature(), *PEnemy), false);
            else if (dir_to_target.Dot(dir_left) > 0.0f)
                AIAction_PerformActionAndWait(CCreatureAction_DragonFireBlastLeft(GetCreature(), *PEnemy), false);
            else
                AIAction_PerformActionAndWait(CCreatureAction_DragonFireBlastForward(GetCreature(), *PEnemy), false);
            if (!IsStillRunnable()) {
                Terminate();
                return;
            }
            StayIdle(2.0f);
        }
        // Inline-budget: the native test of this action result calls IsStillRunnable.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
        if (!IsStillRunnable())
#ifdef _MSC_VER
#pragma inline_depth()
#endif
        {
            Terminate();
            return;
        }
    }
    SetHeadTurning(false);
    WaitForAnimLoop(0.0f);
    if (!IsStillRunnable()) {
        Terminate();
        return;
    }
    ptc_cmm->RemoveMode(71);
    AIAction_PerformActionAndWait(CCreatureAction_DragonHoverOutOf(GetCreature()), false);
    if (!IsStillRunnable()) {
        Terminate();
        return;
    }
    C3DVector target_pos_offset(0.0f, 100.0f, 50.0f);
    target_pos_offset.RotateXY(GetCreature().GetPhysicsTC()->GetFacingAngleXY());
    C3DVector target_pos = GetCreature().GetPos() + target_pos_offset;
    float fly_speed = 30.0f;
    std::vector<C3DVector> movement_points;
    movement_points.push_back(GetCreature().GetPos());
    C3DVector fly_out_pos = GetCreature().GetPos() + GetCreature().GetPhysicsTC()->GetFacingVector()*30.0f + C3DVector(0.0f,0.0f,15.0f);
    if (GoLeft) {
// Inline-budget: the six original exit-path points call the STLport append; the first point expands it.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
        movement_points.push_back(DragonPosOutOfGround3Right);
#ifdef _MSC_VER
#pragma inline_depth()
#endif
// Inline-budget: the six original exit-path points call the STLport append; the first point expands it.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
        movement_points.push_back(DragonPosOutOfGround3Left);
#ifdef _MSC_VER
#pragma inline_depth()
#endif
// Inline-budget: the six original exit-path points call the STLport append; the first point expands it.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
        movement_points.push_back(DragonPosOutOfGround4Left);
#ifdef _MSC_VER
#pragma inline_depth()
#endif
        PreviousFlightPos = DragonPosOutOfGround3Left;
    } else {
// Inline-budget: the six original exit-path points call the STLport append; the first point expands it.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
        movement_points.push_back(DragonPosOutOfGround3Left);
#ifdef _MSC_VER
#pragma inline_depth()
#endif
// Inline-budget: the six original exit-path points call the STLport append; the first point expands it.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
        movement_points.push_back(DragonPosOutOfGround3Right);
#ifdef _MSC_VER
#pragma inline_depth()
#endif
// Inline-budget: the six original exit-path points call the STLport append; the first point expands it.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
        movement_points.push_back(DragonPosOutOfGround4Right);
#ifdef _MSC_VER
#pragma inline_depth()
#endif
        PreviousFlightPos = DragonPosOutOfGround3Right;
    }
    C3DVector take_off_pos = pmarker->GetPos();
    StartFlying(GetCreature(), movement_points, take_off_pos, CCountedPointer<CCreatureActionBase>(0));
    if (!IsStillRunnable()) {
        Terminate();
        return;
    }
// Inline-budget: the exit at 0095284B calls the original tower predicate.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
    if (IsTargetOnATower())
#ifdef _MSC_VER
#pragma inline_depth()
#endif
        CurrentState = DRAGON_STATE_STRAFE;
    else
        CurrentState = ChooseIntoGroundState();
}

int countEvent(const std::vector<int>& ev,int k){int n=0;for(unsigned i=0;i<ev.size();++i)if(ev[i]==k)++n;return n;}
int exercise(int loops,bool left,bool tower,bool beam,bool obstruction,bool facing,int sign,int stop,const std::vector<int>& complete,std::vector<int>& actual){
    CAIStateGroup_JackDragon s;CThing ob;CTCDPhysicalObstruction driver;ob.obstruction=&driver;
    s.GoLeft=left;s.stop=stop;s.tower=tower;turnSign=sign;
    s.dragon.phy.face=C3DVector(facing?1.0f:-1.0f,0,0);
    s.dragon.def.config.MaxHoverLoops=loops;s.dragon.def.config.MaxFirebeamDistance=beam?256.0f:-1.0f;s.dragon.def.config.HoverObstructionRadius=4.0f;
    CTCDPhysicalObstruction::creates=0;CTCDPhysicalObstruction::supplied=obstruction?&ob:0;
    s.UpdateHover();actual=s.events;check(s.dragon.mode.add==1);
    if(stop){check(s.terminated && s.CurrentState==-1 && s.checks==stop && !actual.empty() && actual.back()==11);
        check(actual.size()<=complete.size()+1);for(unsigned i=0;i+1<actual.size();++i)check(actual[i]==complete[i]);return s.checks;}
    int active=tower||loops<0?0:loops;check(!s.terminated && s.dragon.mode.remove==1 && s.route.size()==4);
    check(s.CurrentState==(tower?4:5));check(countEvent(actual,4)==1 && countEvent(actual,24)==1 && countEvent(actual,25)==(!tower));
    check(CTCDPhysicalObstruction::creates==(beam?active:0) && ob.killed==(beam&&active&&obstruction));
    if(ob.killed)check(driver.lifetime==60 && driver.ignore==&s.dragon);
    check(countEvent(actual,1)==(beam?active:0));check(countEvent(actual,23)==active);
    int blast=left||facing||sign<0?2:3;check(countEvent(actual,2)==(!beam&&blast==2?active:0));check(countEvent(actual,3)==(!beam&&blast==3?active:0));
    check(same(s.takeoff,(left?s.m1:s.m2).pos) && same(s.route[0],s.dragon.pos));
    check(same(s.route[1],left?s.DragonPosOutOfGround3Right:s.DragonPosOutOfGround3Left));
    check(same(s.route[2],left?s.DragonPosOutOfGround3Left:s.DragonPosOutOfGround3Right) && same(s.route[2],s.PreviousFlightPos));
    check(same(s.route[3],left?s.DragonPosOutOfGround4Left:s.DragonPosOutOfGround4Right));
    return s.checks;
}
void cases(){
    // All tested horizontal displacements have squared length 64. A near-2 mantissa
    // gives this input a reciprocal-square-root seed just below 1/8.
    GFastOneOverSqrtTable[64]=255;
    for(int loops=-1;loops<=2;++loops)for(int bits=0;bits<64;++bits){
    std::vector<int> expected,dummy,observed;
    int n=exercise(loops,(bits&1)!=0,(bits&2)!=0,(bits&4)!=0,(bits&8)!=0,(bits&16)!=0,(bits&32)?1:-1,0,dummy,expected);
    for(int i=1;i<=n;++i)exercise(loops,(bits&1)!=0,(bits&2)!=0,(bits&4)!=0,(bits&8)!=0,(bits&16)!=0,(bits&32)?1:-1,i,expected,observed);
}}
}
int main(){HoverFixture::cases();puts("00951fb0_TEST PASS");return 0;}