// NScript::CV_OracleScript::CVO_OracleMarker::Main() -- the Oracle's symbol marker: the first time, it shows the Oracle tutorial; then, while the hero stands within four units, each Y / M / C / A ExpressionPerformed the hero performs makes the Oracle face the hero and tell the next story of that Category (people, legends, creatures, history), with a Category intro on the first story and the Whisper / Theresa stories skipped once their quests are resolved.
// Grade: RELOCATION_MATCH -- actual COFF operands checked against their native targets.
// Inline-budget: visible exact CBasicString<char>::Compare body permits pointer CSE; donor literal-false for loop retains the retail per-exit destructor split.
// Leaf: ?Main@CVO_OracleMarker@CV_OracleScript@NScript@@EAEXXZ
// Retail: 0x00EBCF70, 5572 bytes, thiscall.
#define FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CISTHINGALIVE_ORIGINAL_ACCESS
#define FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CPREDICATEBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_FORCEINLINE_CTOR
#include <string.h>
#include "engine/C3DVector.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CV_OracleScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
class CScriptGameResourceObjectMovieBase;
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/TRetVoid.h"
#include "engine/CIsThingAlive.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
inline CScriptThing::~CScriptThing() {}
bool FABLE_FASTCALL IsDistanceBetweenThingsUnder(const CScriptThing& thing_a, const CScriptThing& thing_b, float distance);   // 0x00CBE2FF
// Native Compare 0x00411570 reads both signed character sequences without modifying them.
#ifdef _MSC_VER
#pragma auto_inline(off)
#endif
template<> long FABLE_FASTCALL CBasicString<char>::Compare(const char* a,const char* b)
{
    while (*a || *b) {
        if (*a < *b) return -1;
        if (*a > *b) return 1;
        ++a; ++b;
    }
    return 0;
}
#ifdef _MSC_VER
#pragma auto_inline(on)
#endif
void NScript::CV_OracleScript::CVO_OracleMarker::Main()
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