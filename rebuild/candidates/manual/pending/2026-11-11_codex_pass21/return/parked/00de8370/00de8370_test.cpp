// Actual selected Main body with deterministic dependency fixtures.
#include <cstdio>
#include <cstring>
#include <vector>
#include <string>
#include <cstdlib>
struct Scenario {
 int frames, stop, gold, talkAt, hitAt, specialAt, healAt, startDelay, questionDelay, speechDelay;
 int questionCount, answerIndex, asks, starts, predicates, liveMovies, movieCreates, movieDestroys;
 int livePauses, pauseCreates, pauseDestroys, releases, paid, markers, clears, conversations, people, lines;
 int frameDuringSpeech, selections, answer;
 bool valid, healthy, deadlock;
 std::vector<std::string> events;
 Scenario():frames(0),stop(8),gold(8000),talkAt(-1),hitAt(-1),specialAt(-1),healAt(-1),startDelay(0),questionDelay(0),speechDelay(0),questionCount(0),answerIndex(0),asks(0),starts(0),predicates(0),liveMovies(0),movieCreates(0),movieDestroys(0),livePauses(0),pauseCreates(0),pauseDestroys(0),releases(0),paid(0),markers(0),clears(0),conversations(0),people(0),lines(0),frameDuringSpeech(0),selections(-1),answer(0),valid(true),healthy(true),deadlock(false) {}
 void event(const char* s) {events.push_back(s);}
 int count(const char* s) const {int n=0;for(unsigned i=0;i<events.size();++i)if(events[i]==s)++n;return n;}
};
static Scenario* current;
enum EScriptAIPriority {SCRIPT_AI_PRIORITY_HIGHEST=4};
enum ETextGroupSelectionMethod {GROUP_SELECT_NONE=0,GROUP_SELECT_RANDOM_NO_REPEAT=2};
enum EHeroAbility {HERO_ABILITY_HEAL_LIFE_SPELL=14};
struct TRetVoid {void Return(){}};
class CScriptThing {
public:
 bool MsgIsTalkedToBy(const char* s)const {return !std::strcmp(s,"SCRIPT_NAME_HERO")&&current->frames==current->talkAt;}
 bool MsgIsHitBy(const char* s)const {return !std::strcmp(s,"SCRIPT_NAME_HERO")&&current->frames==current->hitAt;}
 bool MsgIsHitByAnySpecialAbilityFrom(const char* s)const {return !std::strcmp(s,"SCRIPT_NAME_HERO")&&current->frames==current->specialAt;}
 bool MsgIsHitBySpecialAbilityFrom(EHeroAbility a,const char* s)const {return a==HERO_ABILITY_HEAL_LIFE_SPELL&&!std::strcmp(s,"SCRIPT_NAME_HERO")&&current->frames==current->healAt;}
};
class CScriptGameResourceObjectScriptedThingBase {
 CScriptThing thing;
 mutable int speechFrames;
public:
 CScriptGameResourceObjectScriptedThingBase():speechFrames(0){}
 bool IsValid()const{return current->valid;}
 void Release(){++current->releases;current->valid=false;}
 CScriptThing GetScriptThing()const{return thing;}
 void Speak(const CScriptThing&,const char* text,ETextGroupSelectionMethod selection,bool a,bool b,bool c){
  if(a||!b||c)std::abort();current->event(text);current->selections=selection;speechFrames=current->speechDelay;
 }
 bool IsPerformingScriptTask()const {if(speechFrames>0){--speechFrames;++current->frameDuringSpeech;return true;}return false;}
};
class CScriptGameResourceObjectMovieBase {
public:
 CScriptGameResourceObjectMovieBase(){++current->liveMovies;++current->movieCreates;}
 ~CScriptGameResourceObjectMovieBase(){--current->liveMovies;++current->movieDestroys;}
};
class CGameScriptInterfaceBase {
 CScriptThing hero;
public:
 void NewScriptFrame(){if(++current->frames>200){current->deadlock=true;std::abort();}}
 bool StartScriptingEntity(const CScriptThing&,CScriptGameResourceObjectScriptedThingBase&,EScriptAIPriority p){if(p!=SCRIPT_AI_PRIORITY_HIGHEST)std::abort();++current->starts;if(current->startDelay>0){--current->startDelay;return false;}current->valid=true;return true;}
 void EntitySetInFaction(const CScriptThing&,const char* s){current->event(s);}
 void MiniMapAddMarker(const CScriptThing&,const char* s){++current->markers;current->event(s);}
 void SetThingHasInformation(const CScriptThing&,bool a,bool b,bool c){if(a||b||c)std::abort();current->event("information");}
 void StartMovieSequence(const char* s,CScriptGameResourceObjectMovieBase&){if(*s)std::abort();current->event("movie");}
 void GiveHeroYesNoQuestion(const char* q,const char* yes,const char* no,const char* empty,bool enabled){if(std::strcmp(yes,"TEXT_OBJECT_HERO_ANSWER_YES")||std::strcmp(no,"TEXT_OBJECT_HERO_ANSWER_NO")||*empty||!enabled)std::abort();current->event(q);++current->asks;current->questionCount=current->questionDelay;}
 long MsgIsQuestionAnsweredYesOrNo(){if(current->questionCount>0){--current->questionCount;return -1;}++current->answerIndex;return current->answer;}
 long GetHeroGold()const{return current->gold;}
 void MiniMapRemoveMarker(const CScriptThing&){--current->markers;current->event("remove_marker");}
 void ClearThingHasInformation(const CScriptThing&){++current->clears;}
 void GiveHeroGold(long amount){current->gold+=int(amount);current->event("give_hero_gold");}
 void EntityGiveGold(const CScriptThing&,long amount){current->paid+=int(amount);}
 float GetHealth(const CScriptThing&)const{return current->healthy?1.0f:0.0f;}
 const CScriptThing& GetHero()const{return hero;}
 long AddNewConversation(const CScriptThing&,bool a,bool b){if(a||b)std::abort();++current->conversations;return 17;}
 void AddPersonToConversation(long index,const CScriptThing&){if(index!=17)std::abort();++current->people;}
 void AddLineToConversation(long index,const char* s,bool b,const CScriptThing&,const CScriptThing&){if(index!=17||b)std::abort();++current->lines;current->event(s);}
};
class CWideScreenMagicPauseEntities {
public:
 explicit CWideScreenMagicPauseEntities(CGameScriptInterfaceBase&){++current->livePauses;++current->pauseCreates;}
 ~CWideScreenMagicPauseEntities(){--current->livePauses;++current->pauseDestroys;}
};
namespace NScript {
class CIsThingAlive {public:explicit CIsThingAlive(const CScriptThing&){};};
class CQ_RansomVictimScript {
public:
 bool RescuedVictim,RansomHelpersAttacked;
 CScriptGameResourceObjectScriptedThingBase seh_Ransommer;
 CQ_RansomVictimScript():RescuedVictim(false),RansomHelpersAttacked(false){}
 void PlayCutscene(const char* name,const char* line){current->event(name);current->event(line);}
 class CRansommer {
 public:
  CGameScriptInterfaceBase& Game;CScriptThing Me;CQ_RansomVictimScript& ParentClass;
  CRansommer(CGameScriptInterfaceBase& g,CQ_RansomVictimScript& p):Game(g),ParentClass(p){}
  void SetPredicate(const CIsThingAlive&){++current->predicates;}
  bool IsActiveThreadTerminating()const{return current->frames>=current->stop;}
  void Main();
 };
};
}

