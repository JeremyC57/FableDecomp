// Complete reconstructed Main body, with deterministic callee stubs.
#include <cstdio>
#include <cstring>
static char logText[16384];
static void Log(const char* s){std::strcat(logText,s);std::strcat(logText,";");}
static int Frames,Resources;
class C3DVector { public:float X,Y,Z;};
class CCharString { public: const char* Text;CCharString():Text(""){}bool operator==(const char* x)const{return std::strcmp(Text,x)==0;}bool operator!=(const char* x)const{return !(*this==x);} };
struct GameStub;static GameStub* Current;
class CScriptThing { public:
    bool Hero;
    CScriptThing(bool h=false):Hero(h){}
    bool MsgIsTalkedToBy(const char*)const;
    bool MsgIsPresentedWithItem(CCharString&)const;
    bool MsgIsHitBy(const char*)const;
    bool MsgIsHitByAnySpecialAbilityFrom(const char*)const;
    bool MsgIsHitBySpecialAbilityFrom(int,const char*)const;
    const C3DVector& GetPos()const{static C3DVector v={0,0,0};return v;}
};
struct GameStub {
    int TerminateAt,TalkAt,PresentAt,NormalAt,AbilityAt,HealAt,TaskUntil,Answer,Removed,Allies,Given;
    bool HasTeddy,Near;const char* Present;
    CScriptThing Hero;
    void NewScriptFrame(){++Frames;}
    bool StartScriptingEntity(const CScriptThing&,class CScriptGameResourceObjectScriptedThingBase&,int){return true;}
    const CScriptThing& GetHero(){return Hero;}
    bool IsObjectInThingsPossession(const char*,const CScriptThing&){return HasTeddy;}
    void StartMovieSequence(const char*,class CScriptGameResourceObjectMovieBase&){}
    float GetHealth(const CScriptThing&){return 1.0f;}
    void GiveHeroYesNoQuestion(const char*,const char*,const char*,const char*,bool){}
    long MsgIsQuestionAnsweredYesOrNo(){return Answer;}
    CScriptThing GetThingWithScriptName(const char*){return CScriptThing();}
    long AddNewConversation(const CScriptThing&,bool,bool){return 1;}
    void AddPersonToConversation(long,const CScriptThing&){}
    void AddLineToConversation(long,const char* line,bool,const CScriptThing&,const CScriptThing&){Log(line);}
    bool IsCameraPosOnScreen(const C3DVector&){return Frames<TaskUntil;}
    void RemoveThing(const CScriptThing&,bool,bool){++Removed;TerminateAt=Frames+1;}
    void ClearThingHasInformation(const CScriptThing&){}
    void EntitySetThingAsAllyOfThing(const CScriptThing&,const CScriptThing&){++Allies;}
};
static bool BitEvent(int at){return Frames==at;}
bool CScriptThing::MsgIsTalkedToBy(const char*)const{return BitEvent(Current->TalkAt);}
bool CScriptThing::MsgIsPresentedWithItem(CCharString& s)const{s.Text=Current->Present;return BitEvent(Current->PresentAt);}
bool CScriptThing::MsgIsHitBy(const char*)const{return BitEvent(Current->NormalAt);}
bool CScriptThing::MsgIsHitByAnySpecialAbilityFrom(const char*)const{return BitEvent(Current->AbilityAt);}
bool CScriptThing::MsgIsHitBySpecialAbilityFrom(int,const char*)const{return BitEvent(Current->HealAt);}
class CScriptGameResourceObjectMovieBase { public:CScriptGameResourceObjectMovieBase(){++Resources;}~CScriptGameResourceObjectMovieBase(){--Resources;} };
struct CWideScreenMagicPauseEntities {CWideScreenMagicPauseEntities(GameStub&){++Resources;}~CWideScreenMagicPauseEntities(){--Resources;} };
class CScriptGameResourceObjectScriptedThingBase { public:
    CScriptGameResourceObjectScriptedThingBase(){++Resources;}~CScriptGameResourceObjectScriptedThingBase(){--Resources;}
    bool IsValid()const{return true;}void Release(){}
    CScriptThing GetScriptThing()const{return CScriptThing();}
    void Speak(const CScriptThing&,const char* s,int,bool,bool,bool){Log(s);}
    bool IsPerformingScriptTask()const{return false;}
    void MoveToThing(const CScriptThing&,float,int,bool,bool,bool,bool){Current->TaskUntil=Frames+2;Log("run");}
};
struct CIsThingConscious {CIsThingConscious(const CScriptThing&){} };
struct TRetVoid {void Return()const{} };
struct CQ_NewOakValeIntroScript {
    enum EBadDeeds { Dummy=0 };
    bool SpokeAboutFindingTeddy,TeddyRuined;int BadDeeds,GoodDeeds;
    void AddBadDeed(EBadDeeds){++BadDeeds;}void AddGoodDeed(){++GoodDeeds;}
};
struct MasterData {const char* TeddySolution;};
enum {SCRIPT_AI_PRIORITY_HIGHEST=4,GROUP_SELECT_NONE=0,ENTITY_MOVE_RUN=1,HERO_ABILITY_HEAL_LIFE_SPELL=14};
static bool IsDistanceBetweenThingsUnder(const CScriptThing&,const CScriptThing&,float){return Current->Near;}
static bool IsDistanceBetweenThingsOver(const CScriptThing&,const CScriptThing&,float){return false;}
struct Girl {
    GameStub* Game;CScriptThing Me;CQ_NewOakValeIntroScript ParentClass;MasterData* PMasterData;
    bool DoneIntro,FoundTeddy,SpokeAboutFindingTeddy,HeroHitMe;
    void SetPredicate(const CIsThingConscious&){}
    bool IsActiveThreadTerminating()const{return Frames>=Game->TerminateAt;}
    void GivenTeddy(){FoundTeddy=true;Game->HasTeddy=false;++Game->Given;ParentClass.AddGoodDeed();PMasterData->TeddySolution="B";Log("GivenTeddy");}
    void Main();
};
void Girl::Main()

