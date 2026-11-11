// All original Main macros and the complete Main body are copied below.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <cassert>
static int Strings,Things,Handles,Movies,Timers;
class CCharString{ public:std::string text;CCharString():text(){++Strings;}CCharString(const char* p):text(p){++Strings;}CCharString(const CCharString& p):text(p.text){++Strings;}~CCharString(){--Strings;}};
struct GameStub;static GameStub* Current;
class CScriptThing{ public:int id;CScriptThing(int i=0):id(i){++Things;}CScriptThing(const CScriptThing& p):id(p.id){++Things;}~CScriptThing(){--Things;}bool MsgIsTalkedToBy(const CCharString&)const;void SetFriendsWithEverythingFlag(bool);};
class CTimer;static std::vector<CTimer*> TimerList;
class CTimer{ public:long time;CTimer():time(0){++Timers;TimerList.push_back(this);}~CTimer(){--Timers;for(unsigned i=0;i<TimerList.size();++i)if(TimerList[i]==this){TimerList.erase(TimerList.begin()+i);break;}}CTimer& operator=(long t){time=t;return *this;}long GetTime()const{return time;}};
class CScriptGameResourceObjectMovieBase{ public:CScriptGameResourceObjectMovieBase(){++Movies;}~CScriptGameResourceObjectMovieBase(){--Movies;}};
class CScriptGameResourceObjectScriptedThingBase{ public:int id;CScriptGameResourceObjectScriptedThingBase():id(0){++Handles;}~CScriptGameResourceObjectScriptedThingBase(){--Handles;}bool IsValid()const{return id!=0;}void Release(){id=0;}CScriptThing GetScriptThing()const{return CScriptThing(id);}bool IsPerformingScriptTask()const;void Speak(const CScriptThing&,const char*,int,bool,bool,bool);};
struct Parent{long GameState;bool ReachedPlatform;Parent():GameState(3),ReachedPlatform(false){}};
struct GlobalDef{float GUI_RaceTime,GUI_RaceGold;GlobalDef():GUI_RaceTime(10),GUI_RaceGold(17.9f){}};static GlobalDef Def;static GlobalDef* pGlobalScriptDef=&Def;
enum{SCRIPT_AI_PRIORITY_HIGHEST=4,GROUP_SELECT_NONE=0};
struct GameStub{
 int Frame,TerminateAt,StartAt,TaskUntil,TaskLength,QuestionReady,QuestionDelay,RematchDelay,SuccessDelay,CurrentAnswer,InitialAnswer,RematchAnswer,SuccessAnswer,Scenario,RaceRuns,RaceStartedFrame,RaceTalkDelay,PreviousMode,ModeTick,StopMode,StopTick,PauseDepth,AttachCount,KillableCount,PushCalls,FriendsCount,InfoSets,InfoClears,MarkerAdds,MarkerRemoves,Gold,QuestShown;
 bool Conscious,PredicateOkay,Stopped,Near,Reached,BoastAsked,RematchAsked,SuccessAsked,TalkDuringRace;float Health;Parent* P;long* Mode;std::vector<std::string>Lines,Questions,Events;std::vector<int>Speakers,RemovedTimers,AddedTimes;
 GameStub():Frame(0),TerminateAt(95),StartAt(0),TaskUntil(0),TaskLength(1),QuestionReady(0),QuestionDelay(1),RematchDelay(1),SuccessDelay(1),CurrentAnswer(0),InitialAnswer(1),RematchAnswer(0),SuccessAnswer(1),Scenario(0),RaceRuns(0),RaceStartedFrame(0),RaceTalkDelay(3),PreviousMode(-1),ModeTick(0),StopMode(-1),StopTick(-1),PauseDepth(0),AttachCount(0),KillableCount(0),PushCalls(0),FriendsCount(0),InfoSets(0),InfoClears(0),MarkerAdds(0),MarkerRemoves(0),Gold(0),QuestShown(0),Conscious(true),PredicateOkay(true),Stopped(false),Near(true),Reached(true),BoastAsked(false),RematchAsked(false),SuccessAsked(false),TalkDuringRace(true),Health(10),P(0),Mode(0){Current=this;}
 void NewScriptFrame(){++Frame;assert(Frame<1000);for(unsigned i=0;i<TimerList.size();++i)if(TimerList[i]->time>0)--TimerList[i]->time;if(!PredicateOkay)Stopped=true;if(Mode){if(*Mode!=PreviousMode){PreviousMode=(int)*Mode;ModeTick=0;}else ++ModeTick;if(*Mode==1&&RaceRuns&&Frame-RaceStartedFrame>=2)P->ReachedPlatform=Scenario==4?(RaceRuns>=2):Reached;if(*Mode==StopMode&&ModeTick>=StopTick)Stopped=true;}}
 bool StartScriptingEntity(const CScriptThing& t,CScriptGameResourceObjectScriptedThingBase& h,int p){assert(t.id==1&&p==4);if(Frame<StartAt)return false;h.id=1;return true;}
 CScriptThing GetHero(){return CScriptThing(5);}CScriptThing GetThingWithScriptName(const CCharString& n){int id=n.text=="SpeedFriend"?2:n.text=="RaceMarker"?4:0;assert(id);return CScriptThing(id);}CScriptThing GetNearestWithDefName(const CScriptThing& t,const CCharString& n){assert(t.id==1&&n.text=="VILLAGE_GUILD_COMPLEX_INSIDE");return CScriptThing(3);}
 void SetIsPushableByHero(const CScriptThing& t,bool b){assert(t.id==1&&!b);++PushCalls;}void EntityAttachToVillage(const CScriptThing& t,const CScriptThing& v){assert((t.id==1||t.id==2)&&v.id==3);++AttachCount;}void EntitySetAsKillable(const CScriptThing& t,bool a,bool b){assert((t.id==1||t.id==2)&&!a&&b);++KillableCount;}void SetThingHasInformation(const CScriptThing& t,bool a,bool b,bool c){assert(t.id==1&&!a&&b&&!c);++InfoSets;}void ClearThingHasInformation(const CScriptThing& t){assert(t.id==1);++InfoClears;}
 long AddNewConversation(const CScriptThing& t,bool a,bool b){assert(t.id==1&&!a&&!b);return 1;}void AddPersonToConversation(long,const CScriptThing& t){assert(t.id==2);}void AddLineToConversation(long,const CCharString& n,bool b,const CScriptThing& from,const CScriptThing& to){assert(!b&&((from.id==1&&to.id==2)||(from.id==2&&to.id==1)));Lines.push_back(n.text);Speakers.push_back(from.id);}
 void StartMovieSequence(const CCharString& n,CScriptGameResourceObjectMovieBase&){assert(n.text.empty());}void PauseAllNonScriptedEntities(bool b){PauseDepth+=b?1:-1;assert(PauseDepth>=0&&PauseDepth<=1);}float GetHealth(const CScriptThing& t){assert(t.id==1);return Health;}
 void GiveHeroYesNoQuestion(const CCharString& q,const CCharString& y,const CCharString& n,const CCharString& e,bool b){assert(y.text=="TEXT_OBJECT_HERO_ANSWER_YES"&&n.text=="TEXT_OBJECT_HERO_ANSWER_NO"&&e.text.empty()&&b);Questions.push_back(q.text);int delay=QuestionDelay;if(q.text=="TEXT_QST_028_FAST_APPRENTICE_BOAST_QUESTION"){BoastAsked=true;CurrentAnswer=InitialAnswer;}else if(q.text=="TEXT_QST_028_FAST_APPRENTICE_RETURN_FAIL_QUESTION"){RematchAsked=true;CurrentAnswer=RematchAnswer;delay=RematchDelay;}else{assert(q.text=="TEXT_QST_028_FAST_APPRENTICE_SUCCEED_QUESTION");SuccessAsked=true;CurrentAnswer=SuccessAnswer;delay=SuccessDelay;}QuestionReady=Frame+delay;}
 long MsgIsQuestionAnsweredYesOrNo(){return Frame<QuestionReady?-1:CurrentAnswer;}
 void MiniMapAddMarker(const CScriptThing& t,const CCharString& n){assert(t.id==4&&n.text=="HUD_ORB_QUEST_VIGNETTE");++MarkerAdds;Events.push_back("marker");}void MiniMapRemoveMarker(const CScriptThing& t){assert(t.id==4);++MarkerRemoves;Events.push_back("remove_marker");}
 long AddQuestInfoTimer(const CTimer& t,const CCharString& n,float f){assert(n.text=="HUD_CLOCK_ICON"&&f==1);++RaceRuns;RaceStartedFrame=Frame;AddedTimes.push_back((int)t.GetTime());Events.push_back("timer");return 100+RaceRuns;}void RemoveQuestInfoElement(long i){assert(i>=101&&i<=100+RaceRuns);RemovedTimers.push_back((int)i);}void DisplayQuestInfo(bool b){if(b)++QuestShown;Events.push_back(b?"show":"hide");}void GiveHeroGold(long n){Gold+=(int)n;}
};
bool CScriptThing::MsgIsTalkedToBy(const CCharString& n)const{assert(id==1&&n.text=="SCRIPT_NAME_HERO");GameStub& g=*Current;if(*g.Mode==0)return g.Frame==5&&!g.BoastAsked;if(*g.Mode==1)return g.TalkDuringRace&&g.Frame-g.RaceStartedFrame==g.RaceTalkDelay;if(*g.Mode==2)return g.ModeTick>=2&&!g.RematchAsked;if(*g.Mode==3)return g.ModeTick>=3&&!g.SuccessAsked;return false;}
void CScriptThing::SetFriendsWithEverythingFlag(bool b){assert((id==1||id==2)&&b);++Current->FriendsCount;}
bool CScriptGameResourceObjectScriptedThingBase::IsPerformingScriptTask()const{return Current->Frame<Current->TaskUntil;}
void CScriptGameResourceObjectScriptedThingBase::Speak(const CScriptThing& h,const char* n,int v,bool a,bool b,bool c){assert(id==1&&h.id==5&&v==0&&!a&&b&&!c);Current->Lines.push_back(n);Current->Speakers.push_back(1);Current->TaskUntil=Current->Frame+Current->TaskLength;}
struct CWideScreenMagicPauseEntities{GameStub* g;CWideScreenMagicPauseEntities(GameStub& p):g(&p){g->PauseAllNonScriptedEntities(true);}~CWideScreenMagicPauseEntities(){g->PauseAllNonScriptedEntities(false);}};struct CIsThingConscious{bool okay;CIsThingConscious(const CScriptThing& t):okay(Current->Conscious){assert(t.id==1);}};struct TRetVoid{void Return(){}};
static bool IsDistanceBetweenThingsUnder(const CScriptThing& h,const CScriptThing& t,float d){assert(h.id==5&&t.id==1&&d==10);return Current->Near;}
struct SpeedTest{GameStub* Game;CScriptThing Me;Parent Owner;Parent& ParentClass;long RaceMode;SpeedTest(GameStub& g):Game(&g),Me(1),ParentClass(Owner),RaceMode(0){g.P=&Owner;g.Mode=&RaceMode;}void SetPredicate(const CIsThingConscious& p){Game->PredicateOkay=p.okay;}bool IsActiveThreadTerminating()const{return Game->Stopped||Game->Frame>=Game->TerminateAt;}void Main();};
#define T IsActiveThreadTerminating()
#define NSF Game.NewScriptFrame()
#define ST_START \
    if (seh_me.IsValid()) seh_me.Release(); \
    while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) { \
        NSF; \
        if (T) return _Fail_Val.Return(); \
    } \
    if (T) return _Fail_Val.Return();
