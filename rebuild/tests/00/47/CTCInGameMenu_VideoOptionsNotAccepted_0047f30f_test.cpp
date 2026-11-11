// TEST: CTCInGameMenu::VideoOptionsNotAccepted() -- shows (creating on first use) the "video options not accepted" OK dialog, gives it input focus and centres it
// Behaviour test for 0x0047F30F: the row source below (pragmas dropped, unnamed filler virtual slots given empty bodies),
// then recording stubs for every callee and a main that drives each path. C++03; builds with VC7.1 /Od and g++.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifndef _MSC_VER
#define __fastcall
#endif
#define FABLE_FORCEINLINE inline
// CTCInGameMenu::VideoOptionsNotAccepted() -- shows (creating on first use) the "video options not accepted" OK dialog, gives it input focus and centres it
// Leaf: ?VideoOptionsNotAccepted@CTCInGameMenu@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0047F30F, 636 bytes; frame-pointer size TU (`optimize("y", off)` + `("s", on)`).
// Shape: the X centre term is written `(GetBackBufferWidth() * 0.5f) - scale.X * K`; the redundant parentheses add a no-code PAREN
//   tuple that moves the float-argument slot `push ecx` below the X `fsubp`, as retail (the unparenthesised form pushes it first).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CFrontEndManager::GetInstance 0x0041E5F2
// (__fastcall), CManager::CreateComponent(const CCharString&, bool) 0x0041DB1D, vector<CComponent*>::push_back 0x00486024,
// operator new 0x00BFEA1A, CAction::CAction 0x0042BE50, CCountedPointer<CAction>(CAction*) 0x0042AA29 / dtor 0x00429160,
// list<CActionParam*>::push_back 0x0042B646, CManager::GetUIScale 0x0041CF47, screen width/height 0x0041CC14/0x0041CC2B,
// GFGetTime 0x009E1BC0. The OK callback is 0x0047E941 (manifest label VideoOptionsChangedConfirmed is propagated).
// Layout: retail CTCInGameMenu (12-byte containers; engine/CTCInGameMenu.h carries PDB offsets, as the landed
// ConfirmedVideoOptionsChanged 0x0047D012): Components +0x58, PVideoOptionsChangedDialog +0xB4, VideoChangesTimer +0xC0,
// PMousePointer +0xEC. The action parameter is NUISystem::CActionParamSLONG (vtable 0x012366F4, type 3, 12 bytes).

class CCharStringData;
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();                                   // 0x0099EAE0
};
class C2DVector { public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) { }
};
namespace NUISystem {
class CUIScreenUnitConverter { public:                // Ego_r static __fastcall getters
    static float __fastcall GetBackBufferWidth();     // 0x0041CC14
    static float __fastcall GetBackBufferHeight();    // 0x0041CC2B
};
}
double __fastcall GFGetTime();                        // 0x009E1BC0

