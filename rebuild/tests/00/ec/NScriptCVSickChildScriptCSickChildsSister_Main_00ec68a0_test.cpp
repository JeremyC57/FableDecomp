// Faithful complete Main body; deterministic engine, task and resource callees.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <cassert>
static int Strings,Resources,Movies,Pauses,Timers;
class CCharString{ public:std::string text;CCharString(){++Strings;}CCharString(const char*s):text(s){++Strings;}CCharString(const std::string&s):text(s){++Strings;}CCharString(const CCharString&s):text(s.text){++Strings;}~CCharString(){--Strings;}CCharString&operator=(const CCharString&s){text=s.text;return *this;}};
static CCharString operator+(const char*a,const CCharString&b){return CCharString(std::string(a)+b.text);}static CCharString GFIntToCharString(long n){char s[40];std::sprintf(s,"%ld",n);return CCharString(s);}
class C3DVector{ public:float X;C3DVector(float x=0):X(x){}};struct GameStub;static GameStub*Current;
enum {SCRIPT_AI_PRIORITY_HIGHEST=4,GROUP_SELECT_NONE=0,GROUP_SELECT_RANDOM_NO_REPEAT=2,HERO_ABILITY_HEAL_LIFE_SPELL=14,ENTITY_MOVE_WALK=0,ENTITY_MOVE_RUN=1};
namespace NGlobalConsole{bool GDefaultToUsingPhysicsInCutsceneAnimations=true;}
class CScriptThing{ public:int id;CScriptThing(int n=0):id(n){}C3DVector GetPos()const{assert(id==3);return C3DVector(3);}bool MsgIsTalkedToBy(const char*)const;bool MsgIsHitBy(const char*)const;bool MsgIsHitByAnySpecialAbilityFrom(const char*)const;bool MsgIsHitBySpecialAbilityFrom(int,const char*)const;};
struct CIsThingAlive{CScriptThing Thing;CIsThingAlive(const CScriptThing&t):Thing(t){}};struct TRetVoid{void Return(){}};
struct Sister;struct GameStub{
 Sister*Owner;int Frame,TerminateAt,DeadAt,StartFailures,Starts,TaskUntil,TaskLength,StopSpeech,StopAnim,SpeechCount,AnimCount,Follow,StopFollow,ClearCommands,ClearActions,RunMoves,WalkMoves,Facing,PauseCount,CameraTrue,CameraFalse,CameraPoint,InformationFalse,InformationClear,Conversations,AddPeople,Lines,NextTimer,MotherNearAt,MotherIntroAt,FinishedAt,Health,MissingSuffix;bool Stop,Predicate;float HeroMother;
 std::vector<int>TalkAt,HitAt,HitKind;std::vector<std::pair<int,float> >HeroDistance;std::vector<std::string>Speech,Animations,ConvoLines;std::vector<int>Selections;std::map<int,int>Clock;
 GameStub():Owner(0),Frame(0),TerminateAt(35),DeadAt(-1),StartFailures(0),Starts(0),TaskUntil(0),TaskLength(1),StopSpeech(-1),StopAnim(-1),SpeechCount(0),AnimCount(0),Follow(0),StopFollow(0),ClearCommands(0),ClearActions(0),RunMoves(0),WalkMoves(0),Facing(0),PauseCount(0),CameraTrue(0),CameraFalse(0),CameraPoint(0),InformationFalse(0),InformationClear(0),Conversations(0),AddPeople(0),Lines(0),NextTimer(0),MotherNearAt(8),MotherIntroAt(-1),FinishedAt(-1),Health(10),MissingSuffix(30),Stop(false),Predicate(false),HeroMother(20){Current=this;HeroDistance.push_back(std::make_pair(0,8.f));}
 void NewScriptFrame();CScriptThing GetHero(){return CScriptThing(2);}bool StartScriptingEntity(const CScriptThing&,class CScriptGameResourceObjectScriptedThingBase&,int);bool TakeTalk(){if(!TalkAt.empty()&&TalkAt.front()<=Frame){TalkAt.erase(TalkAt.begin());return true;}return false;}int PendingHit(){return !HitAt.empty()&&HitAt.front()<=Frame?HitKind.front():0;}void PopHit(){HitAt.erase(HitAt.begin());HitKind.erase(HitKind.begin());}void Hit(int frame,int kind){HitAt.push_back(frame);HitKind.push_back(kind);}float HeroDist()const{float d=HeroDistance[0].second;for(unsigned i=0;i<HeroDistance.size();++i)if(HeroDistance[i].first<=Frame)d=HeroDistance[i].second;return d;}float MotherDist()const{return Frame>=MotherNearAt?1.f:8.f;}
 long AddNewConversation(const CScriptThing&m,bool a,bool b){assert(m.id==1&&!a&&!b);return ++Conversations;}void AddPersonToConversation(long c,const CScriptThing&h){assert(c==Conversations&&h.id==2);++AddPeople;}void AddLineToConversation(long c,const CCharString&t,bool a,const CScriptThing&m,const CScriptThing&h){assert(c==Conversations&&!a&&m.id==1&&h.id==2);++Lines;ConvoLines.push_back(t.text);}void StartMovieSequence(const CCharString&,class CScriptGameResourceObjectMovieBase&);float GetHealth(const CScriptThing&t){assert(t.id==1);return (float)Health;}void FixMovieSequenceCamera(bool b){b?++CameraTrue:++CameraFalse;}void CameraUseCameraPoint(const char*s,const CScriptThing&t,float f,int a,int b){assert(std::strcmp(s,"CAM_SC_GIRL")==0&&t.id==1&&f==-1&&a==0&&b==-1);++CameraPoint;}CScriptThing GetThingWithScriptName(const char*s){assert(std::strcmp(s,"SickChildsMother")==0);return CScriptThing(3);}void SetThingHasInformation(const CScriptThing&t,bool a,bool b,bool c){assert(t.id==1&&!a&&!b&&!c);++InformationFalse;}void ClearThingHasInformation(const CScriptThing&t){assert(t.id==1);++InformationClear;}void EntitySetFacingAngleTowardsThing(const CScriptThing&m,const CScriptThing&h,bool b){assert(m.id==1&&h.id==2&&!b);++Facing;}void Pause(float f){assert(f==0.8f);++PauseCount;}bool TextEntryExists(const CCharString&t){char b[80];std::sprintf(b,"TEXT_QST_B10_SISTER_REMINDER_POSTINTRO_%d",MissingSuffix);return t.text!=b;}
 int RegisterTimer(){++Timers;Clock[++NextTimer]=0;return NextTimer;}void DeregisterTimer(int i){assert(Clock.erase(i)==1);--Timers;}int GetTimer(int i){assert(Clock.count(i));return Clock[i];}void SetTimer(int i,int t){assert(Clock.count(i));Clock[i]=t;}
};
class CTimer{ public:int Id;CTimer():Id(Current->RegisterTimer()){}~CTimer(){Current->DeregisterTimer(Id);}bool operator==(long n)const{return Current->GetTimer(Id)==n;}CTimer&operator=(long n){Current->SetTimer(Id,(int)n);return *this;}};
class CScriptGameResourceObjectMovieBase{ public:bool Active;CScriptGameResourceObjectMovieBase():Active(false){}~CScriptGameResourceObjectMovieBase(){if(Active)--Movies;}};
void GameStub::StartMovieSequence(const CCharString&n,CScriptGameResourceObjectMovieBase&m){assert(n.text.empty()&&!m.Active);m.Active=true;++Movies;}
struct CWideScreenMagicPauseEntities{CWideScreenMagicPauseEntities(GameStub&){++Pauses;}~CWideScreenMagicPauseEntities(){--Pauses;}};
class CScriptGameResourceObjectScriptedThingBase{ public:bool Active;CScriptGameResourceObjectScriptedThingBase():Active(false){++Resources;}~CScriptGameResourceObjectScriptedThingBase(){--Resources;}bool IsValid()const{return Active;}void Release(){assert(Active);Active=false;}void FollowThing(const CScriptThing&t,float f,bool b){assert(t.id==2&&f==1&&b);++Current->Follow;}void StopFollowingThing(const CScriptThing&t){assert(t.id==2);++Current->StopFollow;}void PlayAnimation(const CCharString&a,bool b,bool c,bool d,bool e,bool f,bool g,bool h){assert(!b&&!c&&!d&&e&&f&&!g&&!h);++Current->AnimCount;Current->Animations.push_back(a.text);Current->TaskUntil=Current->Frame+Current->TaskLength;if(Current->AnimCount==Current->StopAnim)Current->Stop=true;}bool IsPerformingScriptTask(){return Current->Frame<Current->TaskUntil;}CScriptThing GetScriptThing(){return CScriptThing(1);}void Speak(const CScriptThing&h,const CCharString&t,int selection,bool a,bool b,bool c){assert(h.id==2&&!a&&b&&!c);++Current->SpeechCount;Current->Speech.push_back(t.text);Current->Selections.push_back(selection);Current->TaskUntil=Current->Frame+Current->TaskLength;if(Current->SpeechCount==Current->StopSpeech)Current->Stop=true;}void ClearCommands(){++Current->ClearCommands;}void ClearAllActions(){++Current->ClearActions;}void MoveToPosition(const C3DVector&p,float f,int type,bool a,bool b){assert(p.X==3&&f==1&&!a&&b);type==ENTITY_MOVE_RUN?++Current->RunMoves:++Current->WalkMoves;}};
bool GameStub::StartScriptingEntity(const CScriptThing&t,CScriptGameResourceObjectScriptedThingBase&r,int p){assert(t.id==1&&p==4);++Starts;if(Starts<=StartFailures)return false;r.Active=true;return true;}
struct ScriptDef{std::vector<CCharString>SickChildsSisterAttentionAnims;ScriptDef(){SickChildsSisterAttentionAnims.push_back("ATTENTION_A");SickChildsSisterAttentionAnims.push_back("ATTENTION_B");}};static ScriptDef*pGlobalScriptDef;
struct Parent{bool MotherIntroDone,FinishedQuest;Parent():MotherIntroDone(false),FinishedQuest(false){}};
struct Sister{GameStub&Game;CScriptThing Me;Parent ParentClass;bool LeadHeroToSickChild;Sister(GameStub&g):Game(g),Me(1),LeadHeroToSickChild(false){g.Owner=this;}void SetPredicate(const CIsThingAlive&p){assert(p.Thing.id==1);Game.Predicate=true;}bool IsActiveThreadTerminating(){return Game.Stop||Game.Frame>=Game.TerminateAt||(Game.DeadAt>=0&&Game.Frame>=Game.DeadAt);}void Main();};
void GameStub::NewScriptFrame(){assert(++Frame<300);for(std::map<int,int>::iterator i=Clock.begin();i!=Clock.end();++i)if(i->second>0)--i->second;if(MotherIntroAt>=0&&Frame>=MotherIntroAt)Owner->ParentClass.MotherIntroDone=true;if(FinishedAt>=0&&Frame>=FinishedAt)Owner->ParentClass.FinishedQuest=true;}
bool CScriptThing::MsgIsTalkedToBy(const char*s)const{assert(id==1&&std::strcmp(s,"SCRIPT_NAME_HERO")==0);return Current->TakeTalk();}bool CScriptThing::MsgIsHitBy(const char*s)const{assert(id==1&&std::strcmp(s,"SCRIPT_NAME_HERO")==0);if(Current->PendingHit()==1){Current->PopHit();return true;}return false;}bool CScriptThing::MsgIsHitByAnySpecialAbilityFrom(const char*s)const{assert(id==1&&std::strcmp(s,"SCRIPT_NAME_HERO")==0);return Current->PendingHit()>=2;}bool CScriptThing::MsgIsHitBySpecialAbilityFrom(int a,const char*s)const{assert(id==1&&a==HERO_ABILITY_HEAL_LIFE_SPELL&&std::strcmp(s,"SCRIPT_NAME_HERO")==0);bool heal=Current->PendingHit()==3;Current->PopHit();return heal;}
static bool IsDistanceBetweenThingsUnder(const CScriptThing&a,const CScriptThing&b,float d){assert(a.id==1);if(b.id==3)return Current->MotherDist()<d;assert(b.id==2);return Current->HeroDist()<d;}static float GetDistanceBetweenThings(const CScriptThing&a,const CScriptThing&b){assert(b.id==3);return a.id==1?Current->MotherDist():Current->HeroMother;}
void Sister::Main()
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
    if (!LeadHeroToSickChild && !ParentClass.MotherIntroDone) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        seh_me.FollowThing(Game.GetHero(),1.0f,true);
        CTimer GetAttentionTimer;
        while (!Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO") && !ParentClass.MotherIntroDone) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (GetAttentionTimer==0) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                seh_me.StopFollowingThing(Game.GetHero());
                long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
                Game.AddPersonToConversation(HLStyleConversationIndex,Game.GetHero());
                Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_B10_SISTER_CALLS_OVER_10",false,Me,Game.GetHero());
                long RandAnim=rand()%pGlobalScriptDef->SickChildsSisterAttentionAnims.size();
                seh_me.PlayAnimation(pGlobalScriptDef->SickChildsSisterAttentionAnims[RandAnim],false,false,false,true,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                GetAttentionTimer=10;
                seh_me.FollowThing(Game.GetHero(),1.0f,true);
            }
            if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                seh_me.StopFollowingThing(Game.GetHero());
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_SISTER_ON_HIT",GROUP_SELECT_RANDOM_NO_REPEAT,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                seh_me.FollowThing(Game.GetHero(),1.0f,true);
            }
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        seh_me.StopFollowingThing(Game.GetHero());
        if (!ParentClass.MotherIntroDone) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            {
                Game.FixMovieSequenceCamera(true);
                Game.CameraUseCameraPoint("CAM_SC_GIRL",Me,-1.0f,0,-1);
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_SISTER_TOLD_TO_FOLLOW_20",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                Game.FixMovieSequenceCamera(false);
            }
            CScriptThing Mother=Game.GetThingWithScriptName("SickChildsMother");
            Game.SetThingHasInformation(Me,false,false,false);
            {
                enum ELeaderState { LS_RUNNING=0, LS_WALKING=1, LS_STOPPED=2 };
                bool bLeadHeroToPointLatch=false;
                bool MeIsNearerThanThingToPoint=false;
                ELeaderState current_state=LS_RUNNING;
                CTimer CallToHeroTimer;
                seh_me.ClearCommands();
                seh_me.MoveToPosition(Mother.GetPos(),1.0f,ENTITY_MOVE_RUN,false,true);
                while (!IsDistanceBetweenThingsUnder(Me,Mother,2.0f) ) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    MeIsNearerThanThingToPoint=GetDistanceBetweenThings(Me,Mother)<GetDistanceBetweenThings(Game.GetHero(),Mother);
                    if (IsDistanceBetweenThingsUnder(Me,Game.GetHero(),15.0f) || !MeIsNearerThanThingToPoint) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (IsDistanceBetweenThingsUnder(Me,Game.GetHero(),11.0f) || !MeIsNearerThanThingToPoint) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            if (current_state!=LS_RUNNING) {
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                seh_me.ClearCommands();
                                seh_me.MoveToPosition(Mother.GetPos(),1.0f,ENTITY_MOVE_RUN,false,true);
                                current_state=LS_RUNNING;
                            }
                        } else {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            if (current_state!=LS_WALKING) {
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                seh_me.ClearCommands();
                                seh_me.MoveToPosition(Mother.GetPos(),1.0f,ENTITY_MOVE_WALK,false,true);
                                current_state=LS_WALKING;
                            }
                        }
                        bLeadHeroToPointLatch=false;
                        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            seh_me.ClearCommands();
                            CScriptGameResourceObjectMovieBase speech_movie;
                            Game.StartMovieSequence("",speech_movie);
                            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                            if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                                seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_SISTER_THIS_WAY",GROUP_SELECT_NONE,false,true,false);
                                while (seh_me.IsPerformingScriptTask()) {
                                    Game.NewScriptFrame();
                                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                }
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            current_state=LS_STOPPED;
                        }
                        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            seh_me.ClearCommands();
                            CScriptGameResourceObjectMovieBase speech_movie;
                            Game.StartMovieSequence("",speech_movie);
                            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                            if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                                seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_SISTER_ON_HIT",GROUP_SELECT_RANDOM_NO_REPEAT,false,true,false);
                                while (seh_me.IsPerformingScriptTask()) {
                                    Game.NewScriptFrame();
                                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                }
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            current_state=LS_STOPPED;
                        }
                    } else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        seh_me.ClearCommands();
                        seh_me.ClearAllActions();
                        if (!bLeadHeroToPointLatch) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            Game.EntitySetFacingAngleTowardsThing(Me,Game.GetHero(),false);
                            long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
                            Game.AddPersonToConversation(HLStyleConversationIndex,Game.GetHero());
                            Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_B10_SISTER_THIS_WAY",false,Me,Game.GetHero());
                            while (seh_me.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            Game.Pause(0.8f);
                            seh_me.PlayAnimation("STANDARD_WAVE",false,false,false,true,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            bLeadHeroToPointLatch=true;
                            current_state=LS_STOPPED;
                            CallToHeroTimer=5;
                        }
                    }
                    if (bLeadHeroToPointLatch && CallToHeroTimer==0) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        bLeadHeroToPointLatch=false;
                    }
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            LeadHeroToSickChild=true;
        }
    }
    Game.ClearThingHasInformation(Me);
    long LineNumber=10;
    for (; !false; Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (ParentClass.FinishedQuest) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_SISTER_THANKS",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
            else if (!ParentClass.MotherIntroDone) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_SISTER_REMINDER",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
            else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CCharString FullText;
                FullText="TEXT_QST_B10_SISTER_REMINDER_POSTINTRO_"+GFIntToCharString(LineNumber);
                if (!Game.TextEntryExists(FullText)) {
                    LineNumber=10;
                    FullText="TEXT_QST_B10_SISTER_REMINDER_POSTINTRO_"+GFIntToCharString(LineNumber);
                }
                LineNumber+=10;
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),FullText,GROUP_SELECT_NONE,false,true,false);
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
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_SISTER_ON_HIT",GROUP_SELECT_NONE,false,true,false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
        }
    }
}

