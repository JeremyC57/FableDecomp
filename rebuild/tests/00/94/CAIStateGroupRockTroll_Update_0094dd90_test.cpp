#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_UPDATE_NEAREST_INTERESTING
#define FABLE_RETAIL_GFFASTONEOVERSQRT_PORTABLE_BITS_PASS26
#include "engine/C3DVector.h"
#include "engine/EAIAction.h"
unsigned long GFastOneOverSqrtTable[128];
namespace RockFixture {
void check(bool ok) { if (!ok) exit(1); }
struct State {
    std::string trace;
    int checks, cancelAt, predicts, performing, phase, throwCount, idleWaits, poundWaits, endWaits, knockAt;
    float nearPos, nextPos, finalPos, facing, laughDistance, laughTime;
    bool alive, stopping, failThrow;
    long frame;
    State():checks(0),cancelAt(0),predicts(0),performing(0),phase(0),throwCount(0),idleWaits(0),poundWaits(0),endWaits(0),knockAt(0),
        nearPos(2),nextPos(2),finalPos(4),facing(1),laughDistance(1),laughTime(8),alive(true),stopping(false),failThrow(false),frame(20) {}
} *s;
struct CTrollBattleDef { float MinLaughDistance,MaxTimeBetweenKnockdownAndBoast; };
template<class T> struct CDefPointer { T* p; T* operator->() const { return p; } };
struct Def { CTrollBattleDef value;
    void GetSubDef(CDefPointer<const CTrollBattleDef>* out) { value.MinLaughDistance=s->laughDistance; value.MaxTimeBetweenKnockdownAndBoast=s->laughTime; out->p=&value; }
};
struct CTCCombat { void ResetComboChainPos(){s->trace+='R';} };
enum ECombatAnimationHandedness { DefaultHand };
struct Thing {
    bool enemy; Def def; CTCCombat combat;
    explicit Thing(bool e=false):enemy(e){}
    Def& PeekBaseDef(){return def;}
    C3DVector GetPos() const{return C3DVector(enemy?s->finalPos:0.0f,0,0);}
    Thing* GetPhysicsTC(){return this;}
    C3DVector GetFacingVector(){return C3DVector(s->facing,0,0);}
    void GetTC(CTCCombat** out){*out=&combat;}
    void SetLastActionHandedness(ECombatAnimationHandedness x){check(x==DefaultHand);s->trace+='H';}
    bool IsPerformingAnyAction(){
        int* n=s->phase==1?&s->poundWaits:s->phase==2?&s->idleWaits:&s->endWaits;
        if (*n) { --*n;return true; } s->phase=0;return false;
    }
};
struct Brain { bool SuspendSuspendableProcess(){s->trace+='Y';return true;} };
struct World { long GetFrame(){return s->frame;} };
static bool IS_THING_ALIVE(Thing* p){return p && s->alive;}
struct CCreatureAction_PlayCombatAnimation { const char* name;
    CCreatureAction_PlayCombatAnimation(Thing& who,const char* n,bool flag):name(n){check(!who.enemy && flag);}
};
struct CCreatureAction_TrollGroundPound { CCreatureAction_TrollGroundPound(Thing& who,Thing& enemy){check(!who.enemy && enemy.enemy);} };
struct CAIStateGroup_RockTroll {
    Thing creature,enemy; Thing* PEnemy; Brain brain; Brain* PBrain; World world;
    float TROLL_MIN_THROWING_DISTANCE,TROLL_MAX_THROWING_DISTANCE;
    long ShotCounter,EnemyKnockDownFrame;
    CAIStateGroup_RockTroll():enemy(true),PEnemy(&enemy),PBrain(&brain),TROLL_MIN_THROWING_DISTANCE(4),TROLL_MAX_THROWING_DISTANCE(8),ShotCounter(-1),EnemyKnockDownFrame(19){}
    void ForceRiseUp(){s->trace+='U';}
    void Terminate(){s->trace+='T';}
    bool IsStillRunnable(){return ++s->checks!=s->cancelAt;}
    bool IsStopping()const{return s->stopping;}
    Thing& GetCreature(){return creature;}
    World& GetWorld(){return world;}
    C3DVector GetEnemyPredictedPos(){return C3DVector(++s->predicts==1?s->nearPos:s->nextPos,0,0);}
    EAIActionReturn AIAction_PerformActionAndWait(const CCreatureAction_PlayCombatAnimation& a,bool wait){
        check(!wait);
        if (!strcmp(a.name,"LOOK_BACKWARD")) s->trace+='B';
        else if (!strcmp(a.name,"LOOK_FORWARD")) s->trace+='F';
        else if (!strcmp(a.name,"SPECIAL_IDLE")) s->trace+='I';
        else {check(!strcmp(a.name,"SPECIAL_BOAST"));s->trace+='L';}
        return AI_ACTION_FINISHED;
    }
    EAIActionReturn AIAction_PerformAction(const CCreatureAction_TrollGroundPound&){s->trace+='P';s->phase=1;return AI_ACTION_FINISHED;}
    bool HasEnemyBeenKnockedDown(){return ++s->performing==s->knockAt;}
    void AIAction_LookAtThing(EAIActionRepeat repeat,Thing* p,long n,float f){check(repeat==ACTION_REPEAT_UNTIL_FINISHED && p==PEnemy && n==17 && f==0.2f);s->trace+='A';}
    EAIActionReturn ThrowRocks(){s->trace+='S';++s->throwCount; check(s->throwCount<9);++ShotCounter;
        if(ShotCounter==4)s->phase=2;
        if(s->failThrow || s->throwCount>4)return AI_ACTION_FAILED;
        return ShotCounter==4?AI_ACTION_FINISHED:AI_ACTION_IN_PROGRESS;}
    void AIAction_DoNothing(){s->trace+='D';}
    void Update();
};
void CAIStateGroup_RockTroll::Update()
{
    ForceRiseUp();
    if (!IsStillRunnable()) {
        Terminate();
        return;
    }
    CDefPointer<const CTrollBattleDef> ptroll_battle_def;
    GetCreature().PeekBaseDef().GetSubDef(&ptroll_battle_def);
    bool knocked_down = false;
    EAIActionReturn ret;
    if (IS_THING_ALIVE(PEnemy)) {
        if (!IsStillRunnable()) {
            Terminate();
            return;
        }
        C3DVector vector_to_enemy = GetEnemyPredictedPos() - GetCreature().GetPos();
        float dis_to_enemy = vector_to_enemy.GetMagnitude2D();
        if (dis_to_enemy < TROLL_MIN_THROWING_DISTANCE) {
            C3DVector enemy_direction = vector_to_enemy;
            enemy_direction.Normalise();
            if (GetCreature().GetPhysicsTC()->GetFacingVector().Dot(enemy_direction) < 0.5f) {
                ret = AIAction_PerformActionAndWait(CCreatureAction_PlayCombatAnimation(GetCreature(), "LOOK_BACKWARD", true), false);
            } else {
                ret = AIAction_PerformActionAndWait(CCreatureAction_PlayCombatAnimation(GetCreature(), "LOOK_FORWARD", true), false);
            }
            if (!IsStillRunnable()) {
                Terminate();
                return;
            }
            vector_to_enemy = GetEnemyPredictedPos() - GetCreature().GetPos();
            dis_to_enemy = vector_to_enemy.GetMagnitude2D();
            if (dis_to_enemy < TROLL_MIN_THROWING_DISTANCE) {
                ret = AIAction_PerformAction(CCreatureAction_TrollGroundPound(GetCreature(), *PEnemy));
                while (GetCreature().IsPerformingAnyAction()) {
                    if (!IsStillRunnable()) {
                        Terminate();
                        return;
                    }
                    if (HasEnemyBeenKnockedDown())
                        knocked_down = true;
                    PBrain->SuspendSuspendableProcess();
                }
            }
        } else if (dis_to_enemy <= TROLL_MAX_THROWING_DISTANCE) {
            AIAction_LookAtThing(ACTION_REPEAT_UNTIL_FINISHED, PEnemy, 0x11, 0.2f);
            if (!IsStillRunnable()) {
                Terminate();
                return;
            }
            ShotCounter = 0;
            do {
                if (ShotCounter == 0) {
                    EnemyKnockDownFrame = GetWorld().GetFrame();
                    CTCCombat* ptc_combat;
                    GetCreature().GetTC(&ptc_combat);
                    ptc_combat->ResetComboChainPos();
                    GetCreature().SetLastActionHandedness(static_cast<ECombatAnimationHandedness>(0));
                }
                ret = ThrowRocks();
                if (!IsStillRunnable()) {
                    Terminate();
                    return;
                }
                if (HasEnemyBeenKnockedDown())
                    knocked_down = true;
                if (ShotCounter == 4 && ret == AI_ACTION_FINISHED) {
                    CCreatureAction_PlayCombatAnimation action(GetCreature(), "SPECIAL_IDLE", true);
                    while (GetCreature().IsPerformingAnyAction()) {
                        if (!IsStillRunnable()) {
                            Terminate();
                            return;
                        }
                        if (HasEnemyBeenKnockedDown())
                            knocked_down = true;
                        PBrain->SuspendSuspendableProcess();
                    }
                    ShotCounter = 0;
                    if (!knocked_down)
                        ret = AIAction_PerformActionAndWait(action, false);
                }
                if (!IsStillRunnable()) {
                    Terminate();
                    return;
                }
                vector_to_enemy = PEnemy->GetPos() - GetCreature().GetPos();
                dis_to_enemy = vector_to_enemy.GetMagnitude2D();
                if (HasEnemyBeenKnockedDown())
                    knocked_down = true;
            } while ((ret == AI_ACTION_IN_PROGRESS || ret == AI_ACTION_FINISHED) && dis_to_enemy >= TROLL_MIN_THROWING_DISTANCE && dis_to_enemy <= TROLL_MAX_THROWING_DISTANCE && !knocked_down);
            if (dis_to_enemy > TROLL_MAX_THROWING_DISTANCE) {
                Terminate();
                return;
            }
        } else {
            Terminate();
            return;
        }
        while (GetCreature().IsPerformingAnyAction()) {
            AIAction_DoNothing();
            if (!IsStillRunnable()) {
                Terminate();
                return;
            }
        }
        long time_since_knockdown = GetWorld().GetFrame() - EnemyKnockDownFrame;
        if (!IsStopping() && dis_to_enemy >= ptroll_battle_def->MinLaughDistance && knocked_down && time_since_knockdown < ptroll_battle_def->MaxTimeBetweenKnockdownAndBoast) {
            ret = AIAction_PerformActionAndWait(CCreatureAction_PlayCombatAnimation(GetCreature(), "SPECIAL_BOAST", true), false);
            EnemyKnockDownFrame = GetWorld().GetFrame();
        }
    } else {
        Terminate();
    }
    if (IsStopping()) {
        Terminate();
        return;
    }
}

int run(State settings,const char* expected) {
    s=&settings; CAIStateGroup_RockTroll troll; troll.Update();
    if(expected && s->trace!=expected){printf("expected %s observed %s\n",expected,s->trace.c_str());exit(1);}
    if(!expected)check(!s->trace.empty() && s->trace[s->trace.size()-1]=='T' && s->checks==s->cancelAt);
    if(s->trace.find('L')!=std::string::npos)check(troll.EnemyKnockDownFrame==s->frame);
    return s->checks;
}
void exercise(State v,const char* expected){
    int count=run(v,expected);
    for(int i=1;i<=count;++i){v.cancelAt=i;run(v,0);}
}
}
int main(){
    using namespace RockFixture;
    State a;exercise(a,"UFP");
    a.facing=-1; a.poundWaits=2;a.endWaits=2;a.knockAt=1;exercise(a,"UBPYYDDL");
    a.laughDistance=4;exercise(a,"UBPYYDD");
    a.laughDistance=1;a.laughTime=1;exercise(a,"UBPYYDD");
    a.laughTime=8;a.stopping=true;exercise(a,"UBPYYDDT");
    a=State();a.nextPos=4;exercise(a,"UF");
    a=State();a.alive=false;exercise(a,"UT");a.stopping=true;exercise(a,"UTT");
    a=State();a.nearPos=9;exercise(a,"UT");
    a=State();a.nearPos=4;a.failThrow=true;exercise(a,"UARHS");
    a=State();a.nearPos=8;a.finalPos=9;exercise(a,"UARHST");
    a.finalPos=2;exercise(a,"UARHS");
    a.finalPos=8;a.idleWaits=2;a.endWaits=1;exercise(a,"UARHSSSSYYIRHSD");
    a.knockAt=8;exercise(a,"UARHSSSSYYDL");
    a.knockAt=2;exercise(a,"UARHSDL");
    puts("0094dd90_TEST PASS");return 0;
}