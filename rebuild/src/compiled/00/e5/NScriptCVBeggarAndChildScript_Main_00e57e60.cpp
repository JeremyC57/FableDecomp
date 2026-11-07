// NScript::CV_BeggarAndChildScript::Main() -- the Bowerstone beggar and the bully: bind both, wait until one is hit or leaves, then take the hero, the bully and the beggar over for a cut scene: the beggar hit teaches belch (and sneer), the bully hit teaches the battle cry fart (and thanks), the beggar leaving teaches sneer, the bully leaving plays his exit; each outcome ends with the morality change; finally deactivate the quest
// Leaf: ?Main@CV_BeggarAndChildScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E57E60, 3567 bytes; script vtable 0x012E65E8 slot 2.
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/C3DVector.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CV_BeggarAndChildScript.h"
#include "engine/CEntityScriptBinding.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/fable_script_cutscene.h"
#include "engine/CScriptDef.h"
#include "engine/CDefPointer.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
extern CDefPointer<const CScriptDef> pGlobalScriptDef;   // 0x0143E90C
#define BC_T if (IsActiveThreadTerminating()) return
#define BC_START_SCRIPTING(thing, seh) \
    if (seh.IsValid()) \
        seh.Release(); \
    while (!Game->StartScriptingEntity(thing, seh, SCRIPT_AI_PRIORITY_HIGHEST)) { \
        Game->NewScriptFrame(); \
        BC_T; \
    } \
    BC_T
#define BC_TUTORIAL \
    if (Game->IsXbox()) { \
        BC_T; \
        Game->DisplayGameInfo("TEXT_QST_015_TUTORIAL_EXPRESSIONS"); \
        while (!Game->MsgIsGameInfoClickedPast()) { \
            Game->NewScriptFrame(); \
            BC_T; \
        } \
        BC_T; \
    } else { \
        BC_T; \
        Game->DisplayGameInfo("TEXT_QST_015_TUTORIAL_EXPRESSIONS_PC"); \
        while (!Game->MsgIsGameInfoClickedPast()) { \
            Game->NewScriptFrame(); \
            BC_T; \
        } \
        BC_T; \
    } \
    ExpressionTutorialShown = true

void NScript::CV_BeggarAndChildScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CV_BeggarAndChildScript>("LookoutPointBeggar", this, &CLookoutPointBeggar::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_BeggarAndChildScript>("BeggarBully", this, &CBeggarBully::Alloc, 1));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    while (!BeggarHit && !BullyHit && !BeggarLeft && !BullyLeft) {
        Game->NewScriptFrame();
        BC_T;
    }
    BC_T;
    {
        CScriptThing bully = Game->GetThingWithScriptName("BeggarBully");
        CScriptThing beggar = Game->GetThingWithScriptName("LookoutPointBeggar");
        CScriptGameResourceObjectScriptedThingBase seh_hero;
        CScriptGameResourceObjectScriptedThingBase seh_bully;
        CScriptGameResourceObjectScriptedThingBase seh_beggar;
        BC_START_SCRIPTING(Game->GetHero(), seh_hero);
        BC_START_SCRIPTING(bully, seh_bully);
        BC_START_SCRIPTING(beggar, seh_beggar);
        std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
        mymap["HERO"] = seh_hero;
        mymap["BULLY"] = seh_bully;
        mymap["BEGGAR"] = seh_beggar;
        CScriptGameResourceObjectMovieBase movie;
        Game->StartMovieSequence("", movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
        Game->FixMovieSequenceCamera(true);
        RunCutsceneMacro("CS_BANDB_SETUP", mymap, 0, 0, false, true);
        if (BeggarHit) {
            BC_T;
            RunCutsceneMacro("CS_BANDB_BEGGARHIT", mymap, 0, 0, false, true);
            if (!TaughtBelch && !TaughtBattleCry) {
                BC_T;
                RunCutsceneMacro("CS_BANDB_BELCH_TEACH", mymap, 0, 0, false, true);
                if (ExpressionTutorialShown) {
                    BC_TUTORIAL;
                }
                Game->SetPreferredQuickAccessItem("OBJECT_EXPRESSION_BELCH_DUMMY", 2, 10);
            }
            if (!TaughtSneer) {
                BC_T;
                RunCutsceneMacro("CS_BANDB_SNEER_TEACH", mymap, 0, 0, false, true);
                if (!ExpressionTutorialShown) {
                    BC_TUTORIAL;
                }
                Game->SetPreferredQuickAccessItem("OBJECT_EXPRESSION_SNEER_DUMMY", 3, 10);
            }
            RunCutsceneMacro("CS_BANDB_BEGGARLEAVES_2", mymap, 0, 0, false, true);
            Game->GiveHeroMorality(pGlobalScriptDef->BC_EvilMoralityLoss);
        } else if (BullyHit) {
            BC_T;
            RunCutsceneMacro("CS_BANDB_BULLYHIT", mymap, 0, 0, false, true);
            if (!TaughtBelch && !TaughtBattleCry) {
                BC_T;
                RunCutsceneMacro("CS_BANDB_BATTLE_TEACH", mymap, 0, 0, false, true);
                if (!ExpressionTutorialShown) {
                    BC_TUTORIAL;
                }
                Game->SetPreferredQuickAccessItem("OBJECT_EXPRESSION_FART_DUMMY", 3, 10);
            }
            if (!TaughtThanks) {
                BC_T;
                RunCutsceneMacro("CS_BANDB_THANKS_TEACH", mymap, 0, 0, false, true);
            }
            RunCutsceneMacro("CS_BANDB_BULLYLEAVES_2", mymap, 0, 0, false, true);
            Game->GiveHeroMorality(pGlobalScriptDef->BC_GoodMoralityGain);
        } else if (BeggarLeft) {
            BC_T;
            RunCutsceneMacro("CS_BANDB_BEGGARLEAVES_1", mymap, 0, 0, false, true);
            if (!TaughtSneer) {
                BC_T;
                RunCutsceneMacro("CS_BANDB_SNEER_TEACH", mymap, 0, 0, false, true);
                if (!ExpressionTutorialShown) {
                    BC_TUTORIAL;
                }
            }
            RunCutsceneMacro("CS_BANDB_BEGGARLEAVES_2", mymap, 0, 0, false, true);
            Game->GiveHeroMorality(pGlobalScriptDef->BC_EvilMoralityLoss);
        } else if (BullyLeft) {
            BC_T;
            if (BelchedAtBully) {
                BC_T;
                RunCutsceneMacro("CS_BANDB_BULLYLEAVES_BELCH", mymap, 0, 0, false, true);
            } else {
                BC_T;
                RunCutsceneMacro("CS_BANDB_BULLYLEAVES_CRY", mymap, 0, 0, false, true);
            }
            RunCutsceneMacro("CS_BANDB_BULLYLEAVES_1", mymap, 0, 0, false, true);
            Game->NewScriptFrame();
            BC_T;
            Game->NewScriptFrame();
            BC_T;
            RunCutsceneMacro("CS_BANDB_BULLYLEAVES_2", mymap, 0, 0, false, true);
            Game->GiveHeroMorality(pGlobalScriptDef->BC_GoodMoralityGain);
        }
        Game->FixMovieSequenceCamera(false);
    }
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
}