static bool Has(const std::vector<std::string>&v,const char*s){for(unsigned i=0;i<v.size();++i)if(v[i]==s)return true;return false;}static int Count(const std::vector<std::string>&v,const char*s){int n=0;for(unsigned i=0;i<v.size();++i)if(v[i]==s)++n;return n;}static bool Balanced(){return Resources==0&&Movies==0&&Pauses==0&&Timers==0&&Strings==2;}static bool Check(bool b,int n){if(!b)std::printf("case %d failed\n",n);return b;}
int main(){bool ok=true;int n=0;{ScriptDef def;pGlobalScriptDef=&def;
 {GameStub g;Sister o(g);g.TalkAt.push_back(2);o.Main();ok=Check(Balanced()&&g.Predicate&&o.LeadHeroToSickChild&&g.Follow>=1&&g.StopFollow>=1&&g.RunMoves==1&&g.WalkMoves==0&&g.InformationFalse==1&&g.InformationClear==1&&g.CameraTrue==1&&g.CameraFalse==1&&Has(g.Speech,"TEXT_QST_B10_SISTER_TOLD_TO_FOLLOW_20"),++n)&&ok;}
 {GameStub g;Sister o(g);g.MotherIntroAt=4;o.Main();ok=Check(Balanced()&&o.ParentClass.MotherIntroDone&&!o.LeadHeroToSickChild&&g.RunMoves==0&&g.InformationClear==1&&g.CameraTrue==0&&g.Follow>=1&&g.StopFollow>=1,++n)&&ok;}
 {GameStub g;Sister o(g);g.TalkAt.push_back(2);g.HeroDistance[0].second=12;g.HeroDistance.push_back(std::make_pair(6,8.f));g.MotherNearAt=10;o.Main();ok=Check(Balanced()&&o.LeadHeroToSickChild&&g.WalkMoves==1&&g.RunMoves==2,++n)&&ok;}
 {GameStub g;Sister o(g);g.TalkAt.push_back(2);g.HeroDistance[0].second=20;g.MotherNearAt=18;o.Main();ok=Check(Balanced()&&o.LeadHeroToSickChild&&g.ClearActions>2&&g.Facing>=2&&g.PauseCount==g.Facing&&Count(g.Animations,"STANDARD_WAVE")==g.Facing&&g.WalkMoves==0,++n)&&ok;}
 {GameStub g;Sister o(g);g.TalkAt.push_back(2);g.HeroDistance[0].second=20;g.HeroMother=1;o.Main();ok=Check(Balanced()&&o.LeadHeroToSickChild&&g.Facing==0&&g.RunMoves==1,++n)&&ok;}
 {GameStub g;Sister o(g);g.TalkAt.push_back(2);g.TalkAt.push_back(5);o.Main();ok=Check(Balanced()&&Has(g.Speech,"TEXT_QST_B10_SISTER_THIS_WAY")&&g.RunMoves==2,++n)&&ok;}
 for(int kind=1;kind<=3;++kind){GameStub g;Sister o(g);g.Hit(2,kind);g.MotherIntroAt=6;o.Main();ok=Check(Balanced()&&Has(g.Speech,"TEXT_QST_B10_SISTER_ON_HIT")== (kind!=3)&&g.Follow==(kind==3?2:3),++n)&&ok;}
 for(int kind=1;kind<=3;++kind){GameStub g;Sister o(g);g.TalkAt.push_back(2);g.Hit(5,kind);o.Main();ok=Check(Balanced()&&Has(g.Speech,"TEXT_QST_B10_SISTER_ON_HIT")== (kind!=3)&&o.LeadHeroToSickChild&&g.RunMoves==(kind==3?1:2),++n)&&ok;}
 {GameStub g;Sister o(g);o.LeadHeroToSickChild=true;g.TalkAt.push_back(2);g.TalkAt.push_back(5);o.Main();ok=Check(Balanced()&&Count(g.Speech,"TEXT_QST_B10_SISTER_REMINDER")==2&&g.Follow==0&&g.RunMoves==0&&g.InformationClear==1,++n)&&ok;}
 {GameStub g;Sister o(g);o.ParentClass.MotherIntroDone=true;g.TalkAt.push_back(2);g.TalkAt.push_back(5);g.TalkAt.push_back(8);o.Main();ok=Check(Balanced()&&Count(g.Speech,"TEXT_QST_B10_SISTER_REMINDER_POSTINTRO_10")==2&&Count(g.Speech,"TEXT_QST_B10_SISTER_REMINDER_POSTINTRO_20")==1&&g.Follow==0,++n)&&ok;}
 {GameStub g;Sister o(g);o.ParentClass.FinishedQuest=true;o.ParentClass.MotherIntroDone=true;g.TalkAt.push_back(2);o.Main();ok=Check(Balanced()&&Count(g.Speech,"TEXT_QST_B10_SISTER_THANKS")==1&&g.Selections[0]==GROUP_SELECT_NONE,++n)&&ok;}
 for(int kind=1;kind<=3;++kind){GameStub g;Sister o(g);o.LeadHeroToSickChild=true;g.Hit(3,kind);o.Main();ok=Check(Balanced()&&Count(g.Speech,"TEXT_QST_B10_SISTER_ON_HIT")== (kind==3?0:1)&&(kind==3||g.Selections[0]==GROUP_SELECT_NONE),++n)&&ok;}
 {GameStub g;Sister o(g);g.Health=0;g.TalkAt.push_back(2);g.Hit(5,1);o.Main();ok=Check(Balanced()&&g.Speech.empty()&&o.LeadHeroToSickChild&&g.CameraTrue==g.CameraFalse&&g.RunMoves>=1,++n)&&ok;}
 {GameStub g;Sister o(g);g.StartFailures=20;g.TerminateAt=3;o.Main();ok=Check(Balanced()&&g.Starts==2&&g.Follow==0&&g.InformationClear==0,++n)&&ok;}
 {GameStub g;Sister o(g);g.DeadAt=1;o.Main();ok=Check(Balanced()&&g.Predicate&&g.Starts==0&&g.InformationClear==0,++n)&&ok;}
 {GameStub g;Sister o(g);g.StopAnim=1;o.Main();ok=Check(Balanced()&&g.AnimCount==1&&g.InformationClear==0&&g.StopFollow==1,++n)&&ok;}
 {GameStub g;Sister o(g);g.TalkAt.push_back(2);g.StopSpeech=1;o.Main();ok=Check(Balanced()&&!o.LeadHeroToSickChild&&g.SpeechCount==1&&g.CameraTrue==1&&g.CameraFalse==0&&g.InformationClear==0,++n)&&ok;}
 {GameStub g;Sister o(g);g.TalkAt.push_back(2);g.HeroDistance[0].second=20;g.MotherNearAt=99;g.StopAnim=2;o.Main();ok=Check(Balanced()&&!o.LeadHeroToSickChild&&g.Facing==1&&g.InformationClear==0,++n)&&ok;}
 {GameStub g;Sister o(g);o.LeadHeroToSickChild=true;g.TalkAt.push_back(2);g.StopSpeech=1;o.Main();ok=Check(Balanced()&&g.InformationClear==1&&g.SpeechCount==1,++n)&&ok;}
 {GameStub g;Sister o(g);g.TalkAt.push_back(2);g.Hit(5,1);g.StopSpeech=2;o.Main();ok=Check(Balanced()&&!o.LeadHeroToSickChild&&g.SpeechCount==2&&g.InformationClear==0,++n)&&ok;}
 }ok=ok&&Strings==0;std::printf(ok?"00ec68a0_TEST PASS\n":"00ec68a0_TEST FAIL\n");return ok?0:1;}