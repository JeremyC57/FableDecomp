// CTCInventoryExperience::UndoBuyStat(EHeroTrainableStatType stat, bool confirmed) -- ask (yes/no dialog) whether to undo a stat purchase; once confirmed, drop the stat one level, give back its spent experience and forget the undo record when it is used up
// Leaf: ?UndoBuyStat@CTCInventoryExperience@@QAEXW4EHeroTrainableStatType@@_N@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00576ED1, 622 bytes, thiscall, `ret 8`; frame-pointer size TU (`optimize("y", off)` +
//   `("s", on)`, as the landed TU-mates 0x00576C39 / 0x0057713F). Identity: Ego_r.pdb / FableWin.pdb
//   ?UndoBuyStat@CTCInventoryExperience@@QAEXW4EHeroTrainableStatType@@_N@Z (public), right after UndoBuyAbility 0x00576C4F as in
//   Ego_r; it installs the callbacks ?EnsureUndoStat@@YIXXZ 0x005701DA (landed) / ?CancelUndoStat@@YIXXZ 0x0056E974.
// Shape: the UndoBuyAbility twin. Unconfirmed, the pending stat goes to 0x013B8AFC, the dialog (pYesNo +0x1C4) gets the yes callback, state 1, a centred
//   position (backbuffer centre minus 104 x 52 UI units; the y coordinate is computed first) and the no callback; dialog and mouse
//   pointer (+0x258) are parked in 0x013B8B04 / 0x013B8B00 and made the UI manager's concurrent exclusive observers, the pointer
//   observing mouse movement (event 0x19), the dialog everything (set up before the pointer here), and the dialog's two buttons mouse movement. Confirmed: the
//   screen is marked for rebuild (CreateBecauseOfUndo +0x214, DestroyEverything +0x21C), the count-down values saved, the stat
//   lowered one level (CTCHeroExperience::ModifyTrainableStatLevel(stat, -1) 0x006D6810), the record's experience reverted, and the record erased once its Generals list is empty.
// Lever: the x coordinate's first product is parenthesised: its PAREN tuple moves the `fsubp` one slot ahead of the `push ecx` of
//   the 0.0f argument (0x00576F6E), as in UndoBuyAbility 0x00576C4F.
// Globals: 0x013B8AFC / 0x013B8B04 / 0x013B8B00 are file statics with no PDB name; the spellings of the landed EnsureUndoStat
//   0x005701DA are kept (descriptive).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_CTCHEROEXPERIENCE
#define FABLE_RETAIL_EHEROTRAINABLESTATTYPE
#define FABLE_RETAIL_CTCHEROEXPERIENCE_MODIFYLEVEL
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE_UNDO
#define FABLE_RETAIL_CYESNO
#define FABLE_RETAIL_CYESNO_CALLBACKS
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_ERASE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCHeroExperience.h"
#include "engine/CTCInventoryExperience.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CYesNo.h"
#include "engine/NUISystem_CUIScreenUnitConverter.h"
#include "engine/C2DVector.h"
using namespace NUISystem;
void __fastcall EnsureUndoStat();                        // 0x005701DA
void __fastcall CancelUndoStat();                        // 0x0056E974
extern EHeroTrainableStatType g_UndoStat;                // 0x013B8AFC
extern CComponent*  g_pUndoYesNoDialog;                  // 0x013B8B04
extern CComponent*  g_pUndoMousePointer;                 // 0x013B8B00

void CTCInventoryExperience::UndoBuyStat(EHeroTrainableStatType stat, bool confirmed)
{
    if (!confirmed) {
        g_UndoStat = stat;
        pYesNo->SetYesCallBack(EnsureUndoStat);
        pYesNo->ChangeState(1);
        pYesNo->ChangePosition(C2DVector((CUIScreenUnitConverter::GetBackBufferWidth() * 0.5f) - CManager::GetInstance()->GetUIScale().X * 104.0f,
                                         CUIScreenUnitConverter::GetBackBufferHeight() * 0.5f - CManager::GetInstance()->GetUIScale().Y * 52.0f),
                               0.0f, false);
        pYesNo->SetNoCallBack(CancelUndoStat);
        g_pUndoYesNoDialog = pYesNo;
        g_pUndoMousePointer = PMousePointer;
        CManager::GetInstance()->RemoveObserver(g_pUndoYesNoDialog);
        CManager::GetInstance()->AddConcurrentExclusiveObserver(g_pUndoYesNoDialog);
        g_pUndoYesNoDialog->ObserveAllEvents();
        CManager::GetInstance()->RemoveObserver(g_pUndoMousePointer);
        CManager::GetInstance()->AddConcurrentExclusiveObserver(g_pUndoMousePointer);
        g_pUndoMousePointer->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
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
        std::map<EHeroTrainableStatType, SExpSpendUndo>::iterator it = UndoableStatBuys.find(stat);
        GetTCHeroExperience()->ModifyTrainableStatLevel(stat, -1);
        it->second.RevertExperiencePoints(GetTCHeroExperience());
        if (it->second.Generals.size() == 0)
            UndoableStatBuys.erase(it);
        DestroyEverything = true;
    }
}