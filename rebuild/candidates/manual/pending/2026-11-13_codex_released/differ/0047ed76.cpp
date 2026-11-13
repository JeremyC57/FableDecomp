// CTCInGameMenu::SetToSave(long) -- show the pending save dialog and record its time.
// Grade: DIFFER(366v366) -- push ecx at float argument reservation precedes fsubp; retail reverses
// Retail: 0x0047ED76, 366 bytes; thiscall.
// Leaf: ?SetToSave@CTCInGameMenu@@QAEXJ@Z
#ifdef _MSC_VER
#pragma optimize("y", off)
#pragma optimize("s", on)
#endif
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINGAMEMENU
#define FABLE_RETAIL_CTCINGAMEMENU_SAVELOAD
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCInGameMenu.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CText.h"
#include "engine/NUISystem_CUIScreenUnitConverter.h"
#include "engine/CQuestManager.h"
double FABLE_FASTCALL GFGetTime();
using namespace NUISystem;
void CTCInGameMenu::SetToSave(long save_index)
{
 SaveGameIndex=save_index;
 FramesToWaitToSave=2;
 Components.push_back(CManager::GetInstance()->CreateComponent("PC_SAVE_IN_PROGRESS_DIALOG",false));
 if(CQuestManager::Get().IsBetweenQuestStartAndFinishScreens()) {
  CComponent* ptext=Components[Components.size()-1]->FindChild(CCharString("PC_SAVE_IN_PROGRESS_TEXT",-1));
  if(ptext) {
   CText* ptext_real=static_cast<CText*>(ptext);
   ptext_real->SetValue("TEXT_GUI_SAVE_HERO_IN_PROGRESS_PC",true,true);
  }
 }
 Components[Components.size()-1]->Initialise();
 PSaveInProgress=Components[Components.size()-1];
 C2DVector pos(CUIScreenUnitConverter::GetBackBufferWidth()*0.5f-CManager::GetInstance()->GetUIScale().X*204.0f,
               CUIScreenUnitConverter::GetBackBufferHeight()*0.5f-CManager::GetInstance()->GetUIScale().Y*52.0f);
 PSaveInProgress->ChangePosition(pos,0.0f,false);
 SaveInProgressTime=GFGetTime();
}