#define ST_SAY(TEXT) \
    if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) { \
        seh_me.Speak(Game.GetHero(),TEXT,GROUP_SELECT_NONE,false,true,false); \
        while (seh_me.IsPerformingScriptTask()) { \
            NSF; \
            if (T) return _Fail_Val.Return(); \
        } \
        if (T) return _Fail_Val.Return(); \
    }
#define ST_MOVIE \
    CScriptGameResourceObjectMovieBase speech_movie; \
    Game.StartMovieSequence("",speech_movie); \
    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
#define ST_ASK(TEXT) \
    Game.GiveHeroYesNoQuestion(TEXT,"TEXT_OBJECT_HERO_ANSWER_YES","TEXT_OBJECT_HERO_ANSWER_NO","",true); \
    long answer=Game.MsgIsQuestionAnsweredYesOrNo(); \
    while (answer<0) { \
        NSF; \
        if (T) return _Fail_Val.Return(); \
        answer=Game.MsgIsQuestionAnsweredYesOrNo(); \
    } \
    if (T) return _Fail_Val.Return();
#define ST_ME_LINE(TEXT) \
    if (T) return _Fail_Val.Return(); \
    Game.AddLineToConversation(Conversation,TEXT,false,Me,SpeedFriend);
#define ST_FRIEND_LINE(TEXT) \
    if (T) return _Fail_Val.Return(); \
    Game.AddLineToConversation(Conversation,TEXT,false,SpeedFriend,Me);
