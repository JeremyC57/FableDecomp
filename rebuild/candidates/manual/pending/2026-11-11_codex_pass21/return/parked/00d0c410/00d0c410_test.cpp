// Entire final candidate Main, deterministic callees and real control-flow waits.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <cassert>
static int Strings,Things,Handles,Movies,Timers;
struct CCharString{std::string text;CCharString():text(){++Strings;}CCharString(const char*s):text(s){++Strings;}CCharString(const CCharString&s):text(s.text){++Strings;}~CCharString(){--Strings;}};
struct GameStub;static GameStub*Current;
struct CScriptThing{int id;CScriptThing(int i=0):id(i){++Things;}CScriptThing(const CScriptThing&t):id(t.id){++Things;}~CScriptThing(){--Things;}bool MsgIsHitBy(const CCharString&)const;bool MsgIsHitByAnySpecialAbilityFrom(const CCharString&)const;bool MsgIsHitBySpecialAbilityFrom(int,const CCharString&)const;bool MsgIsTalkedToBy(const CCharString&)const;};
struct CTimer;static std::vector<CTimer*>TimerList;
struct CTimer{long time;CTimer():time(0){++Timers;TimerList.push_back(this);}~CTimer(){--Timers;for(unsigned i=0;i<TimerList.size();++i)if(TimerList[i]==this){TimerList.erase(TimerList.begin()+i);break;}}CTimer&operator=(long t){time=t;return *this;}long GetTime()const{return time;}bool operator<=(long t)const{return GetTime()<=t;}};
struct CScriptGameResourceObjectMovieBase{CScriptGameResourceObjectMovieBase(){++Movies;}~CScriptGameResourceObjectMovieBase(){--Movies;}};
struct CScriptGameResourceObjectScriptedThingBase{int id;CScriptGameResourceObjectScriptedThingBase():id(0){++Handles;}~CScriptGameResourceObjectScriptedThingBase(){--Handles;}bool IsValid()const{return id!=0;}void Release();CScriptThing GetScriptThing()const{return CScriptThing(id);}void Speak(const CScriptThing&,const char*,int,bool,bool,bool);void PlayAnimation(const char*,bool,bool,bool,bool,bool,bool,bool);bool IsPerformingScriptTask()const;};
enum{SCRIPT_AI_PRIORITY_HIGHEST=4,GROUP_SELECT_NONE=0,HERO_ABILITY_HEAL_LIFE_SPELL=14};namespace NGlobalConsole{static bool GDefaultToUsingPhysicsInCutsceneAnimations=true;}
struct Parent{bool Gate2Open;Parent():Gate2Open(false){}};struct GlobalDef{long BAC_ForgerGoldAmount,EXPERIENCE_GRANT_SMALL;GlobalDef():BAC_ForgerGoldAmount(50),EXPERIENCE_GRANT_SMALL(25){}};static GlobalDef Def;static GlobalDef*pGlobalScriptDef=&Def;
struct GameStub{Parent*P;int Frame,TerminateAt,StartAt,TaskLength,TaskUntil,QuestionDelay,SecondQuestionDelay,QuestionReady,QuestionIndex,HitKind,HitAt,EnemyAt,TopupAt,GateOpenAt,HeroPassTakenAt,HeroGold,NPCGold,Experience,GiftNPC,GiftHero,RemovedNPC,ClearInfo,EnemyTargets,Starts,ReleaseCalls,WaveCalls,MovieStarts,PauseDepth;bool Alive,PredicateOkay,Near,HeroPass,NPCPass;std::string LongLine;std::vector<int>TalkFrames,Answers;std::vector<std::string>Lines,Questions,Events;
 GameStub():P(0),Frame(0),TerminateAt(30),StartAt(0),TaskLength(1),TaskUntil(0),QuestionDelay(1),SecondQuestionDelay(1),QuestionReady(0),QuestionIndex(-1),HitKind(0),HitAt(-1),EnemyAt(-1),TopupAt(-1),GateOpenAt(-1),HeroPassTakenAt(-1),HeroGold(100),NPCGold(0),Experience(0),GiftNPC(0),GiftHero(0),RemovedNPC(0),ClearInfo(0),EnemyTargets(0),Starts(0),ReleaseCalls(0),WaveCalls(0),MovieStarts(0),PauseDepth(0),Alive(true),PredicateOkay(true),Near(false),HeroPass(false),NPCPass(false){Current=this;Answers.push_back(1);}
 void NewScriptFrame(){++Frame;assert(Frame<300);for(unsigned i=0;i<TimerList.size();++i)if(TimerList[i]->time>0)--TimerList[i]->time;if(TopupAt>=0&&Frame==TopupAt)HeroGold=100;if(P&&GateOpenAt>=0&&Frame>=GateOpenAt)P->Gate2Open=true;if(HeroPassTakenAt>=0&&Frame==HeroPassTakenAt)HeroPass=false;}
 bool StartScriptingEntity(const CScriptThing&t,CScriptGameResourceObjectScriptedThingBase&h,int p){assert(t.id==1&&p==4);++Starts;if(Frame<StartAt)return false;h.id=1;return true;}CScriptThing GetHero(){return CScriptThing(2);}void GiveThingHeroRewardItem(const CScriptThing&t,const CCharString&n,const CCharString&m){assert(t.id==1&&n.text=="OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS"&&m.text.empty());++GiftNPC;NPCPass=true;}bool AreEntitiesEnemies(const CScriptThing&t,const CScriptThing&h){assert(t.id==1&&h.id==2);return EnemyAt>=0&&Frame>=EnemyAt;}bool IsObjectInThingsPossession(const CCharString&n,const CScriptThing&h){assert(h.id==2&&n.text=="OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS");return HeroPass;}
 void EntitySetFacingAngleTowardsThing(const CScriptThing&t,const CScriptThing&h,bool b){assert(t.id==1&&h.id==2&&!b);}long AddNewConversation(const CScriptThing&t,bool a,bool b){assert(t.id==1&&!a&&!b);return 1;}void AddPersonToConversation(long i,const CScriptThing&h){assert(i==1&&h.id==2);}void AddLineToConversation(long i,const CCharString&n,bool b,const CScriptThing&t,const CScriptThing&h){assert(i==1&&!b&&t.id==1&&h.id==2);Lines.push_back(n.text);}void StartMovieSequence(const CCharString&n,CScriptGameResourceObjectMovieBase&){assert(n.text.empty());++MovieStarts;}void PauseAllNonScriptedEntities(bool b){PauseDepth+=b?1:-1;assert(PauseDepth>=0&&PauseDepth<=1);}float GetHealth(const CScriptThing&t){assert(t.id==1);return Health;}float Health;
 void GiveHeroYesNoQuestion(const CCharString&q,const CCharString&y,const CCharString&n,const CCharString&e,bool b){assert(y.text=="TEXT_OBJECT_HERO_ANSWER_YES"&&n.text=="TEXT_OBJECT_HERO_ANSWER_NO"&&e.text.empty()&&b);assert(q.text=="TEXT_QST_009_FORGER_CHAT_QUESTION"||q.text=="TEXT_QST_009_FORGER_SECOND_QUESTION");Questions.push_back(q.text);++QuestionIndex;QuestionReady=Frame+(q.text=="TEXT_QST_009_FORGER_SECOND_QUESTION"?SecondQuestionDelay:QuestionDelay);}long MsgIsQuestionAnsweredYesOrNo(){if(Frame<QuestionReady)return -1;return Answers[QuestionIndex<(int)Answers.size()?QuestionIndex:(int)Answers.size()-1];}long GetHeroGold(){return HeroGold;}
 void GiveHeroObject(const CCharString&n,long v,bool b){assert(n.text=="OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS"&&v==-1&&!b);++GiftHero;HeroPass=true;Events.push_back("hero_pass");}void GiveHeroGold(long a){assert(a==-50);HeroGold+=(int)a;Events.push_back("pay");}void EntityGiveGold(const CScriptThing&t,long n){assert(t.id==1&&n==50);NPCGold+=(int)n;Events.push_back("npc_gold");}void RemoveItemFromContainer(const CScriptThing&t,const CCharString&n){assert(t.id==1&&n.text=="OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS"&&NPCPass);NPCPass=false;++RemovedNPC;Events.push_back("remove_npc_pass");}void GiveHeroExperience(long n){assert(n==25);Experience+=(int)n;Events.push_back("xp");}void ClearThingHasInformation(const CScriptThing&t){assert(t.id==1);++ClearInfo;Events.push_back("clear_info");}void GiveThingBestEnemyTarget(const CScriptThing&t,const CScriptThing&h){assert(t.id==1&&h.id==2);++EnemyTargets;}
};
bool CScriptThing::MsgIsHitBy(const CCharString&n)const{assert(id==1&&n.text=="SCRIPT_NAME_HERO");return Current->Frame==Current->HitAt&&Current->HitKind==1;}bool CScriptThing::MsgIsHitByAnySpecialAbilityFrom(const CCharString&n)const{assert(id==1&&n.text=="SCRIPT_NAME_HERO");return Current->Frame==Current->HitAt&&Current->HitKind>=2;}bool CScriptThing::MsgIsHitBySpecialAbilityFrom(int a,const CCharString&n)const{assert(id==1&&a==14&&n.text=="SCRIPT_NAME_HERO");return Current->Frame==Current->HitAt&&Current->HitKind==3;}bool CScriptThing::MsgIsTalkedToBy(const CCharString&n)const{assert(id==1&&n.text=="SCRIPT_NAME_HERO");for(unsigned i=0;i<Current->TalkFrames.size();++i)if(Current->TalkFrames[i]==Current->Frame)return true;return false;}
void CScriptGameResourceObjectScriptedThingBase::Release(){id=0;++Current->ReleaseCalls;}void CScriptGameResourceObjectScriptedThingBase::Speak(const CScriptThing&h,const char*n,int g,bool a,bool b,bool c){assert(id==1&&h.id==2&&g==0&&!a&&b&&!c);Current->Lines.push_back(n);Current->TaskUntil=Current->Frame+(Current->LongLine==n?100:Current->TaskLength);}void CScriptGameResourceObjectScriptedThingBase::PlayAnimation(const char*n,bool a,bool b,bool c,bool d,bool e,bool f,bool g){assert(id==1&&std::strcmp(n,"ST_WAVE_SPECIAL_02")==0&&!a&&!b&&!c&&d&&e&&!f&&!g);++Current->WaveCalls;Current->TaskUntil=Current->Frame+Current->TaskLength;}bool CScriptGameResourceObjectScriptedThingBase::IsPerformingScriptTask()const{return Current->Frame<Current->TaskUntil;}
struct CIsThingAlive{bool b;CIsThingAlive(const CScriptThing&t):b(Current->Alive){assert(t.id==1);}};struct TRetVoid{void Return(){}};struct CWideScreenMagicPauseEntities{GameStub*g;CWideScreenMagicPauseEntities(GameStub&p):g(&p){g->PauseAllNonScriptedEntities(true);}~CWideScreenMagicPauseEntities(){g->PauseAllNonScriptedEntities(false);}};
static bool IsDistanceBetweenThingsUnder(const CScriptThing&h,const CScriptThing&t,float d){assert(h.id==2&&t.id==1&&d==10);return Current->Near;}
struct Forger{GameStub&Game;CScriptThing Me;Parent ParentClass;bool GivenPass;Forger(GameStub&g):Game(g),Me(1),GivenPass(false){g.P=&ParentClass;g.Health=10;}void SetPredicate(const CIsThingAlive&p){Game.PredicateOkay=p.b;}bool IsActiveThreadTerminating()const{return !Game.PredicateOkay||Game.Frame>=Game.TerminateAt;}void Main();};
void Forger::Main()
{
    TRetVoid _Fail_Val;
    bool TalkedTo=false;
    bool HasPass=true;
    bool PsstAmount=false;
    bool NotAttacked=true;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CTimer ForgeTimer;
    ForgeTimer=0;
    if (!GivenPass) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.GiveThingHeroRewardItem(Me,"OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS","");
        GivenPass=true;
    }
    while (!TalkedTo && NotAttacked) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO")) || Game.AreEntitiesEnemies(Me,Game.GetHero())) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            NotAttacked=false;
        }
        if (ForgeTimer<=0 && IsDistanceBetweenThingsUnder(Game.GetHero(),Me,10.0f) && !ParentClass.Gate2Open && !Game.IsObjectInThingsPossession("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS",Game.GetHero())) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.EntitySetFacingAngleTowardsThing(Me,Game.GetHero(),false);
            seh_me.PlayAnimation("ST_WAVE_SPECIAL_02",false,false,false,true,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
            while (seh_me.IsPerformingScriptTask()) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (!PsstAmount) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                long Conversation=Game.AddNewConversation(Me,false,false);
                Game.AddPersonToConversation(Conversation,Game.GetHero());
                Game.AddLineToConversation(Conversation,"TEXT_QST_009_FORGER_ASIDE_FIRST",false,Me,Game.GetHero());
                PsstAmount=true;
            }
            else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                long Conversation=Game.AddNewConversation(Me,false,false);
                Game.AddPersonToConversation(Conversation,Game.GetHero());
                Game.AddLineToConversation(Conversation,"TEXT_QST_009_FORGER_ASIDE_SECOND",false,Me,Game.GetHero());
                PsstAmount=false;
            }
            ForgeTimer=10;
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (ParentClass.Gate2Open) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_009_FORGER_EARLY_NOT_NEED",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
            else if (Game.IsObjectInThingsPossession("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS",Game.GetHero())) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_009_FORGER_EARLY_GOT_PASS",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
            else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_009_FORGER_CHAT",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                Game.GiveHeroYesNoQuestion("TEXT_QST_009_FORGER_CHAT_QUESTION","TEXT_OBJECT_HERO_ANSWER_YES","TEXT_OBJECT_HERO_ANSWER_NO","",true);
                long answer=Game.MsgIsQuestionAnsweredYesOrNo();
                while (answer<0) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    answer=Game.MsgIsQuestionAnsweredYesOrNo();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (answer==1) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game.GetHeroGold()<pGlobalScriptDef->BAC_ForgerGoldAmount) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game.GetHero(),"TEXT_QST_009_FORGER_FAILED",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                    }
                    else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game.GetHero(),"TEXT_QST_009_FORGER_SUCCESS",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        Game.GiveHeroObject("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS",-1,false);
                        Game.GiveHeroGold(-pGlobalScriptDef->BAC_ForgerGoldAmount);
                        Game.EntityGiveGold(Me,pGlobalScriptDef->BAC_ForgerGoldAmount);
                        Game.RemoveItemFromContainer(Me,"OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS");
                        HasPass=false;
                        Game.GiveHeroExperience(pGlobalScriptDef->EXPERIENCE_GRANT_SMALL);
                        Game.ClearThingHasInformation(Me);
                    }
                }
                else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_009_FORGER_TURNED_DOWN",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
            }
            TalkedTo=true;
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    while (HasPass && NotAttacked) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO")) || Game.AreEntitiesEnemies(Me,Game.GetHero())) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            NotAttacked=false;
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (ParentClass.Gate2Open || Game.IsObjectInThingsPossession("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS",Game.GetHero())) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_009_FORGER_LATE_NOT_NEED",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
            else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.GiveHeroYesNoQuestion("TEXT_QST_009_FORGER_SECOND_QUESTION","TEXT_OBJECT_HERO_ANSWER_YES","TEXT_OBJECT_HERO_ANSWER_NO","",true);
                long answer=Game.MsgIsQuestionAnsweredYesOrNo();
                while (answer<0) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    answer=Game.MsgIsQuestionAnsweredYesOrNo();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (answer==1) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game.GetHeroGold()<pGlobalScriptDef->BAC_ForgerGoldAmount) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game.GetHero(),"TEXT_QST_009_FORGER_SECOND_FAIL",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                    }
                    else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game.GetHero(),"TEXT_QST_009_FORGER_SUCCESS",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        Game.GiveHeroObject("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS",-1,false);
                        Game.GiveHeroGold(-pGlobalScriptDef->BAC_ForgerGoldAmount);
                        Game.EntityGiveGold(Me,pGlobalScriptDef->BAC_ForgerGoldAmount);
                        Game.RemoveItemFromContainer(Me,"OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS");
                        Game.GiveHeroExperience(pGlobalScriptDef->EXPERIENCE_GRANT_SMALL);
                        Game.ClearThingHasInformation(Me);
                        HasPass=false;
                    }
                }
                else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_009_FORGER_TURNED_DOWN",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
            }
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    if (!NotAttacked) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.ClearThingHasInformation(Me);
        Game.GiveThingBestEnemyTarget(Me,Game.GetHero());
    }
    if (seh_me.IsValid()) seh_me.Release();
    for (;;) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}

static bool Balanced(const GameStub&g){return Strings==0&&Things==1&&Handles==0&&Movies==0&&Timers==0&&TimerList.empty()&&g.PauseDepth==0;}
static int Count(const GameStub&g,const char*n){int c=0;for(unsigned i=0;i<g.Lines.size();++i)if(g.Lines[i]==n)++c;return c;}
static bool Sale(const GameStub&g){const char*e[]={"hero_pass","pay","npc_gold","remove_npc_pass","xp","clear_info"};if(g.Events.size()!=6)return false;for(int i=0;i<6;++i)if(g.Events[i]!=e[i])return false;return g.GiftHero==1&&g.RemovedNPC==1&&!g.NPCPass&&g.HeroPass&&g.NPCGold==50&&g.Experience==25;}
static bool Check(bool b,int n){if(!b)std::printf("case %d failed\n",n);return b;}
int main(){bool ok=true;int n=0;
 {GameStub g;Forger o(g);g.TalkFrames.push_back(2);g.TalkFrames.push_back(10);o.Main();ok=Check(Balanced(g)&&Sale(g)&&g.HeroGold==50&&o.GivenPass&&g.GiftNPC==1&&g.Questions.size()==1&&Count(g,"TEXT_QST_009_FORGER_CHAT")==1&&Count(g,"TEXT_QST_009_FORGER_SUCCESS")==1&&g.ReleaseCalls==1&&g.EnemyTargets==0,++n)&&ok;}
 {GameStub g;Forger o(g);g.TalkFrames.push_back(2);g.HeroGold=49;o.Main();ok=Check(Balanced(g)&&g.GiftHero==0&&g.NPCPass&&g.HeroGold==49&&Count(g,"TEXT_QST_009_FORGER_FAILED")==1&&g.Experience==0&&g.ClearInfo==0,++n)&&ok;}
 {GameStub g;Forger o(g);g.TalkFrames.push_back(2);g.Answers[0]=0;o.Main();ok=Check(Balanced(g)&&g.GiftHero==0&&Count(g,"TEXT_QST_009_FORGER_TURNED_DOWN")==1&&g.Questions.size()==1&&g.ReleaseCalls==0,++n)&&ok;}
 {GameStub g;Forger o(g);g.TalkFrames.push_back(2);g.TalkFrames.push_back(8);g.Answers[0]=0;g.Answers.push_back(1);o.Main();ok=Check(Balanced(g)&&Sale(g)&&g.Questions.size()==2&&g.Questions[1]=="TEXT_QST_009_FORGER_SECOND_QUESTION"&&Count(g,"TEXT_QST_009_FORGER_TURNED_DOWN")==1&&Count(g,"TEXT_QST_009_FORGER_SUCCESS")==1,++n)&&ok;}
 {GameStub g;Forger o(g);g.TalkFrames.push_back(2);g.TalkFrames.push_back(8);g.Answers[0]=0;g.Answers.push_back(1);g.HeroGold=20;o.Main();ok=Check(Balanced(g)&&g.GiftHero==0&&Count(g,"TEXT_QST_009_FORGER_SECOND_FAIL")==1&&g.Questions.size()==2,++n)&&ok;}
 {GameStub g;Forger o(g);g.TalkFrames.push_back(2);g.TalkFrames.push_back(8);g.HeroGold=20;g.TopupAt=7;o.Main();ok=Check(Balanced(g)&&Sale(g)&&Count(g,"TEXT_QST_009_FORGER_FAILED")==1&&g.Questions.size()==2,++n)&&ok;}
 {GameStub g;Forger o(g);g.TalkFrames.push_back(2);g.TalkFrames.push_back(8);g.TalkFrames.push_back(13);g.Answers[0]=0;o.Main();ok=Check(Balanced(g)&&g.GiftHero==0&&Count(g,"TEXT_QST_009_FORGER_TURNED_DOWN")==3&&g.Questions.size()==3,++n)&&ok;}
 for(int gate=0;gate<2;++gate){GameStub g;Forger o(g);o.ParentClass.Gate2Open=gate!=0;g.HeroPass=gate==0;g.TalkFrames.push_back(2);g.TalkFrames.push_back(8);o.Main();ok=Check(Balanced(g)&&g.Questions.empty()&&Count(g,gate?"TEXT_QST_009_FORGER_EARLY_NOT_NEED":"TEXT_QST_009_FORGER_EARLY_GOT_PASS")==1&&Count(g,"TEXT_QST_009_FORGER_LATE_NOT_NEED")==1&&g.GiftHero==0,++n)&&ok;}
 {GameStub g;Forger o(g);g.HeroPass=true;g.HeroPassTakenAt=6;g.TalkFrames.push_back(2);g.TalkFrames.push_back(8);o.Main();ok=Check(Balanced(g)&&Sale(g)&&g.Questions.size()==1&&g.Questions[0]=="TEXT_QST_009_FORGER_SECOND_QUESTION"&&Count(g,"TEXT_QST_009_FORGER_EARLY_GOT_PASS")==1,++n)&&ok;}
 {GameStub g;Forger o(g);g.GateOpenAt=6;g.Answers[0]=0;g.TalkFrames.push_back(2);g.TalkFrames.push_back(8);o.Main();ok=Check(Balanced(g)&&g.Questions.size()==1&&Count(g,"TEXT_QST_009_FORGER_LATE_NOT_NEED")==1&&g.GiftHero==0,++n)&&ok;}
 {GameStub g;Forger o(g);g.Near=true;g.TerminateAt=34;o.Main();ok=Check(Balanced(g)&&g.WaveCalls==3&&Count(g,"TEXT_QST_009_FORGER_ASIDE_FIRST")==2&&Count(g,"TEXT_QST_009_FORGER_ASIDE_SECOND")==1&&g.Questions.empty(),++n)&&ok;}
 for(int gate=0;gate<2;++gate){GameStub g;Forger o(g);g.Near=true;g.HeroPass=gate==0;o.ParentClass.Gate2Open=gate!=0;o.Main();ok=Check(Balanced(g)&&g.WaveCalls==0&&g.Lines.empty(),++n)&&ok;}
 for(int hit=1;hit<=3;++hit){GameStub g;Forger o(g);g.HitKind=hit;g.HitAt=2;o.Main();ok=Check(Balanced(g)&&g.EnemyTargets==int(hit!=3)&&g.ClearInfo==int(hit!=3)&&g.ReleaseCalls==int(hit!=3)&&g.GiftHero==0&&g.Questions.empty(),++n)&&ok;}
 {GameStub g;Forger o(g);g.EnemyAt=2;o.Main();ok=Check(Balanced(g)&&g.EnemyTargets==1&&g.ReleaseCalls==1&&g.ClearInfo==1,++n)&&ok;}
 {GameStub g;Forger o(g);g.HitKind=1;g.HitAt=2;g.TalkFrames.push_back(2);o.Main();ok=Check(Balanced(g)&&g.GiftHero==1&&g.EnemyTargets==1&&g.ClearInfo==2&&g.Questions.size()==1,++n)&&ok;}
 {GameStub g;Forger o(g);g.StartAt=100;g.TerminateAt=4;o.Main();ok=Check(Balanced(g)&&g.Frame==4&&!o.GivenPass&&g.GiftNPC==0&&g.MovieStarts==0,++n)&&ok;}
 {GameStub g;Forger o(g);g.Alive=false;o.Main();ok=Check(Balanced(g)&&g.Frame==1&&g.Starts==1&&!o.GivenPass&&g.GiftNPC==0,++n)&&ok;}
 {GameStub g;Forger o(g);g.TalkFrames.push_back(2);g.QuestionDelay=100;g.TerminateAt=5;o.Main();ok=Check(Balanced(g)&&g.Questions.size()==1&&g.GiftHero==0&&g.NPCPass&&g.HeroGold==100,++n)&&ok;}
 {GameStub g;Forger o(g);g.Answers[0]=0;g.TalkFrames.push_back(2);g.TalkFrames.push_back(8);g.SecondQuestionDelay=100;g.TerminateAt=11;o.Main();ok=Check(Balanced(g)&&g.Questions.size()==2&&g.GiftHero==0&&g.NPCPass,++n)&&ok;}
 {GameStub g;Forger o(g);g.TalkFrames.push_back(2);g.LongLine="TEXT_QST_009_FORGER_SUCCESS";g.TerminateAt=7;o.Main();ok=Check(Balanced(g)&&g.GiftHero==0&&g.NPCPass&&Count(g,"TEXT_QST_009_FORGER_SUCCESS")==1&&g.HeroGold==100,++n)&&ok;}
 {GameStub g;Forger o(g);g.Near=true;g.TaskLength=100;g.TerminateAt=4;o.Main();ok=Check(Balanced(g)&&g.WaveCalls==1&&g.Lines.empty()&&g.Questions.empty(),++n)&&ok;}
 {GameStub g;Forger o(g);g.TalkFrames.push_back(2);g.Health=0;o.Main();ok=Check(Balanced(g)&&Sale(g)&&g.Lines.empty()&&g.Questions.size()==1,++n)&&ok;}
 {GameStub g;Forger o(g);o.GivenPass=true;g.NPCPass=true;g.TalkFrames.push_back(2);o.Main();ok=Check(Balanced(g)&&Sale(g)&&g.GiftNPC==0,++n)&&ok;}
 ok=ok&&Strings==0&&Things==0&&Handles==0&&Movies==0&&Timers==0&&TimerList.empty();std::printf(ok?"00d0c410_TEST PASS\n":"00d0c410_TEST FAIL\n");return ok?0:1;
}
