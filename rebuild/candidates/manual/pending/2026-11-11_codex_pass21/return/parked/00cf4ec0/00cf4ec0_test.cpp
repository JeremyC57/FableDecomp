// Whole original Main body, deterministic game/resource callee stubs.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <cassert>
static int Strings,Things,Handles,Movies,Timers;
struct CCharString{std::string text;CCharString():text(){++Strings;}CCharString(const char*s):text(s){++Strings;}CCharString(const CCharString&s):text(s.text){++Strings;}~CCharString(){--Strings;}};
struct GameStub;static GameStub*Current;
struct CScriptThing{int id;CScriptThing(int i=0):id(i){++Things;}CScriptThing(const CScriptThing&t):id(t.id){++Things;}~CScriptThing(){--Things;}};
struct CTimer;static std::vector<CTimer*>TimerList;
struct CTimer{long time;CTimer():time(0){++Timers;TimerList.push_back(this);}~CTimer(){--Timers;for(unsigned i=0;i<TimerList.size();++i)if(TimerList[i]==this){TimerList.erase(TimerList.begin()+i);break;}}CTimer&operator=(long t){time=t;return *this;}long GetTime()const{return time;}};
struct CScriptGameResourceObjectMovieBase{CScriptGameResourceObjectMovieBase(){++Movies;}~CScriptGameResourceObjectMovieBase(){--Movies;}};
struct CScriptGameResourceObjectScriptedThingBase{int id;CScriptGameResourceObjectScriptedThingBase():id(0){++Handles;}~CScriptGameResourceObjectScriptedThingBase(){--Handles;}bool IsValid()const{return id!=0;}void Release();CScriptThing GetScriptThing()const{return CScriptThing(id);}void Speak(const CScriptThing&,const char*,int,bool,bool,bool);bool IsPerformingScriptTask()const;};
struct Parent{bool FansCanLeaveNow,PlayerAttackedFans,InHitCutsceneAlready,OakvaleInsult,ArenaOver;Parent():FansCanLeaveNow(false),PlayerAttackedFans(false),InHitCutsceneAlready(false),OakvaleInsult(false),ArenaOver(false){}};
enum{SCRIPT_AI_PRIORITY_HIGHEST=4,GROUP_SELECT_NONE=0};
struct Line{std::string text;int who,to,frame;bool spoken;Line(const char*t,int w,int a,int f,bool s):text(t),who(w),to(a),frame(f),spoken(s){}};
struct GameStub{Parent*P;int Frame,TerminateAt,QuestAt,AttackAt,LeaveAt,NearAt,TaskLength,TaskUntil,PauseDepth,Starts,ReleaseCalls,MovieStarts,KillOn,KillOff,AddedItem,FailedFan,Conversations;bool Alive,PredicateOkay;float Health[6];std::vector<Line>Lines;std::vector<int>Started,Released,People;
 GameStub():P(0),Frame(0),TerminateAt(100),QuestAt(45),AttackAt(-1),LeaveAt(-1),NearAt(0),TaskLength(1),TaskUntil(0),PauseDepth(0),Starts(0),ReleaseCalls(0),MovieStarts(0),KillOn(0),KillOff(0),AddedItem(0),FailedFan(-1),Conversations(0),Alive(true),PredicateOkay(true){Current=this;for(int i=0;i<6;++i)Health[i]=10;Health[0]=0;}
 void NewScriptFrame(){++Frame;assert(Frame<300);for(unsigned i=0;i<TimerList.size();++i)if(TimerList[i]->time>0)--TimerList[i]->time;if(P){if(AttackAt>=0&&Frame>=AttackAt)P->PlayerAttackedFans=true;if(LeaveAt>=0&&Frame>=LeaveAt)P->FansCanLeaveNow=true;}}
 CScriptThing GetThingWithScriptName(const CCharString&n){int id=n.text=="OakvaleFan"?3:n.text=="BowerstoneFan"?4:n.text=="HookCoastFan"?5:0;assert(id);return CScriptThing(id);}CScriptThing GetHero(){return CScriptThing(2);}bool IsQuestCompleted(const CCharString&n){assert(n.text=="Q_Arena");return QuestAt>=0&&Frame>=QuestAt;}
 void EntitySetAsKillable(const CScriptThing&t,bool a,bool b){assert(t.id>=3&&t.id<=5&&b);a?++KillOn:++KillOff;}bool StartScriptingEntity(const CScriptThing&t,CScriptGameResourceObjectScriptedThingBase&h,int pri){assert(t.id>=3&&t.id<=5&&pri==4);++Starts;Started.push_back(t.id);if(FailedFan==0||FailedFan==t.id)return false;h.id=t.id;return true;}float GetHealth(const CScriptThing&t){assert(t.id==0||(t.id>=3&&t.id<=5));return Health[t.id];}
 void StartMovieSequence(const CCharString&n,CScriptGameResourceObjectMovieBase&){assert(n.text.empty());++MovieStarts;}void PauseAllNonScriptedEntities(bool b){PauseDepth+=b?1:-1;assert(PauseDepth>=0&&PauseDepth<=1);}long AddNewConversation(const CScriptThing&t,bool a,bool b){assert(t.id>=3&&t.id<=5&&!a&&!b);++Conversations;return Conversations;}void AddPersonToConversation(long i,const CScriptThing&t){assert(i==Conversations&&t.id>=2&&t.id<=5);People.push_back(t.id);}void AddLineToConversation(long i,const CCharString&n,bool b,const CScriptThing&from,const CScriptThing&to){assert(i==Conversations&&!b&&from.id>=3&&from.id<=5&&to.id>=2&&to.id<=5);Lines.push_back(Line(n.text.c_str(),from.id,to.id,Frame,false));}void AddItemToContainer(const CScriptThing&t,const CCharString&n){assert(t.id==3&&n.text=="OBJECT_HERO_HELMET_CHAINMAIL");++AddedItem;}
};
void CScriptGameResourceObjectScriptedThingBase::Release(){assert(id>=3&&id<=5);Current->Released.push_back(id);++Current->ReleaseCalls;id=0;}void CScriptGameResourceObjectScriptedThingBase::Speak(const CScriptThing&h,const char*n,int s,bool a,bool b,bool c){assert(id>=3&&id<=5&&h.id==2&&s==0&&!a&&b&&!c);Current->Lines.push_back(Line(n,id,2,Current->Frame,true));Current->TaskUntil=Current->Frame+Current->TaskLength;}bool CScriptGameResourceObjectScriptedThingBase::IsPerformingScriptTask()const{return Current->Frame<Current->TaskUntil;}
struct CIsThingAlive{bool b;CIsThingAlive(const CScriptThing&t):b(Current->Alive){assert(t.id==1);}};struct TRetVoid{void Return(){}};struct CWideScreenMagicPauseEntities{GameStub*g;CWideScreenMagicPauseEntities(GameStub&p):g(&p){g->PauseAllNonScriptedEntities(true);}~CWideScreenMagicPauseEntities(){g->PauseAllNonScriptedEntities(false);}};
static bool IsDistanceBetweenThingsUnder(const CScriptThing&t,const CScriptThing&h,float f){assert(t.id==1&&h.id==2&&f==4);return Current->NearAt>=0&&Current->Frame>=Current->NearAt;}
struct FanMarker{GameStub&Game;CScriptThing Me;Parent ParentClass;FanMarker(GameStub&g):Game(g),Me(1){g.P=&ParentClass;}void SetPredicate(const CIsThingAlive&p){Game.PredicateOkay=p.b;}bool IsActiveThreadTerminating()const{return !Game.PredicateOkay||Game.Frame>=Game.TerminateAt;}void Main();};
void FanMarker::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    CScriptThing OakValeFan = Game.GetThingWithScriptName("OakvaleFan");
    CScriptThing BowerstoneFan = Game.GetThingWithScriptName("BowerstoneFan");
    CScriptThing HookCoastFan = Game.GetThingWithScriptName("HookCoastFan");
    long ChatLevel = 0;
    Game.EntitySetAsKillable(OakValeFan, false, true);
    Game.EntitySetAsKillable(HookCoastFan, false, true);
    Game.EntitySetAsKillable(BowerstoneFan, false, true);
    CScriptGameResourceObjectScriptedThingBase seh_OakValeFan;
    CScriptGameResourceObjectScriptedThingBase seh_BowerstoneFan;
    CScriptGameResourceObjectScriptedThingBase seh_HookCoastFan;
    CTimer ChatTimer;
    ChatTimer = 0;
    while (!ParentClass.FansCanLeaveNow && !Game.IsQuestCompleted("Q_Arena")) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        if (ParentClass.PlayerAttackedFans) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            Game.StartScriptingEntity(OakValeFan, seh_OakValeFan, SCRIPT_AI_PRIORITY_HIGHEST);
            Game.StartScriptingEntity(BowerstoneFan, seh_BowerstoneFan, SCRIPT_AI_PRIORITY_HIGHEST);
            Game.StartScriptingEntity(HookCoastFan, seh_HookCoastFan, SCRIPT_AI_PRIORITY_HIGHEST);
            if (!ParentClass.InHitCutsceneAlready) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                ParentClass.InHitCutsceneAlready = true;
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (Game.GetHealth(seh_OakValeFan.GetScriptThing()) > 0.0f) {
                    seh_OakValeFan.Speak(Game.GetHero(), "TEXT_QST_005_V2_FAN_OAKVALE_CHAT_HIT", GROUP_SELECT_NONE, false, true, false);
                    while (seh_OakValeFan.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
                if (Game.GetHealth(seh_BowerstoneFan.GetScriptThing()) > 0.0f) {
                    seh_BowerstoneFan.Speak(Game.GetHero(), "TEXT_QST_005_V2_FAN_BOWERSTONE_CHAT_HIT", GROUP_SELECT_NONE, false, true, false);
                    while (seh_BowerstoneFan.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
                if (Game.GetHealth(seh_HookCoastFan.GetScriptThing()) > 0.0f) {
                    seh_HookCoastFan.Speak(Game.GetHero(), "TEXT_QST_005_V2_FAN_HOOK_COAST_CHAT_HIT", GROUP_SELECT_NONE, false, true, false);
                    while (seh_HookCoastFan.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
                ParentClass.InHitCutsceneAlready = false;
            } else {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                long HLStyleConversationIndex = Game.AddNewConversation(OakValeFan, false, false);
                Game.AddPersonToConversation(HLStyleConversationIndex, HookCoastFan);
                Game.AddPersonToConversation(HLStyleConversationIndex, BowerstoneFan);
                Game.AddPersonToConversation(HLStyleConversationIndex, Game.GetHero());
                Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_OAKVALE_CHAT_HIT", false, OakValeFan, Game.GetHero());
                Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_BOWERSTONE_CHAT_HIT", false, HookCoastFan, Game.GetHero());
                Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_HOOK_COAST_CHAT_HIT", false, BowerstoneFan, Game.GetHero());
            }
            ParentClass.FansCanLeaveNow = true;
            Game.EntitySetAsKillable(OakValeFan, true, true);
            Game.EntitySetAsKillable(HookCoastFan, true, true);
            Game.EntitySetAsKillable(BowerstoneFan, true, true);
            if (seh_OakValeFan.IsValid())
                seh_OakValeFan.Release();
            if (seh_HookCoastFan.IsValid())
                seh_HookCoastFan.Release();
            if (seh_BowerstoneFan.IsValid())
                seh_BowerstoneFan.Release();
        } else if (IsDistanceBetweenThingsUnder(Me, Game.GetHero(), 4.0f) && ChatTimer.GetTime() <= 0) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            switch (ChatLevel) {
            case 0:
            {
                long HLStyleConversationIndex = Game.AddNewConversation(BowerstoneFan, false, false);
                Game.AddPersonToConversation(HLStyleConversationIndex, OakValeFan);
                Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_EARLY_CHAT_FIRST", false, BowerstoneFan, OakValeFan);
                ChatTimer = 3;
                ChatLevel = 1;
                break;
            }
            case 1:
            {
                long HLStyleConversationIndex = Game.AddNewConversation(OakValeFan, false, false);
                Game.AddPersonToConversation(HLStyleConversationIndex, BowerstoneFan);
                Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_EARLY_CHAT_SECOND", false, OakValeFan, BowerstoneFan);
                ChatTimer = 3;
                ChatLevel = 2;
                break;
            }
            case 2:
            {
                long HLStyleConversationIndex = Game.AddNewConversation(HookCoastFan, false, false);
                Game.AddPersonToConversation(HLStyleConversationIndex, OakValeFan);
                Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_EARLY_CHAT_THIRD", false, HookCoastFan, OakValeFan);
                ChatTimer = 3;
                ChatLevel = 3;
                break;
            }
            case 3:
            {
                long HLStyleConversationIndex = Game.AddNewConversation(HookCoastFan, false, false);
                Game.AddPersonToConversation(HLStyleConversationIndex, OakValeFan);
                Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_EARLY_CHAT_FOURTH", false, HookCoastFan, OakValeFan);
                ChatTimer = 3;
                ChatLevel = 4;
                break;
            }
            case 4:
            {
                long HLStyleConversationIndex = Game.AddNewConversation(BowerstoneFan, false, false);
                Game.AddPersonToConversation(HLStyleConversationIndex, HookCoastFan);
                Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_EARLY_CHAT_FIFTH", false, BowerstoneFan, HookCoastFan);
                ChatTimer = 3;
                ChatLevel = 5;
                break;
            }
            case 5:
            {
                long HLStyleConversationIndex = Game.AddNewConversation(OakValeFan, false, false);
                Game.AddPersonToConversation(HLStyleConversationIndex, HookCoastFan);
                Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_EARLY_CHAT_SIXTH", false, OakValeFan, HookCoastFan);
                ChatTimer = 3;
                ChatLevel = 6;
                break;
            }
            case 6:
            {
                long HLStyleConversationIndex = Game.AddNewConversation(BowerstoneFan, false, false);
                Game.AddPersonToConversation(HLStyleConversationIndex, OakValeFan);
                Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_EARLY_CHAT_SEVENTH", false, BowerstoneFan, OakValeFan);
                ChatLevel = 7;
                ChatTimer = 3;
                break;
            }
            case 7:
            {
                long HLStyleConversationIndex = Game.AddNewConversation(OakValeFan, false, false);
                Game.AddPersonToConversation(HLStyleConversationIndex, BowerstoneFan);
                Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_EARLY_CHAT_EIGTH", false, OakValeFan, BowerstoneFan);
                ChatLevel = 8;
                ChatTimer = 3;
                break;
            }
            case 8:
            {
                long HLStyleConversationIndex = Game.AddNewConversation(BowerstoneFan, false, false);
                Game.AddPersonToConversation(HLStyleConversationIndex, OakValeFan);
                Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_EARLY_CHAT_NINTH", false, BowerstoneFan, OakValeFan);
                ChatLevel = 9;
                ChatTimer = 3;
                ParentClass.OakvaleInsult = true;
                break;
            }
            case 9:
            {
                long HLStyleConversationIndex = Game.AddNewConversation(OakValeFan, false, false);
                Game.AddPersonToConversation(HLStyleConversationIndex, BowerstoneFan);
                Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_EARLY_CHAT_TENTH", false, OakValeFan, BowerstoneFan);
                ChatLevel = 10;
                ChatTimer = 3;
                break;
            }
            case 10:
            {
                long HLStyleConversationIndex = Game.AddNewConversation(BowerstoneFan, false, false);
                Game.AddPersonToConversation(HLStyleConversationIndex, OakValeFan);
                Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_EARLY_CHAT_ELEVENTH", false, BowerstoneFan, OakValeFan);
                ChatLevel = 11;
                ChatTimer = 3;
                break;
            }
            case 11:
            {
                long HLStyleConversationIndex = Game.AddNewConversation(OakValeFan, false, false);
                Game.AddPersonToConversation(HLStyleConversationIndex, BowerstoneFan);
                Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_EARLY_CHAT_TWELFTH", false, OakValeFan, BowerstoneFan);
                ChatLevel = 12;
                ChatTimer = 3;
                break;
            }
            case 12:
            {
                long HLStyleConversationIndex = Game.AddNewConversation(BowerstoneFan, false, false);
                Game.AddPersonToConversation(HLStyleConversationIndex, OakValeFan);
                Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_EARLY_CHAT_OAKVALE_INSULT", false, BowerstoneFan, OakValeFan);
                ChatLevel = 13;
                ChatTimer = 3;
                break;
            }
            case 13:
            {
                long HLStyleConversationIndex = Game.AddNewConversation(OakValeFan, false, false);
                Game.AddPersonToConversation(HLStyleConversationIndex, BowerstoneFan);
                Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_EARLY_CHAT_BOWERSTONE_INSULT", false, OakValeFan, BowerstoneFan);
                ChatTimer = 3;
                ChatLevel = 12;
                break;
            }
            }
        }
    }
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
    ChatLevel = 0;
    while (!ParentClass.FansCanLeaveNow && !ParentClass.ArenaOver) {
        Game.NewScriptFrame();
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
        if (ParentClass.PlayerAttackedFans) {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
            Game.StartScriptingEntity(OakValeFan, seh_OakValeFan, SCRIPT_AI_PRIORITY_HIGHEST);
            Game.StartScriptingEntity(BowerstoneFan, seh_BowerstoneFan, SCRIPT_AI_PRIORITY_HIGHEST);
            Game.StartScriptingEntity(HookCoastFan, seh_HookCoastFan, SCRIPT_AI_PRIORITY_HIGHEST);
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (Game.GetHealth(seh_OakValeFan.GetScriptThing()) > 0.0f) {
                seh_OakValeFan.Speak(Game.GetHero(), "TEXT_QST_005_V2_FAN_OAKVALE_CHAT_HIT", GROUP_SELECT_NONE, false, true, false);
                while (seh_OakValeFan.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
            }
            if (Game.GetHealth(seh_BowerstoneFan.GetScriptThing()) > 0.0f) {
                seh_BowerstoneFan.Speak(Game.GetHero(), "TEXT_QST_005_V2_FAN_BOWERSTONE_CHAT_HIT", GROUP_SELECT_NONE, false, true, false);
                while (seh_BowerstoneFan.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
            }
            if (Game.GetHealth(seh_HookCoastFan.GetScriptThing()) > 0.0f) {
                seh_HookCoastFan.Speak(Game.GetHero(), "TEXT_QST_005_V2_FAN_HOOK_COAST_CHAT_HIT", GROUP_SELECT_NONE, false, true, false);
                while (seh_HookCoastFan.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
            }
            ParentClass.FansCanLeaveNow = true;
            Game.EntitySetAsKillable(OakValeFan, true, true);
            Game.EntitySetAsKillable(HookCoastFan, true, true);
            Game.EntitySetAsKillable(BowerstoneFan, true, true);
            if (seh_OakValeFan.IsValid())
                seh_OakValeFan.Release();
            if (seh_HookCoastFan.IsValid())
                seh_HookCoastFan.Release();
            if (seh_BowerstoneFan.IsValid())
                seh_BowerstoneFan.Release();
        } else if (IsDistanceBetweenThingsUnder(Me, Game.GetHero(), 4.0f) && ChatTimer.GetTime() <= 0) {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
            switch (ChatLevel) {
            case 0:
            {
                long HLStyleConversationIndex = Game.AddNewConversation(OakValeFan, false, false);
                Game.AddPersonToConversation(HLStyleConversationIndex, BowerstoneFan);
                Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_LATE_CHAT_FIRST", false, OakValeFan, BowerstoneFan);
                ChatTimer = 3;
                ChatLevel = 1;
                Game.AddItemToContainer(OakValeFan, "OBJECT_HERO_HELMET_CHAINMAIL");
                break;
            }
            case 1:
            {
                long HLStyleConversationIndex = Game.AddNewConversation(BowerstoneFan, false, false);
                Game.AddPersonToConversation(HLStyleConversationIndex, HookCoastFan);
                Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_LATE_CHAT_SECOND", false, BowerstoneFan, HookCoastFan);
                ChatTimer = 3;
                ChatLevel = 2;
                break;
            }
            case 2:
            {
                long HLStyleConversationIndex = Game.AddNewConversation(OakValeFan, false, false);
                Game.AddPersonToConversation(HLStyleConversationIndex, BowerstoneFan);
                Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_LATE_CHAT_THIRD", false, OakValeFan, BowerstoneFan);
                ChatTimer = 3;
                ChatLevel = 3;
                break;
            }
            case 3:
            {
                long HLStyleConversationIndex = Game.AddNewConversation(HookCoastFan, false, false);
                Game.AddPersonToConversation(HLStyleConversationIndex, BowerstoneFan);
                Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_LATE_CHAT_FOURTH", false, HookCoastFan, BowerstoneFan);
                ChatTimer = 3;
                ChatLevel = 4;
                break;
            }
            case 4:
            {
                long HLStyleConversationIndex = Game.AddNewConversation(OakValeFan, false, false);
                Game.AddPersonToConversation(HLStyleConversationIndex, HookCoastFan);
                Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_LATE_CHAT_FIFTH", false, OakValeFan, HookCoastFan);
                ChatTimer = 3;
                ChatLevel = 5;
                break;
            }
            case 5:
            {
                long HLStyleConversationIndex = Game.AddNewConversation(HookCoastFan, false, false);
                Game.AddPersonToConversation(HLStyleConversationIndex, BowerstoneFan);
                Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_LATE_CHAT_SIXTH", false, HookCoastFan, BowerstoneFan);
                ChatTimer = 3;
                ChatLevel = 6;
                break;
            }
            case 6:
            {
                long HLStyleConversationIndex = Game.AddNewConversation(BowerstoneFan, false, false);
                Game.AddPersonToConversation(HLStyleConversationIndex, HookCoastFan);
                Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_005_V2_FAN_LATE_CHAT_SEVENTH", false, BowerstoneFan, HookCoastFan);
                ChatTimer = 3;
                ChatLevel = 7;
                ParentClass.ArenaOver = true;
                Game.EntitySetAsKillable(OakValeFan, true, true);
                Game.EntitySetAsKillable(HookCoastFan, true, true);
                Game.EntitySetAsKillable(BowerstoneFan, true, true);
                break;
            }
            }
        }
    }
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
    while (true) {
        Game.NewScriptFrame();
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
    }
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
}

static bool Balanced(const GameStub&g){return Strings==0&&Things==1&&Handles==0&&Movies==0&&Timers==0&&TimerList.empty()&&g.PauseDepth==0;}
static int Count(const GameStub&g,const char*n){int c=0;for(unsigned i=0;i<g.Lines.size();++i)if(g.Lines[i].text==n)++c;return c;}
static bool Check(bool b,int n){if(!b)std::printf("case %d failed\n",n);return b;}
static bool HitLines(const GameStub&g,bool conversation){if(g.Lines.size()!=3)return false;const char*names[]={"TEXT_QST_005_V2_FAN_OAKVALE_CHAT_HIT","TEXT_QST_005_V2_FAN_BOWERSTONE_CHAT_HIT","TEXT_QST_005_V2_FAN_HOOK_COAST_CHAT_HIT"};int who[]={3,conversation?5:4,conversation?4:5};for(int i=0;i<3;++i)if(g.Lines[i].text!=names[i]||g.Lines[i].who!=who[i]||g.Lines[i].to!=2||g.Lines[i].spoken==conversation)return false;return true;}
int main(){bool ok=true;int n=0;
 {GameStub g;FanMarker o(g);o.Main();const char*early[]={"FIRST","SECOND","THIRD","FOURTH","FIFTH","SIXTH","SEVENTH","EIGTH","NINTH","TENTH","ELEVENTH","TWELFTH","OAKVALE_INSULT","BOWERSTONE_INSULT"};int who[]={4,3,5,5,4,3,4,3,4,3,4,3,4,3};bool all=g.Lines.size()>=21;for(int i=0;i<14&&all;++i){std::string text=std::string("TEXT_QST_005_V2_FAN_EARLY_CHAT_")+early[i];all=g.Lines[i].text==text&&g.Lines[i].who==who[i]&&g.Lines[i].frame==2+3*i;}const char*late[]={"FIRST","SECOND","THIRD","FOURTH","FIFTH","SIXTH","SEVENTH"};int lw[]={3,4,3,5,3,5,4};int found=0;for(unsigned i=0;i<g.Lines.size();++i)if(g.Lines[i].text.find("TEXT_QST_005_V2_FAN_LATE_CHAT_")==0){if(found>=7)all=false;else all=all&&g.Lines[i].text==std::string("TEXT_QST_005_V2_FAN_LATE_CHAT_")+late[found]&&g.Lines[i].who==lw[found];++found;}ok=Check(Balanced(g)&&all&&found==7&&o.ParentClass.OakvaleInsult&&o.ParentClass.ArenaOver&&g.AddedItem==1&&g.KillOff==3&&g.KillOn==3&&g.Starts==0&&g.MovieStarts==0,++n)&&ok;}
 {GameStub g;FanMarker o(g);g.AttackAt=2;g.QuestAt=-1;o.Main();ok=Check(Balanced(g)&&HitLines(g,false)&&o.ParentClass.FansCanLeaveNow&&!o.ParentClass.InHitCutsceneAlready&&!o.ParentClass.ArenaOver&&g.Starts==3&&g.ReleaseCalls==3&&g.Released[0]==3&&g.Released[1]==5&&g.Released[2]==4&&g.MovieStarts==1&&g.KillOn==3&&g.AddedItem==0,++n)&&ok;}
 {GameStub g;FanMarker o(g);g.AttackAt=2;g.QuestAt=-1;o.ParentClass.InHitCutsceneAlready=true;o.Main();ok=Check(Balanced(g)&&HitLines(g,true)&&o.ParentClass.InHitCutsceneAlready&&o.ParentClass.FansCanLeaveNow&&g.MovieStarts==0&&g.Conversations==1&&g.People.size()==3&&g.People[0]==5&&g.People[1]==4&&g.People[2]==2&&g.ReleaseCalls==3,++n)&&ok;}
 {GameStub g;FanMarker o(g);g.QuestAt=1;g.AttackAt=2;o.Main();ok=Check(Balanced(g)&&HitLines(g,false)&&o.ParentClass.FansCanLeaveNow&&!o.ParentClass.InHitCutsceneAlready&&g.KillOn==3&&g.ReleaseCalls==3&&!o.ParentClass.ArenaOver&&g.AddedItem==0,++n)&&ok;}
 {GameStub g;FanMarker o(g);g.AttackAt=2;g.QuestAt=-1;g.Health[3]=g.Health[4]=g.Health[5]=0;o.Main();ok=Check(Balanced(g)&&g.Lines.empty()&&o.ParentClass.FansCanLeaveNow&&!o.ParentClass.InHitCutsceneAlready&&g.KillOn==3&&g.MovieStarts==1&&g.ReleaseCalls==3,++n)&&ok;}
 {GameStub g;FanMarker o(g);g.AttackAt=2;g.QuestAt=-1;g.FailedFan=0;o.Main();ok=Check(Balanced(g)&&g.Lines.empty()&&g.Starts==3&&g.ReleaseCalls==0&&o.ParentClass.FansCanLeaveNow&&g.KillOn==3,++n)&&ok;}
 {GameStub g;FanMarker o(g);g.AttackAt=2;g.QuestAt=-1;g.FailedFan=4;o.Main();ok=Check(Balanced(g)&&g.Lines.size()==2&&g.Lines[0].who==3&&g.Lines[1].who==5&&g.ReleaseCalls==2&&o.ParentClass.FansCanLeaveNow,++n)&&ok;}
 for(int late=0;late<2;++late){GameStub g;FanMarker o(g);g.QuestAt=late?1:-1;g.AttackAt=2;g.TaskLength=100;g.TerminateAt=4;o.Main();ok=Check(Balanced(g)&&g.Lines.size()==1&&g.Lines[0].who==3&&!o.ParentClass.FansCanLeaveNow&&o.ParentClass.InHitCutsceneAlready==!late&&g.KillOn==0&&g.ReleaseCalls==0,++n)&&ok;}
 {GameStub g;FanMarker o(g);g.QuestAt=1;o.Main();ok=Check(Balanced(g)&&o.ParentClass.ArenaOver&&!o.ParentClass.OakvaleInsult&&g.Lines.size()==7&&g.AddedItem==1&&g.Lines[0].frame==2&&g.Lines[6].frame==20,++n)&&ok;}
 {GameStub g;FanMarker o(g);g.QuestAt=-1;g.TerminateAt=50;o.Main();ok=Check(Balanced(g)&&o.ParentClass.OakvaleInsult&&!o.ParentClass.ArenaOver&&g.AddedItem==0&&Count(g,"TEXT_QST_005_V2_FAN_EARLY_CHAT_OAKVALE_INSULT")==2&&Count(g,"TEXT_QST_005_V2_FAN_EARLY_CHAT_BOWERSTONE_INSULT")==2,++n)&&ok;}
 {GameStub g;FanMarker o(g);g.NearAt=-1;o.Main();ok=Check(Balanced(g)&&g.Lines.empty()&&g.AddedItem==0&&!o.ParentClass.ArenaOver&&!o.ParentClass.OakvaleInsult,++n)&&ok;}
 {GameStub g;FanMarker o(g);g.QuestAt=1;g.NearAt=5;g.TerminateAt=10;o.Main();ok=Check(Balanced(g)&&g.Lines.size()==2&&g.Lines[0].frame==5&&g.Lines[1].frame==8&&g.AddedItem==1&&!o.ParentClass.ArenaOver,++n)&&ok;}
 {GameStub g;FanMarker o(g);o.ParentClass.FansCanLeaveNow=true;o.Main();ok=Check(Balanced(g)&&g.Lines.empty()&&g.KillOff==3&&g.KillOn==0&&g.Starts==0,++n)&&ok;}
 {GameStub g;FanMarker o(g);g.QuestAt=1;o.ParentClass.ArenaOver=true;o.Main();ok=Check(Balanced(g)&&g.Lines.empty()&&g.AddedItem==0&&g.KillOn==0,++n)&&ok;}
 {GameStub g;FanMarker o(g);g.Alive=false;o.Main();ok=Check(Balanced(g)&&g.Frame==1&&g.KillOff==0&&g.Starts==0&&g.Lines.empty(),++n)&&ok;}
 {GameStub g;FanMarker o(g);g.LeaveAt=4;g.QuestAt=-1;o.Main();ok=Check(Balanced(g)&&o.ParentClass.FansCanLeaveNow&&g.Lines.size()==1&&!o.ParentClass.ArenaOver&&g.KillOn==0,++n)&&ok;}
 ok=ok&&Strings==0&&Things==0&&Handles==0&&Movies==0&&Timers==0&&TimerList.empty();std::printf(ok?"00cf4ec0_TEST PASS\n":"00cf4ec0_TEST FAIL\n");return ok?0:1;
}
