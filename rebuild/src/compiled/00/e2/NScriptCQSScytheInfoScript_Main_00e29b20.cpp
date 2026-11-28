// 00e29b20 NScript::CQS_ScytheInfoScript::Main
// Leaf: ?Main@CQS_ScytheInfoScript@NScript@@UAEXXZ
// Flags:
#define FABLE_RETAIL_SCYTHE_INFO_PASS30
#define FABLE_RETAIL_HANGING_TIMER_INLINES_PASS30
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CTIMER_ORIGINAL_ACCESS
#define FABLE_RETAIL_CCHARSTRING
#include "engine/TRetVoid.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CRGBColour.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQS_ScytheInfoScript.h"
#include "engine/CEntityScriptBinding.h"
#include "engine/CTimer.h"

void NScript::CQS_ScytheInfoScript::Main()
{
    TRetVoid _Fail_Val;
    AddEntityScriptBinding(new CEntityScriptBinding<CQS_ScytheInfoScript>("ScytheMarker", this, &CScytheMarker::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQS_ScytheInfoScript>("ScytheNearOracle", this, &CScytheNearOracle::Alloc, 1));
    PostAddScriptedEntities();
    Game.PostAddScriptedEntities();
    Game.SetQuestCardObjective(Game.GetActiveQuestName(), "TEXT_QUEST_AWAKEN_ORACLE_OBJECTIVE_03", "", "NorthernWastes3");
    CTimer SnowspireReminder;
    SnowspireReminder = 100;
    while (Game.IsRegionLoaded("NorthernWastes2")) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        if (SnowspireReminder == 0) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            Game.HeroReceiveMessageFromGuildMaster("TEXT_QST_B03_SCYTHE_RETURN_TO_SNOWSPIRE_REMINDER_10", "", true, true);
            SnowspireReminder = 100;
        }
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    while (!MissionSucceeded) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    Game.SetQuestAsCompleted(Game.GetActiveQuestName(), false, false, false);
    Game.DeactivateQuestLater(Game.GetActiveQuestName(), 0);
    Game.FadeScreenIn();
}