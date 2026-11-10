// Exact target and original awareness predicates; controlled action/handle services.
#include <stdio.h>
#ifndef FABLE_AI_ACTION_ENUMS_H
#define FABLE_AI_ACTION_ENUMS_H
// Exact original Ego_r enum records and native action arguments.
enum EMovementType { AI_MOVEMENT_NULL=0,AI_MOVEMENT_SNEAKING=1,AI_MOVEMENT_SLOW_WALKING=2,AI_MOVEMENT_WALKING=3,AI_MOVEMENT_JOGGING=4,AI_MOVEMENT_RUNNING=5,AI_MOVEMENT_NON_CONSTANT_RUNNING=6,AI_MOVEMENT_FLYING=7,AI_MOVEMENT_HOVER_SLOW=8,AI_MOVEMENT_HOVER_MEDIUM=9,AI_MOVEMENT_HOVER_FAST=10 };
enum EAIActionReturn { AI_ACTION_NULL=0,AI_ACTION_TERMINATE_PROCESS=1,AI_ACTION_FINISHED=2,AI_ACTION_FAILED=3,AI_ACTION_IN_PROGRESS=4 };
enum EAIActionRepeat { ACTION_REPEAT_UNTIL_FINISHED=1,ACTION_REPEAT_CALLBACK=2 };
#endif

static int direct_calls,move_calls,passive_calls,copy_calls,drop_calls,argument_errors;
static int progress_count,terminal_code,lose_after;
static bool passive_result;
class CThing{};
template<class T>class CIntelligentPointer {
public:
    T* Item;bool Copy;
    CIntelligentPointer():Item(0),Copy(false){}
    CIntelligentPointer(const CIntelligentPointer& other):Item(other.Item),Copy(true){++copy_calls;}
    template<class U>CIntelligentPointer(CIntelligentPointer<U>& other):Item(other.Get()),Copy(true){++copy_calls;}
    ~CIntelligentPointer(){if(Copy)++drop_calls;}
    T* Get()const{return Item;}
};
class CTCPerceiveThing {
public:
    float AwarenessTimer;bool LineOfSight;bool DirectAwareness;
    bool IsAwareOfThing()const{return AwarenessTimer!=0.0f;}
    bool IsThingLineOfSight()const{return LineOfSight;}
    bool IsAwarenessOfThingDirect()const{++direct_calls;return DirectAwareness;}
};
static CTCPerceiveThing* test_perception;
static const CThing* expected_thing;
class CAIStateGroupBase {
protected:
    EAIActionReturn AIAction_MoveToThingProximity(EAIActionRepeat repeat,CIntelligentPointer<const CThing> thing,float range,
        bool& nav,EMovementType movement,bool a,bool b,bool c,bool d,float speed){
        if(repeat!=ACTION_REPEAT_UNTIL_FINISHED||thing.Get()!=expected_thing||range!=2.0f||movement!=AI_MOVEMENT_WALKING
           ||!a||b||!c||!d||speed!=0.0f)++argument_errors;
        ++move_calls;nav=(move_calls&1)!=0;
        if(lose_after==move_calls)test_perception->AwarenessTimer=0.0f;
        return (EAIActionReturn)(move_calls<=progress_count?AI_ACTION_IN_PROGRESS:terminal_code);
    }
};
class CAIStateGroupBehaviourBase:public CAIStateGroupBase {
public:
    CIntelligentPointer<const CThing> PTargetThing;
    const CTCPerceiveThing* PTCPerception;
    bool IsAwareOfThing()const{return PTCPerception->IsAwareOfThing()&&PTCPerception->IsAwarenessOfThingDirect()&&PTCPerception->IsThingLineOfSight();}
    bool UpdateFocusAttentionOnThingActive();
    bool UpdateFocusAttentionOnThingPassive(){++passive_calls;return passive_result;}
};
bool CAIStateGroupBehaviourBase::UpdateFocusAttentionOnThingActive()
{
    EAIActionReturn ret=AI_ACTION_FAILED;
    do {
        if(!IsAwareOfThing())
            return false;
        bool nav;
        ret=AIAction_MoveToThingProximity(ACTION_REPEAT_UNTIL_FINISHED,PTargetThing,2.0f,nav,AI_MOVEMENT_WALKING,true,false,true,true,0.0f);
    }while(ret==AI_ACTION_IN_PROGRESS);
    if(ret!=AI_ACTION_FINISHED)
        return false;
    return UpdateFocusAttentionOnThingPassive();
}

int main(){
 unsigned cases=0;const float timers[]={0.0f,-1.0f,2.0f};const int progresses[]={0,1,3};
 for(int ti=0;ti<3;++ti)for(int direct=0;direct<2;++direct)for(int sight=0;sight<2;++sight)
 for(int pi=0;pi<3;++pi)for(int terminal=0;terminal<4;++terminal)for(int success=0;success<2;++success)for(int loss=0;loss<3;++loss){
  CTCPerceiveThing perception;perception.AwarenessTimer=timers[ti];perception.DirectAwareness=direct!=0;perception.LineOfSight=sight!=0;
  CThing thing;CAIStateGroupBehaviourBase state;state.PTargetThing.Item=&thing;state.PTCPerception=&perception;
  test_perception=&perception;expected_thing=&thing;progress_count=progresses[pi];terminal_code=terminal;lose_after=loss;passive_result=success!=0;
  direct_calls=move_calls=passive_calls=copy_calls=drop_calls=argument_errors=0;
  bool result=state.UpdateFocusAttentionOnThingActive();
  bool awake=ti!=0&&direct&&sight;int expected_moves=awake?progress_count+1:0;
  bool loses=awake&&loss>0&&loss<=progress_count;
  if(loses)expected_moves=loss;
  int expected_passive=awake&&!loses&&terminal==AI_ACTION_FINISHED?1:0;
  int expected_direct=ti==0?0:awake?expected_moves:1;
  if(result!=(expected_passive&&success)||move_calls!=expected_moves||passive_calls!=expected_passive||direct_calls!=expected_direct
    ||copy_calls!=expected_moves||drop_calls!=expected_moves||argument_errors||state.PTargetThing.Get()!=&thing)return 1;
  ++cases;
 }
 if(cases!=864)return 2;
 puts("009578a0_TEST PASS");return 0;
}