// 00896b60 CGameScriptInterface::KickOffQuestStartScreen
// Flags:
// Leaf: ?KickOffQuestStartScreen@CGameScriptInterface@@UBEXABVCCharString@@_N1@Z
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CASUSPENDABLEPROCESS_ACTIVE_PASS27
#define FABLE_RETAIL_TUTORIAL_CATEGORIES_PASS26
#define FABLE_RETAIL_CTCQUESTCOMPLETIONUI_GLOBALUPDATE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CTCHERO_TUTORIAL
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CQUESTMANAGER_STARTSCREEN
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_AUTOSAVEQUESTSTART
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "engine/ETutorialCategory.h"
#include "engine/ThingAlive.h"
#include "engine/CTCHero.h"
#include "engine/CTCQuestCompletionUI.h"
#include "engine/CQuestManager.h"
#include "engine/CWorld.h"
#include "engine/CASuspendableProcess.h"
#include "engine/CGameScriptInterface.h"

namespace NGlobalConsole { extern bool GEnableRegionLockingSaveSystem; }

void CGameScriptInterface::KickOffQuestStartScreen(const CCharString& script_name, bool display_screen,
    bool pretend_screen_has_been_displayed_for_save_load_purposes) const
{
    if (!display_screen) {
        CTCQuestCompletionUI* ptc_ui;
        PlayerManager->GetMainPlayer().GetPControlledCreature()->GetTC(&ptc_ui);
        ptc_ui->SaveInitialValues(script_name);
    }
    if (display_screen) {
        CTCQuestCompletionUI* ptc_ui;
        PlayerManager->GetMainPlayer().GetPControlledCreature()->GetTC(&ptc_ui);
        ptc_ui->Activate(CTCQuestCompletionUI::QUEST_START, script_name, true, L"", true);
        if (IsHeroControlledByPlayer()) {
            while (ptc_ui->IsActive() && !ptc_ui->IsQuitting() && !ptc_ui->IsQuittingFadeOut() &&
                   !CASuspendableProcess::GetPActiveProcess()->IsSuspendableProcessStopping())
                NewScriptFrame();
            if (CASuspendableProcess::GetPActiveProcess()->IsSuspendableProcessStopping())
                return;
        }
        CThingPlayerCreature* phero = PlayerManager->GetMainPlayer().GetPControlledCreature();
        if (IS_THING_ALIVE(phero)) {
            CTCHero* ptc_hero = 0;
            phero->GetTC(TCI_HERO, &ptc_hero);
            if (ptc_hero)
                ptc_hero->GiveTutorialForCategory(TUTORIAL_CATEGORY_QUEST_CARD, false);
        }
    } else if (pretend_screen_has_been_displayed_for_save_load_purposes) {
        if (NGlobalConsole::GEnableRegionLockingSaveSystem) {
            CQuestManager::Get().OnQuestStartScreenDisplayed(script_name);
            World->AutoSaveQuestStart();
        }
    }
}