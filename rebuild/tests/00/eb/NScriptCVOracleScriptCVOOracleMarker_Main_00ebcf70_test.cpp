// Complete Oracle Main body with deterministic callees; source logic and cleanup scopes retained.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <cassert>
static int Strings=0, Things=0, Movies=0, Handles=0;
class CCharString { public:
    std::string text;
    CCharString():text(){++Strings;}
    CCharString(const char* s):text(s){++Strings;}
    CCharString(const CCharString& s):text(s.text){++Strings;}
    ~CCharString(){--Strings;}
    CCharString& operator=(const CCharString& s){text=s.text;return *this;}
    CCharString& operator=(const char* s){text=s;return *this;}
    bool operator==(const char* s)const{return text==s;}
    operator const char*()const{return text.c_str();}
};
static CCharString operator+(const char* a,const CCharString& b){return CCharString((std::string(a)+b.text).c_str());}
static CCharString operator+(const CCharString& a,const char* b){return CCharString((a.text+b).c_str());}
static CCharString operator+(const CCharString& a,const CCharString& b){return CCharString((a.text+b.text).c_str());}
class CScriptThing { public:
    int id;
    CScriptThing(int n=0):id(n){++Things;}
    CScriptThing(const CScriptThing& o):id(o.id){++Things;}
    ~CScriptThing(){--Things;}
};
struct TRetVoid{void Return()const{}};
struct CIsThingAlive{CIsThingAlive(const CScriptThing&){}};
class CScriptGameResourceObjectMovieBase{ public:CScriptGameResourceObjectMovieBase(){++Movies;}~CScriptGameResourceObjectMovieBase(){--Movies;}};
struct Event{int frame;const char* text;Event(int f,const char* t):frame(f),text(t){}};
struct Game;
static Game* Current=0;
class CScriptGameResourceObjectScriptedThingBase { public:
    bool started;
    CScriptGameResourceObjectScriptedThingBase():started(false){++Handles;}
    ~CScriptGameResourceObjectScriptedThingBase(){--Handles;}
    CScriptThing GetScriptThing()const{return CScriptThing(3);}
    void Speak(const CScriptThing&,const char*,int,bool,bool,bool);
    bool IsPerformingScriptTask()const;
};
struct Game {
    int Frame,TermFrame,ClickedAt,TaskUntil,TaskFrames,PauseDepth,MovieStarts,FaceCalls,InfoCalls,Checks,TermCheck;
    bool Near,Healthy,CameraFixed;
    std::vector<Event> Events;
    std::vector<std::string> Lines;
    Game():Frame(0),TermFrame(35),ClickedAt(0),TaskUntil(0),TaskFrames(1),PauseDepth(0),MovieStarts(0),FaceCalls(0),InfoCalls(0),Checks(0),TermCheck(-1),Near(true),Healthy(true),CameraFixed(false){Current=this;}
    bool T(){++Checks;return Frame>=TermFrame||(TermCheck>=0&&Checks>=TermCheck);}
    void NewScriptFrame(){++Frame;assert(Frame<1000);}
    CScriptThing GetHero(){return CScriptThing(2);}
    CScriptThing GetThingWithScriptName(const char* s){return CScriptThing(!std::strcmp(s,"OracleY")?3:4);}
    void FadeScreenIn(){}
    void DisplayGameInfo(const char* s){assert(!std::strcmp(s,"TEXT_QST_B20_ORACLE_TUTORIAL"));++InfoCalls;}
    bool MsgIsGameInfoClickedPast(){return Frame>=ClickedAt;}
    bool StartScriptingEntity(const CScriptThing& t,CScriptGameResourceObjectScriptedThingBase& h,int p){assert(t.id==3&&p==4);h.started=true;return true;}
    bool MsgOnExpressionPerformed(CCharString& e){for(unsigned i=0;i<Events.size();++i)if(Events[i].frame==Frame){e=Events[i].text;return true;}return false;}
    void StartMovieSequence(const char* s,CScriptGameResourceObjectMovieBase&){assert(!*s);++MovieStarts;}
    void PauseAllNonScriptedEntities(bool b){PauseDepth+=b?1:-1;assert(PauseDepth>=0&&PauseDepth<=1);}
    void EntitySetFacingAngleTowardsThing(const CScriptThing& h,const CScriptThing& o,bool b){assert(h.id==2&&o.id==3&&!b);++FaceCalls;}
    void FixMovieSequenceCamera(bool b){CameraFixed=b;}
    void CameraUseCameraPoint(const CScriptThing& c,int a,int b,float f,int d,int e){assert(c.id==4&&a==0&&b==0&&f==-1&&d==0&&e==-1);}
    float GetHealth(const CScriptThing& t){assert(t.id==3);return Healthy?100.0f:0.0f;}
};
void CScriptGameResourceObjectScriptedThingBase::Speak(const CScriptThing& h,const char* s,int group,bool a,bool b,bool c){assert(started&&h.id==2&&group>=0&&group<=2&&!a&&b&&!c);Current->Lines.push_back(s);Current->TaskUntil=Current->Frame+Current->TaskFrames;}
bool CScriptGameResourceObjectScriptedThingBase::IsPerformingScriptTask()const{return Current->Frame<Current->TaskUntil;}
struct CWideScreenMagicPauseEntities {
    Game* game;
    CWideScreenMagicPauseEntities(Game& g):game(&g){game->PauseAllNonScriptedEntities(true);}
    ~CWideScreenMagicPauseEntities(){game->PauseAllNonScriptedEntities(false);}
};
struct Master{bool WhisperKilledByHero;long JackBossBattleResult;Master():WhisperKilledByHero(false),JackBossBattleResult(0){}};
struct Oracle {
    ::Game* Game;
    Master* PMasterData;
    CScriptThing Me;
    long TimesPlayedYCat,TimesPlayedMCat,TimesPlayedCCat,TimesPlayedACat;
    bool HaveBeenToldHowToInteract;
    Oracle(::Game& g,Master& m):Game(&g),PMasterData(&m),Me(1),TimesPlayedYCat(0),TimesPlayedMCat(0),TimesPlayedCCat(0),TimesPlayedACat(0),HaveBeenToldHowToInteract(true){}
    void SetPredicate(const CIsThingAlive&){}
    bool IsActiveThreadTerminating(){return Game->T();}
    void Main();
};
enum EScriptAIPriority{SCRIPT_AI_PRIORITY_HIGHEST=4};
enum ETextGroupSelectionMethod{GROUP_SELECT_NONE=0,GROUP_SELECT_RANDOM_NO_REPEAT=2};
static bool IsDistanceBetweenThingsUnder(const CScriptThing& a,const CScriptThing& b,float d){assert(a.id==1&&b.id==2&&d==4);return Current->Near;}

