// NUISystem::COk::ProcessEvent(EEvent event) -- on event 26 or 4 drop back to state 0 and run the OK callback
// Leaf: ProcessEvent
// Retail: Fable.exe (TLC, VC7.1) @ 0x0041CB9D, 58 bytes (`ret 4`).
// Callees: the component's virtual slot 48 (+0xc0, ChangeState(unsigned long)) on the primary `this` (`lea ecx,[esi-4]`),
// then the stored `__fastcall` OK callback.
// Label: COk vtable 0x0122FAD0 slot 1 -- the CObserver subobject's vtable (CObserver slot 1 is the pure event handler,
// `_purecall` in 0x01246020); Ego_r.pdb names ?ProcessEvent@COk@NUISystem@@UAEXW4EEvent@2@@Z and
// ?SetOkCallBack@COk@NUISystem@@UAEXP6IXXZ@Z (the callback is `void (__fastcall*)()`).
// Layout: the override receives `this` at the CObserver subobject (+0x04, 0x14 bytes: vptr, set<EEvent>, bool); the
// callback sits at complete +0x15c, the first own member after the 0x15c-byte retail CChangingStateComponent.
// Shape: a size-optimised TU (`#pragma optimize("s", on)`; the body sits unaligned at 0x0041CB9D among the 0x41xxxx inline
// out-of-line copies): the event in edi, `this` in esi, the primary-base pointer formed with `lea ecx,[esi-4]` before the
// vtable load (the speed build loads the vtable from [esi-4] first and swaps the registers). The two event tests are separate
// `if`s (VC does not thread the second over the virtual call).
#pragma optimize("s", on)
namespace NUISystem {
enum EEvent { EVENT_4 = 4, EVENT_26 = 26 };
struct _Rb_tree_node_base;
struct CEventSet {                                 // STLport set<EEvent>, 12 bytes
    _Rb_tree_node_base* _M_header;
    unsigned long       _M_node_count;
    unsigned char       _M_key_compare;
};
class CObserver {                                  // 0x14 bytes in retail
public:
    virtual void v000();
    virtual void ProcessEvent(EEvent event) = 0;   // slot 1
    CEventSet EventsToObserve;                     // +0x04
    bool      PreventObservation;                  // +0x10
};
class CComponentRoot {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void ChangeState(unsigned long state); // slot 48 (+0xc0)
};
class CChangingStateComponent : public CComponentRoot, public CObserver {
public:
    unsigned char _pad18[0x15c - 0x18];
};
class COk : public CChangingStateComponent {
public:
    void (__fastcall* OkCallBack)();               // +0x15c
    virtual void ProcessEvent(EEvent event);
};

void COk::ProcessEvent(EEvent event)
{
    if (event == EVENT_26 || event == EVENT_4)
        ChangeState(0);
    if (event == EVENT_26 || event == EVENT_4) {
        if (OkCallBack)
            OkCallBack();
    }
}
}