// Complete Fisherman Main body with deterministic game, timer and resource callees.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <cassert>
static int Strings,Things,Handles,Movies,Timers;
class CCharString { public:
 std::string text;CCharString():text(){++Strings;}CCharString(const char* t):text(t){++Strings;}CCharString(const CCharString& t):text(t.text){++Strings;}~CCharString(){--Strings;}
 CCharString& operator=(const CCharString& t){text=t.text;return *this;}CCharString& operator=(const char* t){text=t;return *this;}bool operator<(const CCharString& t)const{return text<t.text;}bool operator==(const char* t)const{return text==t;}operator const char*()const{return text.c_str();}
};
static CCharString operator+(const char* a,const CCharString& b){return CCharString((std::string(a)+b.text).c_str());}
class C3DVector{ public:int id;C3DVector(int n=0):id(n){}};
struct GameStub;static GameStub* Current;
class CScriptThing { public:
 int id;CScriptThing(int n=0):id(n){++Things;}CScriptThing(const CScriptThing& t):id(t.id){++Things;}~CScriptThing(){--Things;}
 C3DVector GetPos()const{return C3DVector(id);}bool IsAlive()const;bool MsgIsTalkedToBy(const char*)const;
};
class CScriptGameResourceObjectMovieBase{ public:CScriptGameResourceObjectMovieBase(){++Movies;}~CScriptGameResourceObjectMovieBase(){--Movies;}};
class CScriptGameResourceObjectScriptedThingBase { public:
 int id;CScriptGameResourceObjectScriptedThingBase():id(0){++Handles;}CScriptGameResourceObjectScriptedThingBase(const CScriptGameResourceObjectScriptedThingBase& h):id(h.id){++Handles;}~CScriptGameResourceObjectScriptedThingBase(){--Handles;}
 CScriptGameResourceObjectScriptedThingBase& operator=(const CScriptGameResourceObjectScriptedThingBase& h){id=h.id;return *this;}
 bool IsValid()const{return id!=0;}void Release(){id=0;}CScriptThing GetScriptThing()const{return CScriptThing(id);}
 void MoveToPosition(const C3DVector&,float,int,bool,bool);bool IsPerformingScriptTask()const;void PlayAnimation(const char*,bool,bool,bool,bool,bool,bool,bool);
};
class CTimer;static std::vector<CTimer*> TimerList;
class CTimer { public:
 int time;CTimer():time(0){++Timers;TimerList.push_back(this);}~CTimer(){--Timers;for(unsigned i=0;i<TimerList.size();++i)if(TimerList[i]==this){TimerList.erase(TimerList.begin()+i);break;}}
 CTimer& operator=(long n){time=(int)n;return *this;}long GetTime()const{return time;}
};
struct CV_FishermanScript {
 bool FishermanMustRunAway,HeroHasSavedFishermanFromWasps,IntroPlayed,QuestCardGiven;long FishermanTasks,WaspsAlive;
 CV_FishermanScript():FishermanMustRunAway(false),HeroHasSavedFishermanFromWasps(false),IntroPlayed(false),QuestCardGiven(false),FishermanTasks(0),WaspsAlive(2){}
};
struct GameStub {
 int Frame,TerminateAt,StartAt,KillWaspsAt,HideGoal,HideMoves,TaskFrames,TaskUntil,ClickedAt,FishingDoneAt,TalkAt,PauseDepth,Created,QuestCards,Objectives,Morality,Completed,IdleCalls,DoorOpens;
 bool Xbox,HasTutorial,Near,PredicateOkay,Stopped,CameraFixed,InterruptIntro;
 CV_FishermanScript* Parent;std::vector<std::string> Lines,Scenes,Info;
 GameStub():Frame(0),TerminateAt(30),StartAt(0),KillWaspsAt(10),HideGoal(1),HideMoves(0),TaskFrames(1),TaskUntil(0),ClickedAt(12),FishingDoneAt(16),TalkAt(14),PauseDepth(0),Created(0),QuestCards(0),Objectives(0),Morality(0),Completed(0),IdleCalls(0),DoorOpens(0),Xbox(false),HasTutorial(true),Near(true),PredicateOkay(true),Stopped(false),CameraFixed(false),InterruptIntro(false),Parent(0){Current=this;}
 void NewScriptFrame(){++Frame;assert(Frame<1000);for(unsigned i=0;i<TimerList.size();++i)if(TimerList[i]->time>0)--TimerList[i]->time;if(Parent&&Frame>=KillWaspsAt)Parent->WaspsAlive=0;if(!PredicateOkay)Stopped=true;}
 bool StartScriptingEntity(const CScriptThing& t,CScriptGameResourceObjectScriptedThingBase& h,int p){assert(p==4||p==2);if(Frame<StartAt)return false;h.id=t.id;return true;}
 CScriptThing GetHero(){return CScriptThing(2);}CScriptThing GetThingWithScriptName(const char* n){int i=0;if(!std::strcmp(n,"MK_INTRO_WASP1"))i=21;else if(!std::strcmp(n,"MK_INTRO_WASP2"))i=22;else if(!std::strcmp(n,"MK_INTRO_WASP3"))i=23;else if(!std::strcmp(n,"M_FishermanHiding"))i=4;else if(!std::strcmp(n,"M_FishermanWindowPos"))i=5;else if(!std::strcmp(n,"FishermanHutDoor"))i=6;else if(!std::strcmp(n,"M_FishermanLookAtRiver"))i=8;assert(i);return CScriptThing(i);}
 CScriptThing GetNearestWithScriptName(const CScriptThing& t,const char* n){assert(t.id==1&&!std::strcmp(n,"FishermanTutorialMain"));return CScriptThing(7);}
 CScriptThing CreateCreature(const char* n,const C3DVector& p,const char* s,bool b){assert(!std::strcmp(n,"CREATURE_HORNET_01")&&!std::strcmp(s,"CutsceneWasp")&&!b&&p.id>=21&&p.id<=23);++Created;return CScriptThing(p.id+10);}
 void StartMovieSequence(const char* s,CScriptGameResourceObjectMovieBase&){assert(!*s);}void PauseAllNonScriptedEntities(bool b){PauseDepth+=b?1:-1;assert(PauseDepth>=0&&PauseDepth<=1);}void FixMovieSequenceCamera(bool b){CameraFixed=b;}
 long AddNewConversation(const CScriptThing&,bool,bool){return 1;}void AddPersonToConversation(long,const CScriptThing& t){assert(t.id==2);}void AddLineToConversation(long,const char* s,bool,const CScriptThing&,const CScriptThing&){Lines.push_back(s);}
 void EntitySetAsScared(const CScriptThing& t,bool b){assert(t.id==1&&!b);}void GiveHeroMorality(long n){Morality+=(int)n;}
 const char* GetActiveQuestName(){return "Q_Fisherman";}void GiveHeroQuestCardDirectly(const char* n,const char* q,bool b){assert(!std::strcmp(n,"OBJECT_QUEST_CARD_FISHERMAN")&&!std::strcmp(q,"Q_Fisherman")&&!b);++QuestCards;}
 void SetQuestCardObjective(const char*,const char* n,const char*,const char*){assert(!std::strcmp(n,"TEXT_QUEST_FISHERMAN_OBJECTIVE_01"));++Objectives;}
 bool IsXbox(){return Xbox;}void DisplayGameInfo(const char* n){Info.push_back(n);}bool MsgIsGameInfoClickedPast(){return Frame>=ClickedAt;}
 void OpenDoor(const CScriptThing& t){assert(t.id==6);++DoorOpens;}void EntitySetAllStategroupsEnabled(const CScriptThing&,bool){}void EntitySetOpinionReactionsEnabled(const CScriptThing&,bool){}void EntitySetDeedReactionsEnabled(const CScriptThing&,bool){}
 void SetThingHasInformation(const CScriptThing&,bool,bool,bool){}bool IsFishingSpotEnabled(const CScriptThing& t){assert(t.id==7);return Frame<FishingDoneAt;}
 void ClearThingHasInformation(const CScriptThing&){}void SetQuestAsCompleted(const char* n,bool a,bool b,bool c){assert(!std::strcmp(n,"Q_Fisherman")&&!a&&b&&!c);++Completed;}
 void EntityForceToLookAtThing(const CScriptThing& t,const CScriptThing& v){assert(t.id==1&&v.id==8);}
};
bool CScriptThing::IsAlive()const{return id==7?Current->HasTutorial:true;}bool CScriptThing::MsgIsTalkedToBy(const char*)const{return Current->Frame==Current->TalkAt;}
void CScriptGameResourceObjectScriptedThingBase::MoveToPosition(const C3DVector& p,float f,int mt,bool a,bool b){assert(id==1&&p.id==4&&f==1&&mt==1&&!a&&b);++Current->HideMoves;Current->TaskUntil=Current->Frame+Current->TaskFrames;}
bool CScriptGameResourceObjectScriptedThingBase::IsPerformingScriptTask()const{return Current->Frame<Current->TaskUntil;}
void CScriptGameResourceObjectScriptedThingBase::PlayAnimation(const char* n,bool,bool,bool,bool,bool,bool,bool){assert(id==1&&!std::strcmp(n,"SCRIPT_CHEER_1"));Current->TaskUntil=Current->Frame+Current->TaskFrames;}
struct CWideScreenMagicPauseEntities{GameStub* g;CWideScreenMagicPauseEntities(GameStub& v):g(&v){g->PauseAllNonScriptedEntities(true);}~CWideScreenMagicPauseEntities(){g->PauseAllNonScriptedEntities(false);}};
struct CIsThingAlive{bool okay;CIsThingAlive(const CScriptThing&):okay(true){}};struct CIsVariableFalse{bool okay;CIsVariableFalse(const bool& b):okay(!b){}};struct PredicatePair{bool okay;PredicatePair(bool b):okay(b){}};static PredicatePair operator&&(const CIsThingAlive& a,const CIsVariableFalse& b){return PredicatePair(a.okay&&b.okay);}
struct TRetVoid{void Return()const{}};
namespace NGlobalConsole{bool GDefaultToUsingPhysicsInCutsceneAnimations=false;}
long NPCAttitudes[3]={0,0,0};CCharString NPCAttitudeStrings[3]={CCharString("LOVE"),CCharString("HATE"),CCharString("NEUTRAL")};
struct GlobalDef{long F_FishermanHelpDelay,FishermanSaveHimMorality,F_FishermanIdleChatDelay;GlobalDef():F_FishermanHelpDelay(2),FishermanSaveHimMorality(5),F_FishermanIdleChatDelay(4){}};static GlobalDef Def;static GlobalDef* pGlobalScriptDef=&Def;
enum{SCRIPT_AI_PRIORITY_HIGHEST=4,SCRIPT_AI_PRIORITY_MEDIUM=2,ENTITY_MOVE_RUN=1};typedef int EScriptEntityMoveType;
static bool IsDistanceFromThingToPositionOver(const CScriptThing& t,const C3DVector& p,float d){assert(t.id==1&&p.id==4&&d==3);return Current->HideMoves<Current->HideGoal;}
static bool IsDistanceBetweenThingsUnder(const CScriptThing& t,const CScriptThing& h,float d){assert(t.id==5&&h.id==2&&d==10);return Current->Near;}
static void RunCutsceneMacro(const char* n,std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>& m,int a,int b,bool c,bool d){assert(m["HERO"].id==2&&m["FISHER"].id==1&&a==0&&b==0&&!c&&d);if(!std::strcmp(n,"CS_FISHERMAN_INTRO")){assert(m["WASP1"].id==31&&m["WASP2"].id==32&&m["WASP3"].id==33);if(Current->InterruptIntro)Current->Frame=Current->TerminateAt;}Current->Scenes.push_back(n);}
struct Fisherman {
 GameStub* Game;CScriptThing Me;CV_FishermanScript Owner;CV_FishermanScript* ParentClass;CTimer ChatterTimer;
 Fisherman(GameStub& g):Game(&g),Me(1),ParentClass(&Owner){g.Parent=&Owner;NPCAttitudes[2]=0;}
 void SetPredicate(const PredicatePair& p){Game->PredicateOkay=p.okay;}bool IsActiveThreadTerminating()const{return Game->Stopped||Game->Frame>=Game->TerminateAt;}void IdleChatter(){++Game->IdleCalls;}void Main();
};
void Fisherman::Main()
{
    // Fixture adapter: bind the original reference to the configured dependency.
    ::GameStub& Game = *this->Game;
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me) && CIsVariableFalse(ParentClass->FishermanMustRunAway));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (!ParentClass->HeroHasSavedFishermanFromWasps) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        ParentClass->FishermanTasks = 1;
        if (seh_me.IsValid()) seh_me.Release();
        while (!Game.StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        {
            CScriptThing CSWasp1 = Game.CreateCreature("CREATURE_HORNET_01", Game.GetThingWithScriptName("MK_INTRO_WASP1").GetPos(), "CutsceneWasp", false);
            CScriptThing CSWasp2 = Game.CreateCreature("CREATURE_HORNET_01", Game.GetThingWithScriptName("MK_INTRO_WASP2").GetPos(), "CutsceneWasp", false);
            CScriptThing CSWasp3 = Game.CreateCreature("CREATURE_HORNET_01", Game.GetThingWithScriptName("MK_INTRO_WASP3").GetPos(), "CutsceneWasp", false);
            CScriptGameResourceObjectScriptedThingBase seh_hero;
            CScriptGameResourceObjectScriptedThingBase seh_wasp1;
            CScriptGameResourceObjectScriptedThingBase seh_wasp2;
            CScriptGameResourceObjectScriptedThingBase seh_wasp3;
            Game.StartScriptingEntity(Game.GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST);
            Game.StartScriptingEntity(CSWasp1, seh_wasp1, SCRIPT_AI_PRIORITY_HIGHEST);
            Game.StartScriptingEntity(CSWasp2, seh_wasp2, SCRIPT_AI_PRIORITY_HIGHEST);
            Game.StartScriptingEntity(CSWasp3, seh_wasp3, SCRIPT_AI_PRIORITY_HIGHEST);
            std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
            mymap["HERO"] = seh_hero;
            mymap["FISHER"] = seh_me;
            mymap["WASP1"] = seh_wasp1;
            mymap["WASP2"] = seh_wasp2;
            mymap["WASP3"] = seh_wasp3;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            Game.FixMovieSequenceCamera(true);
            RunCutsceneMacro("CS_FISHERMAN_INTRO", mymap, 0, 0, false, true);
            Game.FixMovieSequenceCamera(false);
            ParentClass->IntroPlayed = true;
        }
        if (seh_me.IsValid()) seh_me.Release();
        while (!Game.StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        CScriptThing FishermanHides = Game.GetThingWithScriptName("M_FishermanHiding");
        {
            C3DVector local_position = FishermanHides.GetPos();
            float local_range = 3.0f;
            EScriptEntityMoveType local_movetype = ENTITY_MOVE_RUN;
            while (IsDistanceFromThingToPositionOver(seh_me.GetScriptThing(), local_position, local_range)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                seh_me.MoveToPosition(local_position, 1.0f, local_movetype, false, true);
                while (seh_me.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        CScriptThing WindowMarker = Game.GetThingWithScriptName("M_FishermanWindowPos");
        CTimer HelpTimer;
        HelpTimer = pGlobalScriptDef->F_FishermanHelpDelay;
        while (ParentClass->WaspsAlive > 0) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (HelpTimer.GetTime() == 0 && IsDistanceBetweenThingsUnder(WindowMarker, Game.GetHero(), 10.0f)) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                HelpTimer = pGlobalScriptDef->F_FishermanHelpDelay;
                long Conversation = Game.AddNewConversation(Me, false, false);
                Game.AddPersonToConversation(Conversation, Game.GetHero());
                Game.AddLineToConversation(Conversation, "TEXT_QST_023_HELP", false, Me, Game.GetHero());
            }
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.EntitySetAsScared(Me, false);
        ParentClass->HeroHasSavedFishermanFromWasps = true;
        Game.GiveHeroMorality(pGlobalScriptDef->FishermanSaveHimMorality);
        if (seh_me.IsValid()) seh_me.Release();
        while (!Game.StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        {
            CScriptGameResourceObjectScriptedThingBase seh_hero;
            Game.StartScriptingEntity(Game.GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST);
            std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
            mymap["HERO"] = seh_hero;
            mymap["FISHER"] = seh_me;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            Game.FixMovieSequenceCamera(true);
            RunCutsceneMacro("CS_FISHERMAN_THANKS", mymap, 0, 0, false, true);
            Game.FixMovieSequenceCamera(false);
            if (!ParentClass->QuestCardGiven) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_FISHERMAN", Game.GetActiveQuestName(), false);
                ParentClass->QuestCardGiven = true;
                Game.SetQuestCardObjective(Game.GetActiveQuestName(), "TEXT_QUEST_FISHERMAN_OBJECTIVE_01", "", "");
            }
            if (Game.IsXbox()) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.DisplayGameInfo("TEXT_QST_023_FISHING_SPOT_INFO");
                while (!Game.MsgIsGameInfoClickedPast()) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            } else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.DisplayGameInfo("TEXT_QST_023_FISHING_SPOT_INFO_PC");
                while (!Game.MsgIsGameInfoClickedPast()) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
        }
        ParentClass->FishermanTasks = 2;
    }
    Game.OpenDoor(Game.GetThingWithScriptName("FishermanHutDoor"));
    Game.EntitySetAllStategroupsEnabled(Me, false);
    Game.EntitySetOpinionReactionsEnabled(Me, true);
    Game.EntitySetDeedReactionsEnabled(Me, true);
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game.StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_MEDIUM)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    ChatterTimer = pGlobalScriptDef->F_FishermanIdleChatDelay;
    if (ParentClass->FishermanTasks == 2) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.SetThingHasInformation(Me, false, true, false);
        CScriptThing FishingMarker = Game.GetNearestWithScriptName(Me, "FishermanTutorialMain");
        if (FishingMarker.IsAlive()) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            ChatterTimer = pGlobalScriptDef->F_FishermanIdleChatDelay;
            while (Game.IsFishingSpotEnabled(FishingMarker)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                IdleChatter();
                if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    long HLStyleConversationIndex = Game.AddNewConversation(Me, false, false);
                    Game.AddPersonToConversation(HLStyleConversationIndex, Game.GetHero());
                    Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_023_TUTORIAL_10", false, Me, Game.GetHero());
                }
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            ParentClass->FishermanTasks = 3;
        } else {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            long HLStyleConversationIndex = Game.AddNewConversation(Me, false, false);
            Game.AddPersonToConversation(HLStyleConversationIndex, Game.GetHero());
            Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_023_KNOWS_FISHING" + NPCAttitudeStrings[NPCAttitudes[2]], false, Me, Game.GetHero());
            seh_me.PlayAnimation("SCRIPT_CHEER_1", false, false, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false);
            while (seh_me.IsPerformingScriptTask()) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            ParentClass->FishermanTasks = 4;
        }
    }
    Game.ClearThingHasInformation(Me);
    if (ParentClass->FishermanTasks == 3) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (seh_me.IsValid()) seh_me.Release();
        while (!Game.StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        {
            CScriptGameResourceObjectScriptedThingBase seh_hero;
            Game.StartScriptingEntity(Game.GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST);
            std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
            mymap["HERO"] = seh_hero;
            mymap["FISHER"] = seh_me;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            Game.FixMovieSequenceCamera(true);
            if (NPCAttitudeStrings[NPCAttitudes[2]] == "HATE") {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                RunCutsceneMacro("CS_FISHERMAN_CAUGHTFISH_NASTY", mymap, 0, 0, false, true);
            } else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                RunCutsceneMacro("CS_FISHERMAN_CAUGHTFISH", mymap, 0, 0, false, true);
            }
            Game.FixMovieSequenceCamera(false);
        }
        Game.SetQuestAsCompleted(Game.GetActiveQuestName(), false, true, false);
        ParentClass->FishermanTasks = 4;
    }
    if (ParentClass->FishermanTasks == 4) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (seh_me.IsValid()) seh_me.Release();
        while (!Game.StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_MEDIUM)) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.EntityForceToLookAtThing(Me, Game.GetThingWithScriptName("M_FishermanLookAtRiver"));
        ChatterTimer = pGlobalScriptDef->F_FishermanIdleChatDelay;
        while (!IsActiveThreadTerminating()) {
            IdleChatter();
            Game.NewScriptFrame();
        }
    }
}
}

