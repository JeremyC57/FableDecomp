// Selected source 860aa9049ecab1c3631fc2142fb002241c66a8b539d34b37cfe1c621f84a75f8

#include <cassert>
#include <cstdio>
#include <string>
#include <vector>
#include <limits>
struct State{int frame,limit,checks,stopCheck,things,strings,resources,movies,pauseDepth,pauseCalls,predicates,setup,starts,startFailures,taskDelay,taskUntil,releases,talkCounter,presentReads,talkReads,hitReads,anyReads,healReads,possReads,removed,fade,face,story,returnedAt;bool wife,returned,knows,possession,present,talk,hit,ability,heal,shown,stopRemoved,nullItem;float health,meHealth;std::string item;std::vector<int>speech;std::vector<int>events;State():frame(0),limit(6),checks(0),stopCheck(0),things(0),strings(0),resources(0),movies(0),pauseDepth(0),pauseCalls(0),predicates(0),setup(0),starts(0),startFailures(0),taskDelay(0),taskUntil(0),releases(0),talkCounter(0),presentReads(0),talkReads(0),hitReads(0),anyReads(0),healReads(0),possReads(0),removed(0),fade(0),face(0),story(2),returnedAt(99),wife(false),returned(false),knows(false),possession(false),present(false),talk(false),hit(false),ability(false),heal(false),shown(true),stopRemoved(true),nullItem(false),health(10.0f),meHealth(10.0f),item("OBJECT_GHOST_GRANNY_NECKLACE"){}bool term(){++checks;return frame>=limit||(stopCheck&&checks>=stopCheck)||(stopRemoved&&removed>0);}};State*state;
class CCharString{public:std::string s;bool nullValue;CCharString():nullValue(true){++state->strings;}CCharString(const char*p):s(p),nullValue(false){++state->strings;}CCharString(const CCharString&o):s(o.s),nullValue(o.nullValue){++state->strings;}~CCharString(){--state->strings;}bool operator==(const char*p)const{return (nullValue?std::string():s)==p;}};
enum EScriptAIPriority{SCRIPT_AI_PRIORITY_HIGHEST=4};enum ETextGroupSelectionMethod{GROUP_SELECT_NONE=0};enum EHeroAbility{HERO_ABILITY_HEAL_LIFE_SPELL=14};class TRetVoid{public:void Return(){}};
class CScriptThing{public:int id;CScriptThing(int n=-1):id(n){++state->things;}CScriptThing(const CScriptThing&o):id(o.id){++state->things;}~CScriptThing(){--state->things;}void chk(const CCharString&s)const{assert(id==-1&&s.s=="SCRIPT_NAME_HERO");}bool MsgIsPresentedWithItem(CCharString&s)const{assert(id==-1);++state->presentReads;s.s=state->item;s.nullValue=state->nullItem;return state->present;}bool MsgIsTalkedToBy(const CCharString&s)const{chk(s);++state->talkReads;return state->talk;}bool MsgIsHitBy(const CCharString&s)const{chk(s);++state->hitReads;return state->hit;}bool MsgIsHitByAnySpecialAbilityFrom(const CCharString&s)const{chk(s);++state->anyReads;return state->ability;}bool MsgIsHitBySpecialAbilityFrom(EHeroAbility a,const CCharString&s)const{chk(s);assert(a==HERO_ABILITY_HEAL_LIFE_SPELL);++state->healReads;return state->heal;}};
class CScriptGameResourceObjectScriptedThingBase{public:bool valid;CScriptGameResourceObjectScriptedThingBase():valid(false){++state->resources;}~CScriptGameResourceObjectScriptedThingBase(){assert(state->pauseDepth==0);--state->resources;}bool IsValid()const{return valid;}void Release(){assert(valid&&state->pauseDepth==0);valid=false;++state->releases;state->events.push_back(7);}CScriptThing GetScriptThing()const{assert(valid);return CScriptThing(-3);}void Speak(const CScriptThing&h,const char*s,ETextGroupSelectionMethod m,bool a,bool b,bool c){assert(valid&&h.id==-2&&m==GROUP_SELECT_NONE&&!a&&b&&!c&&state->pauseDepth==1&&state->health>0);const char*text[]={"TEXT_QST_026_WIFE_THANKS","TEXT_QST_026_WIFE_GIVE_GIFT","TEXT_QST_026_WIFE_FIND_GIFT","TEXT_QST_026_WIFE_INTRO","TEXT_QST_026_WIFE_INTRO_B","TEXT_QST_026_WIFE_PLEASE_HELP","TEXT_QST_026_WIFE_RUNS_AWAY_01"};int code=-1;for(int i=0;i<7;++i)if(std::string(s)==text[i])code=i;assert(code>=0);state->speech.push_back(code);state->events.push_back(20+code);state->taskUntil=state->frame+state->taskDelay;}bool IsPerformingScriptTask()const{assert(valid);return state->frame<state->taskUntil;}};
class CScriptGameResourceObjectMovieBase{public:CScriptGameResourceObjectMovieBase(){assert(state->movies==0);++state->movies;state->events.push_back(1);}~CScriptGameResourceObjectMovieBase(){assert(state->pauseDepth==0);--state->movies;state->events.push_back(6);}};
class CRGBColour{public:unsigned char R,G,B,A;CRGBColour(unsigned char r,unsigned char g,unsigned char b):R(r),G(g),B(b),A(255){}};
class CGameScriptInterfaceBase{CScriptThing hero;public:CGameScriptInterfaceBase():hero(-2){}void self(const CScriptThing&m)const{assert(m.id==-1);}void NewScriptFrame(){assert(state->frame<255);++state->frame;if(state->frame>=state->returnedAt)state->returned=true;}CScriptThing&GetHero(){return hero;}void EntitySetAsKillable(const CScriptThing&m,bool a,bool b){self(m);assert(!a&&b&&state->setup==0);++state->setup;}void EntitySetAllStategroupsEnabled(const CScriptThing&m,bool b){self(m);assert(!b&&state->setup==1);++state->setup;}void EntitySetOpinionReactionsEnabled(const CScriptThing&m,bool b){self(m);assert(b&&state->setup==2);++state->setup;}void EntitySetDeedReactionsEnabled(const CScriptThing&m,bool b){self(m);assert(b&&state->setup==3);++state->setup;}void EntitySetFacingAngleTowardsThing(const CScriptThing&m,const CScriptThing&h,bool b){self(m);assert(h.id==-2&&b&&state->face==0);++state->face;}bool IsObjectInThingsPossession(const CCharString&s,const CScriptThing&h){assert(s.s=="OBJECT_GHOST_GRANNY_NECKLACE"&&h.id==-2&&!state->returned);++state->possReads;return state->possession;}bool StartScriptingEntity(const CScriptThing&m,CScriptGameResourceObjectScriptedThingBase&r,EScriptAIPriority p){self(m);assert(p==SCRIPT_AI_PRIORITY_HIGHEST&&!r.valid);return r.valid=++state->starts>state->startFailures;}void StartMovieSequence(const CCharString&s,CScriptGameResourceObjectMovieBase&){assert(s.s.empty()&&state->movies==1);state->events.push_back(2);}void PauseAllNonScriptedEntities(bool b){if(b){assert(state->pauseDepth==0);++state->pauseDepth;++state->pauseCalls;state->events.push_back(3);}else{assert(state->pauseDepth==1);--state->pauseDepth;state->events.push_back(5);}}float GetHealth(const CScriptThing&m){assert(m.id==-1||m.id==-3);return m.id==-1?state->meHealth:state->health;}bool FadeScreenOut(float a,float b,CRGBColour c){assert(a==1&&b==1&&c.R==0&&c.G==0&&c.B==0&&c.A==255&&state->pauseDepth==1&&state->fade==0);state->fade=1;state->events.push_back(8);return true;}void Pause(float n){assert(n==2&&state->fade==1&&state->pauseDepth==1);state->fade=2;state->events.push_back(9);}void RemoveThing(const CScriptThing&m,bool a,bool b){self(m);assert(!a&&!b&&state->fade==2&&state->pauseDepth==1);++state->removed;state->fade=3;state->events.push_back(10);}void FadeScreenIn(){assert(state->fade==3&&state->pauseDepth==1);state->fade=0;state->events.push_back(11);}};
class CWideScreenMagicPauseEntities{CGameScriptInterfaceBase*Game;public:CWideScreenMagicPauseEntities(CGameScriptInterfaceBase&g):Game(&g){Game->PauseAllNonScriptedEntities(true);}~CWideScreenMagicPauseEntities(){Game->PauseAllNonScriptedEntities(false);}};
namespace NScript{class CIsThingAlive{CScriptThing Thing;public:CIsThingAlive(const CScriptThing&m):Thing(m){assert(m.id==-1);}};class CQ_SunnyvaleMasterData{public:long OrchardFarmRaidLastCompleted;CQ_SunnyvaleMasterData():OrchardFarmRaidLastCompleted(state->story){}};class CV_GhostGrannyNecklaceScript{public:bool&WifeNeededForCutscene;bool&NecklaceReturned;bool&HeroKnowsAboutNecklace;CV_GhostGrannyNecklaceScript():WifeNeededForCutscene(state->wife),NecklaceReturned(state->returned),HeroKnowsAboutNecklace(state->knows){}class CGhostGrannyDaughterInLaw{public:CGameScriptInterfaceBase&Game;CScriptThing Me;CV_GhostGrannyNecklaceScript&ParentClass;CQ_SunnyvaleMasterData*PMasterData;int&TalkCounter;bool&ShownItem;CGhostGrannyDaughterInLaw(CGameScriptInterfaceBase&g,CV_GhostGrannyNecklaceScript&p,CQ_SunnyvaleMasterData&d):Game(g),Me(-1),ParentClass(p),PMasterData(&d),TalkCounter(state->talkCounter),ShownItem(state->shown){}void SetPredicate(const CIsThingAlive&){++state->predicates;}bool IsActiveThreadTerminating()const{return state->term();}void Main();};};}
void NScript::CV_GhostGrannyNecklaceScript::CGhostGrannyDaughterInLaw::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    Game.EntitySetAsKillable(Me,false,true);
    Game.EntitySetAllStategroupsEnabled(Me,false);
    Game.EntitySetOpinionReactionsEnabled(Me,true);
    Game.EntitySetDeedReactionsEnabled(Me,true);
    CCharString PresentedItem;
    Game.EntitySetFacingAngleTowardsThing(Me,Game.GetHero(),true);
    for (; !false; Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (ParentClass.WifeNeededForCutscene) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (seh_me.IsValid()) seh_me.Release();
        }
        if (Me.MsgIsPresentedWithItem(PresentedItem) && PresentedItem=="OBJECT_GHOST_GRANNY_NECKLACE") {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            ShownItem=true;
        } else {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            ShownItem=false;
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO") || ShownItem) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            {
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (ParentClass.NecklaceReturned) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_026_WIFE_THANKS",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                } else if (!ParentClass.NecklaceReturned && Game.IsObjectInThingsPossession("OBJECT_GHOST_GRANNY_NECKLACE",Game.GetHero())) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_026_WIFE_GIVE_GIFT",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                } else if (ParentClass.HeroKnowsAboutNecklace) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_026_WIFE_FIND_GIFT",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (TalkCounter==0) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (PMasterData->OrchardFarmRaidLastCompleted==2) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                                seh_me.Speak(Game.GetHero(),"TEXT_QST_026_WIFE_INTRO",GROUP_SELECT_NONE,false,true,false);
                                while (seh_me.IsPerformingScriptTask()) {
                                    Game.NewScriptFrame();
                                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                }
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                        } else {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                                seh_me.Speak(Game.GetHero(),"TEXT_QST_026_WIFE_INTRO_B",GROUP_SELECT_NONE,false,true,false);
                                while (seh_me.IsPerformingScriptTask()) {
                                    Game.NewScriptFrame();
                                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                }
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                        }
                    }
                    if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_026_WIFE_PLEASE_HELP",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
            }
            ++TalkCounter;
            if (seh_me.IsValid()) seh_me.Release();
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (Game.GetHealth(Me)<=1.0f) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                {
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game.StartMovieSequence("",speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                    if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_026_WIFE_RUNS_AWAY_01",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    Game.FadeScreenOut(1.0f,1.0f,CRGBColour(0,0,0));
                    Game.Pause(2.0f);
                    Game.RemoveThing(Me,false,false);
                    Game.FadeScreenIn();
                }
            }
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}


