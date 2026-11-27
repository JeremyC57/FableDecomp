#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <setjmp.h>
static void chk(bool x){if(!x)abort();}
enum EScriptEntityMoveType{ENTITY_MOVE_WALK,ENTITY_MOVE_RUN};
enum EScriptAIPriority{SCRIPT_AI_PRIORITY_HIGHEST=4};
enum EScriptingStateGroups{ESSG_NONE,ESSG_WANDER_NEAR=4};
enum Event{CHECK,FAIL,WALKMARK,LOOKMARK,START,RELEASE,MOVEW,MOVER,DISTW,DISTR,LOOK,CENTRE,MINIMUM,MAXIMUM,WANDER,COMBAT,HERO,SEE,ENEMY,FRAME,EXITMARK,FADE};
static int scenario,stop,ordinal,startTries,distanceCalls[2],vicheck,phase,frames,objects,handles;
static std::vector<int> trace,model;
static void emit(Event e){trace.push_back(e);}
static bool bit(int n){return (scenario&(1<<n))!=0;}
struct C3DVector{int id;};
struct CScriptThing{int id;CScriptThing(int v=0):id(v){++objects;}CScriptThing(const CScriptThing& v):id(v.id){++objects;}~CScriptThing(){--objects;}
 bool IsAlive()const{return id==2?bit(0):id==3?bit(1):id==4?bit(2):true;}
 C3DVector GetPos()const{C3DVector r={id};return r;}const char* GetDataString()const{chk(id==1);return "walk";}};
struct CScriptGameResourceObjectScriptedThingBase{bool valid;CScriptGameResourceObjectScriptedThingBase():valid(bit(3)){++handles;}~CScriptGameResourceObjectScriptedThingBase(){--handles;}
 bool IsValid()const{return valid;}void Release(){chk(valid);valid=false;emit(RELEASE);}
 void MoveToPosition(C3DVector to,float a,EScriptEntityMoveType m,bool b,bool c){chk((valid||m==ENTITY_MOVE_WALK)&&a==1&&!b&&c&&to.id==(m==ENTITY_MOVE_WALK?2:4));emit(m==ENTITY_MOVE_WALK?MOVEW:MOVER);}};
struct TRetVoid{void Return(){emit(FAIL);}};
struct GameAPI{
 CScriptThing GetThingWithScriptName(const char* n){if(!strcmp(n,"walk")){emit(WALKMARK);return CScriptThing(2);}chk(!strcmp(n,"M_AT_ConvoyExitCSTrigger"));emit(LOOKMARK);return CScriptThing(3);}
 bool StartScriptingEntity(const CScriptThing& me,CScriptGameResourceObjectScriptedThingBase& h,EScriptAIPriority p){chk(me.id==1&&p==SCRIPT_AI_PRIORITY_HIGHEST&&!h.valid);emit(START);if(bit(4)&&!(startTries++%2))return false;h.valid=!bit(10);return true;}
 void NewScriptFrame(){emit(FRAME);++frames;chk(frames<260);}
 void EntityForceToLookAtThing(const CScriptThing& me,const CScriptThing& m){chk(me.id==1&&m.id==3);emit(LOOK);}
 void SetWanderCentrePoint(const CScriptThing& me,C3DVector pt){chk(me.id==1&&pt.id==1);emit(CENTRE);}
 void SetWanderMinDistance(const CScriptThing& me,float f){chk(me.id==1&&f==0);emit(MINIMUM);}
 void SetWanderMaxDistance(const CScriptThing& me,float f){chk(me.id==1&&f==7.5f);emit(MAXIMUM);}
 void SetScriptingStateGroup(const CScriptThing& me,EScriptingStateGroups g){chk(me.id==1);emit(g==ESSG_NONE?COMBAT:WANDER);}
 CScriptThing GetHero(){emit(HERO);return CScriptThing(5);}
 bool CanThingBe_Seen_ByOtherThing(const CScriptThing& h,const CScriptThing& me){chk(h.id==5&&me.id==1);emit(SEE);return bit(5)&&++vicheck>(bit(6)?1:0);}
 void GiveThingBestEnemyTarget(const CScriptThing& me,const CScriptThing& h){chk(me.id==1&&h.id==5);emit(ENEMY);}
 CScriptThing GetNearestWithDefName(const CScriptThing& me,const char* n){chk(me.id==1&&!strcmp(n,"REGION_EXIT_POINT"));emit(EXITMARK);return CScriptThing(4);}
} api;
static bool IsDistanceBetweenThingsUnder(const CScriptThing& me,const CScriptThing& dest,float f){chk(me.id==1&&f==2&&(dest.id==2||dest.id==4));int k=dest.id==4;emit(k?DISTR:DISTW);return !bit(7)||distanceCalls[k]++%2!=0;}
static void FadeOutAndKillEntity(const CScriptThing& me,float f,bool a,bool b){chk(me.id==1&&f==1&&a&&b);emit(FADE);}
struct Def{float AmbushTradersPhase1GuardWanderDistance;} def={7.5f};static Def* pGlobalScriptDef=&def;
namespace NScript{struct CQ_AmbushTradersScript{bool QuestFailed,QuestFinished;struct CAT_Phase1Guard{GameAPI* Game;CScriptThing Me;CQ_AmbushTradersScript& ParentClass;
 CAT_Phase1Guard(CQ_AmbushTradersScript& p):Game(&api),Me(1),ParentClass(p){}
 bool IsActiveThreadTerminating(){emit(CHECK);return ++ordinal==stop;}void Main();};};}