{
    // Fixture adapter: bind the original reference to the configured dependency.
    ::GameStub& Game = *this->Game;
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingConscious(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    CScriptThing Bully=Game.GetThingWithScriptName("NOVI_Bully");
    for (; !false; Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        CCharString item;
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO") && Game.IsObjectInThingsPossession("OBJECT_TEDDY_BEAR_UNGIVEABLE",Game.GetHero())) {
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
            if (!DoneIntro && HeroHitMe) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_048_TEDDYGIRL_PLEA_POST_BEATEN",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                DoneIntro=true;
            }
            else if (!DoneIntro) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_048_TEDDYGIRL_LOST_TEDDY",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                DoneIntro=true;
            }
            Game.GiveHeroYesNoQuestion("TEXT_QST_048_GIVE_TEDDY_TO_GIRL","TEXT_OBJECT_HERO_ANSWER_YES","TEXT_OBJECT_HERO_ANSWER_NO","",true);
            long answer=Game.MsgIsQuestionAnsweredYesOrNo();
            while (answer<0) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                answer=Game.MsgIsQuestionAnsweredYesOrNo();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (answer==1) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_048_TEDDYGIRL_FOUND_TEDDY",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                GivenTeddy();
            }
            else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_048_TEDDYGIRL_REPEAT_PLEA",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
        }
        else if (Me.MsgIsPresentedWithItem(item) && item=="OBJECT_TEDDY_BEAR_UNGIVEABLE") {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            DoneIntro=true;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                seh_me.Speak(Game.GetHero(),"TEXT_QST_048_TEDDYGIRL_FOUND_TEDDY",GROUP_SELECT_NONE,false,true,false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            GivenTeddy();
        }
        else if (Me.MsgIsPresentedWithItem(item) && item!="OBJECT_TEDDY_BEAR_UNGIVEABLE") {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (!FoundTeddy) {
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
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_048_TEDDYGIRL_DONT_WANT",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
        }
        if (ParentClass.SpokeAboutFindingTeddy && IsDistanceBetweenThingsUnder(Me,Bully,10.0f)) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            long conv=Game.AddNewConversation(Me,false,false);
            Game.AddPersonToConversation(conv,Game.GetHero());
            Game.AddLineToConversation(conv,"TEXT_QST_048_TEDDYGIRL_TEDDY_RUINED",false,Me,Game.GetHero());
            PMasterData->TeddySolution="C";
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptThing Wife=Game.GetThingWithScriptName("NOVI_AffairWife");
            seh_me.MoveToThing(Wife,3.0f,ENTITY_MOVE_RUN,false,false,false,true);
            while (Game.IsCameraPosOnScreen(Me.GetPos()) && !IsDistanceBetweenThingsOver(Me,Game.GetHero(),20.0f)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.RemoveThing(Me,false,true);
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
            if (!FoundTeddy && HeroHitMe) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_048_TEDDYGIRL_PLEA_POST_BEATEN",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                DoneIntro=true;
            }
            else if (!DoneIntro) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_048_TEDDYGIRL_LOST_TEDDY",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                DoneIntro=true;
            }
            else if (FoundTeddy) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_048_TEDDYGIRL_REPEAT_FOUND",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
            else if (ParentClass.TeddyRuined) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_048_TEDDYGIRL_BAD_FEELING",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                Game.ClearThingHasInformation(Me);
            }
            else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_048_TEDDYGIRL_REPEAT_PLEA",GROUP_SELECT_NONE,false,true,false);
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
            Game.EntitySetThingAsAllyOfThing(Me,Game.GetHero());
            Game.EntitySetThingAsAllyOfThing(Game.GetHero(),Me);
            HeroHitMe=true;
            ParentClass.AddBadDeed((CQ_NewOakValeIntroScript::EBadDeeds)2);
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
                seh_me.Speak(Game.GetHero(),"TEXT_QST_048_TEDDYGIRL_DONT_HIT",GROUP_SELECT_NONE,false,true,false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
        }
        if (seh_me.IsValid()) seh_me.Release();
    }
}
}

