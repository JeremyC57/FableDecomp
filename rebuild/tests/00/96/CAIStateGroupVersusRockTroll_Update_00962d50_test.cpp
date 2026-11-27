#include <stdio.h>
#include <stdlib.h>
#include <math.h>
enum EAIActionReturn {AI_ACTION_FINISHED=2,AI_ACTION_IN_PROGRESS=4};
enum EAIActionRepeat {ACTION_REPEAT_UNTIL_FINISHED=1,ACTION_REPEAT_CALLBACK=2};
enum EMovementType {AI_MOVEMENT_RUNNING=5,AI_MOVEMENT_WALKING=3};
namespace NCreatureMode { enum { MODE_STRAFE=25 }; }
struct C3DVector {float x,y,z;C3DVector(float a=0,float b=0,float c=0):x(a),y(b),z(c){}
    C3DVector operator+(const C3DVector& p)const{return C3DVector(x+p.x,y+p.y,z+p.z);}
    C3DVector operator-(const C3DVector& p)const{return C3DVector(x-p.x,y-p.y,z-p.z);}
    C3DVector operator*(float f)const{return C3DVector(x*f,y*f,z*f);}
    void operator+=(const C3DVector& p){*this=*this+p;}
    float Dot(const C3DVector& p)const{return x*p.x+y*p.y+z*p.z;}
    float GetMagnitude2D()const{return (float)sqrt(x*x+y*y);}
    C3DVector& Normalise(){float l=(float)sqrt(Dot(*this));if(l!=0)*this=*this*(1.0f/l);return *this;}
    void RotateXY(float angle){float t=x;if(angle==0.25f){x=-y;y=t;}else if(angle==-0.25f){x=y;y=-t;}else if(angle==-0.5f){x=-x;y=-y;}else abort();}
};
struct Physics{float radius;C3DVector facing;float GetRadius()const{return radius;}C3DVector GetFacingVector()const{return facing;}};
struct CThing {bool alive;Physics physics;C3DVector pos;const C3DVector& GetPos()const{return pos;}Physics* GetPhysicsTC(){return &physics;}};
template<class T>struct CIntelligentPointer{T* p;CIntelligentPointer(T* q=0):p(q){}T* operator->()const{return p;}T& operator*()const{return *p;}};
template<class T>bool IS_THING_ALIVE(const CIntelligentPointer<T>& p){return p.p&&p->alive;}
struct CTCCreatureModeManager {bool active;int adds,removes;bool IsModeActive(unsigned m)const{if(m!=25)abort();return active;}void AddMode(unsigned m){if(m!=25||active)abort();active=true;++adds;}void RemoveMode(unsigned m){if(m!=25||!active)abort();active=false;++removes;}};
struct CTCCreatureNavigation{bool available;int checks;bool IsAbleToNavigateFromPosToPos(const C3DVector& start,const C3DVector&,float r){if(start.x!=1||start.y!=0||r!=2)abort();++checks;return available;}};
struct CThingCreatureBase:CThing{bool strafe;CTCCreatureModeManager mode;CTCCreatureNavigation nav;bool IsAbleToStrafe()const{return strafe;}void GetTC(CTCCreatureModeManager** p){*p=&mode;}void GetTC(CTCCreatureNavigation** p){*p=&nav;}};
struct CCreatureAction_InterruptableMidAttack {CThingCreatureBase* c;CThing* t;CCreatureAction_InterruptableMidAttack(CThingCreatureBase& cc,CThing& tt):c(&cc),t(&tt){}};
struct CBrainOutput{CThing* look;long flags;void SetAsLookAtThing(CThing& t,long f){look=&t;flags=f;}};
struct CAIBrain{CBrainOutput out;int writes;const CBrainOutput& GetOutput()const{return out;}void SetOutput(const CBrainOutput& o){out=o;++writes;}};
struct World{unsigned long seed;unsigned long* GetWorldSeed(){return &seed;}};
static int random_count;static unsigned rolls[2];static unsigned long* required_seed;
unsigned GFRandom(unsigned n,unsigned long* seed){if(seed!=required_seed||n!=100||random_count>=2)abort();return rolls[random_count++];}
struct CAIStateGroup_VersusRockTroll{
    CThingCreatureBase me;CIntelligentPointer<CThingCreatureBase> PTargetThing;CIntelligentPointer<CThing> PArenaCentreThing;
    CAIBrain* PBrain;World world;bool GoClockwise,started,stop_after,charge_first,charge_later;int charge_checks,steps,walks,hits,back,moves,ends;
    C3DVector last;bool navflag;
    CThingCreatureBase& GetCreature(){return me;} World& GetWorld(){return world;}
    bool IsStillRunnable(){return started&&(!stop_after||steps==0);}bool IsScorpionChargingAtMe(){++charge_checks;return charge_checks==1?charge_first:charge_later;}
    void Terminate(){++ends;}
    void GetChargeDodgePos(C3DVector&);void Update();
    EAIActionReturn AIAction_MoveToThingProximity(EAIActionRepeat rep,CIntelligentPointer<CThingCreatureBase> p,float dist,bool&,EMovementType mt,bool a,bool b,bool c,bool e,float f){
        if(rep!=1||mt!=5||!a||!b||!c||e||f!=0||p.p!=PTargetThing.p||dist!=5.5f)abort();++steps;++walks;return AI_ACTION_FINISHED;}
    EAIActionReturn AIAction_PerformActionAndWait(const CCreatureAction_InterruptableMidAttack& action,bool wait){if(wait||action.c!=&me||action.t!=PTargetThing.p)abort();++steps;++hits;return AI_ACTION_FINISHED;}
    EAIActionReturn AIAction_MoveToPosProximity(EAIActionRepeat rep,const C3DVector& p,float d,bool&,EMovementType mt,bool a,bool b,bool c,CThing* target,bool e,float f){
        if(rep!=1||p.x!=1||p.y!=0||d!=2||mt!=5||!a||!b||!c||target||e||f!=0)abort();++steps;++back;return AI_ACTION_FINISHED;}
    EAIActionReturn AIAction_MoveToPos(EAIActionRepeat rep,const C3DVector& p,bool&,EMovementType mt,bool a,bool b,CThing* t,bool e,float f){
        if(rep!=2||mt!=3||!b||t||e||f!=0)abort();last=p;navflag=a;++steps;++moves;return moves==1?AI_ACTION_IN_PROGRESS:AI_ACTION_FINISHED;}
};
namespace {
const float ROCK_TROLL_SAFE_DISTANCE = 10.5f;
const float MOVE_SEGMENT_DISTANCE = 2.0f;
}
void CAIStateGroup_VersusRockTroll::Update()
{
    EAIActionReturn ret=AI_ACTION_FINISHED;
    if (IsStillRunnable()) {
        CTCCreatureModeManager* ptc_cmm;
        GetCreature().GetTC(&ptc_cmm);
        C3DVector move_to_pos;
        C3DVector vector_to_target=PTargetThing->GetPos()-GetCreature().GetPos();
        C3DVector dir_to_target(vector_to_target);
        dir_to_target.Normalise();
        if (vector_to_target.GetMagnitude2D()>0.8f && GFRandom(100,GetWorld().GetWorldSeed())<30) {
            if (ptc_cmm->IsModeActive(NCreatureMode::MODE_STRAFE)) ptc_cmm->RemoveMode(NCreatureMode::MODE_STRAFE);
            const float attack_dis=PTargetThing->GetPhysicsTC()->GetRadius()+3.5f;
            const C3DVector start_pos=GetCreature().GetPos();
            bool nav;
            ret=AIAction_MoveToThingProximity(ACTION_REPEAT_UNTIL_FINISHED,PTargetThing,attack_dis,nav,AI_MOVEMENT_RUNNING,true,true,true,false,0.0f);
            if (!IsStillRunnable()) {
                Terminate();
                return;
            }
            vector_to_target=PTargetThing->GetPos()-GetCreature().GetPos();
            if (vector_to_target.GetMagnitude2D()<attack_dis+0.2f)
                ret=AIAction_PerformActionAndWait(CCreatureAction_InterruptableMidAttack(GetCreature(),*PTargetThing),false);
            if (!IsStillRunnable()) {
                Terminate();
                return;
            }
            ret=AIAction_MoveToPosProximity(ACTION_REPEAT_UNTIL_FINISHED,start_pos,2.0f,nav,AI_MOVEMENT_RUNNING,true,true,true,0,false,0.0f);
            if (!IsStillRunnable()) {
                Terminate();
                return;
            }
        } else {
            if (GetCreature().IsAbleToStrafe() && !ptc_cmm->IsModeActive(NCreatureMode::MODE_STRAFE)) ptc_cmm->AddMode(NCreatureMode::MODE_STRAFE);
            C3DVector dir_to_move(dir_to_target);
            if (GoClockwise) dir_to_move.RotateXY(0.25f); else dir_to_move.RotateXY(-0.25f);
            move_to_pos=PTargetThing->GetPos()-dir_to_target*ROCK_TROLL_SAFE_DISTANCE+dir_to_move*MOVE_SEGMENT_DISTANCE;
            CTCCreatureNavigation* ptc_nav;
            GetCreature().GetTC(&ptc_nav);
            if (!ptc_nav->IsAbleToNavigateFromPosToPos(GetCreature().GetPos(),move_to_pos,2.0f)) {
                dir_to_move.RotateXY(-0.5f);
                move_to_pos=PTargetThing->GetPos()-dir_to_target*ROCK_TROLL_SAFE_DISTANCE+dir_to_move*MOVE_SEGMENT_DISTANCE;
            }
            do {
                bool nav;
                ret=AIAction_MoveToPos(ACTION_REPEAT_CALLBACK,move_to_pos,nav,AI_MOVEMENT_WALKING,!GetCreature().IsAbleToStrafe(),true,0,false,0.0f);
                if (GetCreature().IsAbleToStrafe() && IsStillRunnable()) {
                    CBrainOutput output(PBrain->GetOutput());
                    output.SetAsLookAtThing(*PTargetThing,0xa7);
                    PBrain->SetOutput(output);
                }
            } while(ret==AI_ACTION_IN_PROGRESS && IsStillRunnable());
        }
    }
    if (!IsStillRunnable()) Terminate();
}

