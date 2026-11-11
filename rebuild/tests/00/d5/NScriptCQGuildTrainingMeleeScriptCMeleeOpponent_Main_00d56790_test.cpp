// Full Main body; deterministic callee stubs for tutorial state, UI, messages and resources.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <cassert>
static int Strings,Things,Handles,Movies,Timers;
class CCharString{ public:std::string text;CCharString():text(){++Strings;}CCharString(const char* p):text(p){++Strings;}CCharString(const CCharString& p):text(p.text){++Strings;}~CCharString(){--Strings;}};
class C3DVector{ public:int id;C3DVector(int i=0):id(i){}};
struct GameStub;static GameStub* Current;
class CScriptThing{ public:int id;C3DVector pos;CScriptThing(int i=0):id(i),pos(i){++Things;}CScriptThing(const CScriptThing& p):id(p.id),pos(p.pos){++Things;}~CScriptThing(){--Things;}const C3DVector& GetPos()const{return pos;}bool MsgIsTalkedToBy(const CCharString&)const;bool MsgIsHitBy(const CCharString&)const;bool MsgIsHitByWithWeapon(const CCharString&,const CCharString&)const;void SetFriendsWithEverythingFlag(bool)const;};
class CTimer;static std::vector<CTimer*> TimerList;
class CTimer{ public:int time;CTimer():time(0){++Timers;TimerList.push_back(this);}~CTimer(){--Timers;for(unsigned i=0;i<TimerList.size();++i)if(TimerList[i]==this){TimerList.erase(TimerList.begin()+i);break;}}CTimer& operator=(long v){time=(int)v;return *this;}long GetTime()const{return time;}};
class CScriptGameResourceObjectMovieBase{ public:CScriptGameResourceObjectMovieBase(){++Movies;}~CScriptGameResourceObjectMovieBase(){--Movies;}};
class CScriptGameResourceObjectScriptedThingBase{ public:int id;CScriptGameResourceObjectScriptedThingBase():id(0){++Handles;}~CScriptGameResourceObjectScriptedThingBase(){--Handles;}bool IsValid()const{return id!=0;}void Release(){id=0;}void ClearCommands();CScriptThing GetScriptThing()const{return CScriptThing(id);}bool IsPerformingScriptTask()const;void MoveToPosition(const C3DVector&,float,int,bool,bool);void Speak(const CScriptThing&,const char*,int,bool,bool,bool);};
struct Parent{bool TalkedToWhisper,WhisperStopWalking,WhisperArrived,EarlyHitWhisper,MeleeOpponentReset,MeleeRepeatKnown,MeleeRepeating;long TutorialState,GenericTutorialCounter;Parent():TalkedToWhisper(true),WhisperStopWalking(false),WhisperArrived(false),EarlyHitWhisper(false),MeleeOpponentReset(false),MeleeRepeatKnown(true),MeleeRepeating(false),TutorialState(3),GenericTutorialCounter(0){}};
struct GlobalDef{float GUI_MinHealth;GlobalDef():GUI_MinHealth(5){}};static GlobalDef Def;static GlobalDef* pGlobalScriptDef=&Def;
enum{SCRIPT_AI_PRIORITY_HIGHEST=4,GROUP_SELECT_NONE=0,ENTITY_MOVE_RUN=1};
struct GameStub{
 int Frame,TerminateAt,StartAt,TaskUntil,TaskLength,Phase,Tick,Rounds,Phase3Limit,Phase4Limit,Phase6Limit,StopPhase,StopTick,InfoDelay,ClickedAt,PauseDepth,MoveCalls,KillableCalls,HealthChanges,ClearEnemy,FriendsOff,MainAcquires,Variant;
 bool Xbox,Stopped,SwordHits,PlainHits,LowHealth,RepeatOnce,KeepWaitingTalk,StartWithMeeting;Parent* P;std::vector<std::string>Lines,Info,Styles;std::vector<int>Speakers;
 GameStub():Frame(0),TerminateAt(1000),StartAt(0),TaskUntil(0),TaskLength(1),Phase(0),Tick(0),Rounds(0),Phase3Limit(10),Phase4Limit(20),Phase6Limit(30),StopPhase(-1),StopTick(-1),InfoDelay(2),ClickedAt(0),PauseDepth(0),MoveCalls(0),KillableCalls(0),HealthChanges(0),ClearEnemy(0),FriendsOff(0),MainAcquires(0),Variant(1),Xbox(false),Stopped(false),SwordHits(true),PlainHits(false),LowHealth(false),RepeatOnce(false),KeepWaitingTalk(false),StartWithMeeting(false),P(0){Current=this;}
 void NewScriptFrame(){++Frame;assert(Frame<1500);for(unsigned i=0;i<TimerList.size();++i)if(TimerList[i]->time>0)--TimerList[i]->time;if(StartWithMeeting&&Phase==0&&Frame>=5)P->TutorialState=3;if(!KeepWaitingTalk)P->TalkedToWhisper=true;if(Phase){++Tick;if(Phase==3&&Tick>Phase3Limit)P->TutorialState=4;if(Phase==4&&Tick>Phase4Limit)P->TutorialState=6;if(Phase==6&&Tick>Phase6Limit)P->TutorialState=7;if(Phase==StopPhase&&Tick>=StopTick)Stopped=true;}if(RepeatOnce&&Rounds==1&&P->MeleeOpponentReset){P->TutorialState=3;P->MeleeRepeating=false;Phase=0;}}
 bool StartScriptingEntity(const CScriptThing& t,CScriptGameResourceObjectScriptedThingBase& h,int p){assert(t.id==1&&p==4);if(Frame<StartAt)return false;h.id=1;++MainAcquires;return true;}
 CScriptThing GetHero(){return CScriptThing(2);}CScriptThing GetThingWithScriptName(const CCharString& n){int id=n.text=="M_MeleeOpponentStand"?3:n.text=="TheRealGuildmaster"?4:n.text=="MeleeThunder"?5:0;assert(id);return CScriptThing(id);}
 void EntitySetAsKillable(const CScriptThing& t,bool a,bool b){assert(t.id==1&&!a&&b);++KillableCalls;}
 void EntitySetCombatType(const CScriptThing& t,const CCharString& n){assert(t.id==1);Styles.push_back(n.text);Tick=0;if(n.text=="HERO_WHISPER_TUTORIAL_ATTACK_STYLE_DONT_ATTACK"){Phase=3;++Rounds;}else if(n.text=="HERO_WHISPER_TUTORIAL_VS_BLOCK_ATTACK_STYLE")Phase=4;else{assert(n.text=="HERO_WHISPER_TUTORIAL_ATTACK_STYLE");Phase=6;}}
 void GiveThingBestEnemyTarget(const CScriptThing& a,const CScriptThing& b){assert(a.id==1&&b.id==2);}void EntitySetInFaction(const CScriptThing& t,const CCharString& n){assert(t.id==1&&n.text=="FACTION_BANDITS");}
 bool IsXbox(){return Xbox;}void DisplayGameInfo(const CCharString& n){Info.push_back(n.text);ClickedAt=Frame+InfoDelay;}bool MsgIsGameInfoClickedPast(){return Frame>=ClickedAt;}
 void ModifyThingHealth(const CScriptThing& t,float f,bool b){assert(t.id==1&&f==1000&&!b);++HealthChanges;}bool IsPlayerCreatureBlocking(){return (Phase==4&&Tick==1)||(Phase==6&&Tick==15);}
 float GetHealth(const CScriptThing& t){assert(t.id==1||t.id==2);return t.id==2&&LowHealth?1:10;}void ClearThingBestEnemyTarget(const CScriptThing& t){assert(t.id==1);++ClearEnemy;}
 long AddNewConversation(const CScriptThing& t,bool a,bool b){assert((t.id==1||t.id==4||t.id==5)&&!a&&!b);Speakers.push_back(t.id);return(long)Speakers.size();}void AddPersonToConversation(long,const CScriptThing& t){assert(t.id==2);}void AddLineToConversation(long,const CCharString& n,bool b,const CScriptThing&,const CScriptThing& t){assert(!b&&t.id==2);Lines.push_back(n.text);}
 void StartMovieSequence(const CCharString& n,CScriptGameResourceObjectMovieBase&){assert(n.text.empty());}void PauseAllNonScriptedEntities(bool b){PauseDepth+=b?1:-1;assert(PauseDepth>=0&&PauseDepth<=1);}
};
void CScriptThing::SetFriendsWithEverythingFlag(bool b)const{assert(id==1&&!b);++Current->FriendsOff;}
bool CScriptThing::MsgIsTalkedToBy(const CCharString& n)const{assert(id==1&&n.text=="SCRIPT_NAME_HERO");return Current->StartWithMeeting&&Current->Phase==0&&Current->Frame==2;}
bool CScriptThing::MsgIsHitByWithWeapon(const CCharString& h,const CCharString& w)const{assert(id==1&&h.text=="SCRIPT_NAME_HERO"&&w.text=="OBJECT_IRON_LONGSWORD");return Current->SwordHits&&Current->Phase==3&&Current->Tick>=1&&Current->Tick<=7;}
bool CScriptThing::MsgIsHitBy(const CCharString& n)const{GameStub& g=*Current;if(id==1){assert(n.text=="SCRIPT_NAME_HERO");return (g.StartWithMeeting&&g.Phase==0&&g.Frame==4)||(g.PlainHits&&g.Phase==3&&g.Tick>=1&&g.Tick<=7)||(g.Phase==4&&g.Tick==20)||(g.Phase==6&&g.Tick==7);}assert(id==2&&n.text=="MeleeOpponent");return (g.Phase==4&&(g.Tick==1||g.Tick==2||g.Tick==12))||(g.Phase==6&&(g.Tick==15||g.Tick==23));}
void CScriptGameResourceObjectScriptedThingBase::ClearCommands(){Current->TaskUntil=Current->Frame;}
bool CScriptGameResourceObjectScriptedThingBase::IsPerformingScriptTask()const{return Current->Frame<Current->TaskUntil;}
void CScriptGameResourceObjectScriptedThingBase::MoveToPosition(const C3DVector& p,float f,int mt,bool a,bool b){assert(id==1&&p.id==3&&f==3&&mt==1&&!a&&b);++Current->MoveCalls;Current->TaskUntil=Current->Frame+Current->TaskLength;}
void CScriptGameResourceObjectScriptedThingBase::Speak(const CScriptThing& h,const char* n,int v,bool a,bool b,bool c){assert(id==1&&h.id==2&&v==0&&!a&&b&&!c);Current->Lines.push_back(n);Current->TaskUntil=Current->Frame+Current->TaskLength;}
struct CWideScreenMagicPauseEntities{GameStub* g;CWideScreenMagicPauseEntities(GameStub& p):g(&p){g->PauseAllNonScriptedEntities(true);}~CWideScreenMagicPauseEntities(){g->PauseAllNonScriptedEntities(false);}};struct TRetVoid{void Return()const{}};
static int FakeRand(){return Current->Variant;}
#define rand FakeRand
struct Opponent{GameStub* Game;CScriptThing Me;Parent Owner;Parent& ParentClass;bool RepeatMelee;Opponent(GameStub& g):Game(&g),Me(1),ParentClass(Owner),RepeatMelee(true){g.P=&Owner;}bool IsActiveThreadTerminating()const{return Game->Stopped||Game->Frame>=Game->TerminateAt;}void Main();};
void Opponent::Main()
{
    // Fixture adapter: bind the original reference to the configured dependency.
    ::GameStub& Game = *this->Game;
{
    TRetVoid _Fail_Val;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game.StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    Game.EntitySetAsKillable(Me, false, true);
    while (!ParentClass.TalkedToWhisper) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    seh_me.MoveToPosition(Game.GetThingWithScriptName("M_MeleeOpponentStand").GetPos(), 3.0f, ENTITY_MOVE_RUN, false, true);
    while (seh_me.IsPerformingScriptTask() && ParentClass.WhisperStopWalking != true) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    seh_me.ClearCommands();
    if (seh_me.IsValid()) seh_me.Release();
    ParentClass.WhisperArrived = true;
    Game.EntitySetAsKillable(Me, false, true);
    while (ParentClass.TutorialState == 1) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game.StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                seh_me.Speak(Game.GetHero(), "TEXT_QST_028_WHISPER_MEET", GROUP_SELECT_NONE, false, true, false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (seh_me.IsValid()) seh_me.Release();
        } else if (Me.MsgIsHitBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            ParentClass.EarlyHitWhisper = true;
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    while (RepeatMelee) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        while (ParentClass.TutorialState != 3) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        ParentClass.MeleeOpponentReset = false;
        Game.EntitySetCombatType(Me, "HERO_WHISPER_TUTORIAL_ATTACK_STYLE_DONT_ATTACK");
        Game.GiveThingBestEnemyTarget(Me, Game.GetHero());
        Game.EntitySetInFaction(Me, "FACTION_BANDITS");
        Me.SetFriendsWithEverythingFlag(false);
        CTimer InsultTimer;
        InsultTimer = 15;
        long BadHit = 0;
        while (ParentClass.TutorialState == 3 && ParentClass.GenericTutorialCounter < 7) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (Me.MsgIsHitByWithWeapon("SCRIPT_NAME_HERO", "OBJECT_IRON_LONGSWORD")) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ParentClass.GenericTutorialCounter++;
                if (InsultTimer.GetTime() <= 8) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    long Conversation = Game.AddNewConversation(Me, false, false);
                    Game.AddPersonToConversation(Conversation, Game.GetHero());
                    Game.AddLineToConversation(Conversation, "TEXT_QST_028_WHISPER_MELEE_HIT_INSULT", false, Me, Game.GetHero());
                    InsultTimer = 15;
                }
            } else if (Me.MsgIsHitBy("SCRIPT_NAME_HERO")) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.ModifyThingHealth(Me, 1000.0f, false);
                if (BadHit == 0) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game.IsXbox()) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        Game.DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_SWORD_WIELD_HELP");
                        while (!Game.MsgIsGameInfoClickedPast()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    } else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        Game.DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_SWORD_WIELD_HELP_PC");
                        while (!Game.MsgIsGameInfoClickedPast()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
                BadHit = (BadHit + 1) % 5;
            }
            if (InsultTimer.GetTime() <= 0) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                long Conversation = Game.AddNewConversation(Me, false, false);
                Game.AddPersonToConversation(Conversation, Game.GetHero());
                Game.AddLineToConversation(Conversation, "TEXT_QST_028_WHISPER_MELEE_WAIT_INSULT", false, Me, Game.GetHero());
                InsultTimer = 15;
            }
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        while (ParentClass.TutorialState != 4) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.EntitySetCombatType(Me, "HERO_WHISPER_TUTORIAL_VS_BLOCK_ATTACK_STYLE");
        Game.GiveThingBestEnemyTarget(Me, Game.GetHero());
        Game.EntitySetInFaction(Me, "FACTION_BANDITS");
        Me.SetFriendsWithEverythingFlag(false);
        BadHit = 0;
        InsultTimer = 15;
        while (ParentClass.TutorialState == 4) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (Game.IsPlayerCreatureBlocking() && Game.GetHero().MsgIsHitBy("MeleeOpponent")) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ParentClass.GenericTutorialCounter++;
                if (InsultTimer.GetTime() <= 8) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    long Conversation = Game.AddNewConversation(Me, false, false);
                    Game.AddPersonToConversation(Conversation, Game.GetHero());
                    Game.AddLineToConversation(Conversation, "TEXT_QST_028_WHISPER_BLOCK_HIT_INSULT", false, Me, Game.GetHero());
                    InsultTimer = 15;
                }
            } else if (Game.GetHero().MsgIsHitBy("MeleeOpponent")) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (BadHit == 0) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    CScriptThing Maze = Game.GetThingWithScriptName("TheRealGuildmaster");
                    long Conversation = Game.AddNewConversation(Maze, false, false);
                    Game.AddPersonToConversation(Conversation, Game.GetHero());
                    Game.AddLineToConversation(Conversation, "TEXT_QST_028_MAZE_BLOCK", false, Maze, Game.GetHero());
                    if (Game.IsXbox()) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        Game.DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_BLOCK_HELP");
                        while (!Game.MsgIsGameInfoClickedPast()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    } else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        Game.DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_BLOCK_HELP_PC");
                        while (!Game.MsgIsGameInfoClickedPast()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    InsultTimer = 15;
                } else if (rand() % 2 == 1) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (InsultTimer.GetTime() <= 8) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        long Conversation = Game.AddNewConversation(Me, false, false);
                        Game.AddPersonToConversation(Conversation, Game.GetHero());
                        Game.AddLineToConversation(Conversation, "TEXT_QST_028_WHISPER_BATTLE_HIT_INSULT", false, Me, Game.GetHero());
                        InsultTimer = 15;
                    }
                }
                BadHit = (BadHit + 1) % 5;
            } else if (Me.MsgIsHitBy("SCRIPT_NAME_HERO")) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (InsultTimer.GetTime() <= 8) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    long Conversation = Game.AddNewConversation(Me, false, false);
                    Game.AddPersonToConversation(Conversation, Game.GetHero());
                    Game.AddLineToConversation(Conversation, "TEXT_QST_028_WHISPER_MELEE_NOT_BLOCK", false, Me, Game.GetHero());
                    InsultTimer = 15;
                }
            }
            if (InsultTimer.GetTime() <= 0) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                long Conversation = Game.AddNewConversation(Me, false, false);
                Game.AddPersonToConversation(Conversation, Game.GetHero());
                Game.AddLineToConversation(Conversation, "TEXT_QST_028_WHISPER_BLOCK_WAIT_INSULT", false, Me, Game.GetHero());
                InsultTimer = 15;
            }
            if (Game.GetHealth(Game.GetHero()) < pGlobalScriptDef->GUI_MinHealth) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.ClearThingBestEnemyTarget(Me);
            }
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        while (ParentClass.TutorialState != 6) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.EntitySetCombatType(Me, "HERO_WHISPER_TUTORIAL_ATTACK_STYLE");
        InsultTimer = 15;
        while (ParentClass.TutorialState == 6) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (Me.MsgIsHitBy("SCRIPT_NAME_HERO")) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (InsultTimer.GetTime() <= 8) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (rand() % 2 == 1) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        long Conversation = Game.AddNewConversation(Me, false, false);
                        Game.AddPersonToConversation(Conversation, Game.GetHero());
                        Game.AddLineToConversation(Conversation, "TEXT_QST_028_WHISPER_MELEE_HIT_INSULT", false, Me, Game.GetHero());
                    } else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        CScriptThing Thunder = Game.GetThingWithScriptName("MeleeThunder");
                        long Conversation = Game.AddNewConversation(Thunder, false, false);
                        Game.AddPersonToConversation(Conversation, Game.GetHero());
                        Game.AddLineToConversation(Conversation, "TEXT_QST_028_THUNDER_MELEE_DEFEND", false, Thunder, Game.GetHero());
                    }
                    InsultTimer = 15;
                }
            } else if (Game.IsPlayerCreatureBlocking() && Game.GetHero().MsgIsHitBy("MeleeOpponent")) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (InsultTimer.GetTime() <= 8) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (rand() % 2 == 1) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        long Conversation = Game.AddNewConversation(Me, false, false);
                        Game.AddPersonToConversation(Conversation, Game.GetHero());
                        Game.AddLineToConversation(Conversation, "TEXT_QST_028_WHISPER_BLOCK_HIT_INSULT", false, Me, Game.GetHero());
                    } else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        CScriptThing Thunder = Game.GetThingWithScriptName("MeleeThunder");
                        long Conversation = Game.AddNewConversation(Thunder, false, false);
                        Game.AddPersonToConversation(Conversation, Game.GetHero());
                        Game.AddLineToConversation(Conversation, "TEXT_QST_028_THUNDER_MELEE_ATTACK", false, Thunder, Game.GetHero());
                    }
                    InsultTimer = 15;
                }
            } else if (Game.GetHero().MsgIsHitBy("MeleeOpponent")) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (InsultTimer.GetTime() <= 8) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (rand() % 2 == 1) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        long Conversation = Game.AddNewConversation(Me, false, false);
                        Game.AddPersonToConversation(Conversation, Game.GetHero());
                        Game.AddLineToConversation(Conversation, "TEXT_QST_028_WHISPER_BATTLE_HIT_INSULT", false, Me, Game.GetHero());
                    } else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        CScriptThing Thunder = Game.GetThingWithScriptName("MeleeThunder");
                        long Conversation = Game.AddNewConversation(Thunder, false, false);
                        Game.AddPersonToConversation(Conversation, Game.GetHero());
                        Game.AddLineToConversation(Conversation, "TEXT_QST_028_THUNDER_MELEE_FINISH", false, Thunder, Game.GetHero());
                    }
                    InsultTimer = 15;
                }
            }
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        while (ParentClass.TutorialState != 7) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        while (!ParentClass.MeleeRepeatKnown) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (ParentClass.MeleeRepeating) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        } else {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            RepeatMelee = false;
        }
        ParentClass.MeleeOpponentReset = true;
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}
}
// Attempts: chunk 810 mo_a1..a10 and chunk 828 p0, m1..m12 (STATUS.tsv / ATTEMPTS.log; best = probe/00d56790_m2.cpp)

