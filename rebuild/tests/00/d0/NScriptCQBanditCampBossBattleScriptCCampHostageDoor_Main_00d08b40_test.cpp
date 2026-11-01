// Fixture (agent680: target body replaced with src/00d08b40.cpp's).
// 00d08b40 behavior fixture: exact selected target body pasted unchanged.
// Real script handles and predicate lifetime; external game methods use a checked state model.
// Native dispatch/ABI and external action implementations are separate evidence obligations.
#include <stdio.h>
#include <string.h>
static int failures=0;static void Check(bool b){if(!b)++failures;}
struct CTestThing{int id,refs;bool alive;CTestThing(int n,bool a):id(n),refs(0),alive(a){}};
static int liveHandles=0,predicateLives=0;
class CScriptThing{CTestThing*data;public:CScriptThing():data(0){++liveHandles;}explicit CScriptThing(CTestThing*p):data(p){++liveHandles;if(data)++data->refs;}CScriptThing(const CScriptThing&o):data(o.data){++liveHandles;if(data)++data->refs;}~CScriptThing(){if(data)--data->refs;--liveHandles;}CScriptThing&operator=(const CScriptThing&o){if(data)--data->refs;data=o.data;if(data)++data->refs;return*this;}bool IsAlive()const;bool MsgIsUsedBy(const char*)const;int ID()const{return data?data->id:-1;}bool AliveValue()const{return data&&data->alive;}};
namespace NScript{class CIsThingAlive{public:CScriptThing Thing;CIsThingAlive(const CScriptThing&t):Thing(t){++predicateLives;}~CIsThingAlive(){--predicateLives;}};class CQ_BanditCampBossBattleScript{public:bool HostagesRescued,PlayGuardTooCloseCutscene;CQ_BanditCampBossBattleScript():HostagesRescued(false),PlayGuardTooCloseCutscene(false){}class CCampHostageDoor;};}
struct CTestModel{int mode,stopAt,checks,frames,predicates,guardGets,heroGets,uses,keyChecks,aliveChecks,distanceChecks,infos,clickedChecks,cutsceneFrames,experience,taken;bool terminating,clicked,near,hasKey;int closeCompletions;NScript::CQ_BanditCampBossBattleScript*parent;CScriptThing*hero;CScriptThing*guard;
 CTestModel():mode(0),stopAt(0),checks(0),frames(0),predicates(0),guardGets(0),heroGets(0),uses(0),keyChecks(0),aliveChecks(0),distanceChecks(0),infos(0),clickedChecks(0),cutsceneFrames(0),experience(0),taken(0),terminating(false),clicked(false),near(false),hasKey(false),closeCompletions(0),parent(0),hero(0),guard(0){}
 void SideEffect(){Check(!terminating&&predicateLives==0);}bool CheckTermination(){Check(predicateLives==0);++checks;if(checks==stopAt)terminating=true;return terminating;}
};
static CTestModel*model;
bool CScriptThing::IsAlive()const{model->SideEffect();++model->aliveChecks;Check(ID()==2||ID()==-1);return AliveValue();}
bool CScriptThing::MsgIsUsedBy(const char*name)const{model->SideEffect();Check(ID()==1&&strcmp(name,"SCRIPT_NAME_HERO")==0&&!model->parent->HostagesRescued);++model->uses;return model->mode!=1;}
bool IsDistanceBetweenThingsUnder(const CScriptThing&a,const CScriptThing&b,float distance){model->SideEffect();Check(a.ID()==0&&b.ID()==2&&distance==5.0f&&b.AliveValue());++model->distanceChecks;return model->near;}
class CGameScriptInterfaceBase{public:void NewScriptFrame(){model->SideEffect();++model->frames;if(model->parent->PlayGuardTooCloseCutscene){++model->cutsceneFrames;if(model->cutsceneFrames%2==0){model->parent->PlayGuardTooCloseCutscene=false;model->near=false;++model->closeCompletions;}}}CScriptThing GetThingWithScriptName(const char*s){model->SideEffect();Check(strcmp(s,"CampHostageGuard")==0&&model->guardGets==0&&model->frames==1);++model->guardGets;return *model->guard;}CScriptThing GetHero(){model->SideEffect();++model->heroGets;return *model->hero;}bool IsObjectInThingsPossession(const char*s,const CScriptThing&t){model->SideEffect();Check(strcmp(s,"OBJECT_BANDIT_CAMP_HOSTAGE_KEY")==0&&t.ID()==0);++model->keyChecks;return model->hasKey;}void GiveHeroExperience(long n){model->SideEffect();Check(n==256&&!model->parent->HostagesRescued&&model->hasKey&&(!model->guard->AliveValue()||!model->near));Check(model->experience==0);++model->experience;}void TakeObjectFromHero(const char*s){model->SideEffect();Check(strcmp(s,"OBJECT_BANDIT_CAMP_HOSTAGE_KEY")==0&&model->parent->HostagesRescued&&model->experience==1&&model->hasKey);Check(model->taken==0);++model->taken;model->hasKey=false;}void DisplayGameInfo(const char*s){model->SideEffect();Check(strcmp(s,"TEXT_QST_009_NEED_KEY")==0&&!model->hasKey&&!model->parent->HostagesRescued);++model->infos;}bool MsgIsGameInfoClickedPast(){model->SideEffect();Check(model->infos==1);++model->clickedChecks;if(model->clickedChecks==3){model->clicked=true;model->hasKey=true;}return model->clicked;}};
static CGameScriptInterfaceBase game;static CGameScriptInterfaceBase*Game=&game;
class CScriptDef{public:long EXPERIENCE_GRANT_LARGE;};template<class T>class CDefPointer{public:T*ptr;T*operator->()const{return ptr;}};static CScriptDef globalDef;static CDefPointer<const CScriptDef>pGlobalScriptDef;
namespace NScript{
class CEntityScriptBase{public:CScriptThing Me;CEntityScriptBase(const CScriptThing&m):Me(m){}bool IsActiveThreadTerminating(){return model->CheckTermination();}void SetPredicate(const CIsThingAlive&p){Check(!model->terminating&&predicateLives==1&&p.Thing.ID()==Me.ID()&&model->predicates==0);++model->predicates;}};
class CQ_BanditCampBossBattleScript::CCampHostageDoor:public CEntityScriptBase{protected:CQ_BanditCampBossBattleScript&ParentClass;private:void Main();public:CCampHostageDoor(const CScriptThing&m,CQ_BanditCampBossBattleScript&p):CEntityScriptBase(m),ParentClass(p){}void Run(){Main();}};
void CQ_BanditCampBossBattleScript::CCampHostageDoor::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptThing guard = Game->GetThingWithScriptName("CampHostageGuard");
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        if (!ParentClass.HostagesRescued && Me.MsgIsUsedBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) {
#pragma inline_depth(0)
                    return;
#pragma inline_depth(255)
            }
            if (Game->IsObjectInThingsPossession("OBJECT_BANDIT_CAMP_HOSTAGE_KEY", Game->GetHero())) {
                if (IsActiveThreadTerminating()) {
#pragma inline_depth(0)
                    return;
#pragma inline_depth(255)
                }
                if (guard.IsAlive() && IsDistanceBetweenThingsUnder(Game->GetHero(), guard, 5.0f)) {
                    if (IsActiveThreadTerminating()) {
#pragma inline_depth(0)
                    return;
#pragma inline_depth(255)
                    }
                    ParentClass.PlayGuardTooCloseCutscene = true;
                    while (ParentClass.PlayGuardTooCloseCutscene) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) {
#pragma inline_depth(0)
                    return;
#pragma inline_depth(255)
                        }
                    }
                } else {
                    if (IsActiveThreadTerminating()) {
#pragma inline_depth(0)
                    return;
#pragma inline_depth(255)
                    }
                    Game->GiveHeroExperience(pGlobalScriptDef->EXPERIENCE_GRANT_LARGE);
                    ParentClass.HostagesRescued = true;
                    Game->TakeObjectFromHero("OBJECT_BANDIT_CAMP_HOSTAGE_KEY");
#pragma inline_depth(0)
                    return;
#pragma inline_depth(255)
                }
            } else {
                if (IsActiveThreadTerminating()) {
#pragma inline_depth(0)
                    return;
#pragma inline_depth(255)
                }
                Game->DisplayGameInfo("TEXT_QST_009_NEED_KEY");
                while (!Game->MsgIsGameInfoClickedPast()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) {
#pragma inline_depth(0)
                    return;
#pragma inline_depth(255)
                    }
                }
            }
            if (IsActiveThreadTerminating()) {
#pragma inline_depth(0)
                    return;
#pragma inline_depth(255)
            }
        }
        Game->NewScriptFrame();
    }
}

}

