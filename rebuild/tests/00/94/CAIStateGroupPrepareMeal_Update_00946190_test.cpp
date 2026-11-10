#include <cstdio>
#include <cstring>
#ifndef FABLE_AI_ACTION_ENUMS_H
#define FABLE_AI_ACTION_ENUMS_H
// Exact original Ego_r enum records and native action arguments.
enum EMovementType { AI_MOVEMENT_NULL=0,AI_MOVEMENT_SNEAKING=1,AI_MOVEMENT_SLOW_WALKING=2,AI_MOVEMENT_WALKING=3,AI_MOVEMENT_JOGGING=4,AI_MOVEMENT_RUNNING=5,AI_MOVEMENT_NON_CONSTANT_RUNNING=6,AI_MOVEMENT_FLYING=7,AI_MOVEMENT_HOVER_SLOW=8,AI_MOVEMENT_HOVER_MEDIUM=9,AI_MOVEMENT_HOVER_FAST=10 };
enum EAIActionReturn { AI_ACTION_NULL=0,AI_ACTION_TERMINATE_PROCESS=1,AI_ACTION_FINISHED=2,AI_ACTION_FAILED=3,AI_ACTION_IN_PROGRESS=4 };
enum EAIActionRepeat { ACTION_REPEAT_UNTIL_FINISHED=1,ACTION_REPEAT_CALLBACK=2 };
#endif

class C3DVector {public:float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){} C3DVector(const C3DVector& v):X(v.X),Y(v.Y),Z(v.Z){} const C3DVector operator+(const C3DVector& v)const{return C3DVector(X+v.X,Y+v.Y,Z+v.Z);} };
class CRightHandedSet {public:C3DVector Forward;const C3DVector& GetForward()const{return Forward;} };
struct Scenario {int MoveRepeat,MoveResult,LookResult,IdleRepeat;int Moves,Looks,Idles,CreatureGets,PosGets,Components,Adds,Removes,Terminates,Errors;int Trace[40],Used;C3DVector Goal,Look;Scenario(){MoveRepeat=MoveResult=LookResult=IdleRepeat=0;Moves=Looks=Idles=CreatureGets=PosGets=Components=Adds=Removes=Terminates=Errors=Used=0;}void Event(int x){Trace[Used++]=x;} };
static Scenario* Current;
class CTCCreatureModeManager {public:bool AddMode(unsigned long mode){Current->Errors+=mode!=14;Current->Adds++;Current->Event(5);return true;}void RemoveMode(unsigned long mode){Current->Errors+=mode!=14;Current->Removes++;Current->Event(7);} };
class CThing {};
class CThingAICreature:public CThing {public:C3DVector Position;CTCCreatureModeManager Modes;const C3DVector& PeekPos()const{Current->PosGets++;Current->Event(2);return Position;}template<class T>bool GetTC(T** p){Current->Components++;Current->Event(4);*p=&Modes;return true;} };
class CAIStateGroupBase {protected:
 CThingAICreature Creature;
 CThingAICreature& GetCreature()const{Current->CreatureGets++;return const_cast<CThingAICreature&>(Creature);}
 EAIActionReturn AIAction_MoveToPos(EAIActionRepeat repeat,C3DVector pos,bool& arrived,EMovementType move,bool a,bool b,const CThing* thing,bool c,float speed){Current->Errors+=repeat!=ACTION_REPEAT_CALLBACK||move!=AI_MOVEMENT_WALKING||!a||!b||thing!=0||!c||speed!=0;Current->Errors+=pos.X!=Current->Goal.X||pos.Y!=Current->Goal.Y||pos.Z!=Current->Goal.Z;arrived=true;Current->Event(1);return ++Current->Moves<=Current->MoveRepeat?AI_ACTION_IN_PROGRESS:(EAIActionReturn)Current->MoveResult;}
 EAIActionReturn AIAction_LookAtPos(EAIActionRepeat repeat,C3DVector pos,long frames,float speed){Current->Errors+=repeat!=ACTION_REPEAT_UNTIL_FINISHED||frames!=17;unsigned int bits=0;std::memcpy(&bits,&speed,4);Current->Errors+=bits!=0x3b360a13U;Current->Look=pos;Current->Looks++;Current->Event(3);return (EAIActionReturn)Current->LookResult;}
 EAIActionReturn AIAction_DoNothing(){Current->Event(6);return ++Current->Idles<=Current->IdleRepeat?AI_ACTION_IN_PROGRESS:AI_ACTION_FINISHED;}
 void Terminate(){Current->Terminates++;Current->Event(8);}
};
class CAIStateGroup_PrepareMeal:public CAIStateGroupBase {public:void Update();void Configure(){ActionPointPos=C3DVector(3,4,5);ActionOrientation.Forward=C3DVector(0.5f,-1,2);Creature.Position=C3DVector(10,20,30);}private:C3DVector ActionPointPos;CRightHandedSet ActionOrientation;};
void CAIStateGroup_PrepareMeal::Update()
{
    bool arrived;
    EAIActionReturn result=AI_ACTION_FAILED;
    do {
        result=AIAction_MoveToPos(ACTION_REPEAT_CALLBACK,ActionPointPos,arrived,AI_MOVEMENT_WALKING,true,true,0,true,0.0f);
    } while(result==AI_ACTION_IN_PROGRESS);
    if(result!=AI_ACTION_FINISHED){Terminate();return;}
    C3DVector forward=ActionOrientation.GetForward();
    C3DVector pos=GetCreature().PeekPos()+forward;
    result=AIAction_LookAtPos(ACTION_REPEAT_UNTIL_FINISHED,pos,0x11,0.0027777f);
    if(result!=AI_ACTION_FINISHED){Terminate();return;}
    CTCCreatureModeManager* modes;
    GetCreature().GetTC(&modes);
    modes->AddMode(0xe);
    while(AIAction_DoNothing()==AI_ACTION_IN_PROGRESS){}
    modes->RemoveMode(0xe);
    Terminate();
}

int main(){int ok=1,cases=0;int repeats[3]={0,1,3};for(int m=1;m<=3;++m)for(int l=1;l<=3;++l)for(int n=0;n<3;++n)for(int idle=0;idle<3;++idle){Scenario s;Current=&s;s.Goal=C3DVector(3,4,5);s.MoveRepeat=repeats[n];s.MoveResult=m;s.LookResult=l;s.IdleRepeat=repeats[idle];CAIStateGroup_PrepareMeal action;action.Configure();action.Update();bool moved=m==2,looked=moved&&l==2;ok &= s.Errors==0&&s.Moves==s.MoveRepeat+1&&s.Looks==(moved?1:0)&&s.PosGets==(moved?1:0)&&s.Components==(looked?1:0)&&s.CreatureGets==(moved?1:0)+(looked?1:0);ok &= s.Adds==(looked?1:0)&&s.Removes==s.Adds&&s.Idles==(looked?s.IdleRepeat+1:0)&&s.Terminates==1;int at=0;for(int j=0;j<s.Moves;++j)ok &= s.Trace[at++]==1;if(moved){ok &= s.Trace[at++]==2&&s.Trace[at++]==3;ok &= s.Look.X==10.5f&&s.Look.Y==19&&s.Look.Z==32;}if(looked){ok &= s.Trace[at++]==4&&s.Trace[at++]==5;for(int j=0;j<s.Idles;++j)ok &= s.Trace[at++]==6;ok &= s.Trace[at++]==7;}ok &= s.Trace[at++]==8&&at==s.Used;++cases;}ok &= cases==81;std::printf(ok?"00946190_TEST PASS\n":"00946190_TEST FAIL\n");return ok?0:1;}