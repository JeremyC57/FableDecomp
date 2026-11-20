// NUISystem::CEditBox::CEditBox(long def) -- the changing-state base, the edit state cleared (max length 50), register with the UI manager
//   for key-down / char-down events, then take the char limit, IME, leading-space and password settings from the UI def
// Leaf: ??0CEditBox@NUISystem@@QAE@J@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x005407B0, thiscall, `ret 4`. Callees: CChangingStateComponent(long) 0x0052CC50, CManager::GetInstance
//   0x0041E5F2 (+ slot 2 AddObserver), CObserver::ObserveEvent 0x0052DA20 (x2, UI_EVENT_KEY_DOWN 33 / UI_EVENT_CHAR_DOWN 34),
//   CComponent::LoadAndReturnDef 0x00530EC0 (hidden result), the STLport list sentinels (malloc), the CDefPointer release.
// Identity (two sources): Ego_r.pdb / FableWin.pdb ??0CEditBox@NUISystem@@QAE@J@Z; the only caller CManager::CreateComponent 0x0041D21B (landed,
//   already binding this spelling) constructs the edit box here; the body stores CEditBox's vtables 0x01246B8C / 0x01246B64 / 0x01246B5C
//   and initialises exactly the engine/NUISystem_CEditBox.h members (CursorPos +0x168 ... MaxLength +0x180) and reads the CUIDef members
//   PasswordBox / EditBoxCharLimit / EditBoxUsesIME / DisallowSpaceAsFirstChar (engine/NUISystem_CUIDef.h).
// Was: EditBoxNative::EditBoxNative over local EditBoxBaseNative / ObserverNative / UIDefNative / ActionList models (base lists, a quarantine
//   CComponent under a renaming macro), binding 0x0052CC50 as ?InitializeChangingStateComponent@EditBoxBaseNative@@QAEXJ@Z
//   (rebuild/src/compiled/00/54/NUISystemCEditBox_CEditBox_005407b0.cpp).
// Header: engine/NUISystem_CEditBox.h, engine/NUISystem_CUIDef.h (FABLE_RETAIL_CUIDEF), engine/NUISystem_CManager.h, engine/CDefPointer.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CUIDEF
#define FABLE_RETAIL_STLP_LIST
#include "engine/CDefPointer.h"
#include "engine/NUISystem_CUIDef.h"
#include "engine/NUISystem_CEditBox.h"
#include "engine/NUISystem_CManager.h"
namespace NUISystem {
CEditBox::CEditBox(long def)
    : CChangingStateComponent(def)
    , CursorPos(0)
    , LeftMostCharacter(0)
    , RightMostCharacter(0)
    , Adjustment(0)
    , Active(true)
    , IsPasswordBox(false)
    , UsesIME(false)
    , DeleteAllOnKeyPress(false)
    , MaxLength(50)
{
    CManager::GetInstance()->AddObserver(this);
    ObserveEvent(UI_EVENT_KEY_DOWN);
    ObserveEvent(UI_EVENT_CHAR_DOWN);
    CDefPointer<const CUIDef> definition = LoadAndReturnDef();
    if (definition->EditBoxCharLimit > 0)
        MaxLength = definition->EditBoxCharLimit;
    UsesIME = definition->EditBoxUsesIME;
    DisallowSpaceAsFirstChar = definition->DisallowSpaceAsFirstChar;
    IsPasswordBox = definition->PasswordBox;
}
}