int scenarios=0;void run(State&s){++scenarios;state=&s;{CGameScriptInterfaceBase game;NScript::CQ_SunnyvaleMasterData data;NScript::CV_GhostGrannyNecklaceScript parent;NScript::CV_GhostGrannyNecklaceScript::CGhostGrannyDaughterInLaw daughter(game,parent,data);daughter.Main();}assert(s.things==0&&s.strings==0&&s.resources==0&&s.movies==0&&s.pauseDepth==0&&s.predicates==1&&s.fade==0);if(s.frame>1)assert(s.setup==4&&s.face==1);}
int count(const State&s,int code){int n=0;for(unsigned i=0;i<s.speech.size();++i)if(s.speech[i]==code)++n;return n;}
int main(){
 {State s;run(s);assert(s.speech.empty()&&s.starts==0&&!s.shown&&s.presentReads==5);}
 {State s;s.talk=true;run(s);assert(s.talkCounter==5&&s.releases==5&&s.pauseCalls==5&&count(s,3)==1&&count(s,5)==5&&s.speech.size()==6);}
 for(int flags=0;flags<256;++flags){State s;s.limit=2;s.returned=(flags&1)!=0;s.knows=(flags&2)!=0;s.possession=(flags&4)!=0;s.present=(flags&8)!=0;s.talk=(flags&16)!=0;s.wife=(flags&32)!=0;s.story=(flags&64)?0:2;s.talkCounter=(flags&128)?4:0;int before=s.talkCounter;run(s);bool engaged=s.talk||s.present;assert(s.shown==s.present&&s.talkCounter==before+(engaged?1:0)&&s.pauseCalls==(engaged?1:0)&&s.releases==(engaged?1:0));if(engaged){if(s.returned)assert(s.speech.size()==1&&s.speech[0]==0&&s.possReads==0);else if(s.possession)assert(s.speech.size()==1&&s.speech[0]==1);else if(s.knows)assert(s.speech.size()==1&&s.speech[0]==2);else{assert(s.speech.size()==static_cast<unsigned>(before?1:2)&&s.speech.back()==5);if(!before)assert(s.speech[0]==(s.story==2?3:4));}}else assert(s.speech.empty());}
 const char*items[]={"OBJECT_GHOST_GRANNY_NECKLACE","object_ghost_granny_necklace","OBJECT_GHOST_GRANNY_NECKLACE_X","","OBJECT_GHOST_GRANNY_NECKLAC"};
 for(int p=0;p<2;++p)for(int t=0;t<2;++t)for(int n=0;n<2;++n)for(int i=0;i<5;++i){State s;s.limit=2;s.present=p!=0;s.talk=t!=0;s.nullItem=n!=0;s.item=items[i];run(s);bool show=p&&!n&&i==0;assert(s.shown==show&&s.talkCounter==(t||show?1:0));}
 float values[7]={-1.0f,0.0f,0.5f,1.0f,1.5f,10.0f,std::numeric_limits<float>::quiet_NaN()};
 for(int bits=0;bits<8;++bits)for(int m=0;m<7;++m)for(int h=0;h<7;++h){State s;s.limit=2;s.hit=(bits&1)!=0;s.ability=(bits&2)!=0;s.heal=(bits&4)!=0;s.meHealth=values[m];s.health=values[h];run(s);bool flee=(s.hit||(s.ability&&!s.heal))&&m<4;assert(s.removed==(flee?1:0)&&s.pauseCalls==(flee?1:0)&&s.talkCounter==0);assert(count(s,6)==(flee&&h>=2&&h<6?1:0));if(s.hit)assert(s.anyReads==0&&s.healReads==0);if(!s.hit&&!s.ability)assert(s.healReads==0);}
 for(int h=0;h<7;++h)for(int path=0;path<5;++path){State s;s.limit=2;s.talk=true;s.health=values[h];s.returned=path==0;s.possession=path==1;s.knows=path==2;s.story=path==4?0:2;run(s);bool positive=h>=2&&h<6;assert(s.talkCounter==1&&s.releases==1&&s.pauseCalls==1&&s.speech.size()==static_cast<unsigned>(positive?(path<3?1:2):0));}
 for(int fail=0;fail<6;++fail)for(int delay=0;delay<6;++delay){State s;s.limit=20;s.talk=true;s.returned=true;s.startFailures=fail;s.taskDelay=delay;run(s);assert(!s.speech.empty()&&s.starts>fail&&count(s,0)==static_cast<int>(s.speech.size()));}
 {State s;s.talk=true;s.hit=true;s.meHealth=1;run(s);assert(s.removed==1&&s.talkCounter==1&&s.pauseCalls==2&&s.releases==1&&s.speech.size()==3&&s.speech[0]==3&&s.speech[1]==5&&s.speech[2]==6);}
 {State s;s.talk=true;s.returnedAt=3;run(s);assert(count(s,3)==1&&count(s,5)==2&&count(s,0)==3&&s.talkCounter==5);}
 {State s;s.hit=true;s.meHealth=1;s.stopRemoved=false;s.wife=true;s.limit=4;run(s);assert(s.removed==3&&s.releases==2&&s.talkCounter==0);}
 for(int stop=1;stop<=200;++stop)for(int path=0;path<20;++path){State s;s.stopCheck=stop;s.limit=10;s.talk=true;switch(path){case 0:s.returned=true;break;case 1:s.possession=true;break;case 2:s.knows=true;break;case 3:break;case 4:s.story=0;break;case 5:s.talkCounter=4;break;case 6:s.wife=true;break;case 7:s.talk=false;s.present=true;break;case 8:s.taskDelay=3;break;case 9:s.startFailures=30;break;case 10:s.health=0;break;case 11:s.talk=false;s.hit=true;s.meHealth=1;break;case 12:s.hit=true;s.meHealth=1;break;case 13:s.ability=true;s.meHealth=0.5f;s.taskDelay=3;break;case 14:s.talk=false;s.ability=true;s.heal=true;break;case 15:s.returnedAt=4;break;case 16:s.present=true;s.item="OTHER";break;case 17:s.present=true;s.nullItem=true;break;case 18:s.health=std::numeric_limits<float>::quiet_NaN();break;case 19:s.talk=false;break;}run(s);}
 assert(scenarios==4764);std::puts("00e8bef0_TEST PASS");return 0;
}