static bool Balanced(const GameStub& g){return Strings==0&&Things==1&&Handles==0&&Movies==0&&Timers==0&&TimerList.empty()&&g.PauseDepth==0;}
static int Count(const GameStub& g,const char* n){int c=0;for(unsigned i=0;i<g.Lines.size();++i)if(g.Lines[i]==n)++c;return c;}
static int InfoCount(const GameStub& g,const char* n){int c=0;for(unsigned i=0;i<g.Info.size();++i)if(g.Info[i]==n)++c;return c;}
int main(){bool ok=true;
 for(int variant=0;variant<2;++variant){GameStub g;Opponent o(g);g.Variant=variant;g.LowHealth=true;o.Main();ok=ok&&Balanced(g)&&!o.RepeatMelee&&o.Owner.WhisperArrived&&o.Owner.MeleeOpponentReset&&o.Owner.GenericTutorialCounter==8&&g.Rounds==1&&g.Styles.size()==3&&g.FriendsOff==2&&g.MoveCalls==1&&g.KillableCalls==2&&g.ClearEnemy>0&&Count(g,"TEXT_QST_028_WHISPER_MELEE_HIT_INSULT")>=1&&Count(g,"TEXT_QST_028_MAZE_BLOCK")==1&&InfoCount(g,"TEXT_QST_028_ONSCREENHELP_BLOCK_HELP_PC")==1;if(variant==0)ok=ok&&Count(g,"TEXT_QST_028_THUNDER_MELEE_DEFEND")==1&&Count(g,"TEXT_QST_028_THUNDER_MELEE_ATTACK")==1&&Count(g,"TEXT_QST_028_THUNDER_MELEE_FINISH")==1;else ok=ok&&Count(g,"TEXT_QST_028_WHISPER_MELEE_NOT_BLOCK")==1&&Count(g,"TEXT_QST_028_WHISPER_BLOCK_HIT_INSULT")==1&&Count(g,"TEXT_QST_028_WHISPER_BATTLE_HIT_INSULT")>=2;}
 for(int platform=0;platform<2;++platform){GameStub g;Opponent o(g);g.Xbox=platform!=0;g.SwordHits=false;g.PlainHits=true;g.InfoDelay=0;o.Main();ok=ok&&Balanced(g)&&g.HealthChanges==7&&InfoCount(g,platform?"TEXT_QST_028_ONSCREENHELP_SWORD_WIELD_HELP":"TEXT_QST_028_ONSCREENHELP_SWORD_WIELD_HELP_PC")==2&&InfoCount(g,platform?"TEXT_QST_028_ONSCREENHELP_BLOCK_HELP":"TEXT_QST_028_ONSCREENHELP_BLOCK_HELP_PC")==1&&o.Owner.GenericTutorialCounter==1;}
 {GameStub g;Opponent o(g);g.SwordHits=false;g.Phase3Limit=18;o.Main();ok=ok&&Balanced(g)&&Count(g,"TEXT_QST_028_WHISPER_MELEE_WAIT_INSULT")==1;}
 {GameStub g;Opponent o(g);g.RepeatOnce=true;o.Owner.MeleeRepeating=true;o.Main();ok=ok&&Balanced(g)&&g.Rounds==2&&g.Styles.size()==6&&o.Owner.MeleeOpponentReset&&!o.RepeatMelee;}
 {GameStub g;Opponent o(g);g.StartWithMeeting=true;o.Owner.TutorialState=1;o.Main();ok=ok&&Balanced(g)&&o.Owner.EarlyHitWhisper&&Count(g,"TEXT_QST_028_WHISPER_MEET")==1&&o.Owner.WhisperArrived;}
 for(int platform=0;platform<2;++platform){GameStub g;Opponent o(g);g.Xbox=platform!=0;g.SwordHits=false;g.PlainHits=true;g.InfoDelay=100;g.StopPhase=3;g.StopTick=2;o.Main();ok=ok&&Balanced(g)&&g.Info.size()==1&&!o.Owner.MeleeOpponentReset&&g.Styles.size()==1&&g.Stopped;}
 for(int platform=0;platform<2;++platform){GameStub g;Opponent o(g);g.Xbox=platform!=0;g.InfoDelay=100;g.StopPhase=4;g.StopTick=3;o.Main();ok=ok&&Balanced(g)&&g.Info.size()==1&&!o.Owner.MeleeOpponentReset&&g.Styles.size()==2&&g.Stopped;}
 {GameStub g;Opponent o(g);g.StartWithMeeting=true;o.Owner.TutorialState=1;g.TerminateAt=3;o.Main();ok=ok&&Balanced(g)&&o.Owner.WhisperArrived&&Count(g,"TEXT_QST_028_WHISPER_MEET")==1&&g.Rounds==0;}
 {GameStub g;Opponent o(g);g.TaskLength=5;g.TerminateAt=3;o.Main();ok=ok&&Balanced(g)&&!o.Owner.WhisperArrived&&g.MoveCalls==1&&g.Styles.empty();}
 {GameStub g;Opponent o(g);g.StartAt=10;g.TerminateAt=3;o.Main();ok=ok&&Balanced(g)&&g.KillableCalls==0&&g.Styles.empty()&&!o.Owner.WhisperArrived;}
 {GameStub g;Opponent o(g);o.Owner.TalkedToWhisper=false;g.KeepWaitingTalk=true;g.TerminateAt=3;o.Main();ok=ok&&Balanced(g)&&g.MoveCalls==0&&g.KillableCalls==1&&!o.Owner.WhisperArrived;}
 {GameStub g;Opponent o(g);g.StopPhase=6;g.StopTick=8;o.Main();ok=ok&&Balanced(g)&&g.Styles.size()==3&&!o.Owner.MeleeOpponentReset&&g.Stopped;}
 ok=ok&&Strings==0&&Things==0&&Handles==0&&Movies==0&&Timers==0;std::printf(ok?"00d56790_TEST PASS\n":"00d56790_TEST FAIL\n");return ok?0:1;
}