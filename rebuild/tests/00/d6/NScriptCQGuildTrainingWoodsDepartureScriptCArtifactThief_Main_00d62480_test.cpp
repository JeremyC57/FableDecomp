// Executes the complete reconstructed Main body with deterministic callee stubs.
#include <cstdio>
#include <cstring>
#include <cstddef>
static char logText[16384];
static void Log(const char* s) { std::strcat(logText,s); std::strcat(logText,";"); }
class C3DVector { public: float X,Y,Z; };
struct GameStub;
static GameStub* Current;
static int Frames, TimerValue, Resources;
class CScriptThing { public:
    unsigned Normal, Ability, Heal, Talk;
    bool MsgIsHitBy(const char*)const { return Frames<31 && ((Normal>>Frames)&1u)!=0; }
    bool MsgIsHitByAnySpecialAbilityFrom(const char*)const { return Frames<31 && ((Ability>>Frames)&1u)!=0; }
    bool MsgIsHitBySpecialAbilityFrom(int,const char*)const { return Frames<31 && ((Heal>>Frames)&1u)!=0; }
    bool MsgIsTalkedToBy(const char*)const { return Frames<31 && ((Talk>>Frames)&1u)!=0; }
    const C3DVector& GetPos()const { static C3DVector v={0,0,0};return v; }
};
struct GameStub {
    int TerminateAt, Gold, Given, Removed, Paid, Answer, TaskUntil;
    bool Near;
    CScriptThing Hero;
    void NewScriptFrame(){++Frames;if(TimerValue>0)--TimerValue;}
    bool StartScriptingEntity(const CScriptThing&,class CScriptGameResourceObjectScriptedThingBase&,int){return true;}
    const CScriptThing& GetHero(){return Hero;}
    void EntitySetAsKillable(const CScriptThing&,bool,bool){}
    void SetThingHasInformation(const CScriptThing&,bool,bool,bool){}
    long AddNewConversation(const CScriptThing&,bool,bool){return 1;}
    void AddPersonToConversation(long,const CScriptThing&){}
    void EntitySetFacingAngleTowardsThing(const CScriptThing&,const CScriptThing&,bool){}
    void AddLineToConversation(long,const char* line,bool,const CScriptThing&,const CScriptThing&){Log(line);}
    void StartMovieSequence(const char*,class CScriptGameResourceObjectMovieBase&){}
    float GetHealth(const CScriptThing&){return 1.0f;}
    void GiveHeroObject(const char*,long,bool){++Given;Log("lamp");}
    void RemoveItemFromContainer(const CScriptThing&,const char*){++Removed;}
    void GiveHeroYesNoQuestion(const char*,const char*,const char*,const char*,bool){}
    long MsgIsQuestionAnsweredYesOrNo(){return Answer;}
    long GetHeroGold(){return Gold;}
    void GiveHeroGold(long n){Gold+=(int)n;}
    void EntityGiveGold(const CScriptThing&,long n){Paid+=(int)n;}
    CScriptThing GetThingWithScriptName(const char*){CScriptThing t={0,0,0,0};return t;}
};
class CTimer { public:
    CTimer(){++Resources;}~CTimer(){--Resources;}
    CTimer& operator=(long n){TimerValue=(int)n;return *this;}
    long GetTime()const{return TimerValue;}
};
class CScriptGameResourceObjectMovieBase { public: CScriptGameResourceObjectMovieBase(){++Resources;}~CScriptGameResourceObjectMovieBase(){--Resources;} };
struct CWideScreenMagicPauseEntities { CWideScreenMagicPauseEntities(GameStub&){++Resources;}~CWideScreenMagicPauseEntities(){--Resources;} };
class CScriptGameResourceObjectScriptedThingBase { public:
    CScriptGameResourceObjectScriptedThingBase(){++Resources;}~CScriptGameResourceObjectScriptedThingBase(){--Resources;}
    bool IsValid()const{return false;}void Release(){}
    void PlayAnimation(const char*,bool,bool,bool,bool,bool,bool,bool){}
    CScriptThing GetScriptThing()const{CScriptThing t={0,0,0,0};return t;}
    void Speak(const CScriptThing&,const char* line,int,bool,bool,bool){Log(line);}
    bool IsPerformingScriptTask()const{return Frames<Current->TaskUntil;}
    void MoveToPosition(const C3DVector&,float,int type,bool,bool){Current->TaskUntil=Frames+3;Log(type==1?"run":"walk");}
};
struct CIsThingConscious { CIsThingConscious(const CScriptThing&){} };
namespace NGlobalConsole { const bool GDefaultToUsingPhysicsInCutsceneAnimations=false; }
struct ScriptDef { unsigned GUI_LampCost; } Def={100};
static ScriptDef* pGlobalScriptDef=&Def;
enum {SCRIPT_AI_PRIORITY_HIGHEST=4,GROUP_SELECT_NONE=0,ENTITY_MOVE_WALK=0,ENTITY_MOVE_RUN=1,HERO_ABILITY_HEAL_LIFE_SPELL=14};
static bool IsDistanceBetweenThingsUnder(const CScriptThing&,const CScriptThing&,float){return Current->Near;}
static void FadeOutAndKillEntity(const CScriptThing&,float,bool,bool){Log("fade");}
struct Thief {
    GameStub* Game;CScriptThing Me;bool HoldingArtifact,AlreadyTalkedTo,NotAttacked;
    void SetPredicate(const CIsThingConscious&){}
    bool IsActiveThreadTerminating()const{return Frames>=Game->TerminateAt;}
    void Main();
};
void Thief::Main()

