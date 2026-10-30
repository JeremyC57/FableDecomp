// CTCInGameMenu::VideoOptionsChanged() -- shows (creating on first use) the "video options changed" OK dialog, gives it input focus, starts the revert timer and centres it
// Grade: DIFFER(752v752) -- one scheduling swap: retail pushes the ChangePosition float argument slot (push ecx) after the X fsubp, ours before (twins 0x0047E9AE, 0x0047F30F)
// Leaf: ?VideoOptionsChanged@CTCInGameMenu@@QAEXXZ
// Tried: C2DVector ctor/copy forms, temp orders, RTM+QFE (6 grades, 2026-10-23); chunk 537 (RTM): real PDB names throughout (NUISystem::CUIScreenUnitConverter::GetBackBufferWidth/Height, CManager::GetUIScale const, CComponent::ChangePosition(const C2DVector&, float, bool)), same-TU noinline GetBackBuffer*/GetUIScale bodies, a named temporary C2DVector, a dialog local, named x/y locals, `/ 2.0f`, negation forms and compound `-=`: all inert. | next: lane gap: scheduling -- VC7.1 RTM places the ChangePosition float-slot `push ecx` before the X `fsubp` for every source form tried; shared with 0x0047E651 / 0x0047E9AE / 0x0047F30F.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0047E651, 752 bytes; frame-pointer size TU (`optimize("y", off)` + `("s", on)`).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, NUISystem::CManager::GetInstance 0x0041E5F2
//   (__fastcall), CManager::CreateComponent(const CCharString&, bool) 0x0041DB1D, vector<CComponent*>::push_back 0x00486024,
//   operator new 0x00BFEA1A, CAction::CAction 0x0042BE50, CCountedPointer<CAction>(CAction*) 0x0042AA29 / dtor 0x00429160,
//   list<CActionParam*>::push_back 0x0042B646, CManager::GetUIScale 0x0041CF47, CUIScreenUnitConverter::GetBackBufferWidth /
//   GetBackBufferHeight 0x0041CC14 / 0x0041CC2B.
// Virtual slots (vtables 0x0124608C CComponent, 0x0122F884 CYesNo, 0x01249CCC CText, 0x01230134 CManager/CObservable):
//   FindChild 3, ChangePosition 31, Initialise 43, ChangeState(unsigned long) 48, AddAction 80, GetCurrentStateIndex 87,
//   SetYesCallBack 142 / SetNoCallBack 143, CText::SetValue 143-145, CObserver ObserveEvent 3 / ObserveAllEvents 6,
//   CObservable RemoveObserver 5 / AddConcurrentExclusiveObserver 9. GFGetTime 0x009E1BC0.
//   The OK/cancel callback is VideoOptionsChangedConfirmed 0x0047D9E3.
#include "engine/CCountedPointer.h"
#pragma optimize("y", off)
#pragma optimize("s", on)

class CCharStringData;
class CWideString;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();                                   // 0x0099EAE0
};
class C2DVector {
public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) { }
};
double __fastcall GFGetTime();                        // 0x009E1BC0
extern "C" void* __cdecl malloc(unsigned int size);
extern "C" void __cdecl free(void* block);
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size);
namespace std {
struct __true_type { };
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc> class list {       // STLport 4.6 (4 bytes)
public:
    void* _M_node;
    void push_back(const _Tp& __x);                   // 0x0042B646 (CActionParam*)
};
template <class _Tp, class _Alloc> class vector {     // STLport 4.6 (12 bytes)
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    void push_back(const _Tp& __x);                   // 0x00486024 (CComponent*)
protected:
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __true_type&, unsigned int __fill_len, bool __atend = false);   // 0x00485DBC
};
}

