// Complete Main body, deterministic conversations, movement, combat and resources.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <cassert>
static int Strings,Resources,Movies,Pauses,Timers;
struct CCharString{std::string text;CCharString(){++Strings;}CCharString(const char*s):text(s){++Strings;}CCharString(const CCharString&s):text(s.text){++Strings;}~CCharString(){--Strings;}};
struct C3DVector{int Id;C3DVector(int i=0):Id(i){}};struct GameStub;static GameStub*Current;
enum EHeroAbility{HERO_ABILITY_HEAL_LIFE_SPELL=14};enum EScriptEntityMoveType{ENTITY_MOVE_WALK=0};enum {SCRIPT_AI_PRIORITY_HIGHEST=4,GROUP_SELECT_NONE=0};
struct CScriptThing{int Id;CScriptThing(int i=0):Id(i){}C3DVector GetPos()const{assert(Id==4||Id==5);return C3DVector(Id);}bool MsgIsTalkedToBy(const char*)const;bool MsgIsHitBy(const char*)const;bool MsgIsHitByAnySpecialAbilityFrom(const char*)const;bool MsgIsHitBySpecialAbilityFrom(EHeroAbility,const char*)const;};struct CIsThingAlive{CScriptThing Thing;CIsThingAlive(const CScriptThing&t):Thing(t){}};struct TRetVoid{void Return(){}};struct Guard;
struct GameStub{Guard*Owner;int Frame,TerminateAt,DeadAt,RescuedAt,KilledAt,PocketAt,Health,StartFailures,Starts,Releases,TaskUntil,TaskLength,StopSpeech,SpeechCount,ClearCommands,InfoClear,AttackCount,Facings,Conversations,AddPeople,Lines,Position,MoveTarget,FirstMoves,SecondMoves,NextTimer;bool Predicate,Stop;float HeroDistance,HostageDistance;std::vector<int>TalkAt,HitAt,HitKind;std::vector<std::string>Speech,ConvoLines;std::vector<int>ConvoRecipients;std::map<int,int>Clock;
 GameStub():Owner(0),Frame(0),TerminateAt(75),DeadAt(-1),RescuedAt(35),KilledAt(-1),PocketAt(-1),Health(10),StartFailures(0),Starts(0),Releases(0),TaskUntil(0),TaskLength(1),StopSpeech(-1),SpeechCount(0),ClearCommands(0),InfoClear(0),AttackCount(0),Facings(0),Conversations(0),AddPeople(0),Lines(0),Position(4),MoveTarget(0),FirstMoves(0),SecondMoves(0),NextTimer(0),Predicate(false),Stop(false),HeroDistance(5),HostageDistance(5){Current=this;}
 void NewScriptFrame();CScriptThing GetHero(){return CScriptThing(2);}bool StartScriptingEntity(const CScriptThing&,struct CScriptGameResourceObjectScriptedThingBase&,int);bool TakeTalk(){if(!TalkAt.empty()&&TalkAt.front()<=Frame){TalkAt.erase(TalkAt.begin());return true;}return false;}int PendingHit(){return !HitAt.empty()&&HitAt.front()<=Frame?HitKind.front():0;}void PopHit(){HitAt.erase(HitAt.begin());HitKind.erase(HitKind.begin());}void Hit(int at,int kind){HitAt.push_back(at);HitKind.push_back(kind);}bool MsgOnHeroPickedPocket(const CScriptThing&t){assert(t.Id==1);if(PocketAt>=0&&Frame>=PocketAt){PocketAt=-1;return true;}return false;}
 CScriptThing GetNearestWithScriptName(const CScriptThing&t,const char*s){assert(t.Id==1&&std::strcmp(s,"CampHostage")==0);return CScriptThing(3);}CScriptThing GetThingWithScriptName(const char*s){assert(std::strcmp(s,"GuardFirstMarker")==0||std::strcmp(s,"GuardSecondMarker")==0);return CScriptThing(std::strcmp(s,"GuardFirstMarker")==0?4:5);}void EntitySetFacingAngleTowardsThing(const CScriptThing&a,const CScriptThing&b,bool f){assert(a.Id==1&&b.Id==3&&!f);++Facings;}
 long AddNewConversation(const CScriptThing&t,bool a,bool b){assert(t.Id==1&&!a&&!b);return ++Conversations;}void AddPersonToConversation(long c,const CScriptThing&t){assert(c==Conversations&&(t.Id==2||t.Id==3));ConvoRecipients.push_back(t.Id);++AddPeople;}void AddLineToConversation(long c,const CCharString&t,bool f,const CScriptThing&a,const CScriptThing&b){assert(c==Conversations&&!f&&a.Id==1&&b.Id==ConvoRecipients.back());ConvoLines.push_back(t.text);++Lines;}void StartMovieSequence(const CCharString&,struct CScriptGameResourceObjectMovieBase&);float GetHealth(const CScriptThing&t){assert(t.Id==1);return(float)Health;}void ClearThingHasInformation(const CScriptThing&t){assert(t.Id==1);++InfoClear;}void GiveThingBestEnemyTarget(const CScriptThing&a,const CScriptThing&b){assert(a.Id==1&&b.Id==2);++AttackCount;}
 int RegisterTimer(){++Timers;Clock[++NextTimer]=0;return NextTimer;}void DeregisterTimer(int n){assert(Clock.erase(n)==1);--Timers;}int GetTimer(int n){assert(Clock.count(n));return Clock[n];}void SetTimer(int n,int v){assert(Clock.count(n));Clock[n]=v;}
};
struct CTimer{int Id;CTimer():Id(Current->RegisterTimer()){}~CTimer(){Current->DeregisterTimer(Id);}long GetTime()const{return Current->GetTimer(Id);}CTimer&operator=(long n){Current->SetTimer(Id,n);return *this;}};struct CScriptGameResourceObjectMovieBase{bool Active;CScriptGameResourceObjectMovieBase():Active(false){}~CScriptGameResourceObjectMovieBase(){if(Active)--Movies;}};void GameStub::StartMovieSequence(const CCharString&t,CScriptGameResourceObjectMovieBase&m){assert(t.text.empty()&&!m.Active);m.Active=true;++Movies;}struct CWideScreenMagicPauseEntities{CWideScreenMagicPauseEntities(GameStub&){++Pauses;}~CWideScreenMagicPauseEntities(){--Pauses;}};
struct CScriptGameResourceObjectScriptedThingBase{bool Active;CScriptGameResourceObjectScriptedThingBase():Active(false){++Resources;}~CScriptGameResourceObjectScriptedThingBase(){--Resources;}bool IsValid()const{return Active;}void Release(){assert(Active);Active=false;++Current->Releases;}bool IsPerformingScriptTask(){return Current->Frame<Current->TaskUntil;}CScriptThing GetScriptThing(){return CScriptThing(1);}void MoveToPosition(const C3DVector&p,float f,EScriptEntityMoveType type,bool a,bool b){assert((p.Id==4||p.Id==5)&&f==1&&type==0&&!a&&b);p.Id==4?++Current->FirstMoves:++Current->SecondMoves;Current->MoveTarget=p.Id;Current->TaskUntil=Current->Frame+Current->TaskLength;}void ClearCommands(){++Current->ClearCommands;Current->TaskUntil=Current->Frame;Current->MoveTarget=0;}void Speak(const CScriptThing&h,const CCharString&t,int selection,bool a,bool b,bool c){assert(h.Id==2&&selection==GROUP_SELECT_NONE&&!a&&b&&!c);++Current->SpeechCount;Current->Speech.push_back(t.text);Current->TaskUntil=Current->Frame+Current->TaskLength;if(Current->SpeechCount==Current->StopSpeech)Current->Stop=true;}};
bool GameStub::StartScriptingEntity(const CScriptThing&t,CScriptGameResourceObjectScriptedThingBase&r,int p){assert(t.Id==1&&p==4);++Starts;if(Starts<=StartFailures)return false;r.Active=true;return true;}
struct Parent{bool HostagesRescued,HostageKilled;Parent():HostagesRescued(false),HostageKilled(false){}};struct Guard{GameStub&Game;CScriptThing Me;Parent ParentClass;bool DropPass;Guard(GameStub&g):Game(g),Me(1),DropPass(false){g.Owner=this;}void SetPredicate(const CIsThingAlive&p){assert(p.Thing.Id==1);Game.Predicate=true;}bool IsActiveThreadTerminating(){return Game.Stop||Game.Frame>=Game.TerminateAt||(Game.DeadAt>=0&&Game.Frame>=Game.DeadAt);}void Main();};
void GameStub::NewScriptFrame(){assert(++Frame<400);for(std::map<int,int>::iterator i=Clock.begin();i!=Clock.end();++i)if(i->second>0)--i->second;if(MoveTarget&&Frame>=TaskUntil){Position=MoveTarget;MoveTarget=0;}if(RescuedAt>=0&&Frame>=RescuedAt)Owner->ParentClass.HostagesRescued=true;if(KilledAt>=0&&Frame>=KilledAt)Owner->ParentClass.HostageKilled=true;}
bool CScriptThing::MsgIsTalkedToBy(const char*s)const{assert(Id==1&&std::strcmp(s,"SCRIPT_NAME_HERO")==0);return Current->TakeTalk();}bool CScriptThing::MsgIsHitBy(const char*s)const{assert(Id==1&&std::strcmp(s,"SCRIPT_NAME_HERO")==0);if(Current->PendingHit()==1){Current->PopHit();return true;}return false;}bool CScriptThing::MsgIsHitByAnySpecialAbilityFrom(const char*s)const{assert(Id==1&&std::strcmp(s,"SCRIPT_NAME_HERO")==0);return Current->PendingHit()>=2;}bool CScriptThing::MsgIsHitBySpecialAbilityFrom(EHeroAbility a,const char*s)const{assert(Id==1&&a==HERO_ABILITY_HEAL_LIFE_SPELL&&std::strcmp(s,"SCRIPT_NAME_HERO")==0);bool b=Current->PendingHit()==3;Current->PopHit();return b;}
static bool IsDistanceBetweenThingsUnder(const CScriptThing&a,const CScriptThing&b,float d){assert(a.Id==1);float n=b.Id==2?Current->HeroDistance:b.Id==3?Current->HostageDistance:Current->Position==b.Id?0.f:8.f;return n<d;}static bool IsDistanceBetweenThingsOver(const CScriptThing&a,const CScriptThing&b,float d){assert(a.Id==1&&(b.Id==4||b.Id==5));return(Current->Position==b.Id?0.f:8.f)>d;}
void Guard::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid())
        seh_me.Release();
    while (!Game.StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    CTimer GoadingTimer;
    GoadingTimer = 0;
    long GoadLevel = 0;
    bool SpokenTo = false;
    CScriptThing Hostage = Game.GetNearestWithScriptName(Me, "CampHostage");
    Game.EntitySetFacingAngleTowardsThing(Me, Hostage, false);
    long MarkerAt = 1;
    bool Turning = false;
    CTimer TurnTimer;
    DropPass = true;
    while (!ParentClass.HostagesRescued) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        if (Game.MsgOnHeroPickedPocket(Me)) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            DropPass = false;
        }
        if (IsDistanceBetweenThingsUnder(Me, Game.GetHero(), 7.0f) && IsDistanceBetweenThingsUnder(Me, Hostage, 12.0f) && GoadingTimer.GetTime() <= 0) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            long Conversation = Game.AddNewConversation(Me, false, false);
            Game.AddPersonToConversation(Conversation, Hostage);
            switch (GoadLevel) {
            case 0:
                Game.AddLineToConversation(Conversation, "TEXT_QST_009_HOSTAGE_GUARD_FIRST", false, Me, Hostage);
                GoadLevel++;
                break;
            case 1:
                Game.AddLineToConversation(Conversation, "TEXT_QST_009_HOSTAGE_GUARD_SECOND", false, Me, Hostage);
                GoadLevel++;
                break;
            case 2:
                Game.AddLineToConversation(Conversation, "TEXT_QST_009_HOSTAGE_GUARD_THIRD", false, Me, Hostage);
                GoadLevel++;
                break;
            case 3:
                Game.AddLineToConversation(Conversation, "TEXT_QST_009_HOSTAGE_GUARD_FOURTH", false, Me, Hostage);
                GoadLevel++;
                break;
            case 4:
                Game.AddLineToConversation(Conversation, "TEXT_QST_009_HOSTAGE_GUARD_FIFTH", false, Me, Hostage);
                GoadLevel++;
                break;
            case 5:
                Game.AddLineToConversation(Conversation, "TEXT_QST_009_HOSTAGE_GUARD_SIXTH", false, Me, Hostage);
                GoadLevel = 6;
                break;
            case 6:
                Game.AddLineToConversation(Conversation, "TEXT_QST_009_HOSTAGE_GUARD_SEVENTH", false, Me, Hostage);
                GoadLevel = 5;
                break;
            }
            GoadingTimer = 10;
        }
        if (MarkerAt == 1) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            if (IsDistanceBetweenThingsOver(Me, Game.GetThingWithScriptName("GuardFirstMarker"), 2.0f) && !seh_me.IsPerformingScriptTask()) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                seh_me.MoveToPosition(Game.GetThingWithScriptName("GuardFirstMarker").GetPos(), 1.0f, (EScriptEntityMoveType)0, false, true);
            } else {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                if (TurnTimer.GetTime() <= 0) {
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    if (!Turning) {
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                        Turning = true;
                        TurnTimer = 10;
                    } else {
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                        Turning = false;
                        MarkerAt = 2;
                    }
                }
            }
        } else {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            if (IsDistanceBetweenThingsOver(Me, Game.GetThingWithScriptName("GuardSecondMarker"), 2.0f) && !seh_me.IsPerformingScriptTask()) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                seh_me.MoveToPosition(Game.GetThingWithScriptName("GuardSecondMarker").GetPos(), 1.0f, (EScriptEntityMoveType)0, false, true);
            } else {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                if (TurnTimer.GetTime() <= 0) {
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    if (!Turning) {
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                        Turning = true;
                        TurnTimer = 10;
                    } else {
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                        Turning = false;
                        MarkerAt = 1;
                    }
                }
            }
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            seh_me.ClearCommands();
            if (!SpokenTo) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game.GetHero(), "TEXT_QST_009_HOSTAGE_GUARD_FIRST_CHAT", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
                Game.EntitySetFacingAngleTowardsThing(Me, Hostage, false);
                SpokenTo = true;
            } else {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game.GetHero(), "TEXT_QST_009_HOSTAGE_GUARD_SECOND_CHAT", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
                Game.EntitySetFacingAngleTowardsThing(Me, Hostage, false);
            }
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom((EHeroAbility)HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO")) || ParentClass.HostageKilled) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            Game.ClearThingHasInformation(Me);
            Game.GiveThingBestEnemyTarget(Me, Game.GetHero());
            if (seh_me.IsValid())
                seh_me.Release();
            while (true) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    SpokenTo = false;
    Game.ClearThingHasInformation(Me);
    for (; !false; Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        if (IsDistanceBetweenThingsOver(Me, Game.GetThingWithScriptName("GuardFirstMarker"), 2.0f) && !seh_me.IsPerformingScriptTask()) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            seh_me.MoveToPosition(Game.GetThingWithScriptName("GuardFirstMarker").GetPos(), 1.0f, (EScriptEntityMoveType)0, false, true);
        }
        if (IsDistanceBetweenThingsUnder(Me, Game.GetHero(), 7.0f) && IsDistanceBetweenThingsUnder(Me, Game.GetThingWithScriptName("GuardFirstMarker"), 12.0f) && GoadingTimer.GetTime() <= 0) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            long Conversation = Game.AddNewConversation(Me, false, false);
            Game.AddPersonToConversation(Conversation, Game.GetHero());
            switch (GoadLevel) {
            case 0:
                Game.AddLineToConversation(Conversation, "TEXT_QST_009_HOSTAGE_GUARD_LATE_CONVO_FIRST", false, Me, Game.GetHero());
                GoadLevel++;
                break;
            case 1:
                Game.AddLineToConversation(Conversation, "TEXT_QST_009_HOSTAGE_GUARD_LATE_CONVO_SECOND", false, Me, Game.GetHero());
                GoadLevel++;
                break;
            case 2:
                Game.AddLineToConversation(Conversation, "TEXT_QST_009_HOSTAGE_GUARD_LATE_CONVO_THIRD", false, Me, Game.GetHero());
                GoadLevel++;
                break;
            case 3:
                Game.AddLineToConversation(Conversation, "TEXT_QST_009_HOSTAGE_GUARD_LATE_CONVO_FOURTH", false, Me, Game.GetHero());
                GoadLevel++;
                break;
            case 4:
                Game.AddLineToConversation(Conversation, "TEXT_QST_009_HOSTAGE_GUARD_LATE_CONVO_FIFTH", false, Me, Game.GetHero());
                GoadLevel++;
                break;
            case 5:
                Game.AddLineToConversation(Conversation, "TEXT_QST_009_HOSTAGE_GUARD_LATE_CONVO_SIXTH", false, Me, Game.GetHero());
                Game.GiveThingBestEnemyTarget(Me, Game.GetHero());
                if (seh_me.IsValid())
                    seh_me.Release();
                while (true) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            GoadingTimer = 10;
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            seh_me.ClearCommands();
            if (!SpokenTo) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game.GetHero(), "TEXT_QST_009_HOSTAGE_GUARD_LATE_SPEAK_FIRST", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
                SpokenTo = true;
            } else {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game.GetHero(), "TEXT_QST_009_HOSTAGE_GUARD_LATE_SPEAK_SECOND", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
            }
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom((EHeroAbility)HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            seh_me.ClearCommands();
            {
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game.GetHero(), "TEXT_QST_009_HOSTAGE_GUARD_LATE_HIT", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
                Game.GiveThingBestEnemyTarget(Me, Game.GetHero());
                if (seh_me.IsValid())
                    seh_me.Release();
            }
            while (true) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}

static bool Has(const std::vector<std::string>&v,const char*s){for(unsigned i=0;i<v.size();++i)if(v[i]==s)return true;return false;}static int Count(const std::vector<std::string>&v,const char*s){int n=0;for(unsigned i=0;i<v.size();++i)if(v[i]==s)++n;return n;}static bool Balanced(){return Strings==0&&Resources==0&&Movies==0&&Pauses==0&&Timers==0;}static bool Check(bool b,int n){if(!b)std::printf("case %d failed\n",n);return b;}
int main(){bool ok=true;int n=0;
 {GameStub g;Guard o(g);o.Main();ok=Check(Balanced()&&g.Predicate&&o.DropPass&&g.AttackCount==1&&g.Releases==1&&g.FirstMoves>0&&g.SecondMoves>0&&g.Facings==1&&Has(g.ConvoLines,"TEXT_QST_009_HOSTAGE_GUARD_FIRST")&&Has(g.ConvoLines,"TEXT_QST_009_HOSTAGE_GUARD_LATE_CONVO_SIXTH")&&g.InfoClear==1,++n)&&ok;}
 {GameStub g;Guard o(g);g.RescuedAt=200;g.TerminateAt=96;o.Main();ok=Check(Balanced()&&g.Conversations==10&&Count(g.ConvoLines,"TEXT_QST_009_HOSTAGE_GUARD_SIXTH")==3&&Count(g.ConvoLines,"TEXT_QST_009_HOSTAGE_GUARD_SEVENTH")==2&&g.AttackCount==0&&g.InfoClear==0&&g.SecondMoves>=3,++n)&&ok;}
 {GameStub g;Guard o(g);o.ParentClass.HostagesRescued=true;o.Main();ok=Check(Balanced()&&g.Conversations==6&&g.ConvoRecipients[0]==2&&g.AttackCount==1&&Has(g.ConvoLines,"TEXT_QST_009_HOSTAGE_GUARD_LATE_CONVO_FIRST")&&g.SecondMoves==0,++n)&&ok;}
 {GameStub g;Guard o(g);g.RescuedAt=65;g.TerminateAt=90;o.Main();ok=Check(Balanced()&&g.AttackCount==1&&g.ConvoLines.back()=="TEXT_QST_009_HOSTAGE_GUARD_LATE_CONVO_SIXTH"&&g.ConvoRecipients.front()==3&&g.ConvoRecipients.back()==2,++n)&&ok;}
 {GameStub g;Guard o(g);g.RescuedAt=60;g.TerminateAt=90;o.Main();ok=Check(Balanced()&&g.AttackCount==0&&g.ConvoLines.back()=="TEXT_QST_009_HOSTAGE_GUARD_SIXTH"&&g.Conversations>6,++n)&&ok;}
 {GameStub g;Guard o(g);g.PocketAt=3;o.Main();ok=Check(Balanced()&&!o.DropPass&&g.AttackCount==1,++n)&&ok;}
 {GameStub g;Guard o(g);g.PocketAt=45;o.Main();ok=Check(Balanced()&&o.DropPass,++n)&&ok;}
 {GameStub g;Guard o(g);g.TalkAt.push_back(3);g.TalkAt.push_back(6);g.TalkAt.push_back(38);g.TalkAt.push_back(41);o.Main();ok=Check(Balanced()&&g.Speech.size()==4&&g.Speech[0]=="TEXT_QST_009_HOSTAGE_GUARD_FIRST_CHAT"&&g.Speech[1]=="TEXT_QST_009_HOSTAGE_GUARD_SECOND_CHAT"&&g.Speech[2]=="TEXT_QST_009_HOSTAGE_GUARD_LATE_SPEAK_FIRST"&&g.Speech[3]=="TEXT_QST_009_HOSTAGE_GUARD_LATE_SPEAK_SECOND"&&g.Facings==3,++n)&&ok;}
 for(int kind=1;kind<=3;++kind){GameStub g;Guard o(g);g.RescuedAt=200;g.Hit(4,kind);o.Main();ok=Check(Balanced()&&g.AttackCount==(kind==3?0:1)&&g.Releases==(kind==3?0:1)&&g.Speech.empty()&&g.InfoClear==(kind==3?0:1),++n)&&ok;}
 for(int kind=1;kind<=3;++kind){GameStub g;Guard o(g);o.ParentClass.HostagesRescued=true;g.HeroDistance=20;g.Hit(4,kind);o.Main();ok=Check(Balanced()&&g.AttackCount==(kind==3?0:1)&&g.Releases==(kind==3?0:1)&&Has(g.Speech,"TEXT_QST_009_HOSTAGE_GUARD_LATE_HIT")== (kind!=3)&&g.InfoClear==1,++n)&&ok;}
 {GameStub g;Guard o(g);g.RescuedAt=200;g.KilledAt=5;g.HeroDistance=20;o.Main();ok=Check(Balanced()&&g.AttackCount==1&&g.Releases==1&&g.InfoClear==1&&g.Speech.empty(),++n)&&ok;}
 {GameStub g;Guard o(g);o.ParentClass.HostagesRescued=true;o.ParentClass.HostageKilled=true;g.HeroDistance=20;o.Main();ok=Check(Balanced()&&g.AttackCount==0&&g.Speech.empty()&&g.InfoClear==1,++n)&&ok;}
 {GameStub g;Guard o(g);g.HeroDistance=8;o.Main();ok=Check(Balanced()&&g.Conversations==0&&g.AttackCount==0&&g.SecondMoves>0,++n)&&ok;}
 {GameStub g;Guard o(g);g.RescuedAt=200;g.HostageDistance=13;o.Main();ok=Check(Balanced()&&g.Conversations==0&&g.AttackCount==0&&g.SecondMoves>0,++n)&&ok;}
 {GameStub g;Guard o(g);g.Position=5;g.TerminateAt=8;g.RescuedAt=200;o.Main();ok=Check(Balanced()&&g.FirstMoves==1&&g.SecondMoves==0&&g.Position==4,++n)&&ok;}
 {GameStub g;Guard o(g);g.StartFailures=100;g.TerminateAt=4;o.Main();ok=Check(Balanced()&&g.Starts==3&&g.Facings==0&&!o.DropPass,++n)&&ok;}
 {GameStub g;Guard o(g);g.DeadAt=1;o.Main();ok=Check(Balanced()&&g.Predicate&&g.Starts==0&&!o.DropPass,++n)&&ok;}
 for(int phase=0;phase<2;++phase){for(int speech=1;speech<=2;++speech){GameStub g;Guard o(g);if(phase)o.ParentClass.HostagesRescued=true;g.TalkAt.push_back(3);g.TalkAt.push_back(6);g.StopSpeech=speech;g.HeroDistance=20;o.Main();ok=Check(Balanced()&&g.SpeechCount==speech&&g.AttackCount==0&&g.InfoClear==phase,++n)&&ok;}}
 {GameStub g;Guard o(g);o.ParentClass.HostagesRescued=true;g.Hit(3,1);g.StopSpeech=1;o.Main();ok=Check(Balanced()&&g.SpeechCount==1&&g.AttackCount==0&&g.Releases==0,++n)&&ok;}
 {GameStub g;Guard o(g);g.Health=0;g.TalkAt.push_back(3);g.TalkAt.push_back(6);g.Hit(38,1);o.Main();ok=Check(Balanced()&&g.Speech.empty()&&g.AttackCount==1&&g.Releases==1,++n)&&ok;}
 std::printf(ok?"00d09010_TEST PASS\n":"00d09010_TEST FAIL\n");return ok?0:1;}
