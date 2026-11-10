// CTCInventoryExperience::UndoBuyAbility(EHeroAbility ability, bool confirmed) -- ask (yes/no dialog) whether to undo an ability purchase; once confirmed, drop the ability one level, give back its spent experience and forget the undo record when it is used up
// Leaf: ?UndoBuyAbility@CTCInventoryExperience@@QAEXW4EHeroAbility@@_N@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00576C4F, 642 bytes, thiscall, `ret 8`; frame-pointer size TU (`optimize("y", off)` +
//   `("s", on)`, as the landed TU-mates 0x00576C39 / 0x0057713F). Identity: Ego_r.pdb / FableWin.pdb
//   ?UndoBuyAbility@CTCInventoryExperience@@QAEXW4EHeroAbility@@_N@Z (public); it installs the callbacks ?EnsureUndoAbility@@YIXXZ
//   0x00570009 / ?CancelUndoAbility@@YIXXZ 0x0056E866 (landed), which call it back with `confirmed`; UndoBuyStat 0x00576ED1 follows.
// Shape: unconfirmed, the pending ability goes to 0x013B8B08, the dialog (pYesNo +0x1C4) gets the yes callback, state 1, a centred
//   position (backbuffer centre minus 104 x 52 UI units; the y coordinate is computed first) and the no callback; dialog and mouse
//   pointer (+0x258) are parked in 0x013B8B04 / 0x013B8B00 and made the UI manager's concurrent exclusive observers, the pointer
//   observing mouse movement (event 0x19), the dialog everything, and the dialog's two buttons mouse movement. Confirmed: the
//   screen is marked for rebuild (CreateBecauseOfUndo +0x214, DestroyEverything +0x21C), the count-down values saved, the ability
//   lowered one level, the record's experience reverted, and the record erased once its Generals list is empty.
// Lever: the x coordinate's first product is parenthesised: its PAREN tuple moves the `fsubp` one slot ahead of the `push ecx` of
//   the 0.0f argument (0x00576CEC); the same lever fixes UndoBuyStat 0x00576ED1 and OpenInventoryInternalPC 0x00577AC0.
// Globals: 0x013B8B08 / 0x013B8B04 / 0x013B8B00 are file statics with no PDB name; the spellings of the landed EnsureUndoAbility
//   0x00570009 are kept (descriptive).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_CTCHEROEXPERIENCE
#define FABLE_RETAIL_EHEROTRAINABLESTATTYPE
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE_UNDO
#define FABLE_RETAIL_CTCINVENTORYABILITIES
#define FABLE_RETAIL_CTCINVENTORYABILITIES_SETLEVEL
#define FABLE_RETAIL_CTCINVENTORYABILITIES_MAXLEVEL_FIRST
#define FABLE_RETAIL_CYESNO
#define FABLE_RETAIL_CYESNO_CALLBACKS
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_ERASE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCHeroExperience.h"
#include "engine/CTCInventoryExperience.h"
#include "engine/CTCInventoryAbilities.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CYesNo.h"
#include "engine/NUISystem_CUIScreenUnitConverter.h"
#include "engine/C2DVector.h"
using namespace NUISystem;
void __fastcall EnsureUndoAbility();                     // 0x00570009
void __fastcall CancelUndoAbility();                     // 0x0056E866
extern EHeroAbility g_UndoAbility;                       // 0x013B8B08
extern CComponent*  g_pUndoYesNoDialog;                  // 0x013B8B04
extern CComponent*  g_pUndoMousePointer;                 // 0x013B8B00

void CTCInventoryExperience::UndoBuyAbility(EHeroAbility ability, bool confirmed)
{
    if (!confirmed) {
        g_UndoAbility = ability;
        pYesNo->SetYesCallBack(EnsureUndoAbility);
        pYesNo->ChangeState(1);
        pYesNo->ChangePosition(C2DVector((CUIScreenUnitConverter::GetBackBufferWidth() * 0.5f) - CManager::GetInstance()->GetUIScale().X * 104.0f,
                                         CUIScreenUnitConverter::GetBackBufferHeight() * 0.5f - CManager::GetInstance()->GetUIScale().Y * 52.0f),
                               0.0f, false);
        pYesNo->SetNoCallBack(CancelUndoAbility);
        g_pUndoYesNoDialog = pYesNo;
        g_pUndoMousePointer = PMousePointer;
        CManager::GetInstance()->RemoveObserver(g_pUndoYesNoDialog);
        CManager::GetInstance()->AddConcurrentExclusiveObserver(g_pUndoYesNoDialog);
        CManager::GetInstance()->RemoveObserver(g_pUndoMousePointer);
        CManager::GetInstance()->AddConcurrentExclusiveObserver(g_pUndoMousePointer);
        g_pUndoMousePointer->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
        g_pUndoYesNoDialog->ObserveAllEvents();
        CComponent* yes = pYesNo->FindChild("PC_YESNO_GAME_YESBUTTON");
        if (yes) {
            CManager::GetInstance()->AddConcurrentExclusiveObserver(yes);
            yes->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
        }
        CComponent* no = pYesNo->FindChild("PC_YESNO_GAME_NOBUTTON");
        if (no) {
            CManager::GetInstance()->AddConcurrentExclusiveObserver(no);
            no->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
        }
    } else {
        CreateBecauseOfUndo = true;
        SaveExperienceValuesForCountDown();
        std::map<EHeroAbility, SExpSpendUndo>::iterator it = UndoableAbilityBuys.find(ability);
        GetTCInventoryAbilities()->SetAbilityLevelTo(ability, GetTCInventoryAbilities()->GetAbilityLevel(ability) - 1);
        it->second.RevertExperiencePoints(GetTCHeroExperience());
        if (it->second.Generals.size() == 0)
            UndoableAbilityBuys.erase(it);
        DestroyEverything = true;
    }
}