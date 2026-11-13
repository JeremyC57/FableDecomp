// NScript::CQ_HangingTreeEvilScript::Introduction() -- the Hanging Tree (evil) intro: collect the HTE_FreedomFighter bandits and take each and the hero under scripted control, cast them as HERO / BAN1..BAN4 and play CS_HANGINGTREE_EVIL_INTRO inside a paused, camera-fixed movie sequence; then three script frames, the quest start screen once, flag IntroCutscenePlayed and wait until the bandits have started
// Grade: DIFFER(1500v1516) -- per-site inline split: retail inlines STLport vector<STB>::resize(n) and calls the STB default ctor 0x007E72A0 / dtor 0x007E74D0 for its temporary while expanding the hero handle's; VC7.1 inlines resize(n) here only when ~STB is out of line (33 masked diff lines)
// Next idea: find a landed caller of resize(n, x) 0x00CD3CCB whose resize(n) site byte-matches and copy its header opt-ins (STB ctor/dtor visibility) instead of a row-local model
// Leaf: ?Introduction@CQ_HangingTreeEvilScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D6D080, 1516 bytes, thiscall, plain `ret`, no EH frame. Literals "HTE_FreedomFighter"
//   0x012D2050, "HERO" 0x01255174, "BAN1".."BAN4" 0x012D1F04 / 0x012D1EFC / 0x012D1EF4 / 0x012D241C, "" 0x0122D70E,
//   "CS_HANGINGTREE_EVIL_INTRO" 0x012D2400, "Q_HangingTreeEvil" 0x012C4E4C.
// Identity (two sources): Ego_r.pdb / FableWin.pdb name set ?Introduction@CQ_HangingTreeEvilScript@NScript@@QAEXXZ (no other
//   unplaced no-argument member of the class plays the intro cut scene); the only caller is CQ_HangingTreeEvilScript::DoMission
//   0x00D6BFA0 (call at 0x00D6C1F8, right after SpawnParty); the body plays CS_HANGINGTREE_EVIL_INTRO and sets the PDB member
//   IntroCutscenePlayed. Catalog label `_global::CQ_HangingTreeEvil_Init` is a gap-scan placeholder.
// Layout: engine/CQ_HangingTreeEvilScript.h (Game +0x40, IntroCutscenePlayed +0x48, BanditsStarted +0x49,
//   QuestStartScreenShown +0x54); the Introduction declaration is the chunk 831 opt-in FABLE_RETAIL_CQ_HANGINGTREEEVILSCRIPT_INTRODUCTION.
// Callees: CCharString 0x0099EBF0 / ~ 0x0099EAE0, CBaseObject() 0x0099A380 / operator= 0x0099A3B0 / ~ 0x0099A430,
//   CScriptGameResourceObjectScriptedThingBase() 0x007E72A0 / ~ 0x007E74D0 (resize's default value), vector<STB>::resize
//   0x00CD3CCB, map operator[] 0x00CD3D2E / _M_erase 0x00CD344A, movie PImp dtor 0x006E7AB0, RunCutsceneMacro 0x00CBFB7D,
//   IsActiveThreadTerminating 0x00CB7940, malloc 0x00BFEA0E / free 0x00BFEA14 / operator delete 0x00BFE9BC; CGameScriptInterfaceBase
//   slots 7 NewScriptFrame, 8 StartScriptingEntity, 70 GetHero, 75 GetAllThingsWithScriptName, 309 KickOffQuestStartScreen,
//   370 StartMovieSequence, 371 FixMovieSequenceCamera, 379 PauseAllNonScriptedEntities.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CQ_HANGINGTREEEVILSCRIPT_INTRODUCTION
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();   // 0x006E7AB0
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CQ_HangingTreeEvilScript.h"
#include "engine/fable_script_cutscene.h"
// vector<STB>::resize(n, x) 0x00CD3CCB stays a call (declared-only specialization); STLport's inline resize(n) is expanded.
template <> void std::vector<CScriptGameResourceObjectScriptedThingBase>::resize(unsigned int, const CScriptGameResourceObjectScriptedThingBase&);
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };

void NScript::CQ_HangingTreeEvilScript::Introduction()
{
    {
        std::vector<CScriptThing> bandits;
        std::vector<CScriptGameResourceObjectScriptedThingBase> banditHandles;
        Game->GetAllThingsWithScriptName("HTE_FreedomFighter", bandits);
        banditHandles.resize(bandits.size());
        for (unsigned int i = 0; i < bandits.size(); ++i)
            Game->StartScriptingEntity(bandits[i], banditHandles[i], SCRIPT_AI_PRIORITY_HIGHEST);
        CScriptGameResourceObjectScriptedThingBase heroHandle;
        Game->StartScriptingEntity(Game->GetHero(), heroHandle, SCRIPT_AI_PRIORITY_HIGHEST);
        std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> cast;
        cast["HERO"] = heroHandle;
        cast["BAN1"] = banditHandles[0];
        cast["BAN2"] = banditHandles[1];
        cast["BAN3"] = banditHandles[2];
        cast["BAN4"] = banditHandles[3];
        CScriptGameResourceObjectMovieBase movie;
        Game->StartMovieSequence("", movie);
        CWideScreenMagicPauseEntities pause(*Game);
        Game->FixMovieSequenceCamera(true);
        RunCutsceneMacro("CS_HANGINGTREE_EVIL_INTRO", cast, 0, 0, false, true);
        Game->FixMovieSequenceCamera(false);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    if (!QuestStartScreenShown) {
        if (IsActiveThreadTerminating())
            return;
        Game->KickOffQuestStartScreen("Q_HangingTreeEvil", true, false);
        QuestStartScreenShown = true;
    }
    IntroCutscenePlayed = true;
    while (!BanditsStarted) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
}