static bool Balanced(const GameStub& g){return Strings==3&&Things==1&&Handles==0&&Movies==0&&Timers==1&&TimerList.size()==1&&g.PauseDepth==0;}
static unsigned Count(const GameStub& g,const char* n){unsigned count=0;for(unsigned i=0;i<g.Lines.size();++i)if(g.Lines[i]==n)++count;return count;}
static bool Scene(const GameStub& g,const char* n){for(unsigned i=0;i<g.Scenes.size();++i)if(g.Scenes[i]==n)return true;return false;}
int main(){bool ok=true;
 {GameStub g;Fisherman f(g);NPCAttitudes[2]=1;f.Main();ok=ok&&Balanced(g)&&f.Owner.IntroPlayed&&f.Owner.HeroHasSavedFishermanFromWasps&&f.Owner.QuestCardGiven&&f.Owner.FishermanTasks==4&&g.Created==3&&g.HideMoves==1&&g.QuestCards==1&&g.Objectives==1&&g.Morality==5&&g.Completed==1&&g.DoorOpens==1&&!g.CameraFixed&&g.Scenes.size()==3&&Scene(g,"CS_FISHERMAN_CAUGHTFISH_NASTY")&&Count(g,"TEXT_QST_023_HELP")>=2&&Count(g,"TEXT_QST_023_TUTORIAL_10")==1&&g.Info.size()==1&&g.Info[0]=="TEXT_QST_023_FISHING_SPOT_INFO_PC";}
 {GameStub g;Fisherman f(g);f.Owner.HeroHasSavedFishermanFromWasps=true;f.Owner.FishermanTasks=2;g.HasTutorial=false;f.Main();ok=ok&&Balanced(g)&&f.Owner.FishermanTasks==4&&Count(g,"TEXT_QST_023_KNOWS_FISHINGLOVE")==1&&g.Completed==0&&g.QuestCards==0&&g.IdleCalls>0;}
 {GameStub g;Fisherman f(g);f.Owner.HeroHasSavedFishermanFromWasps=true;f.Owner.FishermanTasks=3;f.Main();ok=ok&&Balanced(g)&&f.Owner.FishermanTasks==4&&g.Completed==1&&g.Scenes.size()==1&&Scene(g,"CS_FISHERMAN_CAUGHTFISH");}
 {GameStub g;Fisherman f(g);g.Xbox=true;g.HasTutorial=false;g.HideGoal=0;g.ClickedAt=0;f.Owner.WaspsAlive=0;f.Owner.QuestCardGiven=true;f.Main();ok=ok&&Balanced(g)&&g.QuestCards==0&&g.Objectives==0&&g.Morality==5&&g.Info.size()==1&&g.Info[0]=="TEXT_QST_023_FISHING_SPOT_INFO"&&f.Owner.FishermanTasks==4;}
 for(int platform=0;platform<2;++platform){GameStub g;Fisherman f(g);g.Xbox=platform!=0;g.HideGoal=0;f.Owner.WaspsAlive=0;g.ClickedAt=100;g.TerminateAt=5;f.Main();ok=ok&&Balanced(g)&&g.Frame==5&&g.Info.size()==1&&f.Owner.FishermanTasks==1&&f.Owner.HeroHasSavedFishermanFromWasps&&g.Completed==0;}
 {GameStub g;Fisherman f(g);g.InterruptIntro=true;g.TerminateAt=3;f.Main();ok=ok&&Balanced(g)&&f.Owner.IntroPlayed&&!f.Owner.HeroHasSavedFishermanFromWasps&&g.Info.empty();}
 {GameStub g;Fisherman f(g);f.Owner.HeroHasSavedFishermanFromWasps=true;f.Owner.FishermanTasks=2;g.FishingDoneAt=100;g.TerminateAt=5;g.TalkAt=2;f.Main();ok=ok&&Balanced(g)&&f.Owner.FishermanTasks==2&&Count(g,"TEXT_QST_023_TUTORIAL_10")==1&&g.Completed==0;}
 {GameStub g;Fisherman f(g);g.HideGoal=0;g.KillWaspsAt=100;g.TerminateAt=4;f.Main();ok=ok&&Balanced(g)&&!f.Owner.HeroHasSavedFishermanFromWasps&&f.Owner.IntroPlayed&&g.QuestCards==0;}
 {GameStub g;Fisherman f(g);f.Owner.FishermanMustRunAway=true;f.Main();ok=ok&&Balanced(g)&&g.Frame==1&&g.Scenes.empty()&&g.Stopped;}
 {GameStub g;Fisherman f(g);g.StartAt=10;g.TerminateAt=3;f.Main();ok=ok&&Balanced(g)&&g.Frame==3&&g.Scenes.empty();}
 ok=ok&&Strings==3&&Things==0&&Handles==0&&Movies==0&&Timers==0&&TimerList.empty();std::printf(ok?"00e7c8e0_TEST PASS\n":"00e7c8e0_TEST FAIL\n");return ok?0:1;
}