void NScript::CQ_RansomVictimScript::CRansommer::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    { CScriptGameResourceObjectScriptedThingBase& handle = ParentClass.seh_Ransommer; if (handle.IsValid()) handle.Release(); }
    while (!Game.StartScriptingEntity(Me, ParentClass.seh_Ransommer, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    if (!ParentClass.RescuedVictim) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.EntitySetInFaction(Me, "FACTION_HERO");
        Game.MiniMapAddMarker(Me, "HUD_ORB_QUEST_OPTIONAL");
        Game.SetThingHasInformation(Me, false, false, false);
    }
    if (!ParentClass.RescuedVictim) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        CScriptGameResourceObjectMovieBase speech_movie;
        Game.StartMovieSequence("", speech_movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
        if (!ParentClass.RansomHelpersAttacked) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            ParentClass.PlayCutscene("CS_RANSOM_CAVE_INTERIOR_1A", "NULL");
            Game.GiveHeroYesNoQuestion("TEXT_QST_B06_QUESTION_RANSOM_NORMAL", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
            long YNQAnswer;
            while (!((YNQAnswer = Game.MsgIsQuestionAnsweredYesOrNo()) >= 0)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (YNQAnswer == 1) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHeroGold() >= 7500) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    ParentClass.PlayCutscene("CS_RANSOM_CAVE_INTERIOR_2", "TEXT_CS_B06_RANSOM_PAID_NORMAL_10");
                    ParentClass.RescuedVictim = true;
                    Game.MiniMapRemoveMarker(Me);
                    Game.ClearThingHasInformation(Me);
                    Game.GiveHeroGold(-7500);
                    Game.EntityGiveGold(Me, 7500);
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game.GetHealth(ParentClass.seh_Ransommer.GetScriptThing()) > 0.0f) {
                        ParentClass.seh_Ransommer.Speak(Game.GetHero(), "TEXT_QST_B06_RANSOM_NOT_ENOUGH_GOLD", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false);
                        while (!(!ParentClass.seh_Ransommer.IsPerformingScriptTask())) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
            } else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ParentClass.PlayCutscene("CS_RANSOM_CAVE_INTERIOR_2", "TEXT_CS_B06_RANSOM_REFUSED_NORMAL_10");
            }
        } else {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            ParentClass.PlayCutscene("CS_RANSOM_CAVE_INTERIOR_1B", "NULL");
            Game.GiveHeroYesNoQuestion("TEXT_QST_B06_QUESTION_RANSOM_HIGHER", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
            long YNQAnswer;
            while (!((YNQAnswer = Game.MsgIsQuestionAnsweredYesOrNo()) >= 0)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (YNQAnswer == 1) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHeroGold() >= 10000) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    ParentClass.PlayCutscene("CS_RANSOM_CAVE_INTERIOR_2", "TEXT_CS_B06_RANSOM_PAID_ATTACK_10");
                    ParentClass.RescuedVictim = true;
                    Game.MiniMapRemoveMarker(Me);
                    Game.ClearThingHasInformation(Me);
                    Game.GiveHeroGold(-10000);
                    Game.EntityGiveGold(Me, 10000);
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game.GetHealth(ParentClass.seh_Ransommer.GetScriptThing()) > 0.0f) {
                        ParentClass.seh_Ransommer.Speak(Game.GetHero(), "TEXT_QST_B06_RANSOM_NOT_ENOUGH_GOLD", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false);
                        while (!(!ParentClass.seh_Ransommer.IsPerformingScriptTask())) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
            } else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ParentClass.PlayCutscene("CS_RANSOM_CAVE_INTERIOR_2", "TEXT_CS_B06_RANSOM_REFUSED_ATTACK_10");
            }
        }
    }
    for (; !false; Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (!ParentClass.RescuedVictim) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(ParentClass.seh_Ransommer.GetScriptThing()) > 0.0f) {
                    ParentClass.seh_Ransommer.Speak(Game.GetHero(), "TEXT_QST_B06_RANSOM_ASK", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false);
                    while (!(!ParentClass.seh_Ransommer.IsPerformingScriptTask())) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (!ParentClass.RansomHelpersAttacked) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game.GiveHeroYesNoQuestion("TEXT_QST_B06_QUESTION_RANSOM_NORMAL", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
                    long YNQAnswer;
                    while (!((YNQAnswer = Game.MsgIsQuestionAnsweredYesOrNo()) >= 0)) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (YNQAnswer == 1) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game.GetHeroGold() >= 7500) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            ParentClass.RescuedVictim = true;
                            if (Game.GetHealth(ParentClass.seh_Ransommer.GetScriptThing()) > 0.0f) {
                                ParentClass.seh_Ransommer.Speak(Game.GetHero(), "TEXT_QST_B06_RANSOM_THANKS_10", GROUP_SELECT_NONE, false, true, false);
                                while (!(!ParentClass.seh_Ransommer.IsPerformingScriptTask())) {
                                    Game.NewScriptFrame();
                                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                }
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            Game.MiniMapRemoveMarker(Me);
                            Game.ClearThingHasInformation(Me);
                            Game.GiveHeroGold(-7500);
                            Game.EntityGiveGold(Me, 7500);
                        } else {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            if (Game.GetHealth(ParentClass.seh_Ransommer.GetScriptThing()) > 0.0f) {
                                ParentClass.seh_Ransommer.Speak(Game.GetHero(), "TEXT_QST_B06_RANSOM_NOT_ENOUGH_GOLD", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false);
                                while (!(!ParentClass.seh_Ransommer.IsPerformingScriptTask())) {
                                    Game.NewScriptFrame();
                                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                }
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                        }
                    } else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game.GetHealth(ParentClass.seh_Ransommer.GetScriptThing()) > 0.0f) {
                            ParentClass.seh_Ransommer.Speak(Game.GetHero(), "TEXT_QST_B06_RANSOM_ASK_REMIND", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false);
                            while (!(!ParentClass.seh_Ransommer.IsPerformingScriptTask())) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                    }
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game.GiveHeroYesNoQuestion("TEXT_QST_B06_QUESTION_RANSOM_HIGHER", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
                    long YNQAnswer;
                    while (!((YNQAnswer = Game.MsgIsQuestionAnsweredYesOrNo()) >= 0)) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (YNQAnswer == 1) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game.GetHeroGold() >= 10000) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            ParentClass.RescuedVictim = true;
                            if (Game.GetHealth(ParentClass.seh_Ransommer.GetScriptThing()) > 0.0f) {
                                ParentClass.seh_Ransommer.Speak(Game.GetHero(), "TEXT_QST_B06_RANSOM_THANKS_10", GROUP_SELECT_NONE, false, true, false);
                                while (!(!ParentClass.seh_Ransommer.IsPerformingScriptTask())) {
                                    Game.NewScriptFrame();
                                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                }
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            Game.MiniMapRemoveMarker(Me);
                            Game.ClearThingHasInformation(Me);
                            Game.GiveHeroGold(-10000);
                            Game.EntityGiveGold(Me, 10000);
                        } else {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            if (Game.GetHealth(ParentClass.seh_Ransommer.GetScriptThing()) > 0.0f) {
                                ParentClass.seh_Ransommer.Speak(Game.GetHero(), "TEXT_QST_B06_RANSOM_NOT_ENOUGH_GOLD", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false);
                                while (!(!ParentClass.seh_Ransommer.IsPerformingScriptTask())) {
                                    Game.NewScriptFrame();
                                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                }
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                        }
                    } else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game.GetHealth(ParentClass.seh_Ransommer.GetScriptThing()) > 0.0f) {
                            ParentClass.seh_Ransommer.Speak(Game.GetHero(), "TEXT_QST_B06_RANSOM_ASK_REMIND", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false);
                            while (!(!ParentClass.seh_Ransommer.IsPerformingScriptTask())) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                    }
                }
            } else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(ParentClass.seh_Ransommer.GetScriptThing()) > 0.0f) {
                    ParentClass.seh_Ransommer.Speak(Game.GetHero(), "TEXT_QST_B06_RANSOM_DONE", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false);
                    while (!(!ParentClass.seh_Ransommer.IsPerformingScriptTask())) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.EntitySetInFaction(Me, "FACTION_BANDITS");
            long HLStyleConversationIndex = Game.AddNewConversation(Me, false, false);
            Game.AddPersonToConversation(HLStyleConversationIndex, Game.GetHero());
            Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_B06_RANSOM_ON_HIT_10", false, Me, Game.GetHero());
            { CQ_RansomVictimScript& parent = ParentClass; if (parent.seh_Ransommer.IsValid()) parent.seh_Ransommer.Release(); }
            while (true) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}

