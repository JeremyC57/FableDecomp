// CGameScriptInterface::KickOffQuestStartScreen(const CCharString& questName, bool showScreen, bool autoSave) const -- without the screen just store the quest card's initial values in the hero's quest completion UI (then, when asked and quest screens are enabled, record the quest as started and autosave); with it activate the quest start screen and, while the hero is player-controlled, yield script frames until it closes (stopping with the script runner); then give the hero the quest-start tutorial (category 0x1C)
// Grade: DIFFER(437v445) -- shared-header rebase (new CTCQuestCompletionUI.h; CQuestManager OnQuestStartScreenDisplayed / CWorld AutoSaveQuestStart opt-ins): every instruction matches (relocation-masked); only the exit-block placement differs: retail places the merged epilogue after the !showScreen arm (0x00896BE6) and every later exit jumps back to it with near jumps while the tutorial tail keeps a duplicated epilogue; VC7.1 puts ours at the function end (short jumps, 8 bytes shorter). 24 return-structure probes (if/else, early returns, `||` splits, arm order, inline/forceinline qualified GiveHeroTutorial body) all keep it at the end.
// Leaf: ?KickOffQuestStartScreen@CGameScriptInterface@@UBEXABVCCharString@@_N1@Z
// Callees (PDB spellings): CPlayerManager::GetMainPlayer() 0x00449970, CPlayer::GetPControlledCreature() const 0x00487DC0,
//   CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (TCI_QUEST_COMPLETION_UI 252, TCI_HERO 41),
//   CTCQuestCompletionUI::SaveInitialValues 0x005E4860 / Activate 0x005E4740, g_bQuestFinishScreensEnabled 0x01375741,
//   CQuestManager::OnQuestStartScreenDisplayed 0x004B2FE0 (singleton 0x013B89FC), CWorld::AutoSaveQuestStart 0x004A0CD0,
//   CWideString(const wchar_t*) 0x0099B6B0 / ~CWideString 0x0099B510 (the implicit L"" temporary), own slots 394
//   IsHeroControlledByPlayer / 7 NewScriptFrame, g_pScriptRunner 0x013D2838 (StopRunning +5), CTCHero::GiveTutorialForCategory 0x006C9FA0.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00896B60, 445 bytes (+ int3 padding), thiscall `ret 0Ch`.
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
#include "engine/CTCHero.h"
#include "engine/CTCQuestCompletionUI.h"
#include "engine/CQuestManager.h"
#include "engine/CWorld.h"
#include "engine/CSuspendableProcessMicrothread.h"
#include "engine/CGameScriptInterface.h"

extern bool g_bQuestFinishScreensEnabled;                                // 0x01375741
extern CSuspendableProcessMicrothread* g_pScriptRunner;                  // 0x013D2838

void CGameScriptInterface::KickOffQuestStartScreen(const CCharString& questName, bool showScreen, bool autoSave) const
{
    if (!showScreen) {
        CTCQuestCompletionUI* ui;
        PlayerManager->GetMainPlayer().GetPControlledCreature()->GetTC(&ui);
        ui->SaveInitialValues(questName);
        if (autoSave && g_bQuestFinishScreensEnabled) {
            CQuestManager::Get().OnQuestStartScreenDisplayed(questName);
            World->AutoSaveQuestStart();
        }
    }
    else {
        CTCQuestCompletionUI* ui;
        PlayerManager->GetMainPlayer().GetPControlledCreature()->GetTC(&ui);
        ui->Activate((CTCQuestCompletionUI::CompletionType)0, questName, true, L"", true);
        if (IsHeroControlledByPlayer()) {
            while (ui->IsActive() && !ui->IsQuitting() && !ui->IsQuittingFadeOut() && !g_pScriptRunner->StopRunning)
                NewScriptFrame();
            if (g_pScriptRunner->StopRunning)
                return;
        }
        CThingPlayerCreature* hero = PlayerManager->GetMainPlayer().GetPControlledCreature();
        CTCHero* tc;
        if (hero && !hero->IsDead() && hero->GetTC(TCI_HERO, &tc) && tc)
            tc->GiveTutorialForCategory((ETutorialCategory)0x1c, false);
    }
}