static void Reset(GameStub& g,Girl& t,MasterData& d) {
    // The stubs have no ownership fields; initialise each test's event inputs explicitly.
    Frames=0;Resources=0;logText[0]=0;g.TerminateAt=12;g.TalkAt=g.PresentAt=g.NormalAt=g.AbilityAt=g.HealAt=-1;g.TaskUntil=0;g.Answer=1;g.Removed=g.Allies=g.Given=0;g.HasTeddy=false;g.Near=false;g.Present="";g.Hero.Hero=true;
    t.Game=&g;t.PMasterData=&d;t.DoneIntro=t.FoundTeddy=t.SpokeAboutFindingTeddy=t.HeroHitMe=false;t.ParentClass.SpokeAboutFindingTeddy=t.ParentClass.TeddyRuined=false;t.ParentClass.BadDeeds=t.ParentClass.GoodDeeds=0;d.TeddySolution="";Current=&g;
}
static bool Has(const char* s){return std::strstr(logText,s)!=0;}
int main(){bool ok=true;GameStub g;Girl t;MasterData d;
    Reset(g,t,d);g.NormalAt=2;g.TalkAt=4;g.HasTeddy=true;t.Main();ok=ok&&t.HeroHitMe&&t.FoundTeddy&&t.ParentClass.BadDeeds==1&&t.ParentClass.GoodDeeds==1&&g.Given==1&&g.Allies==2&&Has("PLEA_POST_BEATEN;")&&Has("REPEAT_FOUND;")&&Resources==0;
    Reset(g,t,d);g.PresentAt=2;g.Present="OBJECT_APPLE";t.Main();ok=ok&&!t.FoundTeddy&&Has("DONT_WANT;")&&Resources==0;
    Reset(g,t,d);g.PresentAt=2;g.Present="OBJECT_TEDDY_BEAR_UNGIVEABLE";g.TalkAt=4;t.Main();ok=ok&&t.FoundTeddy&&t.DoneIntro&&g.Given==1&&Has("REPEAT_FOUND;")&&std::strcmp(d.TeddySolution,"B")==0&&Resources==0;
    Reset(g,t,d);g.TalkAt=2;g.HasTeddy=true;g.Answer=0;t.Main();ok=ok&&!t.FoundTeddy&&g.Given==0&&Has("LOST_TEDDY;")&&Has("REPEAT_PLEA;")&&Resources==0;
    Reset(g,t,d);t.ParentClass.SpokeAboutFindingTeddy=true;g.Near=true;t.Main();ok=ok&&g.Removed==1&&Has("TEDDY_RUINED;")&&Has("run;")&&std::strcmp(d.TeddySolution,"C")==0&&Resources==0;
    Reset(g,t,d);g.AbilityAt=g.HealAt=2;t.Main();ok=ok&&!t.HeroHitMe&&t.ParentClass.BadDeeds==0&&g.Allies==0&&Resources==0;
    Reset(g,t,d);g.NormalAt=g.AbilityAt=g.HealAt=2;t.Main();ok=ok&&t.HeroHitMe&&t.ParentClass.BadDeeds==1&&g.Allies==2&&Has("DONT_HIT;")&&Resources==0;
    Reset(g,t,d);g.TerminateAt=1;t.Main();ok=ok&&logText[0]==0&&Resources==0;
    std::printf(ok?"00daf080_TEST PASS\n":"00daf080_TEST FAIL\n");return ok?0:1;
}