static bool check(bool v,int line){if(!v)std::fprintf(stderr,"ransom fixture failure at %d\n",line);return v;}
#define CHECK(x) do {if(!check((x),__LINE__))return 1;}while(0)
static void run(Scenario& s,NScript::CQ_RansomVictimScript& p){current=&s;CGameScriptInterfaceBase g;NScript::CQ_RansomVictimScript::CRansommer r(g,p);r.Main();}
static bool cleaned(const Scenario& s){return s.liveMovies==0&&s.livePauses==0&&s.movieCreates==s.movieDestroys&&s.pauseCreates==s.pauseDestroys&&!s.deadlock;}
int main(){
 // Price, exact boundary, refusal and insufficient-money branches, both modes.
 for(int attacked=0;attacked<2;++attacked){
  const int price=attacked?10000:7500;
  for(int choice=0;choice<2;++choice)for(int delta=-1;delta<=1;++delta){
   Scenario s;NScript::CQ_RansomVictimScript p;p.RansomHelpersAttacked=attacked!=0;
   s.answer=choice;s.gold=price+delta;run(s,p);
   const bool paid=choice==1&&delta>=0;
   CHECK(p.RescuedVictim==paid);CHECK(s.paid==(paid?price:0));CHECK(s.gold==price+delta-(paid?price:0));
   CHECK(s.asks==1&&s.predicates==1&&s.starts==1&&s.releases==1&&s.frames==s.stop&&cleaned(s));
   CHECK(s.count(attacked?"CS_RANSOM_CAVE_INTERIOR_1B":"CS_RANSOM_CAVE_INTERIOR_1A")==1);
   CHECK(s.count("TEXT_QST_B06_RANSOM_NOT_ENOUGH_GOLD")==int(choice==1&&delta<0));
   CHECK(s.markers==(paid?0:1)&&s.clears==int(paid));
  }
 }
 // Initial question and speech waits can be cancelled, cleaning both resources.
 for(int mode=0;mode<4;++mode){Scenario s;NScript::CQ_RansomVictimScript p;p.RansomHelpersAttacked=(mode&1)!=0;s.stop=3;s.answer=1;s.gold=0;
  if(mode<2)s.questionDelay=10;else s.speechDelay=10;
  run(s,p);CHECK(s.frames==3&&!p.RescuedVictim&&s.paid==0&&cleaned(s));CHECK(s.movieCreates==1&&s.pauseCreates==1);
 }
 // Completed waits preserve the payment/speech paths.
 {Scenario s;NScript::CQ_RansomVictimScript p;s.questionDelay=2;s.answer=1;run(s,p);CHECK(s.frames==8&&p.RescuedVictim&&s.paid==7500&&s.answerIndex==1&&cleaned(s));}
 {Scenario s;NScript::CQ_RansomVictimScript p;s.speechDelay=2;s.answer=1;s.gold=0;run(s,p);CHECK(s.frameDuringSpeech==2&&!p.RescuedVictim&&cleaned(s));}
 // Cancellation before resource acquisition, including a failing start attempt.
 for(int n=1;n<=3;++n){Scenario s;NScript::CQ_RansomVictimScript p;s.stop=n;s.startDelay=10;run(s,p);CHECK(s.frames==n&&s.asks==0&&s.movieCreates==0&&cleaned(s));}
 // Already rescued skips the initial scene but talks with the DONE line.
 {Scenario s;NScript::CQ_RansomVictimScript p;p.RescuedVictim=true;s.talkAt=2;run(s,p);CHECK(s.asks==0&&s.count("TEXT_QST_B06_RANSOM_DONE")==1&&s.movieCreates==1&&cleaned(s));}
 // A repeated conversation must use the correct question and reminder in both modes.
 for(int a=0;a<2;++a){Scenario s;NScript::CQ_RansomVictimScript p;p.RansomHelpersAttacked=a!=0;s.talkAt=3;s.answer=0;run(s,p);
  CHECK(s.asks==2&&s.count("TEXT_QST_B06_RANSOM_ASK")==1&&s.count("TEXT_QST_B06_RANSOM_ASK_REMIND")==1&&s.movieCreates==2&&cleaned(s));
 }
 // Dead scripted actor suppresses speech calls, but not question/control flow.
 {Scenario s;NScript::CQ_RansomVictimScript p;s.answer=1;s.gold=0;s.healthy=false;s.talkAt=3;run(s,p);CHECK(s.asks==2&&s.count("TEXT_QST_B06_RANSOM_NOT_ENOUGH_GOLD")==0&&s.count("TEXT_QST_B06_RANSOM_ASK")==0&&cleaned(s));}
 // Direct hit and non-healing special hit turn hostile. Healing-only does not.
 for(int kind=0;kind<3;++kind){Scenario s;NScript::CQ_RansomVictimScript p;p.RescuedVictim=true;if(kind==0)s.hitAt=2;else{s.specialAt=2;if(kind==2)s.healAt=2;}run(s,p);
  const int hostile=kind<2?1:0;CHECK(s.count("FACTION_BANDITS")==hostile&&s.conversations==hostile&&s.people==hostile&&s.lines==hostile&&s.releases==1+hostile&&cleaned(s));
 }
 // Invalid initial handle does not release; acquiring it still succeeds.
 {Scenario s;NScript::CQ_RansomVictimScript p;p.RescuedVictim=true;s.valid=false;run(s,p);CHECK(s.releases==0&&s.starts==1&&cleaned(s));}
 std::puts("00de8370_TEST PASS");return 0;
}
