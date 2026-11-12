// Execute the selected Main body with deterministic game interfaces and resource counters.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <cassert>
static int Strings=0,Timers=0,Movies=0;
class CCharString { public:
 std::string Text;
 CCharString():Text(){++Strings;} CCharString(const char* p):Text(p){++Strings;}
 CCharString(const CCharString& p):Text(p.Text){++Strings;} ~CCharString(){--Strings;}
 CCharString& operator=(const char* p){Text=p;return *this;}
 CCharString& operator=(const CCharString& p){Text=p.Text;return *this;}
 bool operator==(const char* p)const{return Text==p;}
 operator const char*()const{return Text.c_str();}
};
struct GameStub;static GameStub* Current=0;
class CScriptThing { public:
 int Id;CScriptThing(int i=0):Id(i){}
 bool MsgExpressionPerformedTo(CCharString&)const;bool MsgIsTalkedToBy(const char*)const;
};
class CScriptGameResourceObjectScriptedThingBase { public:
 bool Valid;CScriptGameResourceObjectScriptedThingBase():Valid(false){}
 bool IsValid()const{return Valid;}void Release();
 CScriptThing GetScriptThing()const{return CScriptThing(1);}
 void Speak(const CScriptThing&,const char*,int,bool,bool,bool);
 void PlayAnimation(const char*,bool,bool,bool,bool,bool,bool,bool);
 bool IsPerformingScriptTask()const;
};
class CScriptGameResourceObjectMovieBase{ public:CScriptGameResourceObjectMovieBase(){++Movies;}~CScriptGameResourceObjectMovieBase(){--Movies;}};
struct GameStub {
 int Frame,StopAt,Checks,StopCheck,StartDelays,TaskFrames,TaskUntil,PauseDepth,TimerValue,TimerFrame,LastTimerSet;
 int Removed,Releases,StartedMovies,Pushable,Info,Conversations,Morality,Leaves,Predicates;
 bool Near,Healthy,Talk,ExpressionConsumed,TalkConsumed,CancelOnRelease,CancelOnLeave,Cancelled;
 const char* Expression;std::vector<std::string> Lines;std::vector<std::string> ConversationLines;std::vector<std::string> Animations;
 GameStub():Frame(0),StopAt(3),Checks(0),StopCheck(0),StartDelays(0),TaskFrames(0),TaskUntil(0),PauseDepth(0),TimerValue(0),TimerFrame(0),LastTimerSet(0),Removed(0),Releases(0),StartedMovies(0),Pushable(0),Info(0),Conversations(0),Morality(0),Leaves(0),Predicates(0),Near(false),Healthy(true),Talk(false),ExpressionConsumed(false),TalkConsumed(false),CancelOnRelease(false),CancelOnLeave(false),Cancelled(false),Expression(0){Current=this;}
 bool T(){++Checks;return Cancelled||Frame>=StopAt||(StopCheck&&Checks>=StopCheck);}
 void NewScriptFrame(){++Frame;assert(Frame<1000);}
 bool StartScriptingEntity(const CScriptThing& t,CScriptGameResourceObjectScriptedThingBase& h,int p){assert(t.Id==1&&p==4);if(StartDelays){--StartDelays;return false;}h.Valid=true;return true;}
 void RemoveThing(const CScriptThing& t,bool a,bool b){assert(t.Id==1&&!a&&b);++Removed;}
 void SetIsPushableByHero(CScriptThing t,bool b){assert(t.Id==1&&!b);++Pushable;}
 void SetThingHasInformation(const CScriptThing& t,bool a,bool b,bool c){assert(t.Id==1&&!a&&!b&&!c);++Info;}
 CScriptThing GetHero(){return CScriptThing(2);}
 long AddNewConversation(const CScriptThing& t,bool a,bool b){assert(t.Id==1&&!a&&!b);return ++Conversations;}
 void AddPersonToConversation(long c,const CScriptThing& t){assert(c>0&&t.Id==2);}
 void AddLineToConversation(long c,const CCharString& text,bool b,const CScriptThing& a,const CScriptThing& d){assert(c>0&&!b&&a.Id==1&&d.Id==2);ConversationLines.push_back(text.Text);}
 void StartMovieSequence(const CCharString& s,CScriptGameResourceObjectMovieBase&){assert(s.Text.empty());++StartedMovies;}
 float GetHealth(const CScriptThing& t){assert(t.Id==1);return Healthy?1.0f:0.0f;}
 void GiveHeroMorality(long n){Morality+=n;}
};
bool CScriptThing::MsgExpressionPerformedTo(CCharString& s)const{assert(Id==1);if(Current->Expression&&!Current->ExpressionConsumed){Current->ExpressionConsumed=true;s=Current->Expression;return true;}return false;}
bool CScriptThing::MsgIsTalkedToBy(const char* s)const{assert(Id==1&&!std::strcmp(s,"SCRIPT_NAME_HERO"));if(Current->Talk&&!Current->TalkConsumed){Current->TalkConsumed=true;return true;}return false;}
void CScriptGameResourceObjectScriptedThingBase::Release(){Valid=false;++Current->Releases;if(Current->CancelOnRelease)Current->Cancelled=true;}
void CScriptGameResourceObjectScriptedThingBase::Speak(const CScriptThing& h,const char* t,int group,bool a,bool b,bool c){assert(Valid&&h.Id==2&&group==0&&!a&&b&&!c);Current->Lines.push_back(t);Current->TaskUntil=Current->Frame+Current->TaskFrames;}
void CScriptGameResourceObjectScriptedThingBase::PlayAnimation(const char* s,bool a,bool b,bool c,bool d,bool,bool e,bool f){assert(!a&&!b&&c&&!d&&!e&&!f);Current->Animations.push_back(s);}
bool CScriptGameResourceObjectScriptedThingBase::IsPerformingScriptTask()const{return Current->Frame<Current->TaskUntil;}
class CTimer { public:
 CTimer(){++Timers;Current->TimerValue=0;Current->TimerFrame=Current->Frame;}~CTimer(){--Timers;}
 CTimer& operator=(long n){Current->TimerValue=(int)n;Current->LastTimerSet=(int)n;Current->TimerFrame=Current->Frame;return *this;}
 long GetTime()const{int n=Current->TimerValue-(Current->Frame-Current->TimerFrame);return n>0?n:0;}
};
struct CWideScreenMagicPauseEntities{GameStub* G;CWideScreenMagicPauseEntities(GameStub& g):G(&g){++G->PauseDepth;}~CWideScreenMagicPauseEntities(){--G->PauseDepth;}};
struct CIsThingAlive{CIsThingAlive(const CScriptThing& t){assert(t.Id==1);}};
struct TRetVoid{void Return(){}};
struct ScriptDef{long MadeWomanLaugh;};static ScriptDef Def={5};static ScriptDef* pGlobalScriptDef=&Def;
namespace NGlobalConsole{bool GDefaultToUsingPhysicsInCutsceneAnimations=false;}
enum{SCRIPT_AI_PRIORITY_HIGHEST=4,GROUP_SELECT_NONE=0};
static bool IsDistanceBetweenThingsUnder(const CScriptThing& a,const CScriptThing& b,float f){assert(a.Id==1&&b.Id==2&&f==15.0f);return Current->Near;}
struct Woman {
 GameStub* Game;CScriptThing Me;CScriptGameResourceObjectScriptedThingBase seh_me;
 struct Parent{bool LaughingWomanKilled;Parent():LaughingWomanKilled(false){}}ParentClass;
 bool GivenObject;long TimesMadeLaugh;
 Woman(GameStub& g):Game(&g),Me(1),GivenObject(false),TimesMadeLaugh(0){}
 void SetPredicate(const CIsThingAlive&){++Game->Predicates;}
 bool IsActiveThreadTerminating(){return Game->T();}
 void DoLeavingCutscene(){++Game->Leaves;if(Game->CancelOnLeave)Game->Cancelled=true;}
 void Main();
};

