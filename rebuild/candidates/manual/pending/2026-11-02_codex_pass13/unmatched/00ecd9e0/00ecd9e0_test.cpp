// Exact selected SickChildsMother body with independent event, argument, cleanup and branch assertions.
#include <cassert>
#include <cstdio>
#include <string>
#include <vector>
#include <limits>
struct State {
 int frame,limit,checks,stop,things,strings,resources,movies,depth,paused,unpaused,attempts,failures,taskDelay,taskUntil,questionDelay,questionCalls,answer,predicates,release,info,clear,intros,outros,cards,objectives,takes,ally,physicalChecks,abilityChecks,healChecks;
 bool intro,finished,valid,potion,talk,nearby,physical,ability,heal;float health;std::vector<std::string> lines,events;
 State():frame(0),limit(2),checks(0),stop(0),things(0),strings(0),resources(0),movies(0),depth(0),paused(0),unpaused(0),attempts(0),failures(0),taskDelay(0),taskUntil(0),questionDelay(0),questionCalls(0),answer(0),predicates(0),release(0),info(0),clear(0),intros(0),outros(0),cards(0),objectives(0),takes(0),ally(0),physicalChecks(0),abilityChecks(0),healChecks(0),intro(false),finished(false),valid(false),potion(false),talk(false),nearby(false),physical(false),ability(false),heal(false),health(100){}
 bool term(){++checks;return frame>=limit||(stop&&checks>=stop);}
 void event(const char*s){events.push_back(s);}
};State*state;
class CCharString{public:std::string text;CCharString(const char*s):text(s){++state->strings;}CCharString(const CCharString&s):text(s.text){++state->strings;}~CCharString(){--state->strings;}};
class CScriptThing{public:int id;CScriptThing(int i=-1):id(i){++state->things;}CScriptThing(const CScriptThing&s):id(s.id){++state->things;}~CScriptThing(){--state->things;}void target(const CCharString&s)const{assert(id==-1&&s.text=="SCRIPT_NAME_HERO");}bool MsgIsTalkedToBy(const CCharString&s){target(s);return state->talk;}bool MsgIsHitBy(const CCharString&s){target(s);++state->physicalChecks;return state->physical;}bool MsgIsHitByAnySpecialAbilityFrom(const CCharString&s){target(s);++state->abilityChecks;return state->ability;}bool MsgIsHitBySpecialAbilityFrom(int a,const CCharString&s){target(s);assert(a==14);++state->healChecks;return state->heal;}};
enum EScriptAIPriority{SCRIPT_AI_PRIORITY_HIGHEST=4};enum ETextGroupSelectionMethod{GROUP_SELECT_NONE=0,GROUP_SELECT_RANDOM_NO_REPEAT=2};enum EHeroAbility{HERO_ABILITY_HEAL_LIFE_SPELL=14};class TRetVoid{public:void Return(){}};
class CScriptGameResourceObjectScriptedThingBase{public:bool valid;CScriptGameResourceObjectScriptedThingBase():valid(state->valid){++state->resources;}~CScriptGameResourceObjectScriptedThingBase(){--state->resources;assert(!state->depth&&!state->movies);}bool IsValid()const{return valid;}void Release(){assert(valid&&!state->depth);valid=false;++state->release;state->event("release");}CScriptThing GetScriptThing()const{assert(valid);return CScriptThing(-1);}void Speak(const CScriptThing&h,const char*s,ETextGroupSelectionMethod m,bool a,bool b,bool c){assert(h.id==-2&&valid&&!a&&b&&!c&&state->depth==1);std::string line(s);assert(m==(line=="TEXT_QST_B10_MOTHER_ASK_CURE"?GROUP_SELECT_RANDOM_NO_REPEAT:GROUP_SELECT_NONE));state->lines.push_back(line);state->event("speak");state->taskUntil=state->frame+state->taskDelay;}bool IsPerformingScriptTask()const{return state->frame<state->taskUntil;}};
class CScriptGameResourceObjectMovieBase{public:CScriptGameResourceObjectMovieBase(){assert(!state->movies);++state->movies;state->event("movie+");}~CScriptGameResourceObjectMovieBase(){assert(!state->depth);--state->movies;state->event("movie-");}};
class CGameScriptInterfaceBase{CScriptThing hero;public:CGameScriptInterfaceBase():hero(-2){}void NewScriptFrame(){assert(state->frame<255);++state->frame;state->event("frame");}bool StartScriptingEntity(const CScriptThing&m,CScriptGameResourceObjectScriptedThingBase&r,int p){assert(m.id==-1&&p==4);r.valid=++state->attempts>state->failures;return r.valid;}CScriptThing&GetHero(){return hero;}bool IsObjectInThingsPossession(const CCharString&o,const CScriptThing&t){assert(o.text=="POTION"&&t.id==-2);return state->potion;}void SetThingHasInformation(const CScriptThing&m,bool a,bool b,bool c){assert(m.id==-1&&!a&&b&&!c);++state->info;}void ClearThingHasInformation(const CScriptThing&m){assert(m.id==-1&&state->depth==1);++state->clear;state->event("clear");}void StartMovieSequence(const CCharString&s,CScriptGameResourceObjectMovieBase&){assert(s.text.empty()&&state->movies==1&&!state->depth);}void PauseAllNonScriptedEntities(bool b){if(b){assert(!state->depth);++state->depth;++state->paused;state->event("pause");}else{assert(state->depth==1);--state->depth;++state->unpaused;state->event("unpause");}}CCharString GetActiveQuestName(){return CCharString("QUEST");}void GiveHeroQuestCardDirectly(const CCharString&card,const CCharString&q,bool b){assert(card.text=="OBJECT_QUEST_CARD_SICK_CHILD"&&q.text=="QUEST"&&!b&&state->depth==1&&state->intros==1);++state->cards;state->event("card");}void SetQuestCardObjective(const CCharString&q,const CCharString&o,const CCharString&a,const CCharString&b){assert(q.text=="QUEST"&&o.text=="TEXT_QUEST_SICK_CHILD_OBJECTIVE_01"&&a.text.empty()&&b.text.empty()&&state->cards==1);++state->objectives;state->event("objective");}void GiveHeroYesNoQuestion(const CCharString&q,const CCharString&y,const CCharString&n,const CCharString&e,bool b){assert(q.text=="TEXT_QST_B10_MOTHER_CURE_QUESTION"&&y.text=="TEXT_OBJECT_HERO_ANSWER_YES"&&n.text=="TEXT_OBJECT_HERO_ANSWER_NO"&&e.text.empty()&&b&&state->depth==1);state->event("question");}long MsgIsQuestionAnsweredYesOrNo(){++state->questionCalls;return state->questionCalls<=state->questionDelay?-1:state->answer;}float GetHealth(const CScriptThing&m){assert(m.id==-1&&state->depth==1);return state->health;}void TakeObjectFromHero(const CCharString&s){assert(s.text=="POTION"&&state->outros==1&&state->depth==1);++state->takes;state->event("take");}void EntitySetThingAsAllyOfThing(const CScriptThing&a,const CScriptThing&b){assert(state->depth==1);if(state->ally%2==0)assert(a.id==-1&&b.id==-2);else assert(a.id==-2&&b.id==-1);++state->ally;state->event("ally");}};
class CWideScreenMagicPauseEntities{protected:CGameScriptInterfaceBase* Game;public:CWideScreenMagicPauseEntities(CGameScriptInterfaceBase& game):Game(&game){Game->PauseAllNonScriptedEntities(true);}~CWideScreenMagicPauseEntities(){Game->PauseAllNonScriptedEntities(false);}};
struct CScriptDef{const char*EndPotion;CScriptDef():EndPotion("POTION"){}};CScriptDef def;CScriptDef*pGlobalScriptDef=&def;
bool IsDistanceBetweenThingsUnder(const CScriptThing&a,const CScriptThing&b,float d){assert(a.id==-1&&b.id==-2&&d==7.0f);return state->nearby;}
namespace NScript {class CIsThingAlive{public:CIsThingAlive(const CScriptThing&m){assert(m.id==-1);}};class CV_SickChildScript{public:CScriptGameResourceObjectScriptedThingBase seh_Mother;bool MotherIntroDone,FinishedQuest;CV_SickChildScript():MotherIntroDone(state->intro),FinishedQuest(state->finished){}void PlayCutscene(CCharString name,CCharString arg){assert(state->depth==1&&arg.text=="NULL");if(name.text=="CS_SICK_INTRO"){++state->intros;state->event("intro");}else{assert(name.text=="CS_SICK_OUTRO");++state->outros;state->event("outro");}}class CSickChildsMother{public:CGameScriptInterfaceBase&Game;CScriptThing Me;CV_SickChildScript&ParentClass;CSickChildsMother(CGameScriptInterfaceBase&g,CV_SickChildScript&p):Game(g),ParentClass(p){}void SetPredicate(const CIsThingAlive&){++state->predicates;}bool IsActiveThreadTerminating()const{return state->term();}void Main();};};}