namespace NUISystem {
enum EEvent { UI_EVENT_MOUSE_MOVEMENT = 25 };
enum EActionType { UI_ACTION_TYPE_SEND_EVENT = 231 };
class CActionParam { public:
    long Type;                                        // +0x04
    CActionParam(long type) : Type(type) { }
    virtual void a00() {}
};
class CActionParamSLONG : public CActionParam { public:             // vtable 0x012366F4
    long Value;                                       // +0x08
    CActionParamSLONG(long value) : CActionParam(3), Value(value) { }
    virtual void a00() {}
};
class CActionParamList { public:                             // STLport list<CActionParam*>
    void* _M_node;
    void push_back(CActionParam* const& value);       // 0x0042B646
};
class CAction { public:                                      // retail 0x10 bytes
    long             ActionType;                      // +0x00
    CActionParamList UIActionParams;                  // +0x04
    void*            ThingActionParams;               // +0x08
    bool             Valid;                           // +0x0C
    CAction();                                        // 0x0042BE50
};
struct SCountBlock;
}
template <class T> class CCountedPointer { public:             // test model of engine/CCountedPointer.h
    T* Data; NUISystem::SCountBlock* Info;
    CCountedPointer(T* data) : Data(data), Info(0) { }
    ~CCountedPointer() { delete Data; }
    T* operator->() const { return Data; }
};
namespace NUISystem {

#define V10(n) virtual void n##0() {} virtual void n##1() {} virtual void n##2() {} virtual void n##3() {} virtual void n##4() {} \
    virtual void n##5() {} virtual void n##6() {} virtual void n##7() {} virtual void n##8() {} virtual void n##9() {}
class CObserver { public:                                    // the observer interface at +4 of every component
    virtual void o00() {} virtual void o01() {} virtual void o02() {}
    virtual void ObserveEvent(long eventType);        // slot 3 (+0x0C)
    virtual void o04() {} virtual void o05() {}
    virtual void ObserveAllEvents();                   // slot 6 (+0x18): Ego_r ?ObserveAllEvents@CObserver@NUISystem@@UAEXXZ (the no-argument slot after IgnoreAllEvents)
};
typedef void (__fastcall* TCallback)();
struct CUIInterface {
    virtual void v00() {} virtual void v01() {} virtual void v02() {}
    virtual class CComponent* FindChild(CCharString name);   // slot 3 (+0x0C)
    virtual void v04() {} virtual void v05() {} virtual void v06() {} virtual void v07() {} virtual void v08() {} virtual void v09() {}
    V10(v1) V10(v2)
    virtual void v30() {}
    virtual void ChangePosition(const C2DVector& pos, float time, long mode);   // slot 31 (+0x7C)
    virtual void v32() {} virtual void v33() {} virtual void v34() {} virtual void v35() {} virtual void v36() {} virtual void v37() {}
    virtual void v38() {} virtual void v39() {} virtual void v40() {} virtual void v41() {} virtual void v42() {}
    virtual void Initialise();                        // slot 43 (+0xAC)
    virtual void v44() {} virtual void v45() {} virtual void v46() {} virtual void v47() {}
    virtual void ChangeState(bool active);              // slot 48 (+0xC0)
    virtual void v49() {}
    V10(v5) V10(v6) V10(v7)
    virtual void AddActionOnLeftUnclicked(CCountedPointer<CAction>& action);   // slot 80 (+0x140)
    virtual void v81() {} virtual void v82() {} virtual void v83() {} virtual void v84() {} virtual void v85() {} virtual void v86() {}
    virtual long GetTargetStateIndex();                          // slot 87 (+0x15C)
    virtual void v88() {} virtual void v89() {}
    V10(v9) V10(w0) V10(w1) V10(w2) V10(w3)
    virtual void w40() {} virtual void w41() {}
    virtual void SetYesCallBack(TCallback callback);   // slot 142 (+0x238)
    virtual void SetNoCallBack(TCallback callback);   // slot 143 (+0x23C)
    virtual void w44() {}
    virtual void SetValue(const CCharString& text, bool a, bool b);   // slot 145 (+0x244)
};
class CComponent : public CUIInterface, public CObserver { public:
};
class CYesNo : public CComponent { public:
};
class CText : public CComponent { public:
};
class CManager { public:
    virtual void m00() {} virtual void m01() {} virtual void m02() {} virtual void m03() {} virtual void m04() {}
    virtual void RemoveObserver(CObserver* observer); // slot 5 (+0x14): CObservable vtable order of the landed 0x0047D0D5
    virtual void m06() {} virtual void m07() {} virtual void m08() {}
    virtual void AddConcurrentExclusiveObserver(CObserver* observer);   // slot 9 (+0x24)
    CComponent* CreateComponent(const CCharString& definition, bool b);   // 0x0041DB1D
    C2DVector GetUIScale() const;                           // 0x0041CF47
    static CManager* __fastcall GetInstance();        // 0x0041E5F2
};
}

struct CComponentVector {                             // retail std::vector<NUISystem::CComponent*> (12 bytes)
    NUISystem::CComponent** m_first;
    NUISystem::CComponent** m_last;
    NUISystem::CComponent** m_end;
    void push_back(NUISystem::CComponent* const& value);   // 0x00486024
};
void CComponentVector::push_back(NUISystem::CComponent* const& value)
{
    if (m_last != m_end)
        *m_last++ = value;
}

void __fastcall VideoOptionsNotAcceptedConfirmed();   // 0x0047E941

class CTCBase { public:                             // the thing-component base: vptr +0x00 (4-byte aligned base sub-object)
    virtual void GlobalUpdate(bool);                  // Ego_r ?GlobalUpdate@CTCInGameMenu@@UAEX_N@Z overrides it
};
class CTCInGameMenu : public CTCBase { public:
    unsigned char          _pad_0x04[0x54];
    CComponentVector       Components;                // +0x58
    unsigned char          _pad_0x64[0x4C];
    NUISystem::CYesNo*     PVideoOptionsNotAcceptedDialog;   // +0xB0
    NUISystem::CYesNo*     PVideoOptionsChangedDialog;       // +0xB4
    unsigned char          _pad_0xb8[0x08];
    double                 VideoChangesTimer;         // +0xC0
    unsigned char          _pad_0xc8[0x24];
    NUISystem::CComponent* PMousePointer;             // +0xEC
    void VideoOptionsNotAccepted();
};


void __fastcall VideoOptionsNotAcceptedConfirmed();   // 0x0047E941

using namespace NUISystem;