#define ST_PRE_ARGUE \
    if (IsDistanceBetweenThingsUnder(Game.GetHero(),Me,10.0f) && SpeedTimer.GetTime()<=0) { \
        if (T) return _Fail_Val.Return(); \
        long Conversation=Game.AddNewConversation(Me,false,false); \
        Game.AddPersonToConversation(Conversation,SpeedFriend); \
        if (LineOfSpeech<=5) { \
            if (T) return _Fail_Val.Return(); \
            switch (LineOfSpeech) { \
            case 1: Game.AddLineToConversation(Conversation,"TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_FIRST_LINE",false,Me,SpeedFriend); break; \
            case 2: Game.AddLineToConversation(Conversation,"TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_SECOND_LINE",false,SpeedFriend,Me); break; \
            case 3: Game.AddLineToConversation(Conversation,"TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_THIRD_LINE",false,Me,SpeedFriend); break; \
            case 4: Game.AddLineToConversation(Conversation,"TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_FOURTH_LINE",false,SpeedFriend,Me); break; \
            case 5: Game.AddLineToConversation(Conversation,"TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_FIFTH_LINE",false,Me,SpeedFriend); break; \
            } \
        } \
        else if (LineOfSpeech%2==0) { \
            ST_FRIEND_LINE("TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_SIXTH_LINE") \
        } \
        else { \
            ST_ME_LINE("TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_SEVENTH_LINE") \
        } \
        SpeedTimer=3; \
        LineOfSpeech++; \
    }
