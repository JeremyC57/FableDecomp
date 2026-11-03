#include <cstdio>
#include <cassert>
#include <string>
#include <vector>
struct Event {bool talk,knight,mage,bandit;Event():talk(false),knight(false),mage(false),bandit(false){}};
struct State {Event e[512];int frame,limit,checks,stopCheck,starts,startFailures,resources,movies,pauses,predicates,things,releases,taskFrames,taskUntil,persistent,damageable,combo,killable,opened,regionOff,regionOn,healthChecks;bool initialValid,healthAfterBandit;float health;std::vector<std::string>text;std::vector<int>suits;State():frame(0),limit(8),checks(0),stopCheck(0),starts(0),startFailures(0),resources(0),movies(0),pauses(0),predicates(0),things(0),releases(0),taskFrames(0),taskUntil(0),persistent(0),damageable(0),combo(0),killable(0),opened(0),regionOff(0),regionOn(0),healthChecks(0),initialValid(false),healthAfterBandit(false),health(1){}bool term(){++checks;return frame>=limit||(stopCheck&&checks>=stopCheck);}};State*state;
class CCharString{public:std::string text;CCharString(const char*s):text(s){}};
enum EScriptAIPriority{SCRIPT_AI_PRIORITY_HIGHEST=4};enum ETextGroupSelectionMethod{GROUP_SELECT_NONE=0};class TRetVoid{public:void Return(){}};
class CScriptThing {public:int kind;CScriptThing(int k=0):kind(k){++state->things;}CScriptThing(const CScriptThing&x):kind(x.kind){++state->things;}~CScriptThing(){--state->things;}bool MsgIsTalkedToBy(const CCharString&s)const{assert(kind==0&&s.text=="SCRIPT_NAME_HERO");return state->e[state->frame].talk;}};
class CScriptGameResourceObjectScriptedThingBase{public:bool valid;CScriptGameResourceObjectScriptedThingBase():valid(state->initialValid){++state->resources;}~CScriptGameResourceObjectScriptedThingBase(){--state->resources;}bool IsValid()const{return valid;}void Release(){assert(valid);valid=false;++state->releases;}CScriptThing GetScriptThing()const{return CScriptThing(3);}bool IsPerformingScriptTask()const{return state->frame<state->taskUntil;}
 void Speak(const CScriptThing&h,const char*s,ETextGroupSelectionMethod m,bool a,bool b,bool c){assert(h.kind==1&&m==GROUP_SELECT_NONE&&!a&&b&&!c&&state->pauses==1&&state->movies==1&&valid);state->text.push_back(s);state->taskUntil=state->frame+state->taskFrames;if(state->healthAfterBandit&&std::string(s)=="TEXT_QST_088_THREE_SUITS_SEEN_BANDIT")state->health=0;}
};
class CScriptGameResourceObjectMovieBase{public:CScriptGameResourceObjectMovieBase(){++state->movies;}~CScriptGameResourceObjectMovieBase(){--state->movies;}};
class CGameScriptInterfaceBase{public:CScriptThing hero;CGameScriptInterfaceBase():hero(1){}void NewScriptFrame(){assert(state->frame<511);++state->frame;}
 bool StartScriptingEntity(const CScriptThing&t,CScriptGameResourceObjectScriptedThingBase&r,EScriptAIPriority p){assert(t.kind==0&&p==SCRIPT_AI_PRIORITY_HIGHEST);++state->starts;if(state->starts<=state->startFailures)return false;r.valid=true;return true;}
 void SetThingPersistent(const CScriptThing&t,bool b){assert(t.kind==0&&b);++state->persistent;}
 void EntitySetAsDamageable(const CScriptThing&t,bool b){assert(t.kind==0&&!b);++state->damageable;}
 void EntitySetAsToAddToComboMultiplierWhenHit(const CScriptThing&t,bool b){assert(t.kind==0&&!b);++state->combo;}
 void EntitySetAsKillable(const CScriptThing&t,bool a,bool b){assert(t.kind==0&&!a&&!b);++state->killable;}
 CScriptThing GetNearestWithDefName(const CScriptThing&t,const CCharString&s){assert(t.kind==0&&s.text=="REGION_EXIT_POINT");return CScriptThing(2);}
 void SetRegionExitAsActive(const CScriptThing&t,bool b){assert(t.kind==2);if(b){assert(state->opened==1);++state->regionOn;}else{assert(!state->opened);++state->regionOff;}}
 void StartMovieSequence(const CCharString&s,CScriptGameResourceObjectMovieBase&){assert(s.text.empty()&&state->movies==1);}
 void PauseAllNonScriptedEntities(bool b){state->pauses+=b?1:-1;assert(state->pauses>=0&&state->pauses<=1);}
 float GetHealth(const CScriptThing&t){assert(t.kind==3);++state->healthChecks;return state->health;}
 CScriptThing&GetHero(){return hero;}
};
class CWideScreenMagicPauseEntities{CGameScriptInterfaceBase*Game;public:CWideScreenMagicPauseEntities(CGameScriptInterfaceBase&g):Game(&g){Game->PauseAllNonScriptedEntities(true);}~CWideScreenMagicPauseEntities(){Game->PauseAllNonScriptedEntities(false);}};
namespace NScript {class CIsThingAlive{public:CIsThingAlive(const CScriptThing&t){assert(t.kind==0&&state->resources==1);}};
class CV_DemonDoorsScript{public:void FadeOutFaceAndOpenDoor(const CScriptThing&t,const CCharString&s){assert(t.kind==0&&s.text=="DD_ThreeSuits"&&state->pauses==1&&state->regionOff==1&&state->regionOn==0);++state->opened;}
 class CDF_ThreeSuits{public:CGameScriptInterfaceBase&Game;CScriptThing Me;CV_DemonDoorsScript&ParentClass;CDF_ThreeSuits(CGameScriptInterfaceBase&g,CV_DemonDoorsScript&p):Game(g),ParentClass(p){}void SetPredicate(const CIsThingAlive&){++state->predicates;}bool IsActiveThreadTerminating()const{return state->term();}
 bool DressedAsGoodKnight(){state->suits.push_back(1);return state->e[state->frame].knight;}bool DressedAsEvilMage(){state->suits.push_back(2);return state->e[state->frame].mage;}bool DressedAsBandit(){state->suits.push_back(3);return state->e[state->frame].bandit;}void Main();};};}