int main(){unsigned int cases=0;globalDef.EXPERIENCE_GRANT_LARGE=256;pGlobalScriptDef.ptr=&globalDef;
 for(int mode=0;mode<6;++mode)for(int stop=1;stop<=24;++stop)for(int guardMissing=0;guardMissing<2;++guardMissing){
  Check(liveHandles==0&&predicateLives==0);CTestThing heroData(0,true),doorData(1,true),guardData(2,mode!=4);{
   CScriptThing hero(&heroData),door(&doorData),guard(guardMissing?0:&guardData);NScript::CQ_BanditCampBossBattleScript quest;quest.HostagesRescued=mode==0;CTestModel state;state.mode=mode;state.stopAt=stop;state.parent=&quest;state.hero=&hero;state.guard=&guard;state.hasKey=mode!=2;state.near=mode==3;model=&state;
   {NScript::CQ_BanditCampBossBattleScript::CCampHostageDoor script(door,quest);int originalHandles=liveHandles;script.Run();Check(liveHandles==originalHandles&&predicateLives==0);}
   Check(state.predicates==1&&state.frames>=1);Check(state.guardGets==(stop==1?0:1));Check(state.experience==state.taken);Check(!state.experience||quest.HostagesRescued);Check(state.experience<=1&&state.infos<=1&&state.closeCompletions<=1);Check(state.terminating?state.checks==stop:state.experience==1);if(mode==0||mode==1){Check(state.terminating&&state.experience==0&&state.keyChecks==0&&state.infos==0&&state.distanceChecks==0);}if(mode!=2)Check(state.infos==0&&state.clickedChecks==0);if(mode!=3||guardMissing||mode==4)Check(state.cutsceneFrames==0);if(stop==24&&mode>=2)Check(state.experience==1);Check(heroData.refs==1&&doorData.refs==1&&guardData.refs==(guardMissing?0:1));++cases;
  }Check(liveHandles==0&&predicateLives==0);Check(heroData.refs==0&&doorData.refs==0&&guardData.refs==0);
 }
 Check(cases==288);if(failures){fprintf(stderr,"failures=%d cases=%u\n",failures,cases);return 1;}puts("00d08b40_TEST PASS");return 0;
}