// _unattributed::EnterStateAndAttach(COwnerObject* owner) -- switch the state machine to state 4, register this object with the owner's embedded observer list (slot 8), let the embedded link at +0x78 bind owner + our +4 record (slot 2), then set the state word to 1
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C208E0, 59 bytes. Manifest label "CScrollableList::OnDeactivate" is
// propagated and rejected: the body lives in the no-RTTI 0x00C2xxxx module, next to the state-switch helper
// it calls, and touches a 16-bit state word at +0xAC that no NUISystem list has. Callers 0x00C215EB and
// 0x00C219F9 call it as `this->f(arg)` followed by the state helper with 1. The class is not recoverable
// from the bytes, so the leaf states what the body does.
// Callees: the state-change helper @ 0x00C20720 (`__thiscall`, one long; a jump table over the word at
// +0xAC), the owner's embedded object at +0x194 slot 8 (+0x20) with `this`, our embedded object at +0x78
// slot 2 (+8) with (owner, &our +4 record).
//
// Shape: `push 4 ; mov esi,ecx ; call` (the argument is loaded only after the first call), both embedded
// objects are member OBJECTS (`mov eax,[edi+194h] ; lea ecx,[edi+194h]`), arguments pushed right to left,
// the final `mov word ptr [esi+0ACh],1` scheduled after `pop edi`.
// Leaf: EnterStateAndAttach

struct CStateRecord;                             // the 0x74-byte record at +4 handed to the link

struct CObserverList {                           // embedded polymorphic member of the owner (+0x194)
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual void v4(); virtual void v5(); virtual void v6(); virtual void v7();
    virtual void Register(void* observer);       // slot 8 (+0x20)
};

struct COwnerObject {
    unsigned char _pad_0x00[0x194];
    CObserverList m_observers;                   // +0x194
};

struct CStateLink {                              // embedded polymorphic member of this object (+0x78)
    virtual void v0(); virtual void v1();
    virtual void Bind(COwnerObject* owner, CStateRecord* record);   // slot 2 (+8)
};

struct CStateMachineObject {
    void*          __vftable;                    // +0x00
    unsigned char  m_record[0x74];               // +0x04 (address passed as CStateRecord*)
    CStateLink     m_link;                       // +0x78
    unsigned char  _pad_0x7c[0xac - 0x7c];
    unsigned short m_state;                      // +0xAC
    void ChangeState(long state);                // 0x00C20720
    void EnterStateAndAttach(COwnerObject* owner);
};

void CStateMachineObject::EnterStateAndAttach(COwnerObject* owner)
{
    ChangeState(4);
    owner->m_observers.Register(this);
    m_link.Bind(owner, reinterpret_cast<CStateRecord*>(m_record));
    m_state = 1;
}