void NScript::CV_DemonDoorsScript::CDF_ThreeSuits::Main()
{
    CScriptGameResourceObjectScriptedThingBase seh_me;
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    Game.SetThingPersistent(Me,true);
    Game.EntitySetAsDamageable(Me,false);
    Game.EntitySetAsToAddToComboMultiplierWhenHit(Me,false);
    Game.EntitySetAsKillable(Me,false,false);
    CScriptThing region_exit=Game.GetNearestWithDefName(Me,"REGION_EXIT_POINT");
    Game.SetRegionExitAsActive(region_exit,false);
    enum { BEGINNING, GOOD_KNIGHT, EVIL_MAGE, BANDIT } WhatSuitIsNext=BEGINNING;
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    do {
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            switch (WhatSuitIsNext) {
            case BEGINNING:
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_088_THREE_SUITS_INTRO",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                WhatSuitIsNext=GOOD_KNIGHT;
            case GOOD_KNIGHT:
                if (DressedAsGoodKnight()) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_088_THREE_SUITS_SEEN_KNIGHT",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    WhatSuitIsNext=EVIL_MAGE;
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_088_THREE_SUITS_WANT_KNIGHT",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    break;
                }
            case EVIL_MAGE:
                if (DressedAsEvilMage()) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_088_THREE_SUITS_SEEN_MAGE",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    WhatSuitIsNext=BANDIT;
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_088_THREE_SUITS_WANT_MAGE",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    break;
                }
            case BANDIT:
                if (DressedAsBandit()) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_088_THREE_SUITS_SEEN_BANDIT",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_088_THREE_SUITS_OPENING",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    ParentClass.FadeOutFaceAndOpenDoor(Me,"DD_ThreeSuits");
                    Game.SetRegionExitAsActive(region_exit,true);
                    return _Fail_Val.Return();
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_088_THREE_SUITS_WANT_BANDIT",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    break;
                }
            }
        }
        Game.NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}

