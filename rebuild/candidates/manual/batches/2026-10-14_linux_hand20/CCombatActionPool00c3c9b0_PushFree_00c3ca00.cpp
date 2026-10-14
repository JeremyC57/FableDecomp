// CCombatActionPool_00c3c9b0::PushFree(CPooledCombatAction*) -- link the node at the head of the free chain and count it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3CA00, length 36 bytes (+ int3 padding).
// Callees: CPooledCombatAction::Link(prev, next) @ 0x00C4CED0 (`push [this+8] ; push 0`).
// The manifest label (CCombatSequenceBase::PushAction) is propagated; see 0x00C3C9B0 (PushActive) for the
// pool identification. Free head at +8, free count at +0xc. Called by the recycler at 0x00C3D610 when the
// pool's free count is not negative.
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

void CCombatActionPool_00c3c9b0::PushFree(CPooledCombatAction* action)
{
    action->Link(0, m_pFreeHead);
    m_pFreeHead = action;
    ++m_freeCount;
}
