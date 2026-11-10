// CTCInGameMenu::PCSetToLoad(long) -- ask to load the selected save and route pointer input to its dialog.
// Grade: DIFFER(493v493) -- two instruction schedule swaps: final coordinate push/fsubp and third observer global/vtable load
// Retail: 0x0047DD9D, 493 bytes; thiscall.
// Leaf: ?PCSetToLoad@CTCInGameMenu@@QAEXJ@Z
#ifdef _MSC_VER
#pragma optimize("y",off)
#pragma optimize("s",on)
#endif
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINGAMEMENU
#define FABLE_RETAIL_CTCINGAMEMENU_SAVELOAD
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CYESNO
#define FABLE_RETAIL_CYESNO_CALLBACKS
#include "engine/ETCInterfaceType.h"
#include "engine/CTCInGameMenu.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CYesNo.h"
#include "engine/NUISystem_CUIScreenUnitConverter.h"
void FABLE_FASTCALL PCLoad();
void FABLE_FASTCALL PCCancelQuitToFrontEnd();
extern NUISystem::CComponent* g_pPCMousePointer;
extern NUISystem::CYesNo* g_pPCYesNoDialog;
using namespace NUISystem;
void CTCInGameMenu::PCSetToLoad(long index)
{
 SaveGameToLoad=index;
 PYesNo->SetYesCallBack(PCLoad);
 PYesNo->ChangeState(UI_STATE_ACTIVE);
 C2DVector pos(CUIScreenUnitConverter::GetBackBufferWidth()*0.5f-CManager::GetInstance()->GetUIScale().X*104.0f,
               CUIScreenUnitConverter::GetBackBufferHeight()*0.5f-CManager::GetInstance()->GetUIScale().Y*52.0f);
 PYesNo->ChangePosition(pos,0.0f,false);
 PYesNo->SetNoCallBack(PCCancelQuitToFrontEnd);
 g_pPCYesNoDialog=PYesNo;
 g_pPCMousePointer=PMousePointer;
 CManager::GetInstance()->RemoveObserver(g_pPCYesNoDialog);
 CManager::GetInstance()->AddConcurrentExclusiveObserver(g_pPCYesNoDialog);
 CManager::GetInstance()->RemoveObserver(g_pPCMousePointer);
 CManager::GetInstance()->AddConcurrentExclusiveObserver(g_pPCMousePointer);
 g_pPCMousePointer->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
 g_pPCYesNoDialog->ObserveAllEvents();
 CComponent* pclickable=0;
 pclickable=PYesNo->FindChild("PC_YESNO_GAME_YESBUTTON");
 if(pclickable) {
  CManager::GetInstance()->AddConcurrentExclusiveObserver(pclickable);
  pclickable->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
 }
 pclickable=PYesNo->FindChild("PC_YESNO_GAME_NOBUTTON");
 if(pclickable) {
  CManager::GetInstance()->AddConcurrentExclusiveObserver(pclickable);
  pclickable->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
 }
}
