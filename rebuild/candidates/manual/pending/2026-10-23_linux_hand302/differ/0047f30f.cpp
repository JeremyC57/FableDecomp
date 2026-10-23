// CTCInGameMenu::VideoOptionsNotAccepted() -- shows (creating on first use) the "video options not accepted" OK dialog, gives it input focus and centres it
// Grade: DIFFER(636v636) -- one scheduling swap: retail pushes the float argument slot (push ecx) after the x fsubp, ours before (same as twin 0x0047E651)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0047F30F, 636 bytes; frame-pointer size TU (`optimize("y", off)` + `("s", on)`).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CFrontEndManager::GetInstance 0x0041E5F2
// (__fastcall), CManager::CreateComponent(const CCharString&, bool) 0x0041DB1D, vector<CComponent*>::push_back 0x00486024,
// operator new 0x00BFEA1A, CAction::CAction 0x0042BE50, CCountedPointer<CAction>(CAction*) 0x0042AA29 / dtor 0x00429160,
// list<CActionParam*>::push_back 0x0042B646, CManager::GetUIScale 0x0041CF47, screen width/height 0x0041CC14/0x0041CC2B,
// GFGetTime 0x009E1BC0. The OK callback is 0x0047E941 (manifest label VideoOptionsChangedConfirmed is propagated).
// Layout: retail CTCInGameMenu (12-byte containers; engine/CTCInGameMenu.h carries PDB offsets, as the landed
// ConfirmedVideoOptionsChanged 0x0047D012): Components +0x58, PVideoOptionsChangedDialog +0xB4, VideoChangesTimer +0xC0,
// PMousePointer +0xEC. The action parameter is NUISystem::CActionParamSLONG (vtable 0x012366F4, type 3, 12 bytes).
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CCharStringData;
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();                                   // 0x0099EAE0
};
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) { }
};
float GetScreenWidth();                               // 0x0041CC14
float GetScreenHeight();                              // 0x0041CC2B
double __fastcall GFGetTime();                        // 0x009E1BC0

namespace NUISystem {
struct CActionParam {
    long Type;                                        // +0x04
    CActionParam(long type) : Type(type) { }
    virtual void a00();
};
struct CActionParamSLONG : CActionParam {             // vtable 0x012366F4
    long Value;                                       // +0x08
    CActionParamSLONG(long value) : CActionParam(3), Value(value) { }
    virtual void a00();
};
struct CActionParamList {                             // STLport list<CActionParam*>
    void* _M_node;
    void push_back(CActionParam* const& value);       // 0x0042B646
};
struct CAction {                                      // retail 0x10 bytes
    long             ActionType;                      // +0x00
    CActionParamList UIActionParams;                  // +0x04
    void*            ThingActionParams;               // +0x08
    bool             Valid;                           // +0x0C
    CAction();                                        // 0x0042BE50
    void AddUIActionParam(CActionParam* param) { UIActionParams.push_back(param); }
};
struct SCountBlock;
struct CActionPointer {                               // CCountedPointer<CAction>
    CAction*     m_pObject;
    SCountBlock* m_pCount;
    CActionPointer(CAction* object);                  // 0x0042AA29
    ~CActionPointer();                                // 0x00429160
    CAction* operator->() const { return m_pObject; }
};

#define V10(n) virtual void n##0(); virtual void n##1(); virtual void n##2(); virtual void n##3(); virtual void n##4(); \
    virtual void n##5(); virtual void n##6(); virtual void n##7(); virtual void n##8(); virtual void n##9();
struct CObserver {                                    // the observer interface at +4 of every component
    virtual void o00(); virtual void o01(); virtual void o02();
    virtual void ObserveEvent(long eventType);        // slot 3 (+0x0C)
    virtual void o04(); virtual void o05();
    virtual void TakeFocus_slot6();                   // slot 6 (+0x18)
};
typedef void (__fastcall* TCallback)();
struct CComponentBase {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual struct CComponent* FindChild(CCharString name);   // slot 3 (+0x0C)
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    V10(v1) V10(v2)
    virtual void v30();
    virtual void SetPosition(const C2DVector& pos, float time, long mode);   // slot 31 (+0x7C)
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37();
    virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41(); virtual void v42();
    virtual void Initialise();                        // slot 43 (+0xAC)
    virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void SetActive(bool active);              // slot 48 (+0xC0)
    virtual void v49();
    V10(v5) V10(v6) V10(v7)
    virtual void AddAction(const CActionPointer& action);   // slot 80 (+0x140)
    virtual void v81(); virtual void v82(); virtual void v83(); virtual void v84(); virtual void v85(); virtual void v86();
    virtual long GetState();                          // slot 87 (+0x15C)
    virtual void v88(); virtual void v89();
    V10(v9) V10(w0) V10(w1) V10(w2) V10(w3)
    virtual void w40(); virtual void w41();
    virtual void SetOkCallback(TCallback callback);   // slot 142 (+0x238)
    virtual void SetCancelCallback(TCallback callback);   // slot 143 (+0x23C)
    virtual void w44();
    virtual void SetText(const CCharString& text, bool a, bool b);   // slot 145 (+0x244)
};
struct CComponent : CComponentBase, CObserver {
};
struct CYesNo : CComponent {
};
struct CManager {
    virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03(); virtual void m04();
    virtual void AddObserver(CObserver* observer);    // slot 5 (+0x14)
    virtual void m06(); virtual void m07(); virtual void m08();
    virtual void SetFocus(CObserver* observer);       // slot 9 (+0x24)
    CComponent* CreateComponent(const CCharString& definition, bool b);   // 0x0041DB1D
    C2DVector GetUIScale();                           // 0x0041CF47
    static CManager* __fastcall GetInstance();        // 0x0041E5F2
};
}

