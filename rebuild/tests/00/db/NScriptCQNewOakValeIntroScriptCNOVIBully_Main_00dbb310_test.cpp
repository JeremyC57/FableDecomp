// Complete Main body with deterministic game and resource callees.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <map>
#include <vector>
#include <cassert>
static int Strings,Things,Handles,Movies;
class CCharString { public:
 std::string text;
 CCharString():text(){++Strings;}CCharString(const char* s):text(s){++Strings;}CCharString(const CCharString& s):text(s.text){++Strings;}~CCharString(){--Strings;}
 CCharString& operator=(const CCharString& s){text=s.text;return *this;}CCharString& operator=(const char* s){text=s;return *this;}
 bool operator==(const char* s)const{return text==s;}bool operator!=(const char* s)const{return text!=s;}bool operator<(const CCharString& s)const{return text<s.text;}
 operator const char*()const{return text.c_str();}void Sprintf(const char* f,long n){char b[128];std::sprintf(b,f,(int)n);text=b;}
};
class C3DVector{ public:float X,Y,Z;C3DVector():X(0),Y(0),Z(0){}};
struct GameStub;struct Bully;static GameStub* Current;
class CScriptThing { public:
 int id;CScriptThing(int n=0):id(n){++Things;}CScriptThing(const CScriptThing& t):id(t.id){++Things;}~CScriptThing(){--Things;}
 C3DVector GetHomePos()const{return C3DVector();}
 bool MsgIsTalkedToBy(const char*)const;bool MsgIsPresentedWithItem(CCharString&)const;
 bool MsgIsHitBy(const char*)const;bool MsgIsHitByAnySpecialAbilityFrom(const char*)const;bool MsgIsHitBySpecialAbilityFrom(int,const char*)const;bool IsAlive()const;
};
class CScriptGameResourceObjectMovieBase{ public:CScriptGameResourceObjectMovieBase(){++Movies;}~CScriptGameResourceObjectMovieBase(){--Movies;}};
class CScriptGameResourceObjectScriptedThingBase { public:
 int id;CScriptGameResourceObjectScriptedThingBase():id(0){++Handles;}CScriptGameResourceObjectScriptedThingBase(const CScriptGameResourceObjectScriptedThingBase& h):id(h.id){++Handles;}~CScriptGameResourceObjectScriptedThingBase(){--Handles;}
 CScriptGameResourceObjectScriptedThingBase& operator=(const CScriptGameResourceObjectScriptedThingBase& h){id=h.id;return *this;}
 bool IsValid()const{return id!=0;}void Release(){id=0;}CScriptThing GetScriptThing()const{return CScriptThing(id);}
 void MoveToPosition(const C3DVector&,float,int,bool,bool);bool IsPerformingScriptTask()const;
 void Speak(const CScriptThing&,const char*,int,bool,bool,bool);void PlayAnimation(const char*,bool,bool,bool,bool,bool,bool,bool);
};
class CTimer { public:int time;CTimer():time(0){}long GetTime()const{return time;}CTimer& operator=(long n){time=(int)n;return *this;}};
struct Parent {
 bool VictimShake,BullySubdued,BullyRanOff,GivenHeroTeddy,HeroAttackedVictim,VictimComplainsAboutLosingTeddy;
 int GUIBullyHealthCounter,GoodDeeds;CTimer TalkIntermittentTimer;
 Parent():VictimShake(false),BullySubdued(false),BullyRanOff(false),GivenHeroTeddy(false),HeroAttackedVictim(false),VictimComplainsAboutLosingTeddy(false),GUIBullyHealthCounter(-999),GoodDeeds(0){}
 void AddGoodDeed(){++GoodDeeds;}
};
class CRGBColour{ public:CRGBColour(int,int,int,int){}};
struct GameStub {
 int Frame,TerminateAt,StartAt,TaskUntil,TaskFrames,HomeGoal,HomeMoves,PresentAt,NormalAt,NormalAt2,AbilityAt,HealAt,Answer,AnswerAt,PauseDepth,Given,Removed,BarAdds,BarUpdates,BarRemoves,Allies,Cutscenes;
 bool HasTeddy,Near,Healthy,VictimAlive,CameraFixed;const char* Present;Parent* ParentPtr;
 std::vector<int> Talks;std::vector<std::string> Lines,Scenes;
 GameStub():Frame(0),TerminateAt(20),StartAt(0),TaskUntil(0),TaskFrames(1),HomeGoal(0),HomeMoves(0),PresentAt(-1),NormalAt(-1),NormalAt2(-1),AbilityAt(-1),HealAt(-1),Answer(1),AnswerAt(0),PauseDepth(0),Given(0),Removed(0),BarAdds(0),BarUpdates(0),BarRemoves(0),Allies(0),Cutscenes(0),HasTeddy(false),Near(false),Healthy(true),VictimAlive(true),CameraFixed(false),Present(""),ParentPtr(0){Current=this;}
 void NewScriptFrame(){++Frame;assert(Frame<1000);if(ParentPtr){ParentPtr->VictimComplainsAboutLosingTeddy=false;if(ParentPtr->TalkIntermittentTimer.time>0)--ParentPtr->TalkIntermittentTimer.time;}}
 bool StartScriptingEntity(const CScriptThing& t,CScriptGameResourceObjectScriptedThingBase& h,int p){assert(p==4);if(Frame<StartAt)return false;h.id=t.id;return true;}
 CScriptThing GetHero(){return CScriptThing(2);}CScriptThing GetThingWithScriptName(const char* s){assert(!std::strcmp(s,"NOVI_Victim"));return CScriptThing(3);}
 bool IsObjectInThingsPossession(const char*,const CScriptThing&){return HasTeddy;}
 void StartMovieSequence(const char* s,CScriptGameResourceObjectMovieBase&){assert(!*s);}
 void PauseAllNonScriptedEntities(bool b){PauseDepth+=b?1:-1;assert(PauseDepth>=0&&PauseDepth<=1);}
 float GetHealth(const CScriptThing&){return Healthy?100.0f:0.0f;}
 void GiveHeroYesNoQuestion(const char*,const char*,const char*,const char*,bool){}
 long MsgIsQuestionAnsweredYesOrNo(){return Frame>=AnswerAt?Answer:-1;}
 void ClearThingHasInformation(const CScriptThing&){}
 long AddNewConversation(const CScriptThing&,bool,bool){return 1;}void AddPersonToConversation(long,const CScriptThing&){}
 void AddLineToConversation(long,const char* s,bool,const CScriptThing&,const CScriptThing&){Lines.push_back(s);}
 void EntitySetThingAsAllyOfThing(const CScriptThing&,const CScriptThing&){++Allies;}
 long AddQuestInfoBar(float,float,const CRGBColour&,const CRGBColour&,const char*,const char*,float){++BarAdds;return 17;}
 void UpdateQuestInfoBar(long i,float,float,float){assert(i==17);++BarUpdates;}void RemoveQuestInfoElement(long i){assert(i==17);++BarRemoves;}
 void EntitySetFacingAngleTowardsThing(const CScriptThing&,const CScriptThing&,bool){}
 void FixMovieSequenceCamera(bool b){CameraFixed=b;}
 void RemoveThing(const CScriptThing& t,bool a,bool b){assert(t.id==1&&!a&&b);++Removed;}
};
bool CScriptThing::MsgIsTalkedToBy(const char*)const{for(unsigned i=0;i<Current->Talks.size();++i)if(Current->Talks[i]==Current->Frame)return true;return false;}
bool CScriptThing::MsgIsPresentedWithItem(CCharString& s)const{if(Current->PresentAt!=Current->Frame)return false;s=Current->Present;return true;}
bool CScriptThing::MsgIsHitBy(const char*)const{return Current->Frame==Current->NormalAt||Current->Frame==Current->NormalAt2;}
bool CScriptThing::MsgIsHitByAnySpecialAbilityFrom(const char*)const{return Current->Frame==Current->AbilityAt;}
bool CScriptThing::MsgIsHitBySpecialAbilityFrom(int,const char*)const{return Current->Frame==Current->HealAt;}
bool CScriptThing::IsAlive()const{return Current->VictimAlive;}
void CScriptGameResourceObjectScriptedThingBase::MoveToPosition(const C3DVector&,float f,int t,bool a,bool b){assert(f==0&&t==0&&!a&&b);++Current->HomeMoves;Current->TaskUntil=Current->Frame+Current->TaskFrames;}
bool CScriptGameResourceObjectScriptedThingBase::IsPerformingScriptTask()const{return Current->Frame<Current->TaskUntil;}
void CScriptGameResourceObjectScriptedThingBase::Speak(const CScriptThing&,const char* s,int,bool,bool,bool){assert(id==1);Current->Lines.push_back(s);Current->TaskUntil=Current->Frame+Current->TaskFrames;}
void CScriptGameResourceObjectScriptedThingBase::PlayAnimation(const char*,bool,bool,bool,bool,bool,bool,bool){}
struct CWideScreenMagicPauseEntities{GameStub* Game;CWideScreenMagicPauseEntities(GameStub& g):Game(&g){Game->PauseAllNonScriptedEntities(true);}~CWideScreenMagicPauseEntities(){Game->PauseAllNonScriptedEntities(false);}};
struct CIsThingConscious{CIsThingConscious(const CScriptThing&){}};struct TRetVoid{void Return()const{}};
namespace NGlobalConsole{bool GDefaultToUsingPhysicsInCutsceneAnimations=false;}
enum {ENTITY_MOVE_WALK=0,SCRIPT_AI_PRIORITY_HIGHEST=4,GROUP_SELECT_NONE=0,HERO_ABILITY_HEAL_LIFE_SPELL=14};
typedef int EScriptEntityMoveType;
static bool IsDistanceFromThingToPositionOver(const CScriptThing&,const C3DVector&,float d){assert(d==2);return Current->HomeMoves<Current->HomeGoal;}
static bool IsDistanceBetweenThingsUnder(const CScriptThing&,const CScriptThing&,float d){assert(d==15);return Current->Near;}
static void RunCutsceneMacro(const char* n,std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>& cast,int z,std::map<CCharString,CCharString>* text,bool a,bool b){assert(cast["HERO"].id==2&&cast["BRAT"].id==3&&cast["BULLY"].id==1&&z==0&&!a&&b);if(text)assert(!(*text)["$BRATLINE"].text.empty());++Current->Cutscenes;Current->Scenes.push_back(n);}
struct Bully {
 GameStub* Game;CScriptThing Me;Parent ParentClass;long InitialHealth,HitsTaken,IntimidateSpeechLoop;bool DoneIntro,SaidPieceAboutAttackingVictim,SpokenOnFirstProximity;
 Bully(GameStub& g):Game(&g),Me(1),InitialHealth(2),HitsTaken(0),IntimidateSpeechLoop(10),DoneIntro(false),SaidPieceAboutAttackingVictim(false),SpokenOnFirstProximity(false){g.ParentPtr=&ParentClass;}
 void SetPredicate(const CIsThingConscious&){}bool IsActiveThreadTerminating()const{return Game->Frame>=Game->TerminateAt;}
 void GivenTeddy(){++Game->Given;Game->HasTeddy=false;}void Main();
};
void Bully::Main()
{
    // Fixture adapter: bind the original reference to the configured dependency.
    ::GameStub& Game = *this->Game;
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingConscious(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    {
        C3DVector local_position=Me.GetHomePos();
        float local_range=2.0f;
        EScriptEntityMoveType local_movetype=ENTITY_MOVE_WALK;
        while (IsDistanceFromThingToPositionOver(seh_me.GetScriptThing(),local_position,local_range)) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            seh_me.MoveToPosition(local_position,0.0f,local_movetype,false,true);
            while (seh_me.IsPerformingScriptTask()) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptThing victim=Game.GetThingWithScriptName("NOVI_Victim");
    for (; !false; Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (seh_me.IsValid()) seh_me.Release();
        while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (DoneIntro) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CCharString item_presented;
            if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO") && Game.IsObjectInThingsPossession("OBJECT_TEDDY_BEAR_UNGIVEABLE",Game.GetHero())) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_048_BULLY_FOUND_TEDDY_ONE",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                Game.GiveHeroYesNoQuestion("TEXT_QST_048_GIVE_TEDDY_TO_BULLY","TEXT_OBJECT_HERO_ANSWER_YES","TEXT_OBJECT_HERO_ANSWER_NO","",true);
                long YNQAnswer=Game.MsgIsQuestionAnsweredYesOrNo();
                while (YNQAnswer<0) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    YNQAnswer=Game.MsgIsQuestionAnsweredYesOrNo();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (YNQAnswer==1) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    ParentClass.VictimComplainsAboutLosingTeddy=true;
                    while (ParentClass.VictimComplainsAboutLosingTeddy) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_048_BULLY_FOUND_TEDDY_TWO",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    GivenTeddy();
                    Game.ClearThingHasInformation(Me);
                }
            }
            else if (Me.MsgIsPresentedWithItem(item_presented) && item_presented=="OBJECT_TEDDY_BEAR_UNGIVEABLE") {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                ParentClass.VictimComplainsAboutLosingTeddy=true;
                while (ParentClass.VictimComplainsAboutLosingTeddy) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_048_BULLY_FOUND_TEDDY_TWO",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                GivenTeddy();
                Game.ClearThingHasInformation(Me);
            }
            else if (Me.MsgIsPresentedWithItem(item_presented) && item_presented!="OBJECT_TEDDY_BEAR_UNGIVEABLE") {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_048_BULLY_DONT_WANT",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (!DoneIntro) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_048_BULLY_GET_LOST",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                DoneIntro=true;
            }
            else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (ParentClass.HeroAttackedVictim) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (!SaidPieceAboutAttackingVictim) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game.GetHero(),"TEXT_QST_048_BULLY_IN_COMMON",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        SaidPieceAboutAttackingVictim=true;
                    }
                    else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (HitsTaken<3) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                                seh_me.Speak(Game.GetHero(),"TEXT_QST_048_BULLY_NASTY_STREAK",GROUP_SELECT_NONE,false,true,false);
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
                                seh_me.Speak(Game.GetHero(),"TEXT_QST_048_BULLY_DONT_HIT_ME",GROUP_SELECT_NONE,false,true,false);
                                while (seh_me.IsPerformingScriptTask()) {
                                    Game.NewScriptFrame();
                                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                }
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                        }
                    }
                }
                else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_048_BULLY_BADGERING",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
            }
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (ParentClass.GUIBullyHealthCounter==-999) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ParentClass.GUIBullyHealthCounter=Game.AddQuestInfoBar((float)InitialHealth,0.0f,CRGBColour(0,255,0,255),CRGBColour(255,0,0,255),"HUD_QUEST_ICON_GRANDSON","",1.0f);
            }
            Game.EntitySetThingAsAllyOfThing(Me,Game.GetHero());
            Game.EntitySetThingAsAllyOfThing(Game.GetHero(),Me);
            if (++HitsTaken<InitialHealth) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
                Game.AddPersonToConversation(HLStyleConversationIndex,victim);
                Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_048_BULLY_SCRMSG_GET_OFF",false,Me,victim);
                Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_048_VICTIM_REVENGE",false,Me,victim);
                Game.UpdateQuestInfoBar(ParentClass.GUIBullyHealthCounter,(float)(InitialHealth-HitsTaken),-1.0f,-1.0f);
            }
            else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ParentClass.BullySubdued=true;
                Game.RemoveQuestInfoElement(ParentClass.GUIBullyHealthCounter);
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                {
                    CScriptGameResourceObjectScriptedThingBase seh_Hero;
                    if (seh_Hero.IsValid()) seh_Hero.Release();
                    while (!Game.StartScriptingEntity(Game.GetHero(),seh_Hero,SCRIPT_AI_PRIORITY_HIGHEST)) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    CScriptGameResourceObjectScriptedThingBase seh_Victim;
                    if (seh_Victim.IsValid()) seh_Victim.Release();
                    while (!Game.StartScriptingEntity(victim,seh_Victim,SCRIPT_AI_PRIORITY_HIGHEST)) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
                    mymap["HERO"]=seh_Hero;
                    mymap["BRAT"]=seh_Victim;
                    mymap["BULLY"]=seh_me;
                    std::map<CCharString,CCharString> csargs;
                    if (ParentClass.HeroAttackedVictim) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        csargs["$BRATLINE"]="TEXT_QST_048_VICTIM_THANKS_AFTER_HIT";
                    }
                    else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        csargs["$BRATLINE"]="TEXT_QST_048_VICTIM_THANKS";
                    }
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game.StartMovieSequence("",speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                    Game.FixMovieSequenceCamera(true);
                    RunCutsceneMacro("CS_OAKVALEINTRO_BULLYRUN1",mymap,0,&csargs,false,true);
                    if (!ParentClass.GivenHeroTeddy) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        RunCutsceneMacro("CS_OAKVALEINTRO_BULLYRUN2",mymap,0,0,false,true);
                        Game.ClearThingHasInformation(victim);
                        ParentClass.GivenHeroTeddy=true;
                    }
                    else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        RunCutsceneMacro("CS_OAKVALEINTRO_BULLYRUNDUMMY",mymap,0,0,false,true);
                    }
                    Game.FixMovieSequenceCamera(false);
                }
                ParentClass.BullyRanOff=true;
                ParentClass.AddGoodDeed();
                Game.RemoveThing(Me,false,true);
                return _Fail_Val.Return();
            }
        }
        Game.EntitySetFacingAngleTowardsThing(Me,victim,true);
        static float dist=15.0f;
        static long freq=75;
        if (ParentClass.TalkIntermittentTimer.GetTime()==0 && (!SpokenOnFirstProximity || rand()%freq==0) && IsDistanceBetweenThingsUnder(Me,Game.GetHero(),dist) && HitsTaken==0) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            SpokenOnFirstProximity=true;
            ParentClass.TalkIntermittentTimer=3;
            ParentClass.VictimShake=true;
            long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
            Game.AddPersonToConversation(HLStyleConversationIndex,victim);
            CCharString tag;
            tag.Sprintf("TEXT_QST_048_BULLY_SCRMSG_INTIMIDATING_%d",IntimidateSpeechLoop);
            Game.AddLineToConversation(HLStyleConversationIndex,tag,false,Me,victim);
            IntimidateSpeechLoop+=10;
            if (IntimidateSpeechLoop>40) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                IntimidateSpeechLoop=10;
            }
            if (rand()%2==0) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                seh_me.PlayAnimation("ST_OPINION_DISAPPROVAL_SHAKE_FIST",false,false,false,true,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
            }
            else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                seh_me.PlayAnimation("ST_OPINION_DISAPPROVAL_POINT_AT",false,false,false,true,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
            }
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}
}

