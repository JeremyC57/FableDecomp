#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static int testPattern,testLane,testOption;
#define ck(q) do { if (!(q)) { printf("line %d in case %d %d %d\n",__LINE__,testPattern,testLane,testOption); exit(2); } } while(0)
static int logEvents[100],used;
static void ev(int n){ck(used<100);logEvents[used++]=n;}
#define FABLE_RETAIL_ASSERT(e) do {if(!(e)){ev(31);return;}}while(0)
enum EAIActionReturn{AI_ACTION_NULL,AI_ACTION_TERMINATE_PROCESS,AI_ACTION_FINISHED,AI_ACTION_FAILED,AI_ACTION_IN_PROGRESS};
enum EAIActionRepeat{ACTION_REPEAT_UNTIL_FINISHED=1,ACTION_REPEAT_CALLBACK=2};
enum EMovementType{AI_MOVEMENT_RUNNING=5};
struct C3DVector{
 float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}
 C3DVector operator+(const C3DVector&r)const{return C3DVector(X+r.X,Y+r.Y,Z+r.Z);}
 C3DVector operator-(const C3DVector&r)const{return C3DVector(X-r.X,Y-r.Y,Z-r.Z);}
};
static bool at(const C3DVector&v,float x,float y,float z){return v.X==x&&v.Y==y&&v.Z==z;}
struct CRightHandedSet{C3DVector f;const C3DVector& GetForward()const{return f;}};
struct CThing;static int assertion,aliveAt,aliveCalls,carriedAt,carriedCalls;
static int aStatus,bStatus,lStatus,doWait,mode,firstRepeats,secondRepeats,followSteps,idleSteps;
static bool available,picked,doFollow; static CThing *me,*item,*leader,*saved;
struct CTCSingleUserActionPoints{bool GetFreeActionPoint(CThing& c,C3DVector& p,CRightHandedSet& r,long&i){ck(&c==me);ev(2);p=C3DVector(1,2,3);r.f=C3DVector(0,1,0);i=27;return available;}};
struct CTCCarryable{bool IsCarried()const{ev(4);return ++carriedCalls==carriedAt;}};
struct CTCCarrying{CThing* GetThingInPrimarySlot()const{ev(10);return saved;}CThing* RemoveThingFromPrimarySlot(bool b){ck(!b);ev(11);return saved;}};
static CTCSingleUserActionPoints points;static CTCCarryable carryable;static CTCCarrying carrying;
struct CThing {
 C3DVector p;
 bool GetTC(CTCSingleUserActionPoints**v){ck(this==item);ev(1);*v=&points;return assertion!=1;}
 bool GetTC(CTCCarryable**v){ck(this==item);ev(3);*v=&carryable;return assertion!=2;}
 bool GetTC(CTCCarrying**v){ck(this==me);ev(9);*v=&carrying;return assertion!=3;}
 C3DVector GetPos()const{return p;}
};
struct Ptr{
 CThing* p;Ptr(CThing*v=0):p(v){}CThing* operator->()const{return p;}
 CThing& operator*()const{return *p;}CThing* Get()const{return p;}
};
static bool IS_THING_ALIVE(const Ptr& p){if(p.p==item){ev(20);++aliveCalls;return aliveAt<0 || aliveCalls<aliveAt;}ck(p.p==leader);ev(21);return mode!=2;}
struct CCreatureAction_PickUpGenericBox{
 CCreatureAction_PickUpGenericBox(CThing& c,CThing& cr){ck(&c==me&&&cr==item);ev(8);}
 long GetActionID()const{return 67;}~CCreatureAction_PickUpGenericBox(){ev(19);}
};
struct CAIStateGroup_CarryCrateAndFollow{
 Ptr PCrate,PThingToFollow,PPreviouslyCarriedItem;long ActionPointIndex,PickUpActionID;bool GotActionPoint;
 CThing&GetCreature(){return *me;}void Terminate(){ev(18);}
 EAIActionReturn AIAction_MoveToPos(EAIActionRepeat r,const C3DVector& p,bool& done,EMovementType m,bool a,bool b,CThing* c,bool d,float v){
  ck(r==2&&at(p,1,2,3)&&m==5&&a&&b&&c==item&&d&&!v);ev(5);done=true;return firstRepeats-->0?AI_ACTION_IN_PROGRESS:(EAIActionReturn)aStatus;}
 EAIActionReturn AIAction_MoveToPosProximity(EAIActionRepeat r,const C3DVector& p,float z,bool& done,EMovementType m,bool a,bool b,bool c,CThing* t,bool d,float v){
  ck(r==2&&at(p,7,8,9)&&z==1&&m==5&&a&&!b&&c&&t==item&&d&&!v);ev(6);done=true;return secondRepeats-->0?AI_ACTION_IN_PROGRESS:(EAIActionReturn)bStatus;}
 EAIActionReturn AIAction_LookAtPos(EAIActionRepeat r,const C3DVector&p,long n,float f){
  ck(r==1&&n==33&&f==.0027777f);ck(aStatus==3?at(p,7,8,9):at(p,1,4,5));ev(7);return (EAIActionReturn)lStatus;}
 EAIActionReturn AIAction_PerformActionAndWait(const CCreatureAction_PickUpGenericBox&,bool b){ck(!b);ev(12);return picked?AI_ACTION_FINISHED:AI_ACTION_FAILED;}
 EAIActionReturn AIAction_MoveToThingProximity(EAIActionRepeat r,Ptr t,float z,bool& done,EMovementType m,bool a,bool b,bool c,bool d,float f){ck(r==1&&t.p==leader&&z==2&&m==5&&a&&!b&&c&&d&&!f);ev(15);done=true;return followSteps++==0?AI_ACTION_FINISHED:AI_ACTION_FAILED;}
 EAIActionReturn AIAction_DoNothing(){ev(16);return idleSteps++==0?AI_ACTION_IN_PROGRESS:AI_ACTION_FINISHED;}
 bool IsExpressionPerfomedTo(const char*s,Ptr& t){ck(t.p==leader);if(!strcmp(s,"EXPRESSION_FOLLOW")){ev(13);return doFollow;}ck(!strcmp(s,"EXPRESSION_WAIT"));ev(14);return mode==1||(!doWait && followSteps>0);}
 void Update();
};
void CAIStateGroup_CarryCrateAndFollow::Update()
{
    if (!IS_THING_ALIVE(PCrate)) {
        Terminate();
        return;
    }
    CTCSingleUserActionPoints* ptc_single_user_action_points;
    if (!PCrate->GetTC(&ptc_single_user_action_points)) {
        FABLE_RETAIL_ASSERT(false);
    }
    C3DVector pick_up_pos;
    CRightHandedSet pick_up_rhset;
    if (!ptc_single_user_action_points->GetFreeActionPoint(GetCreature(), pick_up_pos, pick_up_rhset, ActionPointIndex)) {
        Terminate();
        return;
    }
    GotActionPoint = true;
    C3DVector facing_vector = pick_up_rhset.GetForward();
    CTCCarryable* ptc_carryable;
    if (!PCrate->GetTC(&ptc_carryable)) {
        FABLE_RETAIL_ASSERT(false);
    }
    EAIActionReturn move_to_item_return_status = AI_ACTION_FAILED;
    do {
        if (!IS_THING_ALIVE(PCrate) || ptc_carryable->IsCarried()) {
            Terminate();
            return;
        }
        bool finished_calculating_route;
        move_to_item_return_status = AIAction_MoveToPos(ACTION_REPEAT_CALLBACK, pick_up_pos, finished_calculating_route, AI_MOVEMENT_RUNNING, true, true, PCrate.Get(), true, 0.0f);
    } while (move_to_item_return_status == AI_ACTION_IN_PROGRESS);
    if (move_to_item_return_status == AI_ACTION_FAILED) {
        do {
            if (!IS_THING_ALIVE(PCrate) || ptc_carryable->IsCarried()) {
                Terminate();
                return;
            }
            bool finished_calculating_route;
            move_to_item_return_status = AIAction_MoveToPosProximity(ACTION_REPEAT_CALLBACK, PCrate->GetPos(), 1.0f, finished_calculating_route, AI_MOVEMENT_RUNNING, true, false, true,
                PCrate.Get(), true, 0.0f);
        } while (move_to_item_return_status == AI_ACTION_IN_PROGRESS);
        if (IS_THING_ALIVE(PCrate)) {
            facing_vector = PCrate->GetPos() - GetCreature().GetPos();
        }
    }
    if (!IS_THING_ALIVE(PCrate) || ptc_carryable->IsCarried() || move_to_item_return_status != AI_ACTION_FINISHED) {
        Terminate();
        return;
    }
    EAIActionReturn face_direction_return_status = AIAction_LookAtPos(ACTION_REPEAT_UNTIL_FINISHED, GetCreature().GetPos() + facing_vector, 0x21, 0.0027777f);
    if (!IS_THING_ALIVE(PCrate) || ptc_carryable->IsCarried() || face_direction_return_status != AI_ACTION_FINISHED) {
        Terminate();
        return;
    }
    CCreatureAction_PickUpGenericBox pick_up_action(GetCreature(), *PCrate);
    PickUpActionID = pick_up_action.GetActionID();
    CTCCarrying* ptc_carrying;
    if (!GetCreature().GetTC(&ptc_carrying)) {
        FABLE_RETAIL_ASSERT(false);
    }
    PPreviouslyCarriedItem = ptc_carrying->GetThingInPrimarySlot();
    ptc_carrying->RemoveThingFromPrimarySlot(false);
    if (AIAction_PerformActionAndWait(pick_up_action, false) != AI_ACTION_FINISHED) {
        Terminate();
        return;
    }
    do {
        if (IsExpressionPerfomedTo("EXPRESSION_FOLLOW", PThingToFollow)) {
            EAIActionReturn follow_thing_return_status = AI_ACTION_FAILED;
            do {
                if (!IS_THING_ALIVE(PThingToFollow) || IsExpressionPerfomedTo("EXPRESSION_WAIT", PThingToFollow))
                    break;
                bool finished_calculating_route;
                follow_thing_return_status = AIAction_MoveToThingProximity(ACTION_REPEAT_UNTIL_FINISHED, PThingToFollow, 2.0f, finished_calculating_route, AI_MOVEMENT_RUNNING, true, false, true, true, 0.0f);
            } while (follow_thing_return_status == AI_ACTION_FINISHED);
        }
    } while (AIAction_DoNothing() == AI_ACTION_IN_PROGRESS);
    Terminate();
}
static int expected[100],en,ecAlive,ecCarried;
static void add(int x){expected[en++]=x;}
static bool alive(){add(20);++ecAlive;return aliveAt<0 || ecAlive<aliveAt;}
static bool freeItem(){if(!alive())return false;add(4);return ++ecCarried!=carriedAt;}
static void expectEnd(bool action){add(18);if(action)add(19);}
static void prediction(int repA,int repB){
 en=ecAlive=ecCarried=0;
 if(!alive()){expectEnd(false);return;}add(1);if(assertion==1){add(31);return;}
 add(2);if(!available){expectEnd(false);return;}add(3);if(assertion==2){add(31);return;}
 for(int i=0;i<=repA;++i){if(!freeItem()){expectEnd(false);return;}add(5);}
 int status=aStatus;
 if(status==3){for(int i=0;i<=repB;++i){if(!freeItem()){expectEnd(false);return;}add(6);}alive();status=bStatus;}
 if(!freeItem()||status!=2){expectEnd(false);return;}add(7);
 if(!freeItem()||lStatus!=2){expectEnd(false);return;}add(8);add(9);
 if(assertion==3){add(31);add(19);return;}add(10);add(11);add(12);
 if(picked){int moved=0;for(int idle=0;idle<2;++idle){add(13);if(doFollow){for(;;){add(21);if(mode==2)break;add(14);if(mode==1||(!doWait&&moved))break;add(15);if(++moved!=1)break;}}add(16);}}
 expectEnd(true);
}
int main(){
 CThing c,t,f,s;me=&c;item=&t;leader=&f;saved=&s;c.p=C3DVector(1,3,5);t.p=C3DVector(7,8,9);
 for(int pattern=0;pattern<256;++pattern)for(int lane=0;lane<30;++lane)for(int opt=0;opt<4;++opt){
  testPattern=pattern;testLane=lane;testOption=opt;
  assertion=(lane==1||lane==2||lane==3)?lane:0;available=lane!=0;
  aliveAt=(lane>=4&&lane<13)?lane-3:-1;carriedAt=(lane>=13&&lane<22)?lane-12:-1;
  aStatus=(pattern&1)?3:((opt==1)?1:2);bStatus=opt==2?3:2;lStatus=opt==3?1:2;
  picked=(pattern&2)!=0;doFollow=(pattern&4)!=0;mode=(pattern>>3)%3;doWait=pattern&32;
  firstRepeats=(pattern>>6)&1;secondRepeats=(pattern>>7)&1;prediction(firstRepeats,secondRepeats);
  CAIStateGroup_CarryCrateAndFollow group;group.PCrate=item;group.PThingToFollow=leader;
  group.GotActionPoint=false;group.PickUpActionID=-1;group.ActionPointIndex=-1;
  used=aliveCalls=carriedCalls=followSteps=idleSteps=0;
  group.Update();
  ck(used==en);
  for(int k=0;k<en;++k)ck(logEvents[k]==expected[k]);
  bool cleared=false,action=false,claimed=false;
  for(int k=0;k<used;++k){if(logEvents[k]==11)cleared=true;if(logEvents[k]==8)action=true;if(logEvents[k]==3)claimed=true;}
  ck(group.PPreviouslyCarriedItem.p==(cleared?saved:0));ck(group.GotActionPoint==claimed);
  ck(group.PickUpActionID==(action?67:-1));
 }
 puts("009663a0_TEST PASS");return 0;
}