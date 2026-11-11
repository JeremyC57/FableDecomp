// Complete Main with deterministic engine stubs and actual std::map ownership.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <cassert>
static int Strings,Resources,Movies,Pauses,Timers,FixedRand;
struct CCharString{std::string text;CCharString(){++Strings;}CCharString(const char*s):text(s){++Strings;}CCharString(const CCharString&s):text(s.text){++Strings;}~CCharString(){--Strings;}CCharString&operator=(const CCharString&s){text=s.text;return *this;}bool operator<(const CCharString&s)const{return text<s.text;}void Sprintf(const char*f,long n){char b[100];std::sprintf(b,f,n);text=b;}};
struct C3DVector{int Id;C3DVector(int i=0):Id(i){}};struct GameStub;static GameStub*Current;enum BriarRoseOpinion{BRIAR_ROSE_OPINION_LOUD=0,BRIAR_ROSE_OPINION_MEDIUM=1,BRIAR_ROSE_OPINION_QUIET=2};enum {SCRIPT_AI_PRIORITY_HIGHEST=4,GROUP_SELECT_NONE=0,GROUP_SELECT_RANDOM_NO_REPEAT=2,ENTITY_MOVE_WALK=0,OPINION_DEED_TYPE_SHOW_TROPHY_GOOD=56};namespace NGlobalConsole{bool GDefaultToUsingPhysicsInCutsceneAnimations=true;}
struct CScriptThing{int Id;CScriptThing(int n=0):Id(n){}bool IsAlive()const;bool MsgIsTalkedToBy(const char*)const;C3DVector GetPos()const{return C3DVector(Id);}};struct CIsThingAlive{CScriptThing Thing;CIsThingAlive(const CScriptThing&t):Thing(t){}};struct TRetVoid{void Return(){}};struct Briar;
struct GameStub{Briar*Owner;int Frame,TerminateAt,DeadAt,StartFailures,FailAfter,Starts,Releases,TaskUntil,TaskLength,StopSpeech,SpeechCount,StopCutscene,StopAnim,AnimCount,ClearCommands,Facings,InfoClear,MarkersRemoved,Conversations,AddedPeople,CameraTrue,CameraFalse,Deeds,NextTimer,OpinionAt,RequiredAtChange,AttackAt,HitCountAt,HitCountValue,Waypoints,Errors,MoveCount;bool Stop,Predicate,MarkerAlive,HeroAlive,VillagerAlive;float Distance;std::vector<int>TalkAt;std::vector<std::string>Speech,Animations,ConvoLines,Opinions,Cutscenes;std::vector<int>Selections;std::map<int,int>Clock;
 GameStub():Owner(0),Frame(0),TerminateAt(16),DeadAt(-1),StartFailures(0),FailAfter(-1),Starts(0),Releases(0),TaskUntil(0),TaskLength(1),StopSpeech(-1),SpeechCount(0),StopCutscene(-1),StopAnim(-1),AnimCount(0),ClearCommands(0),Facings(0),InfoClear(0),MarkersRemoved(0),Conversations(0),AddedPeople(0),CameraTrue(0),CameraFalse(0),Deeds(0),NextTimer(0),OpinionAt(-1),RequiredAtChange(0),AttackAt(-1),HitCountAt(-1),HitCountValue(0),Waypoints(0),Errors(0),MoveCount(0),Stop(false),Predicate(false),MarkerAlive(true),HeroAlive(true),VillagerAlive(true),Distance(20){Current=this;FixedRand=0;}
 void NewScriptFrame();CScriptThing GetHero(){return CScriptThing(2);}bool StartScriptingEntity(const CScriptThing&,struct CScriptGameResourceObjectScriptedThingBase&,int);bool TakeTalk(){if(!TalkAt.empty()&&TalkAt.front()<=Frame){TalkAt.erase(TalkAt.begin());return true;}return false;}CScriptThing GetThingWithScriptName(const CCharString&n){if(n.text=="M_TrophiesLookAtMe")return CScriptThing(3);++Waypoints;return CScriptThing(5);}CScriptThing GetRandomThingWithScriptName(const char*s){assert(std::strcmp(s,"TrophiesIntroWatcher")==0);return CScriptThing(4);}void EntitySetFacingAngleTowardsThing(const CScriptThing&a,const CScriptThing&b,bool f){assert(a.Id==1&&b.Id==3&&!f);++Facings;}void EntitySetAsOpinionSource(const CScriptThing&t,const CCharString&s){assert(t.Id==1);Opinions.push_back(s.text);}void StartMovieSequence(const CCharString&,struct CScriptGameResourceObjectMovieBase&);float GetHealth(const CScriptThing&);void FixMovieSequenceCamera(bool b){b?++CameraTrue:++CameraFalse;}void ClearThingHasInformation(const CScriptThing&t){assert(t.Id==1);++InfoClear;}void MiniMapRemoveMarker(const CScriptThing&t){assert(t.Id==1);++MarkersRemoved;}
 long AddNewConversation(const CScriptThing&t,bool a,bool b){assert(t.Id==4&&!a&&!b);return ++Conversations;}void AddPersonToConversation(long c,const CScriptThing&t){assert(c==Conversations&&t.Id==2);++AddedPeople;}void AddLineToConversation(long c,const CCharString&s,bool a,const CScriptThing&t,const CScriptThing&h){assert(c==Conversations&&!a&&t.Id==4&&h.Id==2);ConvoLines.push_back(s.text);}void EntityPostOpinionDeedToAll(const CScriptThing&t,int deed){assert(t.Id==1&&deed==OPINION_DEED_TYPE_SHOW_TROPHY_GOOD);++Deeds;}void Error(const char*,const char*,const char*,int line){assert(line==729);++Errors;}
 int RegisterTimer(){++Timers;Clock[++NextTimer]=0;return NextTimer;}void DeregisterTimer(int i){assert(Clock.erase(i)==1);--Timers;}int GetTimer(int i){assert(Clock.count(i));return Clock[i];}void SetTimer(int i,int t){assert(Clock.count(i));Clock[i]=t;}
};
struct CTimer{int Id;CTimer():Id(Current->RegisterTimer()){}~CTimer(){Current->DeregisterTimer(Id);}long GetTime()const{return Current->GetTimer(Id);}CTimer&operator=(long n){Current->SetTimer(Id,(int)n);return *this;}};
struct CScriptGameResourceObjectMovieBase{bool Active;CScriptGameResourceObjectMovieBase():Active(false){}~CScriptGameResourceObjectMovieBase(){if(Active)--Movies;}};void GameStub::StartMovieSequence(const CCharString&s,CScriptGameResourceObjectMovieBase&m){assert(s.text.empty()&&!m.Active);m.Active=true;++Movies;}struct CWideScreenMagicPauseEntities{CWideScreenMagicPauseEntities(GameStub&){++Pauses;}~CWideScreenMagicPauseEntities(){--Pauses;}};
struct CScriptGameResourceObjectScriptedThingBase{int Id;CScriptGameResourceObjectScriptedThingBase():Id(0){++Resources;}CScriptGameResourceObjectScriptedThingBase(const CScriptGameResourceObjectScriptedThingBase&r):Id(r.Id){++Resources;}~CScriptGameResourceObjectScriptedThingBase(){--Resources;}bool IsValid()const{return Id!=0;}void Release(){assert(Id);Id=0;++Current->Releases;}void ClearCommands(){++Current->ClearCommands;Current->TaskUntil=Current->Frame;}bool IsPerformingScriptTask(){return Current->Frame<Current->TaskUntil;}CScriptThing GetScriptThing(){return CScriptThing(Id);}void PlayAnimation(const CCharString&s,bool a,bool b,bool c,bool d,bool e,bool f,bool g){assert(!a&&!b&&c&&e&&!f&&!g);Current->Animations.push_back(s.text);++Current->AnimCount;Current->TaskUntil=Current->Frame+Current->TaskLength;if(Current->AnimCount==Current->StopAnim)Current->Stop=true;}void PlayLoopingAnimation(const CCharString&s,int n,bool a,bool b,bool c,bool d,bool e,bool f,bool g){assert(n==1&&!a&&!b&&c&&!d&&e&&!f&&!g);Current->Animations.push_back(s.text);++Current->AnimCount;}void Speak(const CScriptThing&h,const CCharString&s,int selection,bool a,bool b,bool c){assert(Id==1&&h.Id==2&&!a&&b&&!c);++Current->SpeechCount;Current->Speech.push_back(s.text);Current->Selections.push_back(selection);Current->TaskUntil=Current->Frame+Current->TaskLength;if(Current->SpeechCount==Current->StopSpeech)Current->Stop=true;}void MoveToPosition(const C3DVector&,float,int,bool,bool){++Current->MoveCount;}};
struct Parent{BriarRoseOpinion OpinionCurrent,OpinionRequired;long HitByHeroCount;bool AttackSpeechComplete,HitWarningComplete,InsideTavern,DoneApproachMessage1,DoneApproachMessage2,AttackedByHero,DoneBriarIntroSpeech,DoneBriarTrophySpeech,CutsceneStarted;Parent():OpinionCurrent(BRIAR_ROSE_OPINION_LOUD),OpinionRequired(BRIAR_ROSE_OPINION_LOUD),HitByHeroCount(0),AttackSpeechComplete(false),HitWarningComplete(false),InsideTavern(true),DoneApproachMessage1(false),DoneApproachMessage2(false),AttackedByHero(false),DoneBriarIntroSpeech(false),DoneBriarTrophySpeech(false),CutsceneStarted(false){}};
struct Briar{GameStub&Game;CScriptThing Me;Parent ParentClass;CTimer AnimTimer;int Health;Briar(GameStub&g):Game(g),Me(1),Health(10){g.Owner=this;}void SetPredicate(const CIsThingAlive&p){assert(p.Thing.Id==1);Game.Predicate=true;}bool IsActiveThreadTerminating(){return Game.Stop||Game.Frame>=Game.TerminateAt||(Game.DeadAt>=0&&Game.Frame>=Game.DeadAt);}void Main();};
void GameStub::NewScriptFrame(){assert(++Frame<300);for(std::map<int,int>::iterator i=Clock.begin();i!=Clock.end();++i)if(i->second>0)--i->second;if(OpinionAt>=0&&Frame>=OpinionAt)Owner->ParentClass.OpinionRequired=(BriarRoseOpinion)RequiredAtChange;if(AttackAt>=0&&Frame>=AttackAt)Owner->ParentClass.AttackedByHero=true;if(HitCountAt>=0&&Frame>=HitCountAt)Owner->ParentClass.HitByHeroCount=HitCountValue;}
bool GameStub::StartScriptingEntity(const CScriptThing&t,CScriptGameResourceObjectScriptedThingBase&r,int p){assert((t.Id==1||t.Id==2)&&p==4);++Starts;if(Starts<=StartFailures||(FailAfter>=0&&Starts>=FailAfter))return false;r.Id=t.Id;return true;}float GameStub::GetHealth(const CScriptThing&t){assert(t.Id==1);return(float)Owner->Health;}
bool CScriptThing::IsAlive()const{return Id==1?true:Id==2?Current->HeroAlive:Id==3?Current->MarkerAlive:Id==4?Current->VillagerAlive:false;}bool CScriptThing::MsgIsTalkedToBy(const char*s)const{assert(Id==1&&std::strcmp(s,"SCRIPT_NAME_HERO")==0);return Current->TakeTalk();}static bool IsDistanceBetweenThingsUnder(const CScriptThing&a,const CScriptThing&b,float d){assert(a.Id==1&&b.Id==2);return Current->Distance<d;}
static void RunCutsceneMacro(const CCharString&s,std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>&m,int a,int b,bool c,bool d){assert(Current->Owner->ParentClass.CutsceneStarted&&m.size()==2&&m["ROSE"].Id==1&&m["HERO"].Id==2&&a==0&&b==0&&!c&&d);Current->Cutscenes.push_back(s.text);if((int)Current->Cutscenes.size()==Current->StopCutscene)Current->Stop=true;}
struct ScriptDef{int TrophiesOpinionDeedTimer,TrophiesHitCountForBriarAttack,TrophiesBriarRoseAnimTimeVar,TrophiesBriarRoseAnimTime;float TrophiesBriarRoseProximityFar,TrophiesBriarRoseProximityNear;ScriptDef():TrophiesOpinionDeedTimer(5),TrophiesHitCountForBriarAttack(3),TrophiesBriarRoseAnimTimeVar(2),TrophiesBriarRoseAnimTime(6),TrophiesBriarRoseProximityFar(12),TrophiesBriarRoseProximityNear(6){}};static ScriptDef GlobalDef;static ScriptDef*pGlobalScriptDef=&GlobalDef;static int FixtureRand(){return FixedRand;}
#define rand FixtureRand
void Briar::Main()
{
    TRetVoid _Fail_Val;
    CTimer deedTimer;
    long counter=0;
    CScriptThing waypoint;
    CTimer standStillTimer;
    AnimTimer=0;
    CScriptThing LookAtMe=Game.GetThingWithScriptName("M_TrophiesLookAtMe");
    if (LookAtMe.IsAlive()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.EntitySetFacingAngleTowardsThing(Me,LookAtMe,false);
    }
    bool initialise=false;
    deedTimer=pGlobalScriptDef->TrophiesOpinionDeedTimer;
    SetPredicate(CIsThingAlive(Me));
    CScriptGameResourceObjectScriptedThingBase sehMe;
    if (sehMe.IsValid()) sehMe.Release();
    while (!Game.StartScriptingEntity(Me,sehMe,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    for (;; Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (ParentClass.OpinionCurrent!=ParentClass.OpinionRequired) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (ParentClass.OpinionRequired==BRIAR_ROSE_OPINION_LOUD) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.EntitySetAsOpinionSource(Me,"OPINION_SOURCE_BRIAR_ROSE_LOUD");
                ParentClass.OpinionCurrent=BRIAR_ROSE_OPINION_LOUD;
            } else if (ParentClass.OpinionRequired==BRIAR_ROSE_OPINION_MEDIUM) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.EntitySetAsOpinionSource(Me,"OPINION_SOURCE_BRIAR_ROSE_MEDIUM");
                ParentClass.OpinionCurrent=BRIAR_ROSE_OPINION_MEDIUM;
            } else if (ParentClass.OpinionRequired==BRIAR_ROSE_OPINION_QUIET) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.EntitySetAsOpinionSource(Me,"OPINION_SOURCE_BRIAR_ROSE_QUIET");
                ParentClass.OpinionCurrent=BRIAR_ROSE_OPINION_QUIET;
            }
        }
        if (ParentClass.HitByHeroCount>pGlobalScriptDef->TrophiesHitCountForBriarAttack && ParentClass.AttackSpeechComplete && !ParentClass.HitWarningComplete) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (sehMe.IsValid()) sehMe.Release();
            while (!Game.StartScriptingEntity(Me,sehMe,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (Game.GetHealth(sehMe.GetScriptThing())>0.0f) {
                sehMe.Speak(Game.GetHero(),"TEXT_QST_038_BRIAR_WARNING",GROUP_SELECT_RANDOM_NO_REPEAT,false,true,false);
                while (sehMe.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            ParentClass.HitWarningComplete=true;
        }
        if (AnimTimer.GetTime()==0 && sehMe.IsValid()) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            sehMe.ClearCommands();
            if (LookAtMe.IsAlive()) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.EntitySetFacingAngleTowardsThing(Me,LookAtMe,false);
            }
            while (sehMe.IsPerformingScriptTask()) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            long animIdx=rand()%8;
            if (animIdx==0) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                sehMe.PlayAnimation("ST_IDLE",false,false,true,false,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
            } else if (animIdx==1) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                sehMe.PlayAnimation("ST_IDLE_SUBTLE",false,false,true,false,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
            } else if (animIdx==2) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                sehMe.PlayAnimation("SCRIPT_TALK_IDLE_01",false,false,true,false,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
            } else if (animIdx==3) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                sehMe.PlayAnimation("SCRIPT_TALK_IDLE_02",false,false,true,false,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
            } else if (animIdx==4) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                sehMe.PlayAnimation("SCRIPT_TALK_IDLE_03",false,false,true,false,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
            } else if (animIdx==5) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            } else if (animIdx==6) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                sehMe.PlayAnimation("SCRIPT_LISTEN_BASE_IN",false,false,true,false,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
                sehMe.PlayLoopingAnimation("SCRIPT_LISTEN_BASE_LOOP",1,false,false,true,false,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
                sehMe.PlayAnimation("SCRIPT_LISTEN_BASE_OUT",false,false,true,true,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
            } else if (animIdx==7) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                sehMe.PlayAnimation("SCRIPT_FOLD_ARMS_IN",false,false,true,false,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
                sehMe.PlayLoopingAnimation("SCRIPT_FOLD_ARMS_LOOP",1,false,false,true,false,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
                sehMe.PlayAnimation("SCRIPT_FOLD_ARMS_OUT",false,false,true,true,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
            }
            AnimTimer=rand()%pGlobalScriptDef->TrophiesBriarRoseAnimTimeVar+pGlobalScriptDef->TrophiesBriarRoseAnimTime;
        }
        if (Game.GetHero().IsAlive() && ParentClass.InsideTavern) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (!ParentClass.DoneApproachMessage1 && IsDistanceBetweenThingsUnder(Me,Game.GetHero(),pGlobalScriptDef->TrophiesBriarRoseProximityFar)) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptThing villager=Game.GetRandomThingWithScriptName("TrophiesIntroWatcher");
                if (villager.IsAlive()) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    long HLStyleConversationIndex=Game.AddNewConversation(villager,false,false);
                    Game.AddPersonToConversation(HLStyleConversationIndex,Game.GetHero());
                    Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_038_BRIAR_APPROACH_01",false,villager,Game.GetHero());
                }
                ParentClass.DoneApproachMessage1=true;
            }
            if (!ParentClass.DoneApproachMessage2 && IsDistanceBetweenThingsUnder(Me,Game.GetHero(),pGlobalScriptDef->TrophiesBriarRoseProximityNear)) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptThing villager=Game.GetRandomThingWithScriptName("TrophiesIntroWatcher");
                if (villager.IsAlive()) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    long HLStyleConversationIndex=Game.AddNewConversation(villager,false,false);
                    Game.AddPersonToConversation(HLStyleConversationIndex,Game.GetHero());
                    Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_038_BRIAR_APPROACH_02",false,villager,Game.GetHero());
                }
                ParentClass.DoneApproachMessage2=true;
            }
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            sehMe.ClearCommands();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            while (sehMe.IsPerformingScriptTask()) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (ParentClass.AttackedByHero && ParentClass.AttackSpeechComplete) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (sehMe.IsValid()) sehMe.Release();
                while (!Game.StartScriptingEntity(Me,sehMe,SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(sehMe.GetScriptThing())>0.0f) {
                    sehMe.Speak(Game.GetHero(),"TEXT_QST_038_BRIAR_SNIFFY",GROUP_SELECT_RANDOM_NO_REPEAT,false,true,false);
                    while (sehMe.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            } else if (!ParentClass.DoneBriarIntroSpeech) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ParentClass.CutsceneStarted=true;
                if (sehMe.IsValid()) sehMe.Release();
                while (!Game.StartScriptingEntity(Me,sehMe,SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectScriptedThingBase seh_hero;
                Game.StartScriptingEntity(Game.GetHero(),seh_hero,SCRIPT_AI_PRIORITY_HIGHEST);
                std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
                mymap["ROSE"]=sehMe;
                mymap["HERO"]=seh_hero;
                Game.FixMovieSequenceCamera(true);
                RunCutsceneMacro("CS_TROPHIES_INTRO",mymap,0,0,false,true);
                Game.FixMovieSequenceCamera(false);
                ParentClass.CutsceneStarted=false;
                ParentClass.DoneBriarIntroSpeech=true;
            } else if (!ParentClass.DoneBriarTrophySpeech) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ParentClass.CutsceneStarted=true;
                if (sehMe.IsValid()) sehMe.Release();
                while (!Game.StartScriptingEntity(Me,sehMe,SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectScriptedThingBase seh_hero;
                Game.StartScriptingEntity(Game.GetHero(),seh_hero,SCRIPT_AI_PRIORITY_HIGHEST);
                std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
                mymap["ROSE"]=sehMe;
                mymap["HERO"]=seh_hero;
                Game.FixMovieSequenceCamera(true);
                RunCutsceneMacro("CS_TROPHIES_TROPHY",mymap,0,0,false,true);
                Game.FixMovieSequenceCamera(false);
                ParentClass.CutsceneStarted=false;
                ParentClass.DoneBriarTrophySpeech=true;
                Game.ClearThingHasInformation(Me);
                Game.MiniMapRemoveMarker(Me);
            } else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (sehMe.IsValid()) sehMe.Release();
                while (!Game.StartScriptingEntity(Me,sehMe,SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(sehMe.GetScriptThing())>0.0f) {
                    sehMe.Speak(Game.GetHero(),"TEXT_QST_038_BRIAR_BLATHER",GROUP_SELECT_RANDOM_NO_REPEAT,false,true,false);
                    while (sehMe.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
        } else if (ParentClass.AttackedByHero) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (!ParentClass.AttackSpeechComplete) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.ClearThingHasInformation(Me);
                if (sehMe.IsValid()) sehMe.Release();
                while (!Game.StartScriptingEntity(Me,sehMe,SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (Game.GetHealth(sehMe.GetScriptThing())>0.0f) {
                    sehMe.Speak(Game.GetHero(),"TEXT_QST_038_BRIAR_ATTACK",GROUP_SELECT_NONE,false,true,false);
                    while (sehMe.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                ParentClass.AttackSpeechComplete=true;
            }
        }
        if (deedTimer.GetTime()<=0) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.EntityPostOpinionDeedToAll(Me,OPINION_DEED_TYPE_SHOW_TROPHY_GOOD);
            deedTimer=pGlobalScriptDef->TrophiesOpinionDeedTimer;
        }
        if (initialise) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CCharString position_name;
            position_name.Sprintf("M_TrophiesIntroLocation%i",counter);
            waypoint=Game.GetThingWithScriptName(position_name);
            if (!waypoint.IsAlive() || counter>=6) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                waypoint=Game.GetThingWithScriptName("M_TrophiesIntroLocation0");
                counter=0;
            }
            if (!waypoint.IsAlive()) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.Error("waypoint.IsAlive()","This marker should be alive but isn't - Tell Tony","e:\\fabletlc\\fable tlc build repository\\bbbprojects\\fable1_5mainpc\\fablescripting\\V_IntroductionToTrophies.hpp",729);
            }
            sehMe.MoveToPosition(waypoint.GetPos(),0.1f,ENTITY_MOVE_WALK,false,true);
            initialise=false;
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}

static bool Has(const std::vector<std::string>&v,const char*s){for(unsigned i=0;i<v.size();++i)if(v[i]==s)return true;return false;}static int Count(const std::vector<std::string>&v,const char*s){int n=0;for(unsigned i=0;i<v.size();++i)if(v[i]==s)++n;return n;}static bool Balanced(){return Strings==0&&Resources==0&&Movies==0&&Pauses==0&&Timers==1&&Current->Waypoints==0&&Current->Errors==0&&Current->MoveCount==0;}static bool Check(bool b,int n){if(!b)std::printf("case %d failed\n",n);return b;}
int main(){bool ok=true;int n=0;
 {GameStub g;Briar o(g);o.Main();ok=Check(Balanced()&&g.Predicate&&g.Deeds==3&&g.Speech.empty()&&g.AnimCount==3&&g.Facings==4&&!o.ParentClass.DoneApproachMessage1,++n)&&ok;}
 for(int anim=0;anim<8;++anim){GameStub g;Briar o(g);FixedRand=anim;g.TerminateAt=1;o.Main();ok=Check(Balanced()&&g.AnimCount==(anim==5?0:anim>=6?3:1)&&g.Animations.size()==(unsigned)g.AnimCount&&g.ClearCommands==1,++n)&&ok;}
 for(int required=0;required<3;++required){GameStub g;Briar o(g);o.ParentClass.OpinionCurrent=(BriarRoseOpinion)((required+1)%3);o.ParentClass.OpinionRequired=(BriarRoseOpinion)required;o.Main();const char*names[]={"OPINION_SOURCE_BRIAR_ROSE_LOUD","OPINION_SOURCE_BRIAR_ROSE_MEDIUM","OPINION_SOURCE_BRIAR_ROSE_QUIET"};ok=Check(Balanced()&&g.Opinions.size()==1&&g.Opinions[0]==names[required]&&o.ParentClass.OpinionCurrent==required,++n)&&ok;}
 {GameStub g;Briar o(g);g.OpinionAt=4;g.RequiredAtChange=2;o.Main();ok=Check(Balanced()&&g.Opinions.size()==1&&o.ParentClass.OpinionCurrent==BRIAR_ROSE_OPINION_QUIET,++n)&&ok;}
 {GameStub g;Briar o(g);o.ParentClass.OpinionRequired=(BriarRoseOpinion)8;o.Main();ok=Check(Balanced()&&g.Opinions.empty()&&o.ParentClass.OpinionCurrent==BRIAR_ROSE_OPINION_LOUD,++n)&&ok;}
 {GameStub g;Briar o(g);g.Distance=4;o.Main();ok=Check(Balanced()&&g.Conversations==2&&g.AddedPeople==2&&Has(g.ConvoLines,"TEXT_QST_038_BRIAR_APPROACH_01")&&Has(g.ConvoLines,"TEXT_QST_038_BRIAR_APPROACH_02")&&o.ParentClass.DoneApproachMessage1&&o.ParentClass.DoneApproachMessage2,++n)&&ok;}
 {GameStub g;Briar o(g);g.Distance=8;o.Main();ok=Check(Balanced()&&g.Conversations==1&&o.ParentClass.DoneApproachMessage1&&!o.ParentClass.DoneApproachMessage2,++n)&&ok;}
 {GameStub g;Briar o(g);g.Distance=4;g.VillagerAlive=false;o.Main();ok=Check(Balanced()&&g.Conversations==0&&o.ParentClass.DoneApproachMessage1&&o.ParentClass.DoneApproachMessage2,++n)&&ok;}
 for(int reason=0;reason<2;++reason){GameStub g;Briar o(g);g.Distance=4;if(reason)o.ParentClass.InsideTavern=false;else g.HeroAlive=false;o.Main();ok=Check(Balanced()&&g.Conversations==0&&!o.ParentClass.DoneApproachMessage1&&!o.ParentClass.DoneApproachMessage2,++n)&&ok;}
 {GameStub g;Briar o(g);g.TalkAt.push_back(2);g.TalkAt.push_back(5);g.TalkAt.push_back(8);o.Main();ok=Check(Balanced()&&g.Cutscenes.size()==2&&g.Cutscenes[0]=="CS_TROPHIES_INTRO"&&g.Cutscenes[1]=="CS_TROPHIES_TROPHY"&&g.CameraTrue==2&&g.CameraFalse==2&&o.ParentClass.DoneBriarIntroSpeech&&o.ParentClass.DoneBriarTrophySpeech&&!o.ParentClass.CutsceneStarted&&g.InfoClear==1&&g.MarkersRemoved==1&&Has(g.Speech,"TEXT_QST_038_BRIAR_BLATHER"),++n)&&ok;}
 {GameStub g;Briar o(g);o.ParentClass.AttackedByHero=true;g.HitCountAt=4;g.HitCountValue=4;g.TalkAt.push_back(8);o.Main();ok=Check(Balanced()&&Count(g.Speech,"TEXT_QST_038_BRIAR_ATTACK")==1&&Count(g.Speech,"TEXT_QST_038_BRIAR_WARNING")==1&&Count(g.Speech,"TEXT_QST_038_BRIAR_SNIFFY")==1&&o.ParentClass.AttackSpeechComplete&&o.ParentClass.HitWarningComplete&&g.InfoClear==1&&g.Cutscenes.empty(),++n)&&ok;}
 {GameStub g;Briar o(g);o.ParentClass.AttackedByHero=true;o.ParentClass.HitByHeroCount=3;o.Main();ok=Check(Balanced()&&Has(g.Speech,"TEXT_QST_038_BRIAR_ATTACK")&&!Has(g.Speech,"TEXT_QST_038_BRIAR_WARNING")&&!o.ParentClass.HitWarningComplete,++n)&&ok;}
 {GameStub g;Briar o(g);o.ParentClass.HitByHeroCount=4;o.Main();ok=Check(Balanced()&&g.Speech.empty()&&!o.ParentClass.HitWarningComplete,++n)&&ok;}
 {GameStub g;Briar o(g);o.ParentClass.AttackedByHero=true;o.ParentClass.AttackSpeechComplete=true;o.ParentClass.HitByHeroCount=4;o.ParentClass.HitWarningComplete=true;g.TalkAt.push_back(3);o.Main();ok=Check(Balanced()&&g.Speech.size()==1&&g.Speech[0]=="TEXT_QST_038_BRIAR_SNIFFY"&&g.Selections[0]==GROUP_SELECT_RANDOM_NO_REPEAT,++n)&&ok;}
 {GameStub g;Briar o(g);o.Health=0;o.ParentClass.AttackedByHero=true;o.ParentClass.HitByHeroCount=4;g.TalkAt.push_back(6);o.Main();ok=Check(Balanced()&&g.Speech.empty()&&o.ParentClass.AttackSpeechComplete&&o.ParentClass.HitWarningComplete&&g.Cutscenes.empty(),++n)&&ok;}
 {GameStub g;Briar o(g);g.MarkerAlive=false;o.Main();ok=Check(Balanced()&&g.Facings==0&&g.Predicate,++n)&&ok;}
 {GameStub g;Briar o(g);g.StartFailures=100;g.TerminateAt=3;o.Main();ok=Check(Balanced()&&g.Starts==3&&g.AnimCount==0&&g.Deeds==0,++n)&&ok;}
 {GameStub g;Briar o(g);g.DeadAt=0;o.Main();ok=Check(Balanced()&&!g.Predicate&&g.Starts==0&&g.Facings==0,++n)&&ok;}
 {GameStub g;Briar o(g);g.DeadAt=1;o.Main();ok=Check(Balanced()&&g.Predicate&&g.AnimCount==1&&g.Frame==1,++n)&&ok;}
 {GameStub g;Briar o(g);o.ParentClass.AttackedByHero=true;g.StopSpeech=1;o.Main();ok=Check(Balanced()&&Has(g.Speech,"TEXT_QST_038_BRIAR_ATTACK")&&!o.ParentClass.AttackSpeechComplete,++n)&&ok;}
 {GameStub g;Briar o(g);o.ParentClass.AttackSpeechComplete=true;o.ParentClass.HitByHeroCount=4;g.StopSpeech=1;o.Main();ok=Check(Balanced()&&Has(g.Speech,"TEXT_QST_038_BRIAR_WARNING")&&!o.ParentClass.HitWarningComplete,++n)&&ok;}
 {GameStub g;Briar o(g);g.FailAfter=2;g.TalkAt.push_back(2);g.TerminateAt=5;o.Main();ok=Check(Balanced()&&o.ParentClass.CutsceneStarted&&!o.ParentClass.DoneBriarIntroSpeech&&g.Cutscenes.empty()&&g.Releases==1,++n)&&ok;}
 {GameStub g;Briar o(g);g.TalkAt.push_back(2);g.StopCutscene=1;o.Main();ok=Check(Balanced()&&o.ParentClass.DoneBriarIntroSpeech&&!o.ParentClass.CutsceneStarted&&g.CameraTrue==g.CameraFalse&&g.Cutscenes.size()==1,++n)&&ok;}
 {GameStub g;Briar o(g);o.ParentClass.DoneBriarIntroSpeech=true;o.ParentClass.DoneBriarTrophySpeech=true;g.TalkAt.push_back(2);g.StopSpeech=1;o.Main();ok=Check(Balanced()&&Has(g.Speech,"TEXT_QST_038_BRIAR_BLATHER"),++n)&&ok;}
 ok=ok&&Timers==0;std::printf(ok?"00e9e420_TEST PASS\n":"00e9e420_TEST FAIL\n");return ok?0:1;}
