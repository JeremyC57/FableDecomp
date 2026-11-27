#define FABLE_RETAIL_BEARDY_RESET_HAIR_WATCH_PASS29
// Leaf: ?WatchForQuestCardConditions@CV_BeardyBaldyScript@NScript@@QAEXXZ
// Flags: /O2 /Oy
#define FABLE_RETAIL_CSCRIPTTHING_ORIGINAL_ACCESS
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CV_BEARDYBALDYSCRIPT_GOTALKTOBARBER
#define FABLE_RETAIL_CV_BEARDYBALDYSCRIPT_MAIN_PASS27
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CBASICSTRING_COMPARE_BODY
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_COMPARE_ZERO
#define FABLE_RETAIL_BEARDY_ORIGINAL_TEXT_COMPARISON_PASS27
#include "engine/TRetVoid.h"
#include "engine/CV_BeardyBaldyScript.h"
#include "engine/CGameScriptInterfaceBase.h"
inline CScriptThing::CScriptThing() {}
CScriptThing::~CScriptThing() {}
inline bool CScriptThing::MsgIsTalkedToBy(const CCharString& talker) const {
    if (PImp.IsNull()) return false;
    return PImp->MsgIsTalkedToBy(talker);
}
inline bool CScriptThing::IsAlive() const {
    return !PImp.IsNull() && PImp->IsAlive();
}
inline void NScript::CV_BeardyBaldyScript::ResetHairWatch()
{
    AcquiredNewHairdo = false;
    AcquiredNewBeard = false;
    AcquiredNewTash = false;
}
void NScript::CV_BeardyBaldyScript::WatchForQuestCardConditions()
{
    TRetVoid _Fail_Val;
    CCharString defName;
    CCharString regionName;
    CScriptThing beardyBaldyMan;
    while (!Phase1RequirementsComplete) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    Game.GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_BEARDY_BALDY", Game.GetActiveQuestName(), true);
    Game.SetQuestCardObjective(Game.GetActiveQuestName(), "TEXT_QUEST_BEARDY_BALDY_OBJECTIVE_02", "", "");
    GoTalkToBarber();
    if (false) { } else for (;;) {
        do { if (IsActiveThreadTerminating()) return _Fail_Val.Return(); } while (0);
        while (!AcquiredNewHairdo && !AcquiredNewBeard && !AcquiredNewTash) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        Game.SetQuestCardObjective(Game.GetActiveQuestName(), "TEXT_QUEST_BEARDY_BALDY_OBJECTIVE_03", "", "");
        GoTalkToBeardyBaldy();
        ResetHairWatch();
        if (false) { } else for (;;) {
            do { if (IsActiveThreadTerminating()) return _Fail_Val.Return(); } while (0);
            while (!Game.MsgOnRegionLoaded(regionName)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) {
                    // Inline-budget: retail 00E50CF9 calls the 66-byte CScriptThing destructor on this exit.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                    return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                }
            }
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            if (regionName == "BowerstoneSlumsWarehouses")
                break;
            Game.NewScriptFrame();
        }
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        beardyBaldyMan = Game.GetThingWithScriptName("BB_BeardyBaldyMan");
        if (beardyBaldyMan.IsAlive()) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            while (!beardyBaldyMan.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            if (!Phase3RequirementsComplete) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                Game.SetQuestCardObjective(Game.GetActiveQuestName(), "TEXT_QUEST_BEARDY_BALDY_OBJECTIVE_02", "", "");
            }
        }
        GoTalkToBarber();
        Game.NewScriptFrame();
    }
}