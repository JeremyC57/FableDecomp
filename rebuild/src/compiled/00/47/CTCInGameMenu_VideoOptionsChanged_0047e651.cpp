// CTCInGameMenu::VideoOptionsChanged() -- shows (creating on first use) the "video options changed" OK dialog, gives it input focus and centres it
// Leaf: ?VideoOptionsChanged@CTCInGameMenu@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0047E651, 752 bytes; frame-pointer size TU (`optimize("y", off)` + `("s", on)`).
// Shape: the X centre term is written `(GetBackBufferWidth() * 0.5f) - scale.X * K`; the redundant parentheses add a no-code PAREN
//   tuple that moves the float-argument slot `push ecx` below the X `fsubp`, as retail (the unparenthesised form pushes it first).
// Callees: ??0CCharString@@QAE@PBDJ@Z 0x0099EBF0 / ~CCharString 0x0099EAE0, NUISystem::CManager::GetInstance 0x0041E5F2,
//   CManager::CreateComponent(const CCharString&, bool) 0x0041DB1D, vector<NUISystem::CComponent*>::push_back 0x00486024,
//   operator new 0x00BFEA1A, NUISystem::CAction::CAction 0x0042BE50, CCountedPointer<NUISystem::CAction>(CAction*) 0x0042AA29 /
//   ReleaseData 0x00429160, list<NUISystem::CActionParam*>::push_back 0x0042B646, CManager::GetUIScale 0x0041CF47,
//   CUIScreenUnitConverter::GetBackBufferWidth / Height 0x0041CC14 / 0x0041CC2B, GFGetTime 0x009E1BC0; virtuals: CComponent
//   FindChild (3), Initialise (43), ChangePosition (31), ChangeState(unsigned long) (48), AddActionOnLeftUnclicked (80),
//   GetTargetStateIndex (87); CYesNo SetYesCallBack / SetNoCallBack (142 / 143); CText::SetValue(const CCharString&,bool,bool) (145);
//   CObserver ObserveEvent (3) / ObserveAllEvents (6); CObservable RemoveObserver (5) / AddConcurrentExclusiveObserver (9).
//   The callback is VideoOptionsChangedConfirmed 0x0047D9E3 (landed).
#include "fable_compiler.h"
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_CYESNO
#define FABLE_RETAIL_CYESNO_CALLBACKS
#define FABLE_RETAIL_CACTIONPARAMSLONG
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINGAMEMENU
#define FABLE_RETAIL_CTCINGAMEMENU_SAVELOAD
#define FABLE_RETAIL_CTCINGAMEMENU_VIDEODIALOGS
#include "stlport/fable_containers.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CYesNo.h"
#include "engine/NUISystem_CText.h"
#include "engine/NUISystem_CAction.h"
#include "engine/NUISystem_CActionParamSLONG.h"
#include "engine/NUISystem_CUIScreenUnitConverter.h"
#include "engine/GFGetTime.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCInGameMenu.h"


void __fastcall VideoOptionsChangedConfirmed();       // 0x0047D9E3

using namespace NUISystem;

void CTCInGameMenu::VideoOptionsChanged()
{
    if (!PVideoOptionsChangedDialog) {
        CComponent* dialog = CManager::GetInstance()->CreateComponent("PC_DIALOG_GAME_OPTIONS_CHANGED", false);
        dialog->Initialise();
        Components.push_back(dialog);
        PVideoOptionsChangedDialog = static_cast<CYesNo*>(dialog);
        PVideoOptionsChangedDialog->SetYesCallBack(VideoOptionsChangedConfirmed);
        PVideoOptionsChangedDialog->SetNoCallBack(VideoOptionsChangedConfirmed);
        CComponent* okButton = PVideoOptionsChangedDialog->FindChild("PC_OK_GAME_OKBUTTON");
        if (okButton) {
            CCountedPointer<CAction> action(new CAction);
            action->ActionType = UI_ACTION_TYPE_SEND_EVENT;
            action->UIActionParams.push_back(new CActionParamSLONG(4));
            okButton->AddActionOnLeftUnclicked(action);
        }
    } else {
        CText* text = static_cast<CText*>(PVideoOptionsChangedDialog->FindChild("PC_GAME_OPTIONS_CHANGED_TEXT"));
        if (text)
            text->SetValue("TEXT_GUI_OPTIONS_CHANGED_SUCCESSFULLY", true, true);
    }
    if (PVideoOptionsChangedDialog->GetTargetStateIndex() != 1) {
        PVideoOptionsChangedDialog->ChangeState(1);
        CComponent* okButton = PVideoOptionsChangedDialog->FindChild("PC_OK_GAME_OKBUTTON");
        if (okButton) {
            CManager::GetInstance()->AddConcurrentExclusiveObserver(okButton);
            okButton->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
        }
        CManager::GetInstance()->RemoveObserver(PVideoOptionsChangedDialog);
        CManager::GetInstance()->AddConcurrentExclusiveObserver(PVideoOptionsChangedDialog);
        CManager::GetInstance()->RemoveObserver(PMousePointer);
        CManager::GetInstance()->AddConcurrentExclusiveObserver(PMousePointer);
        PMousePointer->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
        PVideoOptionsChangedDialog->ObserveAllEvents();
        VideoChangesTimer = GFGetTime();
    }
    C2DVector pos((CUIScreenUnitConverter::GetBackBufferWidth() * 0.5f) - CManager::GetInstance()->GetUIScale().X * 204.0f,
                  CUIScreenUnitConverter::GetBackBufferHeight() * 0.5f - CManager::GetInstance()->GetUIScale().Y * 52.0f);
    PVideoOptionsChangedDialog->ChangePosition(pos, 0.0f, false);
}