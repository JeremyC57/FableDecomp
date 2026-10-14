// CCombatActionPool_00c3c9b0::PushActive(CPooledCombatAction*) -- link the node at the head of the active chain and count it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3C9B0, length 36 bytes (+ int3 padding).
// Callees: CPooledCombatAction::Link(prev, next) @ 0x00C4CED0 (`push [this+4] ; push 0`).
// The manifest label (CCombatSequenceBase::PushAction) is propagated: this body lives in the 0x00C3Cxxx
// combat-action module and is called from the action creators at 0x00C3CD00 / 0x00C3DA60 / 0x00C4D240
// on the 20-byte pool objects embedded at +0x1c and +0x30 of their owner. Head at +4, count at +0x10;
// the sibling at 0x00C3CA00 is the same operation on the free chain (+8 / +0xc) and 0x00C3C9E0 removes
// from the active chain. Named by what it does.
struct CPooledCombatAction {               // intrusive doubly-linked node, vtable base CVtable_012B8778
    virtual ~CPooledCombatAction();            // slot 0: the deleting destructor (`delete action` calls it with flag 1)
    virtual bool IsSecondaryAction();          // slot 1 (`call [edx+4]`): selects the pool
    virtual void v02();
    virtual void v03();
    virtual void OnHandled();                  // slot 4 (`call [eax+10h]`)
    CPooledCombatAction* m_pPrev;              // +0x04
    CPooledCombatAction* m_pNext;              // +0x08
    void Link(CPooledCombatAction* prev, CPooledCombatAction* next);   // 0x00C4CED0: hook between prev and next
    void Unlink();                                                     // 0x00C4CEF0: drop out of the chain
};

struct CCombatActionPool_00c3c9b0 {           // 20 bytes: an active chain and a free chain of nodes
    void*                m_pOwner;             // +0x00
    CPooledCombatAction* m_pActiveHead;        // +0x04
    CPooledCombatAction* m_pFreeHead;          // +0x08
    long                 m_freeCount;          // +0x0c (negative: recycling disabled)
    long                 m_activeCount;        // +0x10
    void PushActive(CPooledCombatAction* action);   // 0x00C3C9B0
    void RemoveActive(CPooledCombatAction* action); // 0x00C3C9E0
    void PushFree(CPooledCombatAction* action);     // 0x00C3CA00
};

void CCombatActionPool_00c3c9b0::PushActive(CPooledCombatAction* action)
{
    action->Link(0, m_pActiveHead);
    m_pActiveHead = action;
    ++m_activeCount;
}