void Woman::Main()
{
    // Fixture adapter: bind the original reference to the configured dependency.
    ::GameStub& Game = *this->Game;
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    if (ParentClass.LaughingWomanKilled==true) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.RemoveThing(Me,false,true);
    }
    Game.SetIsPushableByHero(Me,false);
    if (!GivenObject) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.SetThingHasInformation(Me,false,false,false);
    }
    CCharString expression;
    CTimer timer;
    for (; !false; Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (timer.GetTime()==0 && IsDistanceBetweenThingsUnder(Me,Game.GetHero(),15.0f)) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            long conversation=Game.AddNewConversation(Me,false,false);
            Game.AddPersonToConversation(conversation,Game.GetHero());
            switch (TimesMadeLaugh) {
            case 0:
                Game.AddLineToConversation(conversation,"TEXT_QST_B10_LAUGHING_WOMAN_NOT_LAUGHED",false,Me,Game.GetHero());
                seh_me.PlayAnimation("STANDARD_TALK_SADNESS",false,false,true,false,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
                break;
            case 1:
                Game.AddLineToConversation(conversation,"TEXT_QST_B10_LAUGHING_WOMAN_LAUGHED_A_BIT",false,Me,Game.GetHero());
                seh_me.PlayAnimation("ST_OPINION_RIDICULE_SNIGGER",false,false,true,false,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
                Game.AddLineToConversation(conversation,"TEXT_QST_B10_LAUGHING_WOMAN_MADE_LAUGH_ONCE",false,Me,Game.GetHero());
                break;
            case 2:
                Game.AddLineToConversation(conversation,"TEXT_QST_B10_LAUGHING_WOMAN_NEARLY_HYSTERICAL",false,Me,Game.GetHero());
                seh_me.PlayAnimation("ST_OPINION_RIDICULE_POINT_AND_LAUGH",false,false,true,false,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
                Game.AddLineToConversation(conversation,"TEXT_QST_B10_LAUGHING_WOMAN_MADE_LAUGH_TWICE",false,Me,Game.GetHero());
                break;
            }
            timer=20;
        }
        if (Me.MsgExpressionPerformedTo(expression) && !GivenObject) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (expression=="EXPRESSION_PELVIC_THRUST" || expression=="EXPRESSION_COCK_A_DOODLE_DO" || expression=="EXPRESSION_CROTCH_GRAB" || expression=="EXPRESSION_KISS_MY_ASS" || expression=="EXPRESSION_FLAMENCO" || expression=="EXPRESSION_COSSACK" || expression=="EXPRESSION_AIR_GUITAR" || expression=="EXPRESSION_BALLET" || expression=="EXPRESSION_SATURDAY_NIGHT_FEVER" || expression=="EXPRESSION_TAP" || expression=="EXPRESSION_GIGGLE") {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ++TimesMadeLaugh;
                CCharString text("TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY");
                if (expression=="EXPRESSION_PELVIC_THRUST") {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    text="TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_VULGAR_ONCE";
                }
                else if (expression=="EXPRESSION_COCK_A_DOODLE_DO") {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    text="TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_COCK_ONCE";
                }
                else if (expression=="EXPRESSION_CROTCH_GRAB") {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    text="TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_CROTCH_ONCE";
                }
                else if (expression=="EXPRESSION_KISS_MY_ASS") {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    text="TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_KISSARSE_ONCE";
                }
                else if (expression=="EXPRESSION_FLAMENCO" || expression=="EXPRESSION_COSSACK" || expression=="EXPRESSION_AIR_GUITAR" || expression=="EXPRESSION_BALLET" || expression=="EXPRESSION_SATURDAY_NIGHT_FEVER" || expression=="EXPRESSION_TAP") {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    text="TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_DANCE_ONCE";
                }
                else if (expression=="EXPRESSION_GIGGLE") {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    text="TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_GIGGLE_ONCE";
                }
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                switch (TimesMadeLaugh) {
                case 1:
                    if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),text,GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    break;
                case 2:
                    if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_LAUGHING_WOMAN_MADE_LAUGH_TWICE",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    break;
                case 3:
                    DoLeavingCutscene();
                    GivenObject=true;
                    Game.GiveHeroMorality(pGlobalScriptDef->MadeWomanLaugh);
                    break;
                default:
                    if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_LAUGHING_WOMAN_MADE_LAUGH_ONCE",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    break;
                }
                timer=20;
            }
            else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CCharString text("TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY");
                if (expression=="EXPRESSION_SHIT") {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    text="TEXT_QST_B10_LAUGHING_WOMAN_RUDE";
                }
                else if (expression=="EXPRESSION_FLIRT") {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    text="TEXT_QST_B10_LAUGHING_WOMAN_FLIRT";
                }
                else if (expression=="EXPRESSION_HEROIC_STANCE") {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    text="TEXT_QST_B10_LAUGHING_WOMAN_HEROPOSE";
                }
                else if (expression=="EXPRESSION_VICTORY_PUMP") {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    text="TEXT_QST_B10_LAUGHING_WOMAN_ARMPUMP";
                }
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),text,GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO") && !GivenObject) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            switch (TimesMadeLaugh) {
            case 0:
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_LAUGHING_WOMAN_INTRO",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                break;
            case 1:
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_LAUGHING_WOMAN_ON_SPEAK_MADE_LAUGH_ONCE",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                break;
            case 2:
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_LAUGHING_WOMAN_ON_SPEAK_MADE_LAUGH_TWICE",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                break;
            }
            timer=timer.GetTime()+5;
        }
        if (GivenObject) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (seh_me.IsValid()) seh_me.Release();
            break;
        }
    }
    if (IsActiveThreadTerminating()) {
// Inline-budget: retail00ED0AD3 joins the called timer cleanup at00ED0AE8.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
        return;
#ifdef _MSC_VER
#pragma inline_depth()
#endif
    }
    Game.RemoveThing(Me,false,true);
