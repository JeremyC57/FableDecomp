// NUISystem::CYesNo::ChangeState(unsigned long) -- register the dialog as a UI-manager observer when it opens, drop it when it closes
// Leaf: ?ChangeState@CYesNo@NUISystem@@UAEXK@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0042D604, length 87 bytes, `ret 4` (__fastcall, one argument).
// Name: slot 48 of NUISystem::CYesNo's primary vtable 0x0122F884 (rtti-vtables.tsv); Ego_r.pdb names
// `?ChangeState@CYesNo@NUISystem@@UAEXK@Z`. The catalog's "CChangingStateComponent_StateChangeHandler_1" is a label.
// Callees: NUISystem::CManager::GetInstance @ 0x0041E5F2 (static, landed), the manager's CObservable slots
// AddObserver (slot 2, +0x08 = 0x0055CA40) and RemoveObserver (slot 5, +0x14 = 0x0055C9C0), the CObserver base's
// ObserveAllEvents (slot 6, +0x18 = 0x0052D7B0: ObserveEvent(0..n) through slot 3), and the qualified base call
// NUISystem::CChangingStateComponent::ChangeState(unsigned long) @ 0x0052CF40.
// Shape: two independent `if`s (state 1 = shown, state 0 = hidden), then the base call. Passing `this` as a CObserver*
// is a derived-to-second-base conversion, so VC emits the null-preserving `neg ; sbb ; and` form (this ? this+4 : 0);
// the +4 base pointer is computed once into edi and reused as the `this` of the ObserveAllEvents call.
// Pragma: #pragma optimize("s", on) -- a size TU: under /O2 VC7.1 tail-duplicates the base call into the state-1 path
// and threads the second test away; retail re-tests `test ebx,ebx` after the state-1 block and has one base call.
// Layout (retail): CChangingStateComponent = root polymorphic base +0, CObserver +4 (retail 0x14 bytes: vptr, a
// 12-byte STLport set<EEvent> EventsToObserve +4, bool PreventObservation +0x10 -- SetPreventObserving 0x0041C580
// writes +0x10), a third polymorphic base +0x18, state to +0x15C; CYesNo adds two dwords (landed ctor 0x0041CADC).
// engine/CObserver.h is the donor's 0x18-byte layout (set 0x10 bytes, flag +0x14), which disagrees with retail, so a
// local model is used.
// Model: local -- engine/CObserver.h is the donor 0x18-byte layout (retail CObserver is 0x14: CChangingStateComponent's
// CComponentBase3 sits at +0x18) and engine/CManager.h is data-only (no virtuals); the observer/observable slots are virtual
// calls, so both are local.
#pragma optimize("s", on)
namespace NUISystem {

enum EEvent { EVENT_NONE = 0 };

class CObserver {                              // retail vtable 0x01246020, 0x14 bytes
public:
    virtual ~CObserver();                      // slot 0
    virtual void Observe(EEvent event) = 0;    // slot 1
    virtual bool IsObservingEvent(EEvent event);   // slot 2 0x0052D900
    virtual void ObserveEvent(EEvent event);   // slot 3 0x0052DA20
    virtual void IgnoreEvent(EEvent event);    // slot 4 0x0052D940
    virtual void IgnoreAllEvents();            // slot 5 0x0052D9A0
    virtual void ObserveAllEvents();           // slot 6 0x0052D7B0
    virtual void SetPreventObserving(bool prevent);   // slot 7 0x0041C580
    unsigned char EventsToObserve[0xc];        // +0x04 STLport set<EEvent>
    bool          PreventObservation;          // +0x10
};

class CObservable {                            // slots 0..5 of NUISystem::CManager's vtable 0x01230134
public:
    virtual void SetEvent(EEvent event);                         // slot 0 0x0055CB10
    virtual void SetOwnedEvent(CObserver* owner, EEvent event);  // slot 1 0x0055CBE0
    virtual void AddObserver(CObserver* observer);               // slot 2 0x0055CA40
    virtual void AddExclusiveObserver(CObserver* observer);      // slot 3 0x0055C930
    virtual void RemoveExclusiveObserver(CObserver* observer);   // slot 4 0x0055C940
    virtual void RemoveObserver(CObserver* observer);            // slot 5 0x0055C9C0
};

class CManager : public CObservable {
public:
    static CManager* GetInstance();            // 0x0041E5F2
};

class CComponentRoot {                         // primary base: vptr only
public:
    virtual void ChangeState(unsigned long state);
};

class CComponentBase3 {                        // base at +0x18
public:
    virtual void v00();
};

class CChangingStateComponent : public CComponentRoot, public CObserver, public CComponentBase3 {
public:
    virtual void ChangeState(unsigned long state);   // 0x0052CF40
    unsigned char _body[0x15c - 0x1c];
};

class CYesNo : public CChangingStateComponent {      // vtables 0x0122F884 / 0x0122F85C / 0x0122F854
public:
    virtual void ChangeState(unsigned long state);
    virtual void Observe(EEvent event);              // 0x0041CB10
    long m_result;                                   // +0x15C
    long m_selection;                                // +0x160
};

void CYesNo::ChangeState(unsigned long state)
{
    if (state == 1) {
        CManager::GetInstance()->AddObserver(this);
        ObserveAllEvents();
    }
    if (state == 0)
        CManager::GetInstance()->RemoveObserver(this);
    CChangingStateComponent::ChangeState(state);
}

}