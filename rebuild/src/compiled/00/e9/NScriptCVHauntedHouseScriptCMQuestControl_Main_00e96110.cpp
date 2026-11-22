// 00e96110 NScript::CV_HauntedHouseScript::CM_QuestControl::Main
// Inline-budget: inline_depth(0) at external-loop exits and final implicit CTimer destruction; fitted retail split, original construct unproven.
// Leaf: ?Main@CM_QuestControl@CV_HauntedHouseScript@NScript@@EAEXXZ
// Flags:
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CM_QUESTCONTROL_MAIN_PASS26
#define FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_STLP_VECTOR_DTOR_OUTOFLINE
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_MERASE_DECL_ONLY
#define FABLE_RETAIL_STLP_TREE_DTOR_DECL_ONLY
#define FABLE_RETAIL_STLP_MAP_DTOR_DECL_ONLY
#define FABLE_RETAIL_STLP_MAP_INDEX_DECL_ONLY
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/C3DVector.h"
#include "engine/CRGBColour.h"
#include "engine/CScriptThing.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterface.h"
#include "engine/CV_HauntedHouseScript.h"
#include "engine/CScriptDef.h"
#include "engine/CDefPointer.h"
#include "engine/CTimer.h"
// Inline-budget: retail 00E9639E keeps the Game receiver across both calls; the original pause guard constructor expands here.
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/TRetVoid.h"
#include "stlport/fable_map.h"
#include "stlport/fable_containers.h"
extern CGameScriptInterface* g_pGameScriptInterface;
extern CDefPointer<const CScriptDef> pGlobalScriptDef;
inline CTimer::CTimer() { TimerIndex = g_pGameScriptInterface->RegisterTimer(); }
inline CTimer::~CTimer() { g_pGameScriptInterface->DeregisterTimer(TimerIndex); }
inline CTimer& CTimer::operator=(long time) { g_pGameScriptInterface->SetTimer(TimerIndex,time); return *this; }
inline long CTimer::GetTime() const { return g_pGameScriptInterface->GetTimer(TimerIndex); }
inline bool CTimer::operator==(long time) const { return GetTime()==time; }
inline CScriptThing::~CScriptThing() { }
void FABLE_FASTCALL RunCutsceneMacro(const CCharString&,std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>&,std::map<CCharString,bool>*,std::map<CCharString,CCharString>*,bool,bool);
namespace NScript {
void CV_HauntedHouseScript::CM_QuestControl::Main()
{
    TRetVoid _Fail_Val;
    while (!ParentClass.InHouse) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    bool PreviouslyKilledEnoughUndead = ParentClass.UndeadBar >= pGlobalScriptDef->HH_UpperBarLimit;
    CTimer DecrementTimer;
    DecrementTimer = 1;
    Game.DisplayQuestInfo(true);
    ParentClass.GuiCounter = Game.AddQuestInfoBar(pGlobalScriptDef->HH_UpperBarLimit, 0.0f, CRGBColour(0, 255, 0, 255),
        CRGBColour(255, 0, 0, 255), "HUD_QUEST_ICON_UNDEAD", "", 1.0f);
    while (ParentClass.UndeadBar < pGlobalScriptDef->HH_UpperBarLimit) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        if (DecrementTimer == 0) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            if (ParentClass.UndeadBar < pGlobalScriptDef->HH_BarDecrementAmount) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                ParentClass.UndeadBar = 0.0f;
            } else {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                ParentClass.UndeadBar -= pGlobalScriptDef->HH_BarDecrementAmount;
            }
            DecrementTimer = (long)pGlobalScriptDef->HH_BarDecrementInterval;
        }
        Game.UpdateQuestInfoBar(ParentClass.GuiCounter, ParentClass.UndeadBar, -1.0f, -1.0f);
    }
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
    Game.RemoveQuestInfoElement(ParentClass.GuiCounter);
    if (!ParentClass.CellarDoorOpen) {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
        std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> mymap;
        CScriptGameResourceObjectMovieBase speech_movie;
        Game.StartMovieSequence("", speech_movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
        ParentClass.CellarDoorOpen = true;
        RunCutsceneMacro("CS_HAUNTED_CELLAROPENS", mymap, 0, 0, false, true);
        CScriptThing DiggingPlace = Game.GetThingWithScriptName("HH_DiggingSpot");
        Game.AddItemToContainer(DiggingPlace, "OBJECT_TATTOO_CARD_BACK_02");
    }
    Game.GiveHeroRenownPoints(pGlobalScriptDef->HH_ClearedHHRenown);
    Game.SetCreatureGeneratorsEnabled("HauntedHouse", true);
    std::vector<CScriptThing> NymphsHere;
    Game.GetAllThingsWithScriptName("M_NymphHere", NymphsHere);
    for (long i = 0; i < NymphsHere.size(); i++) {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
        Game.CreateCreature("CREATURE_LAKE_NYMPH_01", NymphsHere[i].GetPos(), "", false);
        Game.Pause(1.0f);
    }
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
    Game.SetQuestAsCompleted(Game.GetActiveQuestName(), false, true, false);
    Game.DeactivateQuestLater(Game.GetActiveQuestName(), 0);
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
    return;
#ifdef _MSC_VER
#pragma inline_depth()
#endif
}
}   // namespace NScript