// Inline-budget: retail00ED0AEC calls the timer destructor after removal; body-top cancellation expands it.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
}
}
#ifdef _MSC_VER
#pragma inline_depth()
#endif

static bool Balanced(const GameStub& g){return Strings==0&&Timers==0&&Movies==0&&g.PauseDepth==0;}
static bool Line(const GameStub& g,const char* s){return g.Lines.size()==1&&g.Lines[0]==s;}
int main(){bool ok=true;
 const char* funny[]={"EXPRESSION_PELVIC_THRUST","EXPRESSION_COCK_A_DOODLE_DO","EXPRESSION_CROTCH_GRAB","EXPRESSION_KISS_MY_ASS","EXPRESSION_FLAMENCO","EXPRESSION_COSSACK","EXPRESSION_AIR_GUITAR","EXPRESSION_BALLET","EXPRESSION_SATURDAY_NIGHT_FEVER","EXPRESSION_TAP","EXPRESSION_GIGGLE"};
 const char* category[]={"VULGAR","COCK","CROTCH","KISSARSE","DANCE","DANCE","DANCE","DANCE","DANCE","DANCE","GIGGLE"};
 for(int n=0;n<11;++n){GameStub g;Woman w(g);g.Expression=funny[n];w.Main();std::string want=std::string("TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_")+category[n]+"_ONCE";ok=ok&&Balanced(g)&&w.TimesMadeLaugh==1&&!w.GivenObject&&g.Lines.size()==1&&g.Lines[0]==want&&g.Leaves==0&&g.Morality==0;}
 const char* rude[]={"EXPRESSION_SHIT","EXPRESSION_FLIRT","EXPRESSION_HEROIC_STANCE","EXPRESSION_VICTORY_PUMP","UNKNOWN"};
 const char* rudeLines[]={"TEXT_QST_B10_LAUGHING_WOMAN_RUDE","TEXT_QST_B10_LAUGHING_WOMAN_FLIRT","TEXT_QST_B10_LAUGHING_WOMAN_HEROPOSE","TEXT_QST_B10_LAUGHING_WOMAN_ARMPUMP","TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY"};
 for(int n=0;n<5;++n){GameStub g;Woman w(g);g.Expression=rude[n];w.Main();ok=ok&&Balanced(g)&&w.TimesMadeLaugh==0&&Line(g,rudeLines[n]);}
 {GameStub g;Woman w(g);w.TimesMadeLaugh=1;g.Expression=funny[0];w.Main();ok=ok&&Balanced(g)&&w.TimesMadeLaugh==2&&Line(g,"TEXT_QST_B10_LAUGHING_WOMAN_MADE_LAUGH_TWICE")&&g.LastTimerSet==20;}
 {GameStub g;Woman w(g);w.TimesMadeLaugh=2;g.Expression=funny[0];w.Main();ok=ok&&Balanced(g)&&w.TimesMadeLaugh==3&&w.GivenObject&&g.Leaves==1&&g.Morality==5&&g.Removed==1&&g.Releases==1;}
 {GameStub g;Woman w(g);w.TimesMadeLaugh=3;g.Expression=funny[0];w.Main();ok=ok&&Balanced(g)&&w.TimesMadeLaugh==4&&Line(g,"TEXT_QST_B10_LAUGHING_WOMAN_MADE_LAUGH_ONCE")&&!w.GivenObject;}
 const char* talk[]={"TEXT_QST_B10_LAUGHING_WOMAN_INTRO","TEXT_QST_B10_LAUGHING_WOMAN_ON_SPEAK_MADE_LAUGH_ONCE","TEXT_QST_B10_LAUGHING_WOMAN_ON_SPEAK_MADE_LAUGH_TWICE"};
 for(int n=0;n<3;++n){GameStub g;Woman w(g);w.TimesMadeLaugh=n;g.Talk=true;w.Main();ok=ok&&Balanced(g)&&Line(g,talk[n])&&g.LastTimerSet==5;}
 for(int n=0;n<3;++n){GameStub g;Woman w(g);w.TimesMadeLaugh=n;g.Near=true;w.Main();ok=ok&&Balanced(g)&&g.Conversations==1&&g.Animations.size()==1&&g.ConversationLines.size()==(n==0?1u:2u);}
 {GameStub g;Woman w(g);w.GivenObject=true;w.ParentClass.LaughingWomanKilled=true;w.Main();ok=ok&&Balanced(g)&&g.Removed==2&&g.Info==0&&g.Releases==1;}
 {GameStub g;Woman w(g);g.StopAt=1;w.Main();ok=ok&&Balanced(g)&&g.Pushable==0&&g.Info==0;}
 {GameStub g;Woman w(g);g.StartDelays=2;g.StopAt=6;w.Main();ok=ok&&Balanced(g)&&g.StartDelays==0&&g.Pushable==1;}
 {GameStub g;Woman w(g);g.Expression=funny[0];g.TaskFrames=4;g.StopAt=2;w.Main();ok=ok&&Balanced(g)&&g.StartedMovies==1&&g.Lines.size()==1&&g.Removed==0;}
 {GameStub g;Woman w(g);w.GivenObject=true;g.CancelOnRelease=true;w.Main();ok=ok&&Balanced(g)&&g.Releases==1&&g.Removed==0;}
 {GameStub g;Woman w(g);w.TimesMadeLaugh=2;g.Expression=funny[0];g.CancelOnLeave=true;w.Main();ok=ok&&Balanced(g)&&g.Leaves==1&&g.Morality==5&&g.Releases==0&&g.Removed==0;}
 {GameStub g;Woman w(g);g.Healthy=false;g.Expression=rude[0];g.Talk=true;w.Main();ok=ok&&Balanced(g)&&g.Lines.empty()&&g.StartedMovies==2;}
 for(int stop=1;stop<=45;++stop){GameStub g;Woman w(g);g.StopCheck=stop;g.Expression=funny[0];g.TaskFrames=2;g.Talk=true;w.Main();ok=ok&&Balanced(g);}
 std::printf(ok?"00ecf7f0_TEST PASS\n":"00ecf7f0_TEST FAIL\n");return ok?0:1;
}