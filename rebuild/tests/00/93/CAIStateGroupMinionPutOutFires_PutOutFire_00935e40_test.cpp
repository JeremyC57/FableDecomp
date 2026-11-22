#include <stdio.h>
#include <string.h>
#include "engine/EAIAction.h"
#include "engine/EMovementType.h"
#include "rebuild_abi.h"
#define FABLE_RETAIL_CSOUNDPAIR_ORIGINAL_CTORS
#include "engine/CSoundPair.h"
static unsigned int visits,moves,animations,repetitions;
static int move_exit,action_exit,initial_state,final_state;
static bool bad;
struct CTCFlammable {bool IsOnFire() const {return (moves<=repetitions?initial_state:final_state)==3;}};
static CTCFlammable component;
struct Building {
    Building* operator->(){return this;}
    bool PeekTC(const CTCFlammable** out) {
        if((moves<=repetitions?initial_state:final_state)<2)return false;
        *out=&component;return true;
    }
    int GetPos(){return 123;}
};
static bool IS_THING_DEAD(const Building&){++visits;return (moves<=repetitions?initial_state:final_state)==0;}
static bool IS_THING_ALIVE(const Building& b){return !IS_THING_DEAD(b);}
struct CCreatureAction_PlayAnimation {
    CCreatureAction_PlayAnimation(int who,const char* script,const CSoundPair& sound,bool a,bool b,bool c,long e,long f){
        if (who!=77 || strcmp(script,"ST_IDLE") || sound.FirstSound!=0 || sound.LastSound!=0 || a || !b || c || e || f)bad=true;
    }
};
struct CAIStateGroup_MinionPutOutFires {
    Building PBuildingOnFire;
    int GetCreature()const{return 77;}
    EAIActionReturn AIAction_MoveToPosProximity(EAIActionRepeat how,int pos,float distance,bool& nav,EMovementType move,
        bool a,bool b,bool c,const void* target,bool e,float f){
        if(how!=ACTION_REPEAT_CALLBACK || pos!=123 || distance!=3 || nav!=(moves!=0) ||
            move!=AI_MOVEMENT_RUNNING || !a || b || !c || target || !e || f!=0)bad=true;
        nav=true;++moves;
        return moves<=repetitions ? AI_ACTION_IN_PROGRESS : (EAIActionReturn)move_exit;
    }
    EAIActionReturn AIAction_PerformAction(const CCreatureAction_PlayAnimation&){++animations;return (EAIActionReturn)action_exit;}
    bool PutOutFire();
};
bool CAIStateGroup_MinionPutOutFires::PutOutFire()
{
    bool nav_route_calculated = false;
    EAIActionReturn ret;
    do {
        const CTCFlammable* ptc_flammable;
        if (IS_THING_DEAD(PBuildingOnFire) || !PBuildingOnFire->PeekTC(&ptc_flammable) || !ptc_flammable->IsOnFire())
            return false;
        ret = AIAction_MoveToPosProximity(ACTION_REPEAT_CALLBACK, PBuildingOnFire->GetPos(), 3.0f,
            nav_route_calculated, AI_MOVEMENT_RUNNING, true, false, true, 0, true, 0.0f);
    } while (ret == AI_ACTION_IN_PROGRESS);
    if (ret == AI_ACTION_FINISHED && IS_THING_ALIVE(PBuildingOnFire)) {
        const CTCFlammable* ptc_flammable;
        if (PBuildingOnFire->PeekTC(&ptc_flammable) && ptc_flammable->IsOnFire()) {
            if (AIAction_PerformAction(CCreatureAction_PlayAnimation(GetCreature(), "ST_IDLE", CSoundPair(0), false, true, false, 0, 0)) == AI_ACTION_FINISHED)
                return true;
        }
    }
    return false;
}

int main(){
 for(initial_state=0;initial_state<4;++initial_state)
 for(final_state=0;final_state<4;++final_state)
 for(repetitions=0;repetitions<3;++repetitions)
 for(move_exit=0;move_exit<=3;++move_exit)
 for(action_exit=0;action_exit<=4;++action_exit){
    moves=visits=animations=0;bad=false;CAIStateGroup_MinionPutOutFires ai;
    bool success=ai.PutOutFire();
    unsigned int expected_moves=initial_state==3 ? repetitions+1 : 0;
    bool arrived=expected_moves && move_exit==AI_ACTION_FINISHED;
    bool perform=arrived && final_state==3;
    if(bad || moves!=expected_moves || visits!=(expected_moves ? expected_moves : 1)+(arrived?1:0) ||
        animations!=(perform?1u:0u) || success!=(perform && action_exit==AI_ACTION_FINISHED))return 1;
 }
 puts("00935e40_TEST PASS");return 0;
}