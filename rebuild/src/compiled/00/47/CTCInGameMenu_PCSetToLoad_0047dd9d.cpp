// CTCInGameMenu::PCSetToLoad(long index) -- remember the save to load and set the PC yes/no dialog up as the "load this game?" question: yes/no callbacks, shown and centred, then made the active dialog with the mouse pointer and both buttons observed
// Leaf: ?PCSetToLoad@CTCInGameMenu@@QAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0047DD9D, 493 bytes, `ret 4`; frame-pointer size TU (`optimize("y", off)` + `("s", on)`).
// Locals (Ego_d): index (the parameter), pos (C2DVector), pclickable (one CComponent* reused for both buttons). FableWin.pdb debug body
//   0x01AB90C6: same calls; twin of PCSetQuitToFrontEnd 0x0047D296.
// Inferred: the centring brackets `(GetBackBufferWidth() * 0.5f) - ...` / `(GetBackBufferHeight() * 0.5f) - ...` are not shown by any donor
//   (FableWin divides by a double 2.0); both are byte-required here (no-code PAREN tuples move VC7.1's list scheduler: without X the
//   push ecx / fsubp swap and the third observer block's load order flip; without Y that flip alone). Cross-checked in the same TU:
//   SetToSave 0x0047ED76 and the X half in 0x0047E651 / 0x0047E9AE / 0x0047F30F. Owner review with SetToSave (advisor decision 2026-11-19).
#include "fable_compiler.h"
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_CYESNO
#define FABLE_RETAIL_CYESNO_CALLBACKS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINGAMEMENU
#define FABLE_RETAIL_CTCINGAMEMENU_SAVELOAD
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CYesNo.h"
#include "engine/NUISystem_CUIScreenUnitConverter.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCInGameMenu.h"

void __fastcall PCLoad();                              // 0x0047DC69
void __fastcall PCCancelQuitToFrontEnd();              // 0x0047D0D5
extern NUISystem::CYesNo*     g_pPCYesNoDialog;        // 0x013B87A8
extern NUISystem::CComponent* g_pPCMousePointer;       // 0x013B87A4

using namespace NUISystem;

void CTCInGameMenu::PCSetToLoad(long index)
{
    SaveGameToLoad = index;
    PYesNo->SetYesCallBack(PCLoad);
    PYesNo->ChangeState(1);
    C2DVector pos((CUIScreenUnitConverter::GetBackBufferWidth() * 0.5f) - CManager::GetInstance()->GetUIScale().X * 104.0f,
                  (CUIScreenUnitConverter::GetBackBufferHeight() * 0.5f) - CManager::GetInstance()->GetUIScale().Y * 52.0f);
    PYesNo->ChangePosition(pos, 0.0f, false);
    PYesNo->SetNoCallBack(PCCancelQuitToFrontEnd);
    g_pPCYesNoDialog = PYesNo;
    g_pPCMousePointer = PMousePointer;
    CManager::GetInstance()->RemoveObserver(g_pPCYesNoDialog);
    CManager::GetInstance()->AddConcurrentExclusiveObserver(g_pPCYesNoDialog);
    CManager::GetInstance()->RemoveObserver(g_pPCMousePointer);
    CManager::GetInstance()->AddConcurrentExclusiveObserver(g_pPCMousePointer);
    g_pPCMousePointer->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
    g_pPCYesNoDialog->ObserveAllEvents();
    CComponent* pclickable = PYesNo->FindChild("PC_YESNO_GAME_YESBUTTON");
    if (pclickable) {
        CManager::GetInstance()->AddConcurrentExclusiveObserver(pclickable);
        pclickable->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
    }
    pclickable = PYesNo->FindChild("PC_YESNO_GAME_NOBUTTON");
    if (pclickable) {
        CManager::GetInstance()->AddConcurrentExclusiveObserver(pclickable);
        pclickable->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
    }
}