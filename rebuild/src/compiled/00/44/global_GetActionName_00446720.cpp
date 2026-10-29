// _global::GetActionName(const CActionInputControl& control, CWideString& name, bool shortName) -- the display name of an action's bound control: localised mouse-button / special-key text, the keyboard's own key name, or a fallback; true when a real name was produced
// Leaf: ?GetActionName@@
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00446720, 732 bytes of code + the 12-entry mouse-button jump table at 0x004469FC
// (row 784), `ret 4`: /Gr __fastcall (control in ecx, name in edx, shortName on the stack) as the landed callers declare it
// (NUISystem::CKeyRedefiner::UpdateKeyText 0x00557A10, NPlayerGui::CDrawQuestInfo::AddTick 0x0064FB47).
// Callees: CCharString() 0x0099E4B0 / operator=(const char*) 0x0099EFE0 / operator!=(const char*) 0x0099E960 / dtor 0x0099EAE0,
// NGameText::CDataBank::GetTextBySymbol 0x009C95E0 (game bank g_pMainGameComponent 0x013B86A0 +0x14, else the front end's
// 0x013B871C +0x60), CWideString::operator=(const CWideString&) 0x0099B7D0 / operator=(const wchar_t*) 0x0099B800 /
// operator!=(const wchar_t*) 0x0099B150 / dtor 0x0099B510, GFGetSystemManager 0x009A4EC0 -> PInputManager (+0x58) ->
// PKeyboard (+0x28) -> GetKeyName (vtable slot 4; ?GetKeyName@CKeyboard@@UBE_NW4EInputKey@@AAVCWideString@@@Z).
// Layout: Ego_r.pdb CActionInputControl (ControllerType +4, KeyboardKey +8, MouseButton +0x10), enums EControllerType /
// EMouseButtonControl (PC adds buttons 4..8 as 8..12 after MOUSE_WHEEL_MOVEMENT_DOWN); keys are DirectInput codes.
// Shape: switch on the controller type in enum order (VC7.1 lays out the mouse case as the dec-chain fall-through, then the
// keyboard case, the shared `name = L"NULL"; return false;` and the Xbox case last); the keyboard tag lives in the dead
// shortName slot; every mouse case's `tag = ...` call is cross-jumped into one.  Jump-table pairing verified by hand:
// 1 LMB, 2 RMB, 3 MMB, 4 MOVEMENT, 5 WHEEL, 6 WHEEL_UP, 7 WHEEL_DOWN, 8..12 MB4..MB8 -> the same block offsets as retail.
#include <stddef.h>
typedef unsigned short wchar16;          // wchar_t is not a built-in type under the retail flags

class CCharString {
public:
    void* PStringData;
    CCharString();                                      // 0x0099E4B0
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const char* text);     // 0x0099EFE0
    bool operator!=(const char* text) const;            // 0x0099E960
};
class CWideString {
public:
    void* PStringData;
    ~CWideString();                                     // 0x0099B510
    const CWideString& operator=(const CWideString& other);   // 0x0099B7D0
    const CWideString& operator=(const wchar16* text);        // 0x0099B800
    bool operator!=(const wchar16* text) const;         // 0x0099B150
};

namespace NGameText {
class CDataBank {
public:
    const CWideString GetTextBySymbol(const CCharString& symbol) const;   // 0x009C95E0
};
}
class CMainGameComponent { public:
    unsigned char _pad00[0x14];
    NGameText::CDataBank* PGameTextDataBank;            // +0x14
    NGameText::CDataBank* GetGameTextDataBank() const { return PGameTextDataBank; }
};
class CNewFrontendGameComponent { public:
    unsigned char _pad00[0x60];
    NGameText::CDataBank* PGameTextDataBank;            // +0x60
    NGameText::CDataBank* GetGameTextDataBank() const { return PGameTextDataBank; }
};
extern CMainGameComponent* g_pMainGameComponent;                 // 0x013B86A0
extern CNewFrontendGameComponent* g_pNewFrontendGameComponent;   // 0x013B871C

class CKeyboard {
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual bool GetKeyName(long key, CWideString& name) const;   // slot 4 (CKeyboardDX 0x00AB6180)
};
class CInputManager { public:
    unsigned char _pad00[0x28];
    CKeyboard*    PKeyboard;                            // +0x28 (scoped_ptr<CKeyboard>)
};
class CSystemManager { public:
    unsigned char  _pad00[0x58];
    CInputManager* PInputManager;                       // +0x58
};
CSystemManager* GFGetSystemManager();                   // 0x009A4EC0

enum EControllerType { CONTROLLER_NONE = 0, CONTROLLER_XBOX_PAD = 1, CONTROLLER_KEYBOARD = 2, CONTROLLER_MOUSE = 3 };
enum EMouseButtonControl {
    MOUSE_BUTTON_NULL_CONTROL = 0, MOUSE_BUTTON_LEFT_CONTROL = 1, MOUSE_BUTTON_RIGHT_CONTROL = 2, MOUSE_BUTTON_MIDDLE_CONTROL = 3,
    MOUSE_MOVEMENT = 4, MOUSE_WHEEL_MOVEMENT = 5, MOUSE_WHEEL_MOVEMENT_UP = 6, MOUSE_WHEEL_MOVEMENT_DOWN = 7,
    MOUSE_BUTTON_4_CONTROL = 8, MOUSE_BUTTON_5_CONTROL = 9, MOUSE_BUTTON_6_CONTROL = 10, MOUSE_BUTTON_7_CONTROL = 11,
    MOUSE_BUTTON_8_CONTROL = 12
};
enum EInputKey { KEY_NULL = 0, KEY_LSHIFT = 0x2a, KEY_RSHIFT = 0x36, KEY_SPACE = 0x39 };