void Oracle::Main()
{
    // Fixture adapter: bind the original reference to the configured dependency.
    ::Game& Game = *this->Game;
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CCharString PeopleSubject[8]={"WHISPER","THERESA","GUILDMASTER","THUNDER","BRIAR","SCYTHE","MAZE","ARCHEOLOGIST"};
    CCharString LegendsSubject[5]={"DEMON_DOORS","NECROPOLIS","SHIP_DROWNED","SINGING_SWORD","SKORM_AVO"};
    CCharString HistorySubject[7]={"ARCHON","HOOK_COAST","JACK","NOSTRO","ORACLE","PROPHETS","SNOWSPIRE"};
    CCharString CreaturesSubject[5]={"BALVERINES","KRAKENS","SANDGOOSE","SCREAMERS","SUMMONERS"};
    if (!HaveBeenToldHowToInteract) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.FadeScreenIn();
        Game.DisplayGameInfo("TEXT_QST_B20_ORACLE_TUTORIAL");
        while (!Game.MsgIsGameInfoClickedPast()) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        HaveBeenToldHowToInteract=true;
    }
    CCharString ExpressionPerformed;
    CScriptGameResourceObjectScriptedThingBase seh_Oracle;
    Game.StartScriptingEntity(Game.GetThingWithScriptName("OracleY"),seh_Oracle,SCRIPT_AI_PRIORITY_HIGHEST);
    for (; !false; Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        while (IsDistanceBetweenThingsUnder(Me,Game.GetHero(),4.0f)) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (Game.MsgOnExpressionPerformed(ExpressionPerformed)) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (ExpressionPerformed=="EXPRESSION_Y" || ExpressionPerformed=="EXPRESSION_M" || ExpressionPerformed=="EXPRESSION_C" || ExpressionPerformed=="EXPRESSION_A") {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    CCharString Category;
                    CCharString Subject;
                    bool FirstCatPlayed=false;
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game.StartMovieSequence("",speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                    Game.EntitySetFacingAngleTowardsThing(Game.GetHero(),Game.GetThingWithScriptName("OracleY"),false);
                    Game.FixMovieSequenceCamera(true);
                    Game.CameraUseCameraPoint(Game.GetThingWithScriptName("Cam_ForSymbols"),0,0,-1.0f,0,-1);
                    if (ExpressionPerformed=="EXPRESSION_Y") {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        Category="PEOPLE";
                        Subject=PeopleSubject[TimesPlayedYCat%8];
                        if (Subject=="WHISPER" && PMasterData->WhisperKilledByHero) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            Subject=PeopleSubject[1];
                            TimesPlayedYCat=1;
                        }
                        if (Subject=="THERESA" && PMasterData->JackBossBattleResult==1) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            Subject=PeopleSubject[2];
                            TimesPlayedYCat=2;
                        }
                        ++TimesPlayedYCat;
                        if (TimesPlayedYCat==1) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            FirstCatPlayed=true;
                        }
                    } else if (ExpressionPerformed=="EXPRESSION_M") {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        Category="LEGENDS";
                        Subject=LegendsSubject[TimesPlayedMCat%5];
                        ++TimesPlayedMCat;
                        if (TimesPlayedMCat==1) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            FirstCatPlayed=true;
                        }
                    } else if (ExpressionPerformed=="EXPRESSION_A") {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        Category="HISTORY";
                        Subject=HistorySubject[TimesPlayedACat%7];
                        ++TimesPlayedACat;
                        if (TimesPlayedACat==1) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            FirstCatPlayed=true;
                        }
                    } else if (ExpressionPerformed=="EXPRESSION_C") {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        Category="CREATURES";
                        Subject=CreaturesSubject[TimesPlayedCCat%5];
                        ++TimesPlayedCCat;
                        if (TimesPlayedCCat==1) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            FirstCatPlayed=true;
                        }
                    }
                    if (FirstCatPlayed) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game.GetHealth(seh_Oracle.GetScriptThing())>0.0f) {
                            seh_Oracle.Speak(Game.GetHero(),"TEXT_QST_B20_"+Category+"_INTRO",GROUP_SELECT_NONE,false,true,false);
                            while (seh_Oracle.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                    }
                    if (Game.GetHealth(seh_Oracle.GetScriptThing())>0.0f) {
                        seh_Oracle.Speak(Game.GetHero(),"TEXT_QST_B20_TELL_YOU",GROUP_SELECT_RANDOM_NO_REPEAT,false,true,false);
                        while (seh_Oracle.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (Game.GetHealth(seh_Oracle.GetScriptThing())>0.0f) {
                        seh_Oracle.Speak(Game.GetHero(),"TEXT_QST_B20_"+Category+"_"+Subject+"_TITLE",GROUP_SELECT_NONE,false,true,false);
                        while (seh_Oracle.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (Game.GetHealth(seh_Oracle.GetScriptThing())>0.0f) {
                        seh_Oracle.Speak(Game.GetHero(),"TEXT_QST_B20_"+Category+"_"+Subject,GROUP_SELECT_NONE,false,true,false);
                        while (seh_Oracle.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    Game.FixMovieSequenceCamera(false);
                }
            }
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}
}


static bool Balanced(const Game& g){return Strings==0&&Things==1&&Movies==0&&Handles==0&&g.PauseDepth==0;}
static bool Has(const Game& g,const char* s){for(unsigned i=0;i<g.Lines.size();++i)if(g.Lines[i]==s)return true;return false;}
static unsigned Count(const Game& g,const char* s){unsigned n=0;for(unsigned i=0;i<g.Lines.size();++i)if(g.Lines[i]==s)++n;return n;}
int main(){
    bool ok=true;
    { Game g;Master m;Oracle o(g,m);g.Events.push_back(Event(2,"EXPRESSION_Y"));g.Events.push_back(Event(8,"EXPRESSION_M"));g.Events.push_back(Event(14,"EXPRESSION_C"));g.Events.push_back(Event(20,"EXPRESSION_A"));o.Main();
      ok=ok&&Balanced(g)&&g.MovieStarts==4&&g.FaceCalls==4&&!g.CameraFixed&&o.TimesPlayedYCat==1&&o.TimesPlayedMCat==1&&o.TimesPlayedCCat==1&&o.TimesPlayedACat==1&&g.Lines.size()==16;
      ok=ok&&Has(g,"TEXT_QST_B20_PEOPLE_WHISPER_TITLE")&&Has(g,"TEXT_QST_B20_LEGENDS_DEMON_DOORS")&&Has(g,"TEXT_QST_B20_CREATURES_BALVERINES")&&Has(g,"TEXT_QST_B20_HISTORY_ARCHON")&&Count(g,"TEXT_QST_B20_TELL_YOU")==4;
    }
    { Game g;Master m;m.WhisperKilledByHero=true;m.JackBossBattleResult=1;Oracle o(g,m);g.Events.push_back(Event(2,"EXPRESSION_Y"));g.Events.push_back(Event(8,"EXPRESSION_Y"));o.Main();
      ok=ok&&Balanced(g)&&o.TimesPlayedYCat==4&&g.Lines.size()==6&&Has(g,"TEXT_QST_B20_PEOPLE_GUILDMASTER")&&Has(g,"TEXT_QST_B20_PEOPLE_THUNDER")&&!Has(g,"TEXT_QST_B20_PEOPLE_INTRO");
    }
    { Game g;Master m;Oracle o(g,m);o.TimesPlayedMCat=4;g.Events.push_back(Event(2,"EXPRESSION_X"));g.Events.push_back(Event(8,"EXPRESSION_M"));g.Events.push_back(Event(14,"EXPRESSION_M"));o.Main();
      ok=ok&&Balanced(g)&&o.TimesPlayedMCat==6&&g.MovieStarts==2&&g.Lines.size()==6&&Has(g,"TEXT_QST_B20_LEGENDS_SKORM_AVO")&&Has(g,"TEXT_QST_B20_LEGENDS_DEMON_DOORS")&&!Has(g,"TEXT_QST_B20_LEGENDS_INTRO");
    }
    { Game g;Master m;Oracle o(g,m);o.HaveBeenToldHowToInteract=false;g.ClickedAt=8;g.Events.push_back(Event(9,"EXPRESSION_C"));o.Main();
      ok=ok&&Balanced(g)&&o.HaveBeenToldHowToInteract&&g.InfoCalls==1&&o.TimesPlayedCCat==1&&g.Lines.size()==4;
    }
    { Game g;Master m;Oracle o(g,m);o.HaveBeenToldHowToInteract=false;g.ClickedAt=9;g.TermFrame=7;o.Main();
      ok=ok&&Balanced(g)&&!o.HaveBeenToldHowToInteract&&g.InfoCalls==1&&g.MovieStarts==0&&g.Frame==7;
    }
    { Game g;Master m;Oracle o(g,m);g.TaskFrames=3;g.TermFrame=3;g.Events.push_back(Event(2,"EXPRESSION_Y"));o.Main();
      ok=ok&&Balanced(g)&&g.Frame==3&&g.MovieStarts==1&&g.Lines.size()==1&&Has(g,"TEXT_QST_B20_PEOPLE_INTRO");
    }
    { Game g;Master m;Oracle o(g,m);g.Healthy=false;g.Events.push_back(Event(2,"EXPRESSION_A"));o.Main();ok=ok&&Balanced(g)&&o.TimesPlayedACat==1&&g.MovieStarts==1&&g.Lines.empty()&&!g.CameraFixed; }
    { Game g;Master m;Oracle o(g,m);g.Near=false;g.Events.push_back(Event(2,"EXPRESSION_M"));o.Main();ok=ok&&Balanced(g)&&g.MovieStarts==0&&o.TimesPlayedMCat==0&&g.Frame==g.TermFrame; }
    ok=ok&&Strings==0&&Things==0&&Movies==0&&Handles==0;
    std::printf(ok?"00ebcf70_TEST PASS\n":"00ebcf70_TEST FAIL\n");return ok?0:1;
}