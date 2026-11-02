// CVirtualKeyboard::~CVirtualKeyboard() -- deactivate the entry edit box, stop it observing events and deregister it from the UI manager; then the ProfileName string is destroyed
// Leaf: ??1CVirtualKeyboard@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00851720, 65 bytes (int3 padding to 0x00851770), non-virtual, tail `jmp ~CWideString`.
// Identity: callers are CFrontEndManager::DestroyVirtualKeyboard 0x00594FA9 (`delete VirtualKeyboard` = direct dtor call + operator
//   delete: the class has no virtual dtor) and the deleting-destructor COMDAT 0x00594F78 (`call ; test [esp+8],1 ; operator delete`);
//   no vtable store. Ego_r.pdb ??1CVirtualKeyboard@@QAE@XZ in the donor order ??0 / ??1 / Initialize / Cancel / GetProfileName / Confirm
//   (0x00851800 / 820 / 870 / 970 / 9A0 / A30) = retail 0x00851700 / 0x00851720 / 0x00851770 / 0x00851860 / 0x00851890 / 0x00851920
//   (the five neighbours are landed under those names); FableWin.pdb lists ??1CVirtualKeyboard@@QAE@XZ too. The catalog label is right.
// Callees: NUISystem::CEditBox::SetActive(bool) (slot 150, +0x258; Ego_r ?SetActive@CEditBox@NUISystem@@UAEX_N@Z, retail 0x005401B0 right
//   after SetText 0x00540180 as in the donor order), NUISystem::CObserver::IgnoreAllEvents() (slot 5 of the CObserver vtable 0x01246020 /
//   the edit box's CObserver vtable 0x01246B64, 0x0052D9A0 clears the observed-event set; Ego_r ?IgnoreAllEvents@CObserver@NUISystem@@UAEXXZ),
//   NUISystem::CManager::GetInstance() 0x0041E5F2 (?GetInstance@CManager@NUISystem@@SIPAV12@XZ), NUISystem::CObservable::RemoveObserver
//   (slot 5, +0x14, of the CManager vtable 0x01230134 = 0x0055C9C0; Ego_r ?RemoveObserver@CObservable@NUISystem@@UAEXPAVCObserver@2@@Z; RTTI
//   CManager : CObservable), ~CWideString 0x0099B510 (ProfileName at +0, the implicit member destruction).
// Shape: the reverse of Initialize 0x00851770 (SetActive(true), ObserveEvent, AddObserver(PEntryBox)): the CObserver base sits at +4 of
//   the edit box (Ego_r CComponent : CUIInterface, CObserver, CActionParam), so IgnoreAllEvents adjusts `this` by +4 and the RemoveObserver
//   argument is the null-tested `p ? p + 4 : 0` conversion, evaluated before the GetInstance call.
// Header: engine/CVirtualKeyboard.h retail opt-in FABLE_RETAIL_CVIRTUALKEYBOARD (hdr/ overlay, chunk 707): the generated header flattens
//   ProfileName to its data pointer and has no member functions.
// Layout: the NUISystem UI classes have no usable engine header (engine/CObserver.h and engine/CManager.h are flattened global-namespace
//   layouts; none for CComponent / CEditBox / CObservable), so only the vtable slots this row calls are declared, at their retail slots.
#define FABLE_RETAIL_CVIRTUALKEYBOARD
#include "engine/CVirtualKeyboard.h"

namespace NUISystem {
enum EEvent { };
class CUIInterface {
public:
    virtual ~CUIInterface();                              // slot 0
};
class CObserver {                                         // vtable 0x01246020
public:
    virtual ~CObserver();                                 // slot 0
    virtual void ProcessEvent(EEvent event) = 0;          // slot 1 (_purecall)
    virtual bool IsObservingEvent(EEvent event);          // slot 2 0x0052D900
    virtual void ObserveEvent(EEvent event);              // slot 3 0x0052DA20
    virtual void IgnoreEvent(EEvent event);               // slot 4 0x0052D940
    virtual void IgnoreAllEvents();                       // slot 5 0x0052D9A0
};
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
class CComponent : public CUIInterface, public CObserver { // primary vtable from CUIInterface; CObserver at +4
public:
    // slots 1..149 of the component vtable (not called here)
    VF(01) VF(02) VF(03) VF(04) VF(05) VF(06) VF(07) VF(08) VF(09) VF10(1) VF10(2) VF10(3) VF10(4) VF10(5) VF10(6) VF10(7) VF10(8) VF10(9)
    VF10(10) VF10(11) VF10(12) VF10(13) VF(140) VF(141) VF(142) VF(143) VF(144) VF(145) VF(146) VF(147) VF(148) VF(149)
};
#undef VF10
#undef VF
class CEditBox : public CComponent {                      // vtable 0x01246B8C (CObserver part 0x01246B64)
public:
    virtual void SetActive(bool active);                  // slot 150 (+0x258) 0x005401B0
};
class CObservable {                                       // CManager's base (RTTI)
public:
    virtual void SetEvent(EEvent event);                  // slot 0
    virtual void SetOwnedEvent(CObserver* observer, EEvent event);   // slot 1
    virtual void AddObserver(CObserver* observer);        // slot 2 0x0055CA40
    virtual void AddExclusiveObserver(CObserver* observer);          // slot 3
    virtual void RemoveExclusiveObserver(CObserver* observer);       // slot 4
    virtual void RemoveObserver(CObserver* observer);     // slot 5 0x0055C9C0
};
class CManager : public CObservable {                     // vtable 0x01230134
public:
    static CManager* __fastcall GetInstance();            // 0x0041E5F2
};
}

CVirtualKeyboard::~CVirtualKeyboard()
{
    PEntryBox->SetActive(false);
    PEntryBox->IgnoreAllEvents();
    NUISystem::CManager::GetInstance()->RemoveObserver(PEntryBox);
}