struct CComponentVector {                             // retail std::vector<NUISystem::CComponent*> (12 bytes)
    NUISystem::CComponent** m_first;
    NUISystem::CComponent** m_last;
    NUISystem::CComponent** m_end;
    void push_back(NUISystem::CComponent* const& value);   // 0x00486024
};
#pragma auto_inline(off)
__declspec(noinline) void CComponentVector::push_back(NUISystem::CComponent* const& value)
{
    if (m_last != m_end)
        *m_last++ = value;
}
#pragma auto_inline(on)

void __fastcall VideoOptionsNotAcceptedConfirmed();   // 0x0047E941

struct CTCInGameMenu {
    void*                  __vftable;                 // +0x00
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

void CTCInGameMenu::VideoOptionsNotAccepted()
{
    if (!PVideoOptionsNotAcceptedDialog) {
        NUISystem::CComponent* dialog = NUISystem::CManager::GetInstance()->CreateComponent("PC_DIALOG_GAME_OPTIONS_NOT_ACCEPTED", false);
        dialog->Initialise();
        Components.push_back(dialog);
        PVideoOptionsNotAcceptedDialog = (NUISystem::CYesNo*)dialog;
        PVideoOptionsNotAcceptedDialog->SetOkCallback(VideoOptionsNotAcceptedConfirmed);
        PVideoOptionsNotAcceptedDialog->SetCancelCallback(VideoOptionsNotAcceptedConfirmed);
        NUISystem::CComponent* okButton = PVideoOptionsNotAcceptedDialog->FindChild(CCharString("PC_OK_GAME_OKBUTTON", -1));
        if (okButton) {
            NUISystem::CActionPointer action(new NUISystem::CAction);
            action->ActionType = 0xE7;
            action->AddUIActionParam(new NUISystem::CActionParamSLONG(4));
            okButton->AddAction(action);
        }
    }
    PVideoOptionsNotAcceptedDialog->SetActive(true);
    NUISystem::CComponent* okButton = PVideoOptionsNotAcceptedDialog->FindChild(CCharString("PC_OK_GAME_OKBUTTON", -1));
    if (okButton) {
        NUISystem::CManager::GetInstance()->SetFocus(okButton);
        okButton->ObserveEvent(0x19);
    }
    NUISystem::CManager::GetInstance()->AddObserver(PVideoOptionsNotAcceptedDialog);
    NUISystem::CManager::GetInstance()->SetFocus(PVideoOptionsNotAcceptedDialog);
    NUISystem::CManager::GetInstance()->AddObserver(PMousePointer);
    NUISystem::CManager::GetInstance()->SetFocus(PMousePointer);
    PMousePointer->ObserveEvent(0x19);
    PVideoOptionsNotAcceptedDialog->TakeFocus_slot6();
    C2DVector pos(GetScreenWidth() * 0.5f - NUISystem::CManager::GetInstance()->GetUIScale().X * 204.0f,
                  GetScreenHeight() * 0.5f - NUISystem::CManager::GetInstance()->GetUIScale().Y * 52.0f);
    PVideoOptionsNotAcceptedDialog->SetPosition(pos, 0.0f, 0);
}