int main(){
 for(int flags=0;flags<512;++flags)for(int roll=0;roll<3;++roll)for(int distance=0;distance<3;++distance){
    bool active=(flags&1)!=0,clockwise=(flags&2)!=0,strafe=(flags&4)!=0,stop=(flags&8)!=0,mode=(flags&16)!=0,navigable=(flags&32)!=0;
    float angle_y=(flags&64)?-1.0f:1.0f;float axis=(flags&128)?-1.0f:1.0f;bool along_y=(flags&256)!=0;
    const float dx=distance==0?0.5f:distance==1?2.0f:12.0f;
    CThingCreatureBase target;target.alive=true;target.pos=C3DVector(1+(along_y?0:axis*dx),along_y?angle_y*dx:0);target.physics.radius=2;
    CAIStateGroup_VersusRockTroll w;CAIBrain brain;w.me.pos=C3DVector(1,0);w.me.strafe=strafe;w.me.mode.active=mode;w.me.mode.adds=w.me.mode.removes=0;
    w.me.nav.available=navigable;w.me.nav.checks=0;w.GoClockwise=clockwise;w.PBrain=&brain;brain.writes=0;brain.out.look=0;brain.out.flags=0;w.PTargetThing=&target;
    w.started=active;w.stop_after=stop;w.steps=w.walks=w.hits=w.back=w.moves=w.ends=0;w.charge_checks=0;
    rolls[0]=roll==0?29:roll==1?30:99;random_count=0;required_seed=&w.world.seed;
    w.Update();
    bool approach=active&&distance!=0&&roll==0,circle=active&&!approach;
    int executions=circle?(stop?1:2):0;int outputs=circle&&strafe&&!stop?2:0;
    float factor=(clockwise?2.0f:-2.0f)*(navigable?1.0f:-1.0f);
    float expected_x=target.pos.x-(along_y?0:axis)*10.5f-(along_y?angle_y:0)*factor;
    float expected_y=target.pos.y-(along_y?angle_y:0)*10.5f+(along_y?0:axis)*factor;
    if(w.ends!=(!active||(stop&&w.steps!=0))||w.me.nav.checks!=circle||w.walks!=approach||w.hits!=(approach&&!stop&&distance==1)||w.back!=(approach&&!stop)||w.moves!=executions||brain.writes!=outputs)return 1;
    if(random_count!=(active&&distance!=0)||w.me.mode.removes!=(approach&&mode)||w.me.mode.adds!=(circle&&strafe&&!mode))return 2;
    if(executions&&(w.last.x!=expected_x||w.last.y!=expected_y||w.navflag!=!strafe))return 3;
    if(outputs&&(brain.out.look!=&target||brain.out.flags!=0xa7))return 4;
 }
 puts("00962d50_TEST PASS");return 0;
}