{
    // Fixture adapter: bind the original reference to the configured dependency.
    ::GameStub& Game = *this->Game;
{
    SetPredicate(CIsThingConscious(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game.StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    Game.EntitySetAsKillable(Me, false, true);
    Game.SetThingHasInformation(Me, false, true, false);
    HoldingArtifact = true;
    AlreadyTalkedTo = false;
    NotAttacked = true;
    CTimer CommentTimer;
    CommentTimer = 0;
    while (HoldingArtifact && NotAttacked) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (IsDistanceBetweenThingsUnder(Game.GetHero(), Me, 5.5f) && CommentTimer.GetTime() <= 0) {
            if (IsActiveThreadTerminating()) return;
            long Conversation = Game.AddNewConversation(Me, false, false);
            Game.AddPersonToConversation(Conversation, Game.GetHero());
            if (!AlreadyTalkedTo) {
                if (IsActiveThreadTerminating()) return;
                Game.EntitySetFacingAngleTowardsThing(Me, Game.GetHero(), false);
                seh_me.PlayAnimation("ST_WAVE_SPECIAL_02", false, false, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false);
                Game.AddLineToConversation(Conversation, "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_COMMENT_FIRST", false, Me, Game.GetHero());
            } else {
                if (IsActiveThreadTerminating()) return;
                Game.EntitySetFacingAngleTowardsThing(Me, Game.GetHero(), false);
                Game.AddLineToConversation(Conversation, "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_COMMENT_SECOND", false, Me, Game.GetHero());
            }
            CommentTimer = 10;
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return;
            NotAttacked = false;
            if (AlreadyTalkedTo) {
                if (IsActiveThreadTerminating()) return;
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game.GetHero(), "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_HIT", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                }
                Game.GiveHeroObject("OBJECT_HAND_LAMP", -1, false);
                Game.RemoveItemFromContainer(Me, "OBJECT_HAND_LAMP");
            } else {
                if (IsActiveThreadTerminating()) return;
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game.GetHero(), "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_PRE_HIT", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                }
                Game.GiveHeroObject("OBJECT_HAND_LAMP", -1, false);
                Game.RemoveItemFromContainer(Me, "OBJECT_HAND_LAMP");
            }
            Game.EntitySetAsKillable(Me, true, true);
            seh_me.MoveToPosition(Game.GetThingWithScriptName("ArtifactThiefRunMarker").GetPos(), 1.0f, ENTITY_MOVE_RUN, false, true);
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return;
            if (HoldingArtifact) {
                if (IsActiveThreadTerminating()) return;
                if (!AlreadyTalkedTo) {
                    if (IsActiveThreadTerminating()) return;
                    AlreadyTalkedTo = true;
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game.StartMovieSequence("", speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                    if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game.GetHero(), "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_CHAT", GROUP_SELECT_NONE, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return;
                        }
                        if (IsActiveThreadTerminating()) return;
                    }
                    Game.GiveHeroYesNoQuestion("TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
                    long answer = Game.MsgIsQuestionAnsweredYesOrNo();
                    while (answer < 0) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                        answer = Game.MsgIsQuestionAnsweredYesOrNo();
                    }
                    if (IsActiveThreadTerminating()) return;
                    if (answer == 1) {
                        if (IsActiveThreadTerminating()) return;
                        if (Game.GetHeroGold() < pGlobalScriptDef->GUI_LampCost) {
                            if (IsActiveThreadTerminating()) return;
                            if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                                seh_me.Speak(Game.GetHero(), "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION_NO_GOLD", GROUP_SELECT_NONE, false, true, false);
                                while (seh_me.IsPerformingScriptTask()) {
                                    Game.NewScriptFrame();
                                    if (IsActiveThreadTerminating()) return;
                                }
                                if (IsActiveThreadTerminating()) return;
                            }
                        } else {
                            if (IsActiveThreadTerminating()) return;
                            if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                                seh_me.Speak(Game.GetHero(), "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION_YES", GROUP_SELECT_NONE, false, true, false);
                                while (seh_me.IsPerformingScriptTask()) {
                                    Game.NewScriptFrame();
                                    if (IsActiveThreadTerminating()) return;
                                }
                                if (IsActiveThreadTerminating()) return;
                            }
                            Game.GiveHeroObject("OBJECT_HAND_LAMP", -1, false);
                            Game.RemoveItemFromContainer(Me, "OBJECT_HAND_LAMP");
                            Game.GiveHeroGold((long)-pGlobalScriptDef->GUI_LampCost);
                            Game.EntityGiveGold(Me, (long)pGlobalScriptDef->GUI_LampCost);
                            HoldingArtifact = false;
                            seh_me.MoveToPosition(Game.GetThingWithScriptName("ArtifactThiefRunMarker").GetPos(), 1.0f, ENTITY_MOVE_WALK, false, true);
                        }
                    } else {
                        if (IsActiveThreadTerminating()) return;
                        if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                            seh_me.Speak(Game.GetHero(), "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION_NO", GROUP_SELECT_NONE, false, true, false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return;
                            }
                            if (IsActiveThreadTerminating()) return;
                        }
                    }
                } else {
                    if (IsActiveThreadTerminating()) return;
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game.StartMovieSequence("", speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                    if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game.GetHero(), "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_RETURN", GROUP_SELECT_NONE, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return;
                        }
                        if (IsActiveThreadTerminating()) return;
                    }
                    Game.GiveHeroYesNoQuestion("TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_RETURN_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
                    long answer = Game.MsgIsQuestionAnsweredYesOrNo();
                    while (answer < 0) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                        answer = Game.MsgIsQuestionAnsweredYesOrNo();
                    }
                    if (IsActiveThreadTerminating()) return;
                    if (answer == 1) {
                        if (IsActiveThreadTerminating()) return;
                        if (Game.GetHeroGold() < pGlobalScriptDef->GUI_LampCost) {
                            if (IsActiveThreadTerminating()) return;
                            if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                                seh_me.Speak(Game.GetHero(), "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION_NO_GOLD", GROUP_SELECT_NONE, false, true, false);
                                while (seh_me.IsPerformingScriptTask()) {
                                    Game.NewScriptFrame();
                                    if (IsActiveThreadTerminating()) return;
                                }
                                if (IsActiveThreadTerminating()) return;
                            }
                        } else {
                            if (IsActiveThreadTerminating()) return;
                            if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                                seh_me.Speak(Game.GetHero(), "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION_YES", GROUP_SELECT_NONE, false, true, false);
                                while (seh_me.IsPerformingScriptTask()) {
                                    Game.NewScriptFrame();
                                    if (IsActiveThreadTerminating()) return;
                                }
                                if (IsActiveThreadTerminating()) return;
                            }
                            Game.GiveHeroObject("OBJECT_HAND_LAMP", -1, false);
                            Game.RemoveItemFromContainer(Me, "OBJECT_HAND_LAMP");
                            Game.GiveHeroGold((long)-pGlobalScriptDef->GUI_LampCost);
                            Game.EntityGiveGold(Me, (long)pGlobalScriptDef->GUI_LampCost);
                            HoldingArtifact = false;
                            seh_me.MoveToPosition(Game.GetThingWithScriptName("ArtifactThiefRunMarker").GetPos(), 1.0f, ENTITY_MOVE_WALK, false, true);
                        }
                    } else {
                        if (IsActiveThreadTerminating()) return;
                        CScriptGameResourceObjectMovieBase no_movie;
                        Game.StartMovieSequence("", no_movie);
                        CWideScreenMagicPauseEntities PauseNo(Game);
                        if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                            seh_me.Speak(Game.GetHero(), "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_RETURN_QUESTION_NO", GROUP_SELECT_NONE, false, true, false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return;
                            }
                            if (IsActiveThreadTerminating()) return;
                        }
                    }
                }
            }
        }
    }
    if (IsActiveThreadTerminating()) return;
    while (seh_me.IsPerformingScriptTask()) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return;
            if (NotAttacked) {
                if (IsActiveThreadTerminating()) return;
                long Conversation = Game.AddNewConversation(Me, false, false);
                Game.AddPersonToConversation(Conversation, Game.GetHero());
                Game.AddLineToConversation(Conversation, "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_ATTACK", false, Me, Game.GetHero());
                NotAttacked = false;
                Game.EntitySetAsKillable(Me, true, true);
                seh_me.MoveToPosition(Game.GetThingWithScriptName("ArtifactThiefRunMarker").GetPos(), 1.0f, ENTITY_MOVE_RUN, false, true);
            }
        }
    }
    if (IsActiveThreadTerminating()) return;
    FadeOutAndKillEntity(Me, 1.0f, true, true);
}
}
static void Reset(GameStub& g,Thief& t) {
    std::memset(&g,0,sizeof g);std::memset(&t,0,sizeof t);Frames=0;TimerValue=0;Resources=0;logText[0]=0;
    g.TerminateAt=20;g.Answer=1;g.Gold=150;t.Game=&g;Current=&g;
}
static bool Has(const char* s){return std::strstr(logText,s)!=0;}
int main(){bool ok=true;GameStub g;Thief t;
    Reset(g,t);t.Me.Talk=1u<<2;t.Main();ok=ok&&g.Given==1&&g.Removed==1&&g.Gold==50&&g.Paid==100&&!t.HoldingArtifact&&Has("walk;")&&Has("fade;")&&Resources==0;
    Reset(g,t);g.Gold=50;t.Me.Talk=(1u<<2)|(1u<<4);t.Main();ok=ok&&g.Given==0&&g.Gold==50&&t.HoldingArtifact&&Has("NO_GOLD;")&&Has("RETURN;")&&!Has("fade;")&&Resources==0;
    Reset(g,t);g.Answer=0;t.Me.Talk=(1u<<2)|(1u<<4);t.Main();ok=ok&&g.Given==0&&Has("RETURN_QUESTION_NO;")&&Resources==0;
    Reset(g,t);g.TerminateAt=4;t.Me.Ability=1u<<2;t.Me.Heal=1u<<2;t.Main();ok=ok&&g.Given==0&&t.NotAttacked&&t.HoldingArtifact&&!Has("run;")&&Resources==0;
    Reset(g,t);t.Me.Normal=1u<<2;t.Me.Ability=1u<<2;t.Me.Heal=1u<<2;t.Main();ok=ok&&g.Given==1&&!t.NotAttacked&&Has("PRE_HIT;")&&Has("run;")&&Has("fade;")&&Resources==0;
    Reset(g,t);t.Me.Talk=1u<<2;t.Me.Normal=1u<<4;t.Main();ok=ok&&g.Given==1&&g.Paid==100&&!t.NotAttacked&&Has("ATTACK;")&&Has("run;")&&Has("fade;")&&Resources==0;
    Reset(g,t);g.TerminateAt=3;t.Me.Talk=1u<<2;t.Main();ok=ok&&g.Given==1&&!Has("fade;")&&Resources==0;
    Reset(g,t);g.TerminateAt=1;t.Main();ok=ok&&g.Given==0&&logText[0]==0&&Resources==0;
    std::printf(ok?"00d62480_TEST PASS\n":"00d62480_TEST FAIL\n");return ok?0:1;
}