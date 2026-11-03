#include <cassert>
#include <cstdio>
#include <cstring>
#include <vector>
#include <string>
#include <limits>
struct Event { bool physical,special,heal,talk;float damage;Event(bool p,bool s,bool h,bool t,float d):physical(p),special(s),heal(h),talk(t),damage(d){} };
struct State {std::vector<Event> plan;std::vector<std::string> speech;int step,checks,killAt,acquireDelay,acquireCalls,taskDelay,tasks,frames,randomValue,pauses,movies,releases,predicates,opened;bool scriptedAlive,preowned,exitActive;float health;std::vector<char> life;
 State():step(-1),checks(0),killAt(0),acquireDelay(0),acquireCalls(0),taskDelay(0),tasks(0),frames(0),randomValue(0),pauses(0),movies(0),releases(0),predicates(0),opened(0),scriptedAlive(true),preowned(false),exitActive(false),health(0){}
 const Event&Current()const{assert(step>=0&&step<(int)plan.size());return plan[step];}
};
static State* gState;
class CScriptThing {public:int id;CScriptThing(int n=0):id(n){}bool MsgIsHitBy(const char*s){assert(id==1&&std::strcmp(s,"SCRIPT_NAME_HERO")==0);++gState->step;const Event&e=gState->Current();if(e.physical||e.special)gState->health-=e.damage;return e.physical;}
 bool MsgIsHitByAnySpecialAbilityFrom(const char*s){assert(std::strcmp(s,"SCRIPT_NAME_HERO")==0);return gState->Current().special;}
 bool MsgIsHitBySpecialAbilityFrom(int n,const char*s){assert(n==14&&std::strcmp(s,"SCRIPT_NAME_HERO")==0);return gState->Current().heal;}
 bool MsgIsTalkedToBy(const char*s){assert(std::strcmp(s,"SCRIPT_NAME_HERO")==0);return gState->Current().talk;}
};
class CScriptGameResourceObjectMovieBase {public:CScriptGameResourceObjectMovieBase(){assert(gState->movies==0);++gState->movies;gState->life.push_back('M');}~CScriptGameResourceObjectMovieBase(){assert(gState->movies==1&&gState->pauses==0);--gState->movies;gState->life.push_back('m');}};
class CScriptGameResourceObjectScriptedThingBase {bool valid;public:CScriptGameResourceObjectScriptedThingBase():valid(gState->preowned){gState->life.push_back('S');}~CScriptGameResourceObjectScriptedThingBase(){assert(!gState->movies&&!gState->pauses);gState->life.push_back('s');}bool IsValid()const{return valid;}void Release(){assert(valid);valid=false;++gState->releases;}void SetValid(){valid=true;}
 CScriptThing GetScriptThing()const{return CScriptThing(4);}bool IsPerformingScriptTask()const{return gState->tasks>0;}
 void Speak(const CScriptThing&h,const char*text,int group,bool a,bool b,bool c){assert(valid&&h.id==3&&group==0&&!a&&b&&!c&&gState->pauses==1&&gState->movies==1);gState->speech.push_back(text);gState->tasks=gState->taskDelay;}
};
class CGameScriptInterfaceBase {public:void NewScriptFrame(){++gState->frames;if(gState->tasks)--gState->tasks;}bool StartScriptingEntity(const CScriptThing&m,CScriptGameResourceObjectScriptedThingBase&r,int p){assert(m.id==1&&p==4);if(gState->acquireCalls++<gState->acquireDelay)return false;r.SetValid();return true;}
 void SetThingPersistent(const CScriptThing&m,bool b){assert(m.id==1&&b);}void EntitySetMaxHealth(const CScriptThing&m,float h,bool b){assert(m.id==1&&h==1000&&b);}void ModifyThingHealth(const CScriptThing&m,float h,bool b){assert(m.id==1&&h==1000&&!b);gState->health=1000;}
 void EntitySetAsKillable(const CScriptThing&m,bool a,bool b){assert(m.id==1&&!a&&!b);}void EntitySetAsToAddToComboMultiplierWhenHit(const CScriptThing&m,bool b){assert(m.id==1&&!b);}
 CScriptThing GetNearestWithDefName(const CScriptThing&m,const char*s){assert(m.id==1&&std::strcmp(s,"REGION_EXIT_POINT")==0);return CScriptThing(2);}void SetRegionExitAsActive(const CScriptThing&r,bool b){assert(r.id==2);gState->exitActive=b;}
 float GetHealth(const CScriptThing&m){assert(m.id==1||m.id==4);return m.id==4?(gState->scriptedAlive?1.0f:0.0f):gState->health;}
 void StartMovieSequence(const char*s,CScriptGameResourceObjectMovieBase&){assert(!*s&&gState->movies==1);}CScriptThing GetHero(){return CScriptThing(3);}void PauseAllNonScriptedEntities(bool b){if(b){assert(gState->pauses==0&&gState->movies==1);++gState->pauses;gState->life.push_back('P');}else{assert(gState->pauses==1&&gState->movies==1);--gState->pauses;gState->life.push_back('p');}}
};
class CWideScreenMagicPauseEntities {CGameScriptInterfaceBase*Game;public:CWideScreenMagicPauseEntities(CGameScriptInterfaceBase&g):Game(&g){Game->PauseAllNonScriptedEntities(true);}~CWideScreenMagicPauseEntities(){Game->PauseAllNonScriptedEntities(false);}};
class CScriptDef {public:float DemonDoor_DoDamage_DamageLimit;};
template<class T>class CDefPointer{const T*p;public:CDefPointer(const T*x):p(x){}const T*operator->()const{return p;}};
static CScriptDef definition;static CDefPointer<const CScriptDef>pGlobalScriptDef(&definition);
class TRetVoid{public:void Return(){}};
namespace NScript {class CIsThingAlive{public:const CScriptThing&Thing;CIsThingAlive(const CScriptThing&t):Thing(t){}};class CV_DemonDoorsScript{public:void FadeOutFaceAndOpenDoor(const CScriptThing&m,const char*s){assert(m.id==1&&std::strcmp(s,"DD_DoDamage")==0);++gState->opened;}class CDF_DoDamage{public:CGameScriptInterfaceBase&Game;CV_DemonDoorsScript&ParentClass;CScriptThing Me;CDF_DoDamage(CGameScriptInterfaceBase&g,CV_DemonDoorsScript&p):Game(g),ParentClass(p),Me(1){}void SetPredicate(const CIsThingAlive&p){assert(p.Thing.id==1);++gState->predicates;}bool IsActiveThreadTerminating()const{assert(gState->checks<300);return ++gState->checks==gState->killAt;}void Main();};};}
enum EScriptAIPriority{SCRIPT_AI_PRIORITY_HIGHEST=4};enum EHeroAbility{HERO_ABILITY_HEAL_LIFE_SPELL=14};enum ETextGroupSelectionMethod{GROUP_SELECT_NONE=0};
static int PlannedRand(){return gState->randomValue;}
#define rand PlannedRand
void NScript::CV_DemonDoorsScript::CDF_DoDamage::Main()
{
    TRetVoid _Fail_Val;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if(IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
    if(seh_me.IsValid()) seh_me.Release();
    while(!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if(IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
    }
    if(IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
    Game.SetThingPersistent(Me,true);
    Game.EntitySetMaxHealth(Me,1000.0f,true);
    Game.ModifyThingHealth(Me,1000.0f,false);
    Game.EntitySetAsKillable(Me,false,false);
    Game.EntitySetAsToAddToComboMultiplierWhenHit(Me,false);
    CScriptThing region_exit=Game.GetNearestWithDefName(Me,"REGION_EXIT_POINT");
    Game.SetRegionExitAsActive(region_exit,false);
    float start_health=Game.GetHealth(Me);
    for(;true;Game.NewScriptFrame()) {
        if(IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
        if(Me.MsgIsHitBy("SCRIPT_NAME_HERO") ||
            (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") &&
             !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
            if(IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            float new_health=Game.GetHealth(Me);
            if(start_health-new_health>pGlobalScriptDef->DemonDoor_DoDamage_DamageLimit) {
                if(IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
                if(Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_088_DO_DAMAGE_OPENING",GROUP_SELECT_NONE,false,true,false);
                    while(seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if(IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
                    }
                    if(IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
                }
                ParentClass.FadeOutFaceAndOpenDoor(Me,"DD_DoDamage");
                Game.SetRegionExitAsActive(region_exit,true);
                return;
            } else {
                if(IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
                if(rand()%2) {
                    if(IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
                    if(Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_088_DO_DAMAGE_TOO_WEAK_A",GROUP_SELECT_NONE,false,true,false);
                        while(seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if(IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
                        }
                        if(IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
                    }
                } else {
                    if(IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
                    if(Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_088_DO_DAMAGE_TOO_WEAK_B",GROUP_SELECT_NONE,false,true,false);
                        while(seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if(IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
                        }
                        if(IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
                    }
                }
                Game.ModifyThingHealth(Me,1000.0f,false);
                start_health=Game.GetHealth(Me);
            }
        }
        if(Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if(IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if(Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                seh_me.Speak(Game.GetHero(),"TEXT_QST_088_DO_DAMAGE_INTRO",GROUP_SELECT_NONE,false,true,false);
                while(seh_me.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if(IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
                }
                if(IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
            }
        }
    }
}

#undef rand
static const char*Opening="TEXT_QST_088_DO_DAMAGE_OPENING";
static const char*Intro="TEXT_QST_088_DO_DAMAGE_INTRO";
static const char*WeakA="TEXT_QST_088_DO_DAMAGE_TOO_WEAK_A";
static const char*WeakB="TEXT_QST_088_DO_DAMAGE_TOO_WEAK_B";
static std::vector<std::string> Expected(int plan,int parity,bool alive){std::vector<std::string>x;if(!alive)return x;if(plan==1||plan==5||plan==6||plan==8||plan==9)x.push_back(parity?WeakA:WeakB);if(plan==4||plan==5||plan==7||plan==8)x.push_back(Intro);x.push_back(Opening);return x;}
static void SetPlan(State&s,int n){
 if(n==1||n==5)s.plan.push_back(Event(true,false,false,n==5,8));
 if(n==3||n==7)s.plan.push_back(Event(false,true,true,n==7,0));
 if(n==4)s.plan.push_back(Event(false,false,false,true,0));
 if(n==6)s.plan.push_back(Event(true,false,false,false,16));
 if(n==8)s.plan.push_back(Event(false,true,false,true,8));
 if(n==9)s.plan.push_back(Event(true,false,false,false,std::numeric_limits<float>::quiet_NaN()));
 s.plan.push_back(Event(n!=2,n==2,false,false,32));
}
static int Run(int plan,int acquire,int delay,int parity,bool alive,bool preowned,int killAt){State s;gState=&s;s.acquireDelay=acquire;s.taskDelay=delay;s.randomValue=parity;s.scriptedAlive=alive;s.preowned=preowned;s.killAt=killAt;SetPlan(s,plan);definition.DemonDoor_DoDamage_DamageLimit=16;CGameScriptInterfaceBase game;NScript::CV_DemonDoorsScript owner;NScript::CV_DemonDoorsScript::CDF_DoDamage door(game,owner);door.Main();assert(s.predicates==1&&s.pauses==0&&s.movies==0&&s.opened<=1&&s.exitActive==(s.opened==1));assert(!s.life.empty()&&s.life.front()=='S'&&s.life.back()=='s');
 int m=0,p=0;for(unsigned long i=0;i<s.life.size();++i){char c=s.life[i];if(c=='M'){assert(!m&&!p);m=1;}if(c=='P'){assert(m&&!p);p=1;}if(c=='p'){assert(m&&p);p=0;}if(c=='m'){assert(m&&!p);m=0;}}assert(!m&&!p);
 std::vector<std::string>expected=Expected(plan,parity,alive);assert(s.speech.size()<=expected.size());for(unsigned long i=0;i<s.speech.size();++i)assert(s.speech[i]==expected[i]);
 if(killAt==0||killAt>s.checks){assert(s.opened==1&&s.speech==expected&&s.acquireCalls==acquire+1&&s.releases==(preowned?1:0));}else{assert(s.checks==killAt&&!s.opened);}
 return s.checks;
}
int main(){unsigned long scenarios=0;for(int plan=0;plan<10;++plan)for(int ac=0;ac<2;++ac)for(int delay=0;delay<3;++delay)for(int parity=0;parity<2;++parity)for(int alive=0;alive<2;++alive)for(int preowned=0;preowned<2;++preowned){int n=Run(plan,ac*2,delay==2?3:delay,parity,alive!=0,preowned!=0,0);++scenarios;for(int k=1;k<=n+1;++k){Run(plan,ac*2,delay==2?3:delay,parity,alive!=0,preowned!=0,k);++scenarios;}}assert(scenarios>5000);std::fprintf(stderr,"SCENARIOS %lu\n",scenarios);std::puts("00e70bc0_TEST PASS");}