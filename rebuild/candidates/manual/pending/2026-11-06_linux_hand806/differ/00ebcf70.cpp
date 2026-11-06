// NScript::CV_OracleScript::CVO_OracleMarker::Main() -- the Oracle's symbol marker: the first time, it shows the Oracle tutorial; then, while the hero stands within four units, each Y / M / C / A expression the hero performs makes the Oracle face the hero and tell the next story of that category (people, legends, creatures, history), with a category intro on the first story and the Whisper / Theresa stories skipped once their quests are resolved.
// Grade: DIFFER(5492v5572) -- 778 diff lines: retail loads the expression's PStringData / PData once across the four Compare calls of each || / else-if chain, ours reloads it after each call; retail places the non-null compare chain and the category branches out of line after the main exit, ours inline; frame slots (expression +0x1c v +0x18, the edi/esi zero register); two exits call ~CScriptGameResourceObjectMovieBase 0x006E7B80 where ours expands it.
// Next idea: the CSE across Compare suggests expression is compared through a local whose address is not taken (for example a CCharString copy or a const char* local fed by the inline operator==). Try a helper bool for the expression test; /Oa (a6) made it worse.
// Leaf: ?Main@CVO_OracleMarker@CV_OracleScript@NScript@@EAEXXZ
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
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& thing_a, const CScriptThing& thing_b, float distance);   // 0x00CBE2FF
void NScript::CV_OracleScript::CVO_OracleMarker::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CCharString YCat[8]={"WHISPER","THERESA","GUILDMASTER","THUNDER","BRIAR","SCYTHE","MAZE","ARCHEOLOGIST"};
    CCharString MCat[5]={"DEMON_DOORS","NECROPOLIS","SHIP_DROWNED","SINGING_SWORD","SKORM_AVO"};
    CCharString ACat[7]={"ARCHON","HOOK_COAST","JACK","NOSTRO","ORACLE","PROPHETS","SNOWSPIRE"};
    CCharString CCat[5]={"BALVERINES","KRAKENS","SANDGOOSE","SCREAMERS","SUMMONERS"};
    if (!HaveBeenToldHowToInteract) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->FadeScreenIn();
        Game->DisplayGameInfo("TEXT_QST_B20_ORACLE_TUTORIAL");
        while (!Game->MsgIsGameInfoClickedPast()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        HaveBeenToldHowToInteract=true;
    }
    CCharString expression;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    Game->StartScriptingEntity(Game->GetThingWithScriptName("OracleY"),seh_me,SCRIPT_AI_PRIORITY_HIGHEST);
    while (!IsActiveThreadTerminating()) {
        while (IsDistanceBetweenThingsUnder(Me,Game->GetHero(),4.0f)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (Game->MsgOnExpressionPerformed(expression)) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (expression=="EXPRESSION_Y" || expression=="EXPRESSION_M" || expression=="EXPRESSION_C" || expression=="EXPRESSION_A") {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    CCharString category;
                    CCharString subject;
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game->StartMovieSequence("",speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    Game->EntitySetFacingAngleTowardsThing(Game->GetHero(),Game->GetThingWithScriptName("OracleY"),false);
                    Game->FixMovieSequenceCamera(true);
                    Game->CameraUseCameraPoint(Game->GetThingWithScriptName("Cam_ForSymbols"),0,0,-1.0f,0,-1);
                    long times_played=0;
                    if (expression=="EXPRESSION_Y") {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        category="PEOPLE";
                        subject=YCat[TimesPlayedYCat%8];
                        if (subject=="WHISPER" && PMasterData->WhisperKilledByHero) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            subject=YCat[1];
                            TimesPlayedYCat=1;
                        }
                        if (subject=="THERESA" && PMasterData->JackBossBattleResult==1) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            subject=YCat[2];
                            TimesPlayedYCat=2;
                        }
                        times_played=++TimesPlayedYCat;
                    } else if (expression=="EXPRESSION_M") {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        category="LEGENDS";
                        subject=MCat[TimesPlayedMCat%5];
                        times_played=++TimesPlayedMCat;
                    } else if (expression=="EXPRESSION_A") {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        category="HISTORY";
                        subject=ACat[TimesPlayedACat%7];
                        times_played=++TimesPlayedACat;
                    } else if (expression=="EXPRESSION_C") {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        category="CREATURES";
                        subject=CCat[TimesPlayedCCat%5];
                        times_played=++TimesPlayedCCat;
                    }
                    if (times_played==1) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game->GetHero(),"TEXT_QST_B20_"+category+"_INTRO",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                    }
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),"TEXT_QST_B20_TELL_YOU",GROUP_SELECT_RANDOM_NO_REPEAT,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),"TEXT_QST_B20_"+category+"_"+subject+"_TITLE",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),"TEXT_QST_B20_"+category+"_"+subject,GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    Game->FixMovieSequenceCamera(false);
                }
            }
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->NewScriptFrame();
    }
}