void CTCInGameMenu::VideoOptionsNotAccepted()
{
    if (!PVideoOptionsNotAcceptedDialog) {
        CComponent* dialog = CManager::GetInstance()->CreateComponent("PC_DIALOG_GAME_OPTIONS_NOT_ACCEPTED", false);
        dialog->Initialise();
        Components.push_back(dialog);
        PVideoOptionsNotAcceptedDialog = static_cast<CYesNo*>(dialog);
        PVideoOptionsNotAcceptedDialog->SetYesCallBack(VideoOptionsNotAcceptedConfirmed);
        PVideoOptionsNotAcceptedDialog->SetNoCallBack(VideoOptionsNotAcceptedConfirmed);
        CComponent* okButton = PVideoOptionsNotAcceptedDialog->FindChild("PC_OK_GAME_OKBUTTON");
        if (okButton) {
            CCountedPointer<CAction> action(new CAction);
            action->ActionType = UI_ACTION_TYPE_SEND_EVENT;
            action->UIActionParams.push_back(new CActionParamSLONG(4));
            okButton->AddActionOnLeftUnclicked(action);
        }
    }
    PVideoOptionsNotAcceptedDialog->ChangeState(1);
    CComponent* okButton = PVideoOptionsNotAcceptedDialog->FindChild("PC_OK_GAME_OKBUTTON");
    if (okButton) {
        CManager::GetInstance()->AddConcurrentExclusiveObserver(okButton);
        okButton->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
    }
    CManager::GetInstance()->RemoveObserver(PVideoOptionsNotAcceptedDialog);
    CManager::GetInstance()->AddConcurrentExclusiveObserver(PVideoOptionsNotAcceptedDialog);
    CManager::GetInstance()->RemoveObserver(PMousePointer);
    CManager::GetInstance()->AddConcurrentExclusiveObserver(PMousePointer);
    PMousePointer->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
    PVideoOptionsNotAcceptedDialog->ObserveAllEvents();
    C2DVector pos((CUIScreenUnitConverter::GetBackBufferWidth() * 0.5f) - CManager::GetInstance()->GetUIScale().X * 204.0f,
                  CUIScreenUnitConverter::GetBackBufferHeight() * 0.5f - CManager::GetInstance()->GetUIScale().Y * 52.0f);
    PVideoOptionsNotAcceptedDialog->ChangePosition(pos, 0.0f, false);
}

