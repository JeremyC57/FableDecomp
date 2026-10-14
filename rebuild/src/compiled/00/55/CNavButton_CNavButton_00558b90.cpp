// NUISystem::CNavButton::CNavButton(long) -- CClickable(owner) then the three vtables of the component chain
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00558B90, length 39 bytes (+ int3 padding).
// Callees: NUISystem::CClickable::CClickable(long) @ 0x0055B460. Vtables 0x0124B04C / 0x0124B024 /
// 0x0124B01C = NUISystem::CNavButton (rtti-vtables.tsv); the manifest's "CComponentContainer::
// CComponentContainer" label is propagated. The class adds no members of its own: the body is the
// base constructor and the three vtable stores (primary interface at +0, the CObserver base at +4,
// the third interface base at +0x18) that every constructor in this chain emits.
// Retail NUISystem component model (Ego_r.pdb minus 0x30: the retail CHoverable is 0x16c bytes, the
// PDB's 0x19c, CClickable 0x194; engine/CHoverable.h and engine/CObserver.h therefore disagree with
// the bytes, hence the `_Retail` names, as in the landed CDraggable constructor @ 0x0055A9C0).
struct CComponentInterface {                                 // primary vtable at +0
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
};

struct CObserver_Retail {                                    // vtable at +4 (6 slots, then the 12-byte event set and PreventObservation)
    virtual void v0();
    virtual void ProcessEvent();                             // slot 1: CClickable::ProcessEvent @ 0x0055AD60
    virtual void v2();
    virtual void ObserveEvent(long event);                   // slot 3
    virtual void StopObservingEvent(long event);             // slot 4
    virtual void v5();
    unsigned char _body[0x10];
};

struct CEventSinkInterface {                                 // vtable at +0x18
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
};

struct CClickable_Retail : CComponentInterface, CObserver_Retail, CEventSinkInterface {   // retail 0x194 bytes
    unsigned char _body[0x194 - 0x1c];
    CClickable_Retail(long owner);                           // 0x0055B460
};

struct CNavButton : CClickable_Retail {
    virtual void v000();                                     // own primary vtable 0x0124B04C
    virtual void ProcessEvent();                             // own CObserver vtable 0x0124B024 (slot 1 = CClickable::ProcessEvent)
    virtual void v3();                                       // own third-interface vtable 0x0124B01C
    CNavButton(long owner);
};

CNavButton::CNavButton(long owner)
    : CClickable_Retail(owner)
{
}