namespace NUISystem {
class CUIScreenUnitConverter {
public:
    static float __fastcall GetBackBufferWidth();     // 0x0041CC14
    static float __fastcall GetBackBufferHeight();    // 0x0041CC2B
};
enum EActionType { };
enum EActionParamType { };
class CActionParam {                                  // Ego_r.pdb 8 bytes
public:
    CActionParam(EActionParamType type) : ActionParamType(type) { }
    virtual ~CActionParam();
    EActionParamType ActionParamType;                 // +0x04
};
class CActionParamSLONG : public CActionParam {       // vtable 0x012366F4
public:
    CActionParamSLONG(long value) : CActionParam((EActionParamType)3), Value(value) { }
    virtual ~CActionParamSLONG();                     // only vtable slot: the deleting destructor 0x0047CEBD
    long Value;                                       // +0x08
};
class CAction {                                       // retail 0x10 bytes (STLport lists)
public:
    EActionType ActionType;                           // +0x00
    std::list<CActionParam*, std::allocator<CActionParam*> > UIActionParams;   // +0x04
    std::list<long, std::allocator<long> >          ThingActionParams;         // +0x08
    bool        Valid;                                // +0x0C
    CAction();                                        // 0x0042BE50
    void AddParam(CActionParam* param) { UIActionParams.push_back(param); }
};
enum EEvent { };
class CObserver {                                     // at +4 of every component
public:
    virtual void o00(); virtual void o01(); virtual void o02();
    virtual void ObserveEvent(EEvent event);          // slot 3 (+0x0C)
    virtual void o04(); virtual void o05();
    virtual void ObserveAllEvents();                  // slot 6 (+0x18)
};
class CObservable {                                   // CManager vtable 0x01230134
public:
    virtual void b00(); virtual void b01(); virtual void b02(); virtual void b03(); virtual void b04();
    virtual void RemoveObserver(CObserver* observer); // slot 5 (+0x14) 0x0055C9C0
    virtual void b06(); virtual void b07(); virtual void b08();
    virtual void AddConcurrentExclusiveObserver(CObserver* observer);   // slot 9 (+0x24) 0x0055CA90
};
#define V10(n) virtual void n##0(); virtual void n##1(); virtual void n##2(); virtual void n##3(); virtual void n##4(); \
    virtual void n##5(); virtual void n##6(); virtual void n##7(); virtual void n##8(); virtual void n##9();
class CUIInterface {
public:
    virtual void u00(); virtual void u01(); virtual void u02();
};
class CComponent : public CUIInterface, public CObserver {   // RTTI: CUIInterface@0, CObserver@4, CActionParam@24; vtable 0x0124608C
public:
    virtual CComponent* FindChild(CCharString name);  // slot 3 (+0x0C)
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    V10(v1) V10(v2)
    virtual void v30();
    virtual void ChangePosition(const C2DVector& position, float time, bool b);   // slot 31 (+0x7C)
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37();
    virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41(); virtual void v42();
    virtual void Initialise();                        // slot 43 (+0xAC)
    virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void ChangeState(unsigned long state);    // slot 48 (+0xC0)
    virtual void v49();
    V10(v5) V10(v6) V10(v7)
    virtual void AddAction(CCountedPointer<CAction>& action);   // slot 80 (+0x140)
    virtual void v81(); virtual void v82(); virtual void v83(); virtual void v84(); virtual void v85(); virtual void v86();
    virtual unsigned long GetCurrentStateIndex();     // slot 87 (+0x15C)
};
class CChangingStateComponent : public CComponent {
public:
    virtual void v88(); virtual void v89();
    V10(v9) V10(w0) V10(w1) V10(w2) V10(w3)
    virtual void w40(); virtual void w41();
};
class CYesNo : public CChangingStateComponent {       // vtable 0x0122F884
public:
    virtual void SetYesCallBack(void (__fastcall* callBack)());   // slot 142 (+0x238)
    virtual void SetNoCallBack(void (__fastcall* callBack)());    // slot 143 (+0x23C)
};
class CText : public CChangingStateComponent {        // vtable 0x01249CCC
public:
    virtual void w42();
    virtual void SetValue(const CCharString& value, bool a, bool b);                   // slot 145 (+0x244)
    virtual void SetValue(const CWideString& value, bool a, bool b);                   // slot 144 (+0x240)
    virtual void SetValue(unsigned long value, const CWideString& format, bool a);     // slot 143 (+0x23C)
};
class CManager : public CObservable {
public:
    CComponent* CreateComponent(const CCharString& name, bool b);   // 0x0041DB1D
    C2DVector GetUIScale() const;                     // 0x0041CF47
    static CManager* __fastcall GetInstance();        // 0x0041E5F2
};
}

#pragma auto_inline(off)
template <> __declspec(noinline) void std::vector<NUISystem::CComponent*, std::allocator<NUISystem::CComponent*> >::_M_insert_overflow(NUISystem::CComponent** __position, NUISystem::CComponent* const& __x, const std::__true_type&, unsigned int __fill_len, bool __atend)
{
    const unsigned int __old_size = this->_M_finish - this->_M_start;
    const unsigned int __len = __old_size + (__old_size < __fill_len ? __fill_len : __old_size);
    NUISystem::CComponent** __new_start = __len != 0 ? (NUISystem::CComponent**)malloc(__len * sizeof(NUISystem::CComponent*)) : 0;
    NUISystem::CComponent** __new_finish = (NUISystem::CComponent**)((char*)memmove(__new_start, this->_M_start, (char*)__position - (char*)this->_M_start) + ((char*)__position - (char*)this->_M_start));
    for (unsigned int __n = __fill_len; __n > 0; --__n, ++__new_finish)
        *__new_finish = __x;
    if (!__atend)
        __new_finish = (NUISystem::CComponent**)((char*)memmove(__new_finish, __position, (char*)this->_M_finish - (char*)__position) + ((char*)this->_M_finish - (char*)__position));
    if (this->_M_start != 0)
        free(this->_M_start);
    this->_M_start = __new_start;
    this->_M_finish = __new_finish;
    this->_M_end_of_storage = __new_start + __len;
}
template <> __declspec(noinline) void std::vector<NUISystem::CComponent*, std::allocator<NUISystem::CComponent*> >::push_back(NUISystem::CComponent* const& __x)
{
    if (this->_M_finish != this->_M_end_of_storage) {
        *this->_M_finish = __x;
        ++this->_M_finish;
    }
    else
        _M_insert_overflow(this->_M_finish, __x, std::__true_type(), 1UL, true);
}
#pragma auto_inline(on)