void NScript::CV_SickChildScript::CSickChildsMother::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    {
        CScriptGameResourceObjectScriptedThingBase &seh_Mother=ParentClass.seh_Mother;
        if (seh_Mother.IsValid()) seh_Mother.Release();
    }
    while (!Game.StartScriptingEntity(Me,ParentClass.seh_Mother,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    if (Game.IsObjectInThingsPossession(pGlobalScriptDef->EndPotion,Game.GetHero()) || !ParentClass.MotherIntroDone) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.SetThingHasInformation(Me,false,true,false);
    }
    for (; !false; Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (!ParentClass.MotherIntroDone && IsDistanceBetweenThingsUnder(Me,Game.GetHero(),7.0f)) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            ParentClass.PlayCutscene("CS_SICK_INTRO","NULL");
            Game.GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_SICK_CHILD",Game.GetActiveQuestName(),false);
            Game.SetQuestCardObjective(Game.GetActiveQuestName(),"TEXT_QUEST_SICK_CHILD_OBJECTIVE_01","","");
            ParentClass.MotherIntroDone=true;
            Game.ClearThingHasInformation(Me);
        }
        if (ParentClass.MotherIntroDone && Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (Game.IsObjectInThingsPossession(pGlobalScriptDef->EndPotion,Game.GetHero())) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.GiveHeroYesNoQuestion("TEXT_QST_B10_MOTHER_CURE_QUESTION","TEXT_OBJECT_HERO_ANSWER_YES","TEXT_OBJECT_HERO_ANSWER_NO","",true);
                long YNQAnswer;
                while ((YNQAnswer=Game.MsgIsQuestionAnsweredYesOrNo())<0) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (YNQAnswer==1) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    ParentClass.PlayCutscene("CS_SICK_OUTRO","NULL");
                    Game.TakeObjectFromHero(pGlobalScriptDef->EndPotion);
                    Game.ClearThingHasInformation(Me);
                    ParentClass.FinishedQuest=true;
                    break;
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game.GetHealth(ParentClass.seh_Mother.GetScriptThing())>0.0f) {
    ParentClass.seh_Mother.Speak(Game.GetHero(),"TEXT_QST_B10_MOTHER_ASK_CURE",GROUP_SELECT_RANDOM_NO_REPEAT,false,true,false);
    while (ParentClass.seh_Mother.IsPerformingScriptTask()) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}
                }
            } else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(ParentClass.seh_Mother.GetScriptThing())>0.0f) {
    ParentClass.seh_Mother.Speak(Game.GetHero(),"TEXT_QST_B10_MOTHER_CURE_REMIND_10",GROUP_SELECT_NONE,false,true,false);
    while (ParentClass.seh_Mother.IsPerformingScriptTask()) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}
            }
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (Game.GetHealth(ParentClass.seh_Mother.GetScriptThing())>0.0f) {
    ParentClass.seh_Mother.Speak(Game.GetHero(),"TEXT_QST_B10_MOTHER_HIT",GROUP_SELECT_NONE,false,true,false);
    while (ParentClass.seh_Mother.IsPerformingScriptTask()) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}
            Game.EntitySetThingAsAllyOfThing(Me,Game.GetHero());
            Game.EntitySetThingAsAllyOfThing(Game.GetHero(),Me);
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    {
        CScriptGameResourceObjectScriptedThingBase &seh_Mother=ParentClass.seh_Mother;
        if (seh_Mother.IsValid()) seh_Mother.Release();
    }
    for (;;) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
}


void run(State&s){state=&s;{CGameScriptInterfaceBase g;NScript::CV_SickChildScript p;NScript::CV_SickChildScript::CSickChildsMother t(g,p);t.Main();s.intro=p.MotherIntroDone;s.finished=p.FinishedQuest;s.valid=p.seh_Mother.valid;assert(s.predicates==1&&s.movies==0&&s.depth==0&&s.paused==s.unpaused&&s.strings==0&&s.things==2&&s.resources==1);assert(s.outros==s.takes&&s.finished==(s.takes!=0)&&s.cards==s.objectives);}assert(!s.things&&!s.resources&&!s.strings);}
int main(){int cases=0;
 for(int flags=0;flags<64;++flags){State s;s.intro=(flags&1)!=0;s.nearby=(flags&2)!=0;s.talk=(flags&4)!=0;s.potion=(flags&8)!=0;s.physical=(flags&16)!=0;s.answer=(flags&32)?1:0;bool initial=s.intro;bool introduced=!initial&&s.nearby;bool chatted=(initial||introduced)&&s.talk;bool completed=chatted&&s.potion&&s.answer==1;run(s);assert(s.info==int(!initial||s.potion)&&s.intros==int(introduced)&&s.finished==completed);assert(s.cards==int(introduced)&&s.objectives==int(introduced)&&s.intro==(initial||introduced));assert(s.outros==int(completed)&&s.takes==int(completed)&&s.release==int(completed));assert(s.ally==((s.physical&&!completed)?2:0));int lines=0;if(chatted&&!completed){assert(s.lines[lines++]==(s.potion?"TEXT_QST_B10_MOTHER_ASK_CURE":"TEXT_QST_B10_MOTHER_CURE_REMIND_10"));}if(s.physical&&!completed)assert(s.lines[lines++]=="TEXT_QST_B10_MOTHER_HIT");assert(s.lines.size()==unsigned(lines));assert(s.paused==int(introduced)+int(chatted)+int(s.physical&&!completed));++cases;}
 for(int mask=0;mask<8;++mask){State s;s.intro=true;s.physical=(mask&1)!=0;s.ability=(mask&2)!=0;s.heal=(mask&4)!=0;bool hit=s.physical||(s.ability&&!s.heal);run(s);assert(s.ally==int(hit)*2&&s.lines.size()==unsigned(hit));assert(s.physicalChecks==1&&s.abilityChecks==int(!s.physical)&&s.healChecks==int(!s.physical&&s.ability));++cases;}
 for(int h=0;h<4;++h)for(int choice=0;choice<3;++choice){State s;s.intro=true;s.talk=choice<2;s.potion=choice==0;s.physical=choice==2;s.health=h==0?100.0f:h==1?0.0f:h==2?-1.0f:std::numeric_limits<float>::quiet_NaN();run(s);assert(s.lines.size()==unsigned(h==0)&&s.paused==1&&s.ally==(choice==2?2:0));++cases;}
 for(int delay=0;delay<4;++delay)for(int fails=0;fails<4;++fails){State s;s.valid=true;s.nearby=true;s.talk=true;s.potion=true;s.answer=1;s.limit=30;s.failures=fails;s.questionDelay=delay;run(s);assert(s.finished&&s.intro&&s.takes==1&&s.release==2&&s.attempts==fails+1&&s.questionCalls==delay+1&&s.frame==30&&!s.valid);++cases;}
 for(int answer=0;answer<4;++answer){State s;s.intro=true;s.talk=true;s.potion=true;s.answer=answer;run(s);assert(s.finished==(answer==1)&&s.lines.size()==unsigned(answer!=1));++cases;}
 {State s;s.failures=100;s.limit=10;run(s);assert(s.attempts==9&&!s.paused&&!s.finished);++cases;}
 {State s;s.intro=true;s.talk=true;s.potion=true;s.questionDelay=100;s.limit=10;run(s);assert(s.questionCalls==9&&s.lines.empty()&&!s.finished&&s.paused==1);++cases;}
 {State s;s.intro=true;s.talk=true;s.taskDelay=100;s.limit=10;run(s);assert(s.lines.size()==1&&s.paused==1&&!s.finished);++cases;}
 for(int mode=0;mode<12;++mode)for(int stop=1;stop<=100;++stop){State s;s.stop=stop;s.limit=20;s.intro=(mode%2)!=0;s.nearby=mode>=2;s.talk=mode>=3;s.potion=mode>=4;s.answer=mode>=5?1:0;s.physical=mode==6||mode==10;s.taskDelay=mode==7?3:0;s.questionDelay=mode==8?3:0;s.failures=mode==9?4:0;s.valid=mode==10;s.health=mode==11?0.0f:100.0f;run(s);assert(s.checks<=stop);++cases;}
 assert(cases==1307);std::puts("00ecd9e0_TEST PASS");}