void run(State&s){state=&s;{CGameScriptInterfaceBase g;NScript::CV_DemonDoorsScript p;NScript::CV_DemonDoorsScript::CDF_ThreeSuits t(g,p);t.Main();assert(s.resources==0&&s.movies==0&&s.pauses==0&&s.predicates==1&&s.things==2);assert(s.opened==s.regionOn&&s.regionOn<=1);assert(s.persistent==s.damageable&&s.combo==s.killable&&s.killable==s.persistent);}assert(s.things==0);}
std::string line(const char*s){return std::string("TEXT_QST_088_THREE_SUITS_")+s;}
int main(){int cases=0;
 {State s;run(s);assert(s.starts==1&&s.regionOff==1&&s.text.empty()&&s.suits.empty()&&!s.opened);++cases;}
 {State s;for(int n=1;n<8;++n)s.e[n].talk=true;run(s);assert(s.text.size()==8&&s.text[0]==line("INTRO"));for(unsigned n=1;n<s.text.size();++n)assert(s.text[n]==line("WANT_KNIGHT"));assert(s.suits.size()==7);++cases;}
 {State s;s.e[1].talk=true;s.e[1].knight=true;s.e[2].talk=true;s.e[2].mage=true;s.e[3].talk=true;s.e[3].bandit=true;run(s);const char*expected[]={"INTRO","SEEN_KNIGHT","WANT_MAGE","SEEN_MAGE","WANT_BANDIT","SEEN_BANDIT","OPENING"};assert(s.text.size()==7&&s.opened==1&&s.frame==3);for(unsigned n=0;n<7;++n)assert(s.text[n]==line(expected[n]));assert(s.suits.size()==5&&s.suits[0]==1&&s.suits[1]==2&&s.suits[2]==2&&s.suits[3]==3&&s.suits[4]==3);++cases;}
 for(int mask=0;mask<8;++mask)for(int alive=0;alive<2;++alive){State s;s.limit=2;s.health=alive?1:0;s.e[1].talk=true;s.e[1].knight=(mask&1)!=0;s.e[1].mage=(mask&2)!=0;s.e[1].bandit=(mask&4)!=0;run(s);int finalState=!(mask&1)?1:!(mask&2)?2:!(mask&4)?3:4;assert(s.opened==(mask==7)&&s.suits.size()==unsigned(finalState>3?3:finalState));if(alive){assert(s.text.size()==unsigned(finalState+1));assert(s.text[0]==line("INTRO"));assert(s.text.back()==line(finalState==1?"WANT_KNIGHT":finalState==2?"WANT_MAGE":finalState==3?"WANT_BANDIT":"OPENING"));}else assert(s.text.empty());++cases;}
 {State s;s.healthAfterBandit=true;s.e[1].talk=s.e[1].knight=s.e[1].mage=s.e[1].bandit=true;run(s);assert(s.text.size()==4&&s.text.back()==line("SEEN_BANDIT")&&s.healthChecks==5&&s.opened);++cases;}
 {State s;s.initialValid=true;s.startFailures=3;run(s);assert(s.starts==4&&s.releases==1&&s.regionOff==1);++cases;}
 {State s;s.taskFrames=2;s.limit=30;for(int n=1;n<30;++n)s.e[n].talk=s.e[n].knight=s.e[n].mage=s.e[n].bandit=true;run(s);assert(s.text.size()==5&&s.frame==11&&s.opened);++cases;}
 {State s;s.health=-1;s.e[1].talk=true;run(s);assert(s.text.empty()&&s.suits.size()==1&&!s.opened);++cases;}
 for(int mode=0;mode<5;++mode)for(int stop=1;stop<=130;++stop){State s;s.stopCheck=stop;s.limit=25;s.startFailures=mode==0?3:0;s.taskFrames=mode==4?2:0;s.initialValid=mode==0;for(int n=1;n<25;++n){s.e[n].talk=true;s.e[n].knight=mode>=2;s.e[n].mage=mode>=3;s.e[n].bandit=mode>=4;}run(s);assert(s.checks<=stop);++cases;}
 assert(cases==673);std::puts("00e72630_TEST PASS");}