static bool Balanced(const GameStub& g){return Strings==0&&Things==1&&Handles==0&&Movies==0&&g.PauseDepth==0;}
static bool Has(const GameStub& g,const char* s){for(unsigned i=0;i<g.Lines.size();++i)if(g.Lines[i]==s)return true;return false;}
int main(){bool ok=true;
 {GameStub g;Bully b(g);g.HomeGoal=1;g.HomeMoves=0;g.Talks.push_back(4);g.Talks.push_back(8);g.Talks.push_back(12);b.ParentClass.HeroAttackedVictim=true;b.Main();ok=ok&&Balanced(g)&&g.HomeMoves==1&&b.DoneIntro&&b.SaidPieceAboutAttackingVictim&&Has(g,"TEXT_QST_048_BULLY_GET_LOST")&&Has(g,"TEXT_QST_048_BULLY_IN_COMMON")&&Has(g,"TEXT_QST_048_BULLY_NASTY_STREAK");}
 {GameStub g;Bully b(g);b.DoneIntro=true;g.PresentAt=2;g.Present="OBJECT_APPLE";b.Main();ok=ok&&Balanced(g)&&g.Given==0&&Has(g,"TEXT_QST_048_BULLY_DONT_WANT");}
 {GameStub g;Bully b(g);b.DoneIntro=true;g.PresentAt=2;g.Present="OBJECT_TEDDY_BEAR_UNGIVEABLE";b.Main();ok=ok&&Balanced(g)&&g.Given==1&&!b.ParentClass.VictimComplainsAboutLosingTeddy&&Has(g,"TEXT_QST_048_BULLY_FOUND_TEDDY_TWO");}
 {GameStub g;Bully b(g);b.DoneIntro=true;g.HasTeddy=true;g.Talks.push_back(2);g.AnswerAt=4;b.Main();ok=ok&&Balanced(g)&&g.Given==1&&Has(g,"TEXT_QST_048_BULLY_FOUND_TEDDY_ONE")&&Has(g,"TEXT_QST_048_BULLY_FOUND_TEDDY_TWO");}
 {GameStub g;Bully b(g);b.DoneIntro=true;g.HasTeddy=true;g.Talks.push_back(2);g.Answer=0;b.Main();ok=ok&&Balanced(g)&&g.Given==0&&!b.ParentClass.VictimComplainsAboutLosingTeddy&&Has(g,"TEXT_QST_048_BULLY_FOUND_TEDDY_ONE");}
 {GameStub g;Bully b(g);g.NormalAt=2;g.NormalAt2=5;b.Main();ok=ok&&Balanced(g)&&b.HitsTaken==2&&b.ParentClass.BullySubdued&&b.ParentClass.BullyRanOff&&b.ParentClass.GivenHeroTeddy&&b.ParentClass.GoodDeeds==1&&g.Removed==1&&g.BarAdds==1&&g.BarUpdates==1&&g.BarRemoves==1&&g.Allies==4&&g.Cutscenes==2&&g.Scenes[1]=="CS_OAKVALEINTRO_BULLYRUN2"&&!g.CameraFixed;}
 {GameStub g;Bully b(g);b.InitialHealth=1;b.ParentClass.GivenHeroTeddy=true;b.ParentClass.HeroAttackedVictim=true;g.AbilityAt=2;b.Main();ok=ok&&Balanced(g)&&b.ParentClass.BullyRanOff&&g.Cutscenes==2&&g.Scenes[1]=="CS_OAKVALEINTRO_BULLYRUNDUMMY";}
 {GameStub g;Bully b(g);g.AbilityAt=g.HealAt=2;b.Main();ok=ok&&Balanced(g)&&b.HitsTaken==0&&g.BarAdds==0;}
 {GameStub g;Bully b(g);g.NormalAt=g.AbilityAt=g.HealAt=2;b.Main();ok=ok&&Balanced(g)&&b.HitsTaken==1&&g.BarAdds==1;}
 {GameStub g;Bully b(g);g.Near=true;g.TerminateAt=3;b.Main();ok=ok&&Balanced(g)&&b.SpokenOnFirstProximity&&b.ParentClass.VictimShake&&b.IntimidateSpeechLoop==20&&Has(g,"TEXT_QST_048_BULLY_SCRMSG_INTIMIDATING_10");}
 {GameStub g;Bully b(g);g.TerminateAt=1;b.Main();ok=ok&&Balanced(g)&&g.Lines.empty();}
 {GameStub g;Bully b(g);g.Talks.push_back(2);g.TaskFrames=3;g.TerminateAt=3;b.Main();ok=ok&&Balanced(g)&&!b.DoneIntro&&g.Lines.size()==1;}
 {GameStub g;Bully b(g);g.StartAt=9;g.TerminateAt=3;b.Main();ok=ok&&Balanced(g)&&g.Lines.empty();}
 ok=ok&&Strings==0&&Things==0&&Handles==0&&Movies==0;std::printf(ok?"00dbb310_TEST PASS\n":"00dbb310_TEST FAIL\n");return ok?0:1;
}