#define ST_RACE_START \
    RaceTimer=(long)pGlobalScriptDef->GUI_RaceTime; \
    KillRaceTimer=(long)(pGlobalScriptDef->GUI_RaceTime+20.0f); \
    RunningGameTimerID=Game.AddQuestInfoTimer(RaceTimer,"HUD_CLOCK_ICON",1.0f);
#define ST_REMOVE_MARKER \
    if (!ParentClass.ReachedPlatform) { \
        if (T) return _Fail_Val.Return(); \
        Game.MiniMapRemoveMarker(Game.GetThingWithScriptName("RaceMarker")); \
    }
void SpeedTest::Main()
{
    // Fixture adapter: bind the original reference to the configured dependency.
    ::GameStub& Game = *this->Game;
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingConscious(Me));
    NSF;
    if (T) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    ST_START
    Game.SetIsPushableByHero(Me,false);
    CScriptThing SpeedFriend=Game.GetThingWithScriptName("SpeedFriend");
    CScriptThing village=Game.GetNearestWithDefName(Me,"VILLAGE_GUILD_COMPLEX_INSIDE");
    Game.EntityAttachToVillage(Me,village);
    Game.EntityAttachToVillage(SpeedFriend,village);
    Game.EntitySetAsKillable(Me,false,true);
    Game.EntitySetAsKillable(SpeedFriend,false,true);
    Game.SetThingHasInformation(Me,false,true,false);
    Me.SetFriendsWithEverythingFlag(true);
    SpeedFriend.SetFriendsWithEverythingFlag(true);
    RaceMode=0;
    long LineOfSpeech=0;
    CTimer RaceTimer;
    CTimer SpeedTimer;
    CTimer KillRaceTimer;
    SpeedTimer=1;
    long RunningGameTimerID=-1;
    bool Green=false;
    for (; !false; NSF) {
        if (T) return _Fail_Val.Return();
        while (RaceMode==0) {
            NSF;
            if (T) return _Fail_Val.Return();
            if (ParentClass.GameState==3 && !Green) {
                if (T) return _Fail_Val.Return();
                Game.SetThingHasInformation(Me,false,true,false);
                Green=true;
            }
            ST_PRE_ARGUE
            if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                if (T) return _Fail_Val.Return();
                ST_MOVIE
                ST_SAY("TEXT_QST_028_FAST_APPRENTICE_BOAST")
                ST_ASK("TEXT_QST_028_FAST_APPRENTICE_BOAST_QUESTION")
                if (answer==1) {
                    if (T) return _Fail_Val.Return();
                    ST_SAY("TEXT_QST_028_FAST_APPRENTICE_RUN")
                    RaceMode=1;
                    ParentClass.ReachedPlatform=false;
                    Game.MiniMapAddMarker(Game.GetThingWithScriptName("RaceMarker"),"HUD_ORB_QUEST_VIGNETTE");
                    ST_RACE_START
                    Game.DisplayQuestInfo(true);
                    if (!Green) {
                        if (T) return _Fail_Val.Return();
                        Game.SetThingHasInformation(Me,false,true,false);
                        Green=true;
                    }
                }
                else {
                    if (T) return _Fail_Val.Return();
                    ST_SAY("TEXT_QST_028_FAST_APPRENTICE_REFUSE")
                }
            }
        }
        if (T) return _Fail_Val.Return();
        while (RaceMode==1) {
            NSF;
            if (T) return _Fail_Val.Return();
            ST_PRE_ARGUE
            if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                if (T) return _Fail_Val.Return();
                Game.RemoveQuestInfoElement(RunningGameTimerID);
                Game.DisplayQuestInfo(false);
                if (!ParentClass.ReachedPlatform) {
                    if (T) return _Fail_Val.Return();
                    ST_MOVIE
                    ST_SAY("TEXT_QST_028_FAST_APPRENTICE_NO_PLATFORM")
                    RaceMode=2;
                }
                else if (RaceTimer.GetTime()<=0) {
                    if (T) return _Fail_Val.Return();
                    ST_MOVIE
                    ST_SAY("TEXT_QST_028_FAST_APPRENTICE_TOO_SLOW")
                    RaceMode=2;
                    ST_REMOVE_MARKER
                }
                else {
                    if (T) return _Fail_Val.Return();
                    ST_MOVIE
                    ST_SAY("TEXT_QST_028_FAST_APPRENTICE_SUCCEED")
                    RaceMode=3;
                    Game.GiveHeroGold((long)pGlobalScriptDef->GUI_RaceGold);
                    Game.ClearThingHasInformation(Me);
                    LineOfSpeech=0;
                }
            }
            if (KillRaceTimer.GetTime()<=0) {
                if (T) return _Fail_Val.Return();
                Game.RemoveQuestInfoElement(RunningGameTimerID);
                Game.DisplayQuestInfo(false);
                ST_REMOVE_MARKER
                RaceMode=2;
            }
        }
        if (T) return _Fail_Val.Return();
        while (RaceMode==2) {
            NSF;
            if (T) return _Fail_Val.Return();
            ST_PRE_ARGUE
            if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                if (T) return _Fail_Val.Return();
                ST_MOVIE
                ST_SAY("TEXT_QST_028_FAST_APPRENTICE_RETURN_FAIL")
                ST_ASK("TEXT_QST_028_FAST_APPRENTICE_RETURN_FAIL_QUESTION")
                if (answer==1) {
                    if (T) return _Fail_Val.Return();
                    ST_SAY("TEXT_QST_028_FAST_APPRENTICE_RUN")
                    RaceMode=1;
                    ParentClass.ReachedPlatform=false;
                    ST_RACE_START
                    Game.MiniMapAddMarker(Game.GetThingWithScriptName("RaceMarker"),"HUD_ORB_QUEST_VIGNETTE");
                }
                else {
                    if (T) return _Fail_Val.Return();
                    ST_SAY("TEXT_QST_028_FAST_APPRENTICE_REFUSE")
                }
            }
        }
        if (T) return _Fail_Val.Return();
        while (RaceMode==3) {
            NSF;
            if (T) return _Fail_Val.Return();
            if (IsDistanceBetweenThingsUnder(Game.GetHero(),Me,10.0f) && SpeedTimer.GetTime()<=0) {
                if (T) return _Fail_Val.Return();
                long Conversation=Game.AddNewConversation(Me,false,false);
                Game.AddPersonToConversation(Conversation,SpeedFriend);
                if (LineOfSpeech<=5) {
                    if (T) return _Fail_Val.Return();
                    switch (LineOfSpeech) {
                    case 1: Game.AddLineToConversation(Conversation,"TEXT_QST_028_FAST_FRIEND_POST_ARGUE_FIRST_LINE",false,SpeedFriend,Me); break;
                    case 2: Game.AddLineToConversation(Conversation,"TEXT_QST_028_FAST_APPRENTICE_POST_ARGUE_SECOND_LINE",false,Me,SpeedFriend); break;
                    case 3: Game.AddLineToConversation(Conversation,"TEXT_QST_028_FAST_FRIEND_POST_ARGUE_THIRD_LINE",false,SpeedFriend,Me); break;
                    case 4: Game.AddLineToConversation(Conversation,"TEXT_QST_028_FAST_APPRENTICE_POST_ARGUE_FOURTH_LINE",false,Me,SpeedFriend); break;
                    case 5: Game.AddLineToConversation(Conversation,"TEXT_QST_028_FAST_FRIEND_POST_ARGUE_FIFTH_LINE",false,SpeedFriend,Me); break;
                    }
                }
                else if (LineOfSpeech%2==0) {
                    ST_ME_LINE("TEXT_QST_028_FAST_APPRENTICE_POST_ARGUE_SIXTH_LINE")
                }
                else {
                    ST_FRIEND_LINE("TEXT_QST_028_FAST_FRIEND_POST_ARGUE_SEVENTH_LINE")
                }
                SpeedTimer=7;
                LineOfSpeech++;
            }
            if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                if (T) return _Fail_Val.Return();
                ST_MOVIE
                ST_ASK("TEXT_QST_028_FAST_APPRENTICE_SUCCEED_QUESTION")
                if (answer==1) {
                    if (T) return _Fail_Val.Return();
                    ST_SAY("TEXT_QST_028_FAST_APPRENTICE_SUCCEED_QUESTION_YES")
                }
                else {
                    if (T) return _Fail_Val.Return();
                    ST_SAY("TEXT_QST_028_FAST_APPRENTICE_SUCCEED_QUESTION_NO")
                }
                Game.MiniMapRemoveMarker(Game.GetThingWithScriptName("RaceMarker"));
            }
        }
        if (T) return _Fail_Val.Return();
    }
    if (T) return _Fail_Val.Return();
}
}