void __fastcall VideoOptionsChangedConfirmed();       // 0x0047D9E3

class CTCInGameMenu {                                 // retail layout (12-byte containers; Ego_r.pdb offsets +0x10..+0x24)
public:
    void*                  __vftable;                 // +0x00 (CTCBase)
    unsigned char          _pad_0x04[0x54];
    std::vector<NUISystem::CComponent*, std::allocator<NUISystem::CComponent*> > Components;   // +0x58 (PDB +0x68)
    unsigned char          _pad_0x64[0x4C];
    NUISystem::CYesNo*     PVideoOptionsNotAcceptedDialog;   // +0xB0 (PDB +0xD0)
    NUISystem::CYesNo*     PVideoOptionsChangedDialog;       // +0xB4 (PDB +0xD4)
    unsigned char          _pad_0xb8[0x08];
    double                 VideoChangesTimer;         // +0xC0 (PDB +0xE0)
    unsigned char          _pad_0xc8[0x24];
    NUISystem::CComponent* PMousePointer;             // +0xEC (PDB +0x110)
    void VideoOptionsChanged();
};

void CTCInGameMenu::VideoOptionsChanged()
{
    if (!PVideoOptionsChangedDialog) {
        NUISystem::CComponent* dialog = NUISystem::CManager::GetInstance()->CreateComponent("PC_DIALOG_GAME_OPTIONS_CHANGED", false);
        dialog->Initialise();
        Components.push_back(dialog);
        PVideoOptionsChangedDialog = (NUISystem::CYesNo*)dialog;
        PVideoOptionsChangedDialog->SetYesCallBack(VideoOptionsChangedConfirmed);
        PVideoOptionsChangedDialog->SetNoCallBack(VideoOptionsChangedConfirmed);
        NUISystem::CComponent* okButton = PVideoOptionsChangedDialog->FindChild(CCharString("PC_OK_GAME_OKBUTTON", -1));
        if (okButton) {
            CCountedPointer<NUISystem::CAction> action(new NUISystem::CAction);
            action->ActionType = (NUISystem::EActionType)0xE7;
            action->AddParam(new NUISystem::CActionParamSLONG(4));
            okButton->AddAction(action);
        }
    } else {
        NUISystem::CText* text = (NUISystem::CText*)PVideoOptionsChangedDialog->FindChild(CCharString("PC_GAME_OPTIONS_CHANGED_TEXT", -1));
        if (text)
            text->SetValue("TEXT_GUI_OPTIONS_CHANGED_SUCCESSFULLY", true, true);
    }
    if (PVideoOptionsChangedDialog->GetCurrentStateIndex() != 1) {
        PVideoOptionsChangedDialog->ChangeState(1);
        NUISystem::CComponent* okButton = PVideoOptionsChangedDialog->FindChild(CCharString("PC_OK_GAME_OKBUTTON", -1));
        if (okButton) {
            NUISystem::CManager::GetInstance()->AddConcurrentExclusiveObserver(okButton);
            okButton->ObserveEvent((NUISystem::EEvent)0x19);
        }
        NUISystem::CManager::GetInstance()->RemoveObserver(PVideoOptionsChangedDialog);
        NUISystem::CManager::GetInstance()->AddConcurrentExclusiveObserver(PVideoOptionsChangedDialog);
        NUISystem::CManager::GetInstance()->RemoveObserver(PMousePointer);
        NUISystem::CManager::GetInstance()->AddConcurrentExclusiveObserver(PMousePointer);
        PMousePointer->ObserveEvent((NUISystem::EEvent)0x19);
        PVideoOptionsChangedDialog->ObserveAllEvents();
        VideoChangesTimer = GFGetTime();
    }
    C2DVector pos(NUISystem::CUIScreenUnitConverter::GetBackBufferWidth() * 0.5f - NUISystem::CManager::GetInstance()->GetUIScale().X * 204.0f,
                  NUISystem::CUIScreenUnitConverter::GetBackBufferHeight() * 0.5f - NUISystem::CManager::GetInstance()->GetUIScale().Y * 52.0f);
    PVideoOptionsChangedDialog->ChangePosition(pos, 0.0f, false);
}