// ---------------- test stubs ----------------
#define EXPECTED_CALLBACK VideoOptionsNotAcceptedConfirmed
static char g_log[4096];
static int g_bad, g_live;
static void Log(const char* s) { if (strlen(g_log) + strlen(s) < sizeof(g_log) - 1) strcat(g_log, s); else g_bad = 1; }
class CCharStringData { public: char Text[80]; };
CCharString::CCharString(const char* text, long length) { if (length != -1) g_bad = 1; PStringData = new CCharStringData; strncpy(PStringData->Text, text, 79); PStringData->Text[79] = 0; ++g_live; }
CCharString::CCharString(const CCharString& other) { PStringData = new CCharStringData(*other.PStringData); ++g_live; }
CCharString::~CCharString() { delete PStringData; --g_live; }
static float g_screenW = 800.0f, g_screenH = 600.0f;
float __fastcall NUISystem::CUIScreenUnitConverter::GetBackBufferWidth() { return g_screenW; }
float __fastcall NUISystem::CUIScreenUnitConverter::GetBackBufferHeight() { return g_screenH; }
static double g_time = 100.0;
double __fastcall GFGetTime() { return g_time; }
static NUISystem::CYesNo g_dialog, g_button, g_mouse, g_text;
static NUISystem::CComponent* g_created = &g_dialog;
static NUISystem::CComponent* g_child = &g_button;
static const char* Name(const void* p)
{
    if (p == (NUISystem::CUIInterface*)&g_dialog || p == (NUISystem::CObserver*)&g_dialog) return "D";
    if (p == (NUISystem::CUIInterface*)&g_button || p == (NUISystem::CObserver*)&g_button) return "B";
    if (p == (NUISystem::CUIInterface*)&g_mouse || p == (NUISystem::CObserver*)&g_mouse) return "M";
    if (p == (NUISystem::CUIInterface*)&g_text || p == (NUISystem::CObserver*)&g_text) return "T";
    return "?";
}
static char g_buf[256];
namespace NUISystem {
void CActionParamList::push_back(CActionParam* const& value)
{
    sprintf(g_buf, "Param(%ld,%ld);", value->Type, ((CActionParamSLONG*)value)->Value); Log(g_buf);
    delete (CActionParamSLONG*)value;
}
CAction::CAction() : ActionType(0), ThingActionParams(0), Valid(true) { UIActionParams._M_node = 0; }
void CObserver::ObserveEvent(long eventType) { sprintf(g_buf, "Observe(%s,%ld);", Name(this), eventType); Log(g_buf); }
void CObserver::ObserveAllEvents() { sprintf(g_buf, "All(%s);", Name(this)); Log(g_buf); }
CComponent* CUIInterface::FindChild(CCharString name) { sprintf(g_buf, "Find(%s,%s);", Name(this), name.PStringData->Text); Log(g_buf); return g_child; }
void CUIInterface::ChangePosition(const C2DVector& pos, float time, long mode) { sprintf(g_buf, "Pos(%s,%g,%g,%g,%ld);", Name(this), pos.X, pos.Y, time, mode); Log(g_buf); }
void CUIInterface::Initialise() { sprintf(g_buf, "Init(%s);", Name(this)); Log(g_buf); }
void CUIInterface::ChangeState(bool active) { sprintf(g_buf, "Active(%s,%d);", Name(this), (int)active); Log(g_buf); }
void CUIInterface::AddActionOnLeftUnclicked(CCountedPointer<CAction>& action) { sprintf(g_buf, "Action(%s,%ld);", Name(this), action->ActionType); Log(g_buf); }
void CUIInterface::SetValue(const CCharString& text, bool a, bool b) { sprintf(g_buf, "Text(%s,%s,%d,%d);", Name(this), text.PStringData->Text, (int)a, (int)b); Log(g_buf); }
static long g_state = 0;
long CUIInterface::GetTargetStateIndex() { return g_state; }
void CUIInterface::SetYesCallBack(TCallback callback) { sprintf(g_buf, "Ok(%s,%d);", Name(this), callback == EXPECTED_CALLBACK); Log(g_buf); }
void CUIInterface::SetNoCallBack(TCallback callback) { sprintf(g_buf, "Cancel(%s,%d);", Name(this), callback == EXPECTED_CALLBACK); Log(g_buf); }
static CManager g_manager;
CManager* __fastcall CManager::GetInstance() { return &g_manager; }
void CManager::RemoveObserver(CObserver* observer) { sprintf(g_buf, "Remove(%s);", Name(observer)); Log(g_buf); }
void CManager::AddConcurrentExclusiveObserver(CObserver* observer) { sprintf(g_buf, "Concurrent(%s);", Name(observer)); Log(g_buf); }
CComponent* CManager::CreateComponent(const CCharString& definition, bool b) { sprintf(g_buf, "Create(%s,%d);", definition.PStringData->Text, (int)b); Log(g_buf); return g_created; }
C2DVector CManager::GetUIScale() const { return C2DVector(2.0f, 0.5f); }
}
void CTCBase::GlobalUpdate(bool) { }
void __fastcall EXPECTED_CALLBACK() { }
static NUISystem::CComponent* g_store[4];
static void InitMenu(CTCInGameMenu& menu)
{
    menu.Components.m_first = menu.Components.m_last = g_store;
    menu.Components.m_end = g_store + 4;
    menu.PVideoOptionsNotAcceptedDialog = 0;
    menu.PVideoOptionsChangedDialog = 0;
    menu.VideoChangesTimer = -1.0;
    menu.PMousePointer = &g_mouse;
}
static bool Check(const char* what, const char* expected)
{
    bool ok = strcmp(g_log, expected) == 0 && !g_bad && g_live == 0;
    if (!ok) printf("%s\n  got      %s\n  expected %s\n", what, g_log, expected);
    g_log[0] = 0;
    return ok;
}
int main()
{
    bool ok = true;
    CTCInGameMenu menu;
    InitMenu(menu);
    menu.VideoOptionsNotAccepted();   // first use: create, wire the callbacks and the OK action, then show
    ok = Check("create", "Create(PC_DIALOG_GAME_OPTIONS_NOT_ACCEPTED,0);Init(D);Ok(D,1);Cancel(D,1);Find(D,PC_OK_GAME_OKBUTTON);"
        "Param(3,4);Action(B,231);Active(D,1);Find(D,PC_OK_GAME_OKBUTTON);Concurrent(B);Observe(B,25);Remove(D);Concurrent(D);"
        "Remove(M);Concurrent(M);Observe(M,25);All(D);Pos(D,-8,274,0,0);") && ok;
    ok = ok && menu.PVideoOptionsNotAcceptedDialog == &g_dialog && menu.Components.m_last == g_store + 1 && g_store[0] == &g_dialog;
    g_child = 0;                      // second use: no creation; no OK button found
    menu.VideoOptionsNotAccepted();
    ok = Check("reuse", "Active(D,1);Find(D,PC_OK_GAME_OKBUTTON);Remove(D);Concurrent(D);Remove(M);Concurrent(M);Observe(M,25);All(D);"
        "Pos(D,-8,274,0,0);") && ok;
    ok = ok && menu.Components.m_last == g_store + 1;
    puts(ok ? "0047f30f_TEST PASS" : "0047f30f_TEST FAIL");
    return ok ? 0 : 1;
}