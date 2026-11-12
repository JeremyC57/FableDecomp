// Whole source Main body, deterministic callee stubs and resource lifetimes.
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
struct CTimer{long time;CTimer():time(0){++Timers;TimerList.push_back(this);}~CTimer(){--Timers;for(unsigned i=0;i<TimerList.size();++i)if(TimerList[i]==this){TimerList.erase(TimerList.begin()+i);break;}}CTimer&operator=(long t){time=t;return *this;}long GetTime()const{return time;}};
struct CScriptGameResourceObjectMovieBase{CScriptGameResourceObjectMovieBase(){++Movies;}~CScriptGameResourceObjectMovieBase(){--Movies;}};
struct CScriptGameResourceObjectScriptedThingBase{int id;CScriptGameResourceObjectScriptedThingBase():id(0){++Handles;}~CScriptGameResourceObjectScriptedThingBase(){--Handles;}bool IsValid()const{return id!=0;}void Release();CScriptThing GetScriptThing()const{assert(id==1);return CScriptThing(id);}void Speak(const CScriptThing&,const char*,int,bool,bool,bool);bool IsPerformingScriptTask()const;};
enum{SCRIPT_AI_PRIORITY_HIGHEST=4,GROUP_SELECT_NONE=0,HERO_ABILITY_HEAL_LIFE_SPELL=14};struct Parent{bool Gate1Open,AttackedOuterGateGuards;Parent():Gate1Open(false),AttackedOuterGateGuards(false){}};struct GlobalDef{long EXPERIENCE_GRANT_MEDIUM;GlobalDef():EXPERIENCE_GRANT_MEDIUM(100){}};static GlobalDef Def;static GlobalDef*pGlobalScriptDef=&Def;
struct GameStub{Parent*P;int Frame,TerminateAt,StartAt,TaskUntil,TaskLength,DressAt,Clothing,HitKind,HitAt,Starts,ReleaseCalls,EnemyTargets,ClearInfo,DoorOpened,Objective,Experience,PauseDepth;float Health;bool Alive,PredicateOkay,Sleeping;std::string LongLine;std::vector<int>TalkFrames;std::vector<std::string>Lines,Events;
 GameStub():P(0),Frame(0),TerminateAt(30),StartAt(0),TaskUntil(0),TaskLength(1),DressAt(-1),Clothing(31),HitKind(0),HitAt(-1),Starts(0),ReleaseCalls(0),EnemyTargets(0),ClearInfo(0),DoorOpened(0),Objective(0),Experience(0),PauseDepth(0),Health(10),Alive(true),PredicateOkay(true),Sleeping(false){Current=this;}
 void NewScriptFrame(){++Frame;assert(Frame<300);for(unsigned i=0;i<TimerList.size();++i)if(TimerList[i]->time>0)--TimerList[i]->time;if(DressAt>=0&&Frame>=DressAt)Clothing=31;}
 bool StartScriptingEntity(const CScriptThing&t,CScriptGameResourceObjectScriptedThingBase&h,int p){assert(t.id==1&&p==4);++Starts;if(Frame<StartAt)return false;h.id=1;return true;}CScriptThing GetHero(){return CScriptThing(2);}CScriptThing GetThingWithScriptName(const CCharString&n){assert(n.text=="Gate1GuardInner"||n.text=="Gate1");return CScriptThing(n.text=="Gate1GuardInner"?3:5);}CScriptThing GetNearestWithDefName(const CScriptThing&h,const CCharString&n){assert(h.id==2&&n.text=="VILLAGE_BANDIT_CAMP_MAIN");return CScriptThing(4);}bool IsSleepingTime(const CScriptThing&t){assert(t.id==4);return Sleeping;}float GetHealth(const CScriptThing&t){assert(t.id==1);return Health;}
 void EntitySetFacingAngleTowardsThing(const CScriptThing&t,const CScriptThing&h,bool b){assert(t.id==1&&h.id==2&&!b);}long AddNewConversation(const CScriptThing&t,bool a,bool b){assert(t.id==1&&!a&&!b);return 7;}void AddPersonToConversation(long n,const CScriptThing&h){assert(n==7&&h.id==2);}void AddLineToConversation(long n,const CCharString&text,bool b,const CScriptThing&t,const CScriptThing&h){assert(n==7&&!b&&t.id==1&&h.id==2);Lines.push_back(text.text);}void StartMovieSequence(const CCharString&n,CScriptGameResourceObjectMovieBase&){assert(n.text.empty());}void PauseAllNonScriptedEntities(bool b){PauseDepth+=b?1:-1;assert(PauseDepth>=0&&PauseDepth<=1);}
 bool IsWearingClothingItem(const CScriptThing&h,const CCharString&n){assert(h.id==2);int bit=n.text=="OBJECT_HERO_SHIRT_BANDITCAMP"?1:n.text=="OBJECT_HERO_TROUSERS_BANDITCAMP"?2:n.text=="OBJECT_HERO_HAT_BANDITCAMP"?4:n.text=="OBJECT_HERO_BOOTS_BANDITCAMP"?8:n.text=="OBJECT_HERO_GLOVES_BANDITCAMP"?16:0;assert(bit);return (Clothing&bit)!=0;}
 void OpenDoor(const CScriptThing&t){assert(t.id==5);++DoorOpened;Events.push_back("door");}CCharString GetActiveQuestName(){return CCharString("Q_BanditCamp");}void SetQuestCardObjective(const CCharString&q,const CCharString&o,const CCharString&a,const CCharString&b){assert(q.text=="Q_BanditCamp"&&o.text=="TEXT_QUEST_BANDIT_CAMP_OBJECTIVE_05"&&a.text=="BanditCampBoss"&&b.text=="BanditCampEntrance"&&!P->Gate1Open);++Objective;Events.push_back("objective");}void GiveHeroExperience(long n){assert(n==100&&P->Gate1Open);Experience+=(int)n;Events.push_back("xp");}void ClearThingHasInformation(const CScriptThing&t){assert(t.id==1);++ClearInfo;Events.push_back("clear_info");}void GiveThingBestEnemyTarget(const CScriptThing&t,const CScriptThing&h){assert(t.id==1&&h.id==2);++EnemyTargets;}
};
bool CScriptThing::MsgIsHitBy(const CCharString&n)const{assert(id==1&&n.text=="SCRIPT_NAME_HERO");return Current->Frame==Current->HitAt&&Current->HitKind==1;}bool CScriptThing::MsgIsHitByAnySpecialAbilityFrom(const CCharString&n)const{assert(id==1&&n.text=="SCRIPT_NAME_HERO");return Current->Frame==Current->HitAt&&Current->HitKind>=2;}bool CScriptThing::MsgIsHitBySpecialAbilityFrom(int a,const CCharString&n)const{assert(id==1&&a==14&&n.text=="SCRIPT_NAME_HERO");return Current->Frame==Current->HitAt&&Current->HitKind==3;}bool CScriptThing::MsgIsTalkedToBy(const CCharString&n)const{assert(id==1&&n.text=="SCRIPT_NAME_HERO");for(unsigned i=0;i<Current->TalkFrames.size();++i)if(Current->TalkFrames[i]==Current->Frame)return true;return false;}
void CScriptGameResourceObjectScriptedThingBase::Release(){id=0;++Current->ReleaseCalls;}void CScriptGameResourceObjectScriptedThingBase::Speak(const CScriptThing&h,const char*n,int g,bool a,bool b,bool c){assert(id==1&&h.id==2&&g==0&&!a&&b&&!c);Current->Lines.push_back(n);Current->TaskUntil=Current->Frame+(Current->LongLine==n?100:Current->TaskLength);}bool CScriptGameResourceObjectScriptedThingBase::IsPerformingScriptTask()const{return Current->Frame<Current->TaskUntil;}
struct CIsThingAlive{bool b;CIsThingAlive(const CScriptThing&t):b(Current->Alive){assert(t.id==1);}};struct TRetVoid{void Return(){}};struct CWideScreenMagicPauseEntities{GameStub*g;CWideScreenMagicPauseEntities(GameStub&p):g(&p){g->PauseAllNonScriptedEntities(true);}~CWideScreenMagicPauseEntities(){g->PauseAllNonScriptedEntities(false);}};
struct Gate1Outer{GameStub&Game;CScriptThing Me;Parent ParentClass;Gate1Outer(GameStub&g):Game(g),Me(1){g.P=&ParentClass;}void SetPredicate(const CIsThingAlive&p){Game.PredicateOkay=p.b;}bool IsActiveThreadTerminating()const{return !Game.PredicateOkay||Game.Frame>=Game.TerminateAt;}void Main();};
void Gate1Outer::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptThing Gate1GuardInner=Game.GetThingWithScriptName("Gate1GuardInner");
    CTimer MoanTimer;
    MoanTimer=0;
    bool TalkedTo=false;
    bool FailedOnce=false;
    for (; !false; Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (!ParentClass.Gate1Open) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (MoanTimer.GetTime()<=0) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.EntitySetFacingAngleTowardsThing(Me,Game.GetHero(),false);
                long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
                Game.AddPersonToConversation(HLStyleConversationIndex,Game.GetHero());
                if (!TalkedTo) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    TalkedTo=true;
                    Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_009_BANDIT1_CALL_OVER_FIRST",false,Me,Game.GetHero());
                }
                else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_009_BANDIT1_CALL_OVER_SECOND",false,Me,Game.GetHero());
                }
                MoanTimer=10;
            }
            if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (!Game.IsWearingClothingItem(Game.GetHero(),"OBJECT_HERO_SHIRT_BANDITCAMP") && !Game.IsWearingClothingItem(Game.GetHero(),"OBJECT_HERO_TROUSERS_BANDITCAMP") && !Game.IsWearingClothingItem(Game.GetHero(),"OBJECT_HERO_HAT_BANDITCAMP") && !Game.IsWearingClothingItem(Game.GetHero(),"OBJECT_HERO_BOOTS_BANDITCAMP") && !Game.IsWearingClothingItem(Game.GetHero(),"OBJECT_HERO_GLOVES_BANDITCAMP")) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_009_BANDIT1_NO_DISGUISE",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    FailedOnce=true;
                }
                else if (!Game.IsWearingClothingItem(Game.GetHero(),"OBJECT_HERO_SHIRT_BANDITCAMP")) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_009_BANDIT1_NO_SHIRT",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    FailedOnce=true;
                }
                else if (!Game.IsWearingClothingItem(Game.GetHero(),"OBJECT_HERO_TROUSERS_BANDITCAMP")) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_009_BANDIT1_NO_TROUSERS",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    FailedOnce=true;
                }
                else if (!Game.IsWearingClothingItem(Game.GetHero(),"OBJECT_HERO_HAT_BANDITCAMP")) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_009_BANDIT1_NO_HAT",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    FailedOnce=true;
                }
                else if (!Game.IsWearingClothingItem(Game.GetHero(),"OBJECT_HERO_BOOTS_BANDITCAMP")) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_009_BANDIT1_NO_BOOTS",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    FailedOnce=true;
                }
                else if (!Game.IsWearingClothingItem(Game.GetHero(),"OBJECT_HERO_GLOVES_BANDITCAMP")) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_009_BANDIT1_NO_GLOVES",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    FailedOnce=true;
                }
                else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (FailedOnce==true) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game.GetHero(),"TEXT_QST_009_BANDIT1_TRIED_AND_IN",GROUP_SELECT_NONE,false,true,false);
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
                            seh_me.Speak(Game.GetHero(),"TEXT_QST_009_BANDIT1_STRAIGHT_IN",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                    }
                    CScriptThing BanditCamp=Game.GetNearestWithDefName(Game.GetHero(),"VILLAGE_BANDIT_CAMP_MAIN");
                    if (Game.IsSleepingTime(BanditCamp)) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game.GetHero(),"TEXT_QST_009_BANDIT1_BANDITS_SLEEPING",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                    }
                    if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_009_BANDIT1_GATE_OPEN",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    Game.OpenDoor(Game.GetThingWithScriptName("Gate1"));
                    Game.SetQuestCardObjective(Game.GetActiveQuestName(),"TEXT_QUEST_BANDIT_CAMP_OBJECTIVE_05","BanditCampBoss","BanditCampEntrance");
                    ParentClass.Gate1Open=true;
                    Game.GiveHeroExperience(pGlobalScriptDef->EXPERIENCE_GRANT_MEDIUM);
                    Game.ClearThingHasInformation(Me);
                }
            }
        }
        else {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_009_BANDIT1_ASIDE",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (Game.GetHealth(Me)>0.0f) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
                Game.AddPersonToConversation(HLStyleConversationIndex,Game.GetHero());
                Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_009_BANDIT1_ATTACKED_NEW",false,Me,Game.GetHero());
            }
            ParentClass.AttackedOuterGateGuards=true;
            Game.GiveThingBestEnemyTarget(Me,Game.GetHero());
            Game.ClearThingHasInformation(Me);
            if (seh_me.IsValid()) seh_me.Release();
            for (;;) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
        }
        if (ParentClass.AttackedOuterGateGuards) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.GiveThingBestEnemyTarget(Me,Game.GetHero());
            if (seh_me.IsValid()) seh_me.Release();
            for (;;) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}

static int Count(const GameStub&g,const char*n){int c=0;for(unsigned i=0;i<g.Lines.size();++i)if(g.Lines[i]==n)++c;return c;}static bool Balanced(const GameStub&g){return Strings==0&&Things==1&&Handles==0&&Movies==0&&Timers==0&&TimerList.empty()&&g.PauseDepth==0;}static bool Opened(const GameStub&g,const Gate1Outer&o){return o.ParentClass.Gate1Open&&g.DoorOpened==1&&g.Objective==1&&g.Experience==100&&g.ClearInfo==1&&g.Events.size()==4&&g.Events[0]=="door"&&g.Events[1]=="objective"&&g.Events[2]=="xp"&&g.Events[3]=="clear_info";}static bool Check(bool b,int n){if(!b)std::printf("case %d failed\n",n);return b;}
int main(){bool ok=true;int n=0;
 for(int sleeping=0;sleeping<2;++sleeping){GameStub g;Gate1Outer o(g);g.Sleeping=sleeping!=0;g.TalkFrames.push_back(2);g.TalkFrames.push_back(15);o.Main();ok=Check(Balanced(g)&&Opened(g,o)&&Count(g,"TEXT_QST_009_BANDIT1_STRAIGHT_IN")==1&&Count(g,"TEXT_QST_009_BANDIT1_BANDITS_SLEEPING")==sleeping&&Count(g,"TEXT_QST_009_BANDIT1_GATE_OPEN")==1&&Count(g,"TEXT_QST_009_BANDIT1_ASIDE")==1,++n)&&ok;}
 for(int missing=-1;missing<5;++missing){GameStub g;Gate1Outer o(g);g.Clothing=missing<0?0:31^(1<<missing);g.TalkFrames.push_back(2);o.Main();const char*line=missing<0?"TEXT_QST_009_BANDIT1_NO_DISGUISE":missing==0?"TEXT_QST_009_BANDIT1_NO_SHIRT":missing==1?"TEXT_QST_009_BANDIT1_NO_TROUSERS":missing==2?"TEXT_QST_009_BANDIT1_NO_HAT":missing==3?"TEXT_QST_009_BANDIT1_NO_BOOTS":"TEXT_QST_009_BANDIT1_NO_GLOVES";ok=Check(Balanced(g)&&!o.ParentClass.Gate1Open&&g.DoorOpened==0&&Count(g,line)==1&&g.Experience==0,++n)&&ok;}
 {GameStub g;Gate1Outer o(g);g.Clothing=0;g.DressAt=6;g.TalkFrames.push_back(2);g.TalkFrames.push_back(8);o.Main();ok=Check(Balanced(g)&&Opened(g,o)&&Count(g,"TEXT_QST_009_BANDIT1_NO_DISGUISE")==1&&Count(g,"TEXT_QST_009_BANDIT1_TRIED_AND_IN")==1&&Count(g,"TEXT_QST_009_BANDIT1_STRAIGHT_IN")==0,++n)&&ok;}
 {GameStub g;Gate1Outer o(g);o.ParentClass.Gate1Open=true;g.TalkFrames.push_back(2);o.Main();ok=Check(Balanced(g)&&Count(g,"TEXT_QST_009_BANDIT1_ASIDE")==1&&Count(g,"TEXT_QST_009_BANDIT1_CALL_OVER_FIRST")==0&&g.DoorOpened==0&&g.Experience==0,++n)&&ok;}
 {GameStub g;Gate1Outer o(g);o.Main();ok=Check(Balanced(g)&&Count(g,"TEXT_QST_009_BANDIT1_CALL_OVER_FIRST")==1&&Count(g,"TEXT_QST_009_BANDIT1_CALL_OVER_SECOND")==2&&!o.ParentClass.Gate1Open,++n)&&ok;}
 for(int hit=1;hit<=3;++hit){GameStub g;Gate1Outer o(g);g.HitKind=hit;g.HitAt=2;o.Main();ok=Check(Balanced(g)&&o.ParentClass.AttackedOuterGateGuards==(hit!=3)&&g.EnemyTargets==int(hit!=3)&&g.ReleaseCalls==int(hit!=3)&&g.ClearInfo==int(hit!=3)&&Count(g,"TEXT_QST_009_BANDIT1_ATTACKED_NEW")==int(hit!=3),++n)&&ok;}
 {GameStub g;Gate1Outer o(g);o.ParentClass.AttackedOuterGateGuards=true;o.Main();ok=Check(Balanced(g)&&g.EnemyTargets==1&&g.ReleaseCalls==1&&g.ClearInfo==0&&Count(g,"TEXT_QST_009_BANDIT1_ATTACKED_NEW")==0&&Count(g,"TEXT_QST_009_BANDIT1_CALL_OVER_FIRST")==1,++n)&&ok;}
 {GameStub g;Gate1Outer o(g);g.Health=0;g.HitKind=1;g.HitAt=2;o.Main();ok=Check(Balanced(g)&&o.ParentClass.AttackedOuterGateGuards&&g.EnemyTargets==1&&Count(g,"TEXT_QST_009_BANDIT1_ATTACKED_NEW")==0,++n)&&ok;}
 {GameStub g;Gate1Outer o(g);g.Health=0;g.TalkFrames.push_back(2);o.Main();ok=Check(Balanced(g)&&Opened(g,o)&&Count(g,"TEXT_QST_009_BANDIT1_STRAIGHT_IN")==0&&Count(g,"TEXT_QST_009_BANDIT1_GATE_OPEN")==0,++n)&&ok;}
 {GameStub g;Gate1Outer o(g);g.StartAt=100;g.TerminateAt=4;o.Main();ok=Check(Balanced(g)&&g.Frame==4&&g.Lines.empty()&&g.DoorOpened==0,++n)&&ok;}
 {GameStub g;Gate1Outer o(g);g.Alive=false;o.Main();ok=Check(Balanced(g)&&g.Frame==1&&g.Starts==0&&g.Lines.empty(),++n)&&ok;}
 {GameStub g;Gate1Outer o(g);g.TalkFrames.push_back(2);g.LongLine="TEXT_QST_009_BANDIT1_STRAIGHT_IN";g.TerminateAt=4;o.Main();ok=Check(Balanced(g)&&!o.ParentClass.Gate1Open&&g.DoorOpened==0&&Count(g,"TEXT_QST_009_BANDIT1_STRAIGHT_IN")==1,++n)&&ok;}
 {GameStub g;Gate1Outer o(g);g.Sleeping=true;g.TalkFrames.push_back(2);g.LongLine="TEXT_QST_009_BANDIT1_BANDITS_SLEEPING";g.TerminateAt=5;o.Main();ok=Check(Balanced(g)&&!o.ParentClass.Gate1Open&&g.DoorOpened==0&&Count(g,"TEXT_QST_009_BANDIT1_BANDITS_SLEEPING")==1,++n)&&ok;}
 {GameStub g;Gate1Outer o(g);g.TalkFrames.push_back(2);g.LongLine="TEXT_QST_009_BANDIT1_GATE_OPEN";g.TerminateAt=5;o.Main();ok=Check(Balanced(g)&&!o.ParentClass.Gate1Open&&g.DoorOpened==0&&g.Objective==0&&g.Experience==0,++n)&&ok;}
 {GameStub g;Gate1Outer o(g);g.Clothing=0;g.TalkFrames.push_back(2);g.HitKind=1;g.HitAt=3;o.Main();ok=Check(Balanced(g)&&Count(g,"TEXT_QST_009_BANDIT1_NO_DISGUISE")==1&&Count(g,"TEXT_QST_009_BANDIT1_ATTACKED_NEW")==1&&g.EnemyTargets==1&&!o.ParentClass.Gate1Open,++n)&&ok;}
 ok=ok&&Strings==0&&Things==0&&Handles==0&&Movies==0&&Timers==0&&TimerList.empty();std::printf(ok?"00d01630_TEST PASS\n":"00d01630_TEST FAIL\n");return ok?0:1;
}