static bool Balanced(const GameStub& g){return Strings==0&&Things==1&&Handles==0&&Movies==0&&Timers==0&&TimerList.empty()&&g.PauseDepth==0;}
static int Count(const GameStub& g,const char* n){int c=0;for(unsigned i=0;i<g.Lines.size();++i)if(g.Lines[i]==n)++c;return c;}
static bool SpokenBy(const GameStub& g,const char* n,int who){for(unsigned i=0;i<g.Lines.size();++i)if(g.Lines[i]==n&&g.Speakers[i]==who)return true;return false;}
int main(){bool ok=true;
 {GameStub g;SpeedTest o(g);g.InitialAnswer=0;o.Main();ok=ok&&Balanced(g)&&o.RaceMode==0&&g.BoastAsked&&g.RaceRuns==0&&g.MarkerAdds==0&&Count(g,"TEXT_QST_028_FAST_APPRENTICE_REFUSE")==1&&Count(g,"TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_FIFTH_LINE")>0&&SpokenBy(g,"TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_SIXTH_LINE",2)&&SpokenBy(g,"TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_SEVENTH_LINE",1);}
 for(int answer=0;answer<2;++answer){GameStub g;SpeedTest o(g);g.SuccessAnswer=answer;o.Main();ok=ok&&Balanced(g)&&o.RaceMode==3&&g.Gold==17&&g.InfoClears==1&&g.InfoSets==2&&g.AttachCount==2&&g.KillableCount==2&&g.FriendsCount==2&&g.PushCalls==1&&g.RaceRuns==1&&g.AddedTimes[0]==10&&g.RemovedTimers.size()==1&&g.RemovedTimers[0]==101&&g.MarkerAdds==1&&g.MarkerRemoves==1&&g.SuccessAsked&&Count(g,answer?"TEXT_QST_028_FAST_APPRENTICE_SUCCEED_QUESTION_YES":"TEXT_QST_028_FAST_APPRENTICE_SUCCEED_QUESTION_NO")==1&&SpokenBy(g,"TEXT_QST_028_FAST_APPRENTICE_POST_ARGUE_SIXTH_LINE",1)&&SpokenBy(g,"TEXT_QST_028_FAST_FRIEND_POST_ARGUE_SEVENTH_LINE",2);}
 {GameStub g;SpeedTest o(g);g.Reached=false;o.Main();ok=ok&&Balanced(g)&&o.RaceMode==2&&g.Gold==0&&g.RaceRuns==1&&g.RematchAsked&&g.MarkerRemoves==0&&Count(g,"TEXT_QST_028_FAST_APPRENTICE_NO_PLATFORM")==1&&Count(g,"TEXT_QST_028_FAST_APPRENTICE_RETURN_FAIL")==1&&Count(g,"TEXT_QST_028_FAST_APPRENTICE_REFUSE")==1;}
 {GameStub g;SpeedTest o(g);g.RaceTalkDelay=12;o.Main();ok=ok&&Balanced(g)&&o.RaceMode==2&&g.Gold==0&&g.RemovedTimers.size()==1&&g.MarkerRemoves==0&&Count(g,"TEXT_QST_028_FAST_APPRENTICE_TOO_SLOW")==1;}
 for(int reached=0;reached<2;++reached){GameStub g;SpeedTest o(g);g.Reached=reached!=0;g.TalkDuringRace=false;o.Main();ok=ok&&Balanced(g)&&o.RaceMode==2&&g.Gold==0&&g.RemovedTimers.size()==1&&g.MarkerRemoves==(reached?0:1)&&Count(g,"TEXT_QST_028_FAST_APPRENTICE_NO_PLATFORM")==0&&Count(g,"TEXT_QST_028_FAST_APPRENTICE_TOO_SLOW")==0;}
 {GameStub g;SpeedTest o(g);g.Scenario=4;g.RematchAnswer=1;o.Main();ok=ok&&Balanced(g)&&o.RaceMode==3&&g.RaceRuns==2&&g.MarkerAdds==2&&g.RemovedTimers.size()==2&&g.RemovedTimers[0]==101&&g.RemovedTimers[1]==102&&g.Gold==17&&g.QuestShown==1&&Count(g,"TEXT_QST_028_FAST_APPRENTICE_NO_PLATFORM")==1;unsigned timers=0;for(unsigned i=0;i<g.Events.size();++i)if(g.Events[i]=="timer"){++timers;if(timers==1)ok=ok&&i>0&&g.Events[i-1]=="marker";else ok=ok&&i+1<g.Events.size()&&g.Events[i+1]=="marker";}}
 {GameStub g;SpeedTest o(g);g.InitialAnswer=0;g.Health=0;g.Near=false;o.Owner.GameState=0;o.Main();ok=ok&&Balanced(g)&&g.Lines.empty()&&g.Questions.size()==1&&g.InfoSets==1&&g.RaceRuns==0;}
 {GameStub g;SpeedTest o(g);o.Owner.GameState=0;o.Main();ok=ok&&Balanced(g)&&g.InfoSets==2&&o.RaceMode==3&&g.Gold==17;}
 {GameStub g;SpeedTest o(g);g.Conscious=false;o.Main();ok=ok&&Balanced(g)&&g.Frame==1&&g.Stopped&&g.InfoSets==0&&g.RaceRuns==0;}
 {GameStub g;SpeedTest o(g);g.StartAt=100;g.TerminateAt=4;o.Main();ok=ok&&Balanced(g)&&g.Frame==4&&g.InfoSets==0&&g.RaceRuns==0;}
 {GameStub g;SpeedTest o(g);g.TaskLength=3;g.TerminateAt=6;o.Main();ok=ok&&Balanced(g)&&g.Frame==6&&!g.BoastAsked&&g.RaceRuns==0&&Count(g,"TEXT_QST_028_FAST_APPRENTICE_BOAST")==1;}
 {GameStub g;SpeedTest o(g);g.QuestionDelay=100;g.TerminateAt=8;o.Main();ok=ok&&Balanced(g)&&g.BoastAsked&&g.RaceRuns==0&&g.Frame==8;}
 {GameStub g;SpeedTest o(g);g.TerminateAt=10;o.Main();ok=ok&&Balanced(g)&&o.RaceMode==1&&g.RaceRuns==1&&g.MarkerAdds==1&&g.RemovedTimers.empty()&&g.Gold==0;}
 {GameStub g;SpeedTest o(g);g.Reached=false;g.RematchDelay=100;g.StopMode=2;g.StopTick=5;o.Main();ok=ok&&Balanced(g)&&g.RematchAsked&&!g.SuccessAsked&&o.RaceMode==2&&g.Stopped;}
 {GameStub g;SpeedTest o(g);g.SuccessDelay=100;g.StopMode=3;g.StopTick=4;o.Main();ok=ok&&Balanced(g)&&g.SuccessAsked&&g.Gold==17&&g.MarkerRemoves==0&&g.Stopped;}
 ok=ok&&Strings==0&&Things==0&&Handles==0&&Movies==0&&Timers==0;std::printf(ok?"00d3e2e0_TEST PASS\n":"00d3e2e0_TEST FAIL\n");return ok?0:1;
}