class CActionInputControl { public:                           // 28 bytes (Ego_r.pdb)
    long  GameAction;                                   // +0x00
    long  ControllerType;                               // +0x04
    long  KeyboardKey;                                  // +0x08
    long  XboxButton;                                   // +0x0c
    long  MouseButton;                                  // +0x10
    float ControlDirection[2];                          // +0x14
};

bool GetActionName(const CActionInputControl& control, CWideString& name, bool shortName)
{
    switch (control.ControllerType) {
    case CONTROLLER_XBOX_PAD:
        name = (const wchar16*)L"Assigned to Xbox pad - bugger";
        return false;
    case CONTROLLER_KEYBOARD:
        if (control.KeyboardKey != KEY_NULL) {
            CCharString tag;
            switch (control.KeyboardKey) {
            case KEY_LSHIFT: tag = "TEXT_GUI_KEY_LEFTSHIFT"; break;
            case KEY_RSHIFT: tag = "TEXT_GUI_KEY_RIGHTSHIFT"; break;
            case KEY_SPACE:  tag = "TEXT_GUI_KEY_SPACE"; break;
            }
            if (tag != "") {
                if (g_pMainGameComponent)
                    name = g_pMainGameComponent->GetGameTextDataBank()->GetTextBySymbol(tag);
                else if (g_pNewFrontendGameComponent)
                    name = g_pNewFrontendGameComponent->GetGameTextDataBank()->GetTextBySymbol(tag);
                return true;
            }
            if (GFGetSystemManager()->PInputManager->PKeyboard->GetKeyName(control.KeyboardKey, name))
                return true;
        }
        break;
    case CONTROLLER_MOUSE: {
        CCharString tag;
        switch (control.MouseButton) {
        case MOUSE_BUTTON_LEFT_CONTROL:   tag = shortName ? "TEXT_GUI_CONTROL_LMB_ABBREVIATED" : "TEXT_GUI_CONTROL_LMB"; break;
        case MOUSE_BUTTON_RIGHT_CONTROL:  tag = shortName ? "TEXT_GUI_CONTROL_RMB_ABBREVIATED" : "TEXT_GUI_CONTROL_RMB"; break;
        case MOUSE_BUTTON_MIDDLE_CONTROL: tag = shortName ? "TEXT_GUI_CONTROL_MMB_ABBREVIATED" : "TEXT_GUI_CONTROL_MMB"; break;
        case MOUSE_BUTTON_4_CONTROL:      tag = shortName ? "TEXT_GUI_CONTROL_MB4_ABBREVIATED" : "TEXT_GUI_CONTROL_MB4"; break;
        case MOUSE_BUTTON_5_CONTROL:      tag = shortName ? "TEXT_GUI_CONTROL_MB5_ABBREVIATED" : "TEXT_GUI_CONTROL_MB5"; break;
        case MOUSE_BUTTON_6_CONTROL:      tag = shortName ? "TEXT_GUI_CONTROL_MB6_ABBREVIATED" : "TEXT_GUI_CONTROL_MB6"; break;
        case MOUSE_BUTTON_7_CONTROL:      tag = shortName ? "TEXT_GUI_CONTROL_MB7_ABBREVIATED" : "TEXT_GUI_CONTROL_MB7"; break;
        case MOUSE_BUTTON_8_CONTROL:      tag = shortName ? "TEXT_GUI_CONTROL_MB8_ABBREVIATED" : "TEXT_GUI_CONTROL_MB8"; break;
        case MOUSE_MOVEMENT:              tag = "TEXT_GUI_CONTROL_MOUSE_MOVEMENT"; break;
        case MOUSE_WHEEL_MOVEMENT:        tag = "TEXT_GUI_CONTROL_MOUSE_WHEEL"; break;
        case MOUSE_WHEEL_MOVEMENT_UP:     tag = "TEXT_GUI_CONTROL_MOUSE_WHEEL_UP"; break;
        case MOUSE_WHEEL_MOVEMENT_DOWN:   tag = "TEXT_GUI_CONTROL_MOUSE_WHEEL_DOWN"; break;
        default:                          tag = ""; break;
        }
        if (tag != "") {
            if (g_pMainGameComponent)
                name = g_pMainGameComponent->GetGameTextDataBank()->GetTextBySymbol(tag);
            else if (g_pNewFrontendGameComponent)
                name = g_pNewFrontendGameComponent->GetGameTextDataBank()->GetTextBySymbol(tag);
            if (name != (const wchar16*)L"")
                return true;
        }
        return false;
    }
    }
    name = (const wchar16*)L"NULL";
    return false;
}