void NScript::CQ_AmbushTradersScript::CAT_Phase1Guard::Main()
{
    TRetVoid _Fail_Val;
    CScriptGameResourceObjectScriptedThingBase sehMe;
    CScriptThing walkToMkr = Game->GetThingWithScriptName(Me.GetDataString());
    if (walkToMkr.IsAlive()) {
        if (IsActiveThreadTerminating()) {
            _Fail_Val.Return();
            return;
        }
        if (sehMe.IsValid())
            sehMe.Release();
        while (!Game->StartScriptingEntity(Me, sehMe, SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) {
                _Fail_Val.Return();
                return;
            }
        }
        if (IsActiveThreadTerminating()) {
            _Fail_Val.Return();
            return;
        }
        sehMe.MoveToPosition(walkToMkr.GetPos(), 1.0f, ENTITY_MOVE_WALK, false, true);
        while (!IsDistanceBetweenThingsUnder(Me, walkToMkr, 2.0f)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) {
                _Fail_Val.Return();
                return;
            }
        }
        if (IsActiveThreadTerminating()) {
            _Fail_Val.Return();
            return;
        }
        CScriptThing lookAtMkr = Game->GetThingWithScriptName("M_AT_ConvoyExitCSTrigger");

        if (lookAtMkr.IsAlive()) {

            if (IsActiveThreadTerminating()) {
                _Fail_Val.Return();
                return;
            }
            Game->EntityForceToLookAtThing(Me, lookAtMkr);
        }
        if (sehMe.IsValid())
            sehMe.Release();

    }

    Game->SetWanderCentrePoint(Me, Me.GetPos());
    Game->SetWanderMinDistance(Me, 0.0f);
    Game->SetWanderMaxDistance(Me, pGlobalScriptDef->AmbushTradersPhase1GuardWanderDistance);
    Game->SetScriptingStateGroup(Me, ESSG_WANDER_NEAR);
    if (false) {} else for (;; Game->NewScriptFrame()) {
        if (IsActiveThreadTerminating()) {
            _Fail_Val.Return();
            return;
        }
        if (Game->CanThingBe_Seen_ByOtherThing(Game->GetHero(), Me)) {
            if (IsActiveThreadTerminating()) {
                _Fail_Val.Return();
                return;
            }
            Game->SetScriptingStateGroup(Me, ESSG_NONE);
            Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
            do {
                Game->NewScriptFrame();
            } while (!IsActiveThreadTerminating());
            {

                _Fail_Val.Return(); return;

            }
        }
        if (ParentClass.QuestFailed || ParentClass.QuestFinished) {
            if (IsActiveThreadTerminating()) {
                _Fail_Val.Return();
                return;
            }
            if (sehMe.IsValid())
                sehMe.Release();
            while (!Game->StartScriptingEntity(Me, sehMe, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) {
                    _Fail_Val.Return();
                    return;
                }
            }
            if (IsActiveThreadTerminating()) {
                _Fail_Val.Return();
                return;
            }
            CScriptThing Exit = Game->GetNearestWithDefName(Me, "REGION_EXIT_POINT");
            if (Exit.IsAlive()) {
                if (IsActiveThreadTerminating()) {
                    _Fail_Val.Return();
                    return;
                }
                if (sehMe.IsValid()) {
                    if (IsActiveThreadTerminating()) {
                        _Fail_Val.Return();
                        return;
                    }
                    sehMe.MoveToPosition(Exit.GetPos(), 1.0f, ENTITY_MOVE_RUN, false, true);
                }
                while (!IsDistanceBetweenThingsUnder(Me, Exit, 2.0f)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) {
                        _Fail_Val.Return();
                        return;
                    }
                }
                if (IsActiveThreadTerminating()) {
                    _Fail_Val.Return();
                    return;
                }
                FadeOutAndKillEntity(Me, 1.0f, true, true);
            }
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif

        }
#ifdef _MSC_VER
#pragma inline_depth()
#endif

    }
}
static jmp_buf stopModel;
static int expectedChecks;
static void checkAt(){model.push_back(CHECK);if(++expectedChecks==stop){model.push_back(FAIL);longjmp(stopModel,1);}}
static void modelStart(){if(bit(4)){model.push_back(START);model.push_back(FRAME);checkAt();}model.push_back(START);checkAt();}
static void modelDistance(bool exit){if(bit(7)){model.push_back(exit?DISTR:DISTW);model.push_back(FRAME);checkAt();}model.push_back(exit?DISTR:DISTW);checkAt();}
static void oracle(){
 bool valid=bit(3);model.push_back(WALKMARK);
 if(bit(0)){checkAt();if(valid)model.push_back(RELEASE);modelStart();model.push_back(MOVEW);modelDistance(false);
 model.push_back(LOOKMARK);if(bit(1)){checkAt();model.push_back(LOOK);}if(!bit(10))model.push_back(RELEASE);valid=false;}
 model.push_back(CENTRE);model.push_back(MINIMUM);model.push_back(MAXIMUM);model.push_back(WANDER);
 for(int n=0;;++n){checkAt();model.push_back(HERO);model.push_back(SEE);
 if(bit(5)&&n>=(bit(6)?1:0)){checkAt();model.push_back(COMBAT);model.push_back(HERO);model.push_back(ENEMY);for(;;){model.push_back(FRAME);checkAt();}}
 if(bit(8)||bit(9)){checkAt();if(valid)model.push_back(RELEASE);modelStart();valid=!bit(10);model.push_back(EXITMARK);
 if(bit(2)){checkAt();if(valid){checkAt();model.push_back(MOVER);}modelDistance(true);model.push_back(FADE);}}
 model.push_back(FRAME);}
}
int main(){
 for(scenario=0;scenario<2048;++scenario)for(stop=1;stop<=32;++stop){
  objects=handles=ordinal=startTries=distanceCalls[0]=distanceCalls[1]=vicheck=frames=expectedChecks=0;trace.clear();model.clear();
  if(setjmp(stopModel)==0)oracle();
  {NScript::CQ_AmbushTradersScript p;p.QuestFailed=bit(8);p.QuestFinished=bit(9);
   NScript::CQ_AmbushTradersScript::CAT_Phase1Guard guard(p);guard.Main();chk(objects==1&&handles==0);}
  chk(objects==0&&handles==0&&trace==model);
 }
 puts("00f03980_TEST PASS");return 0;
}