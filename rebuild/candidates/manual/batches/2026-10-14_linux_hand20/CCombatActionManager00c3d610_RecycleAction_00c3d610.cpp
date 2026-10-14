// CCombatActionManager_00c3d610::RecycleAction(CPooledCombatAction*) -- notify the action, pull it off its pool's active chain, then park it on the free chain or delete it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3D610, length 75 bytes (+ int3 padding).
// Callees: OnHandled (vslot 4, `call [eax+10h]`), IsSecondaryAction (vslot 1, `call [edx+4]`),
// CCombatActionPool::RemoveActive @ 0x00C3C9E0, CCombatActionPool::PushFree @ 0x00C3CA00, the action's
// deleting destructor (vslot 0 with flag 1: `delete action`).
// The manifest label (CCombatSequenceBase::HandleAction) is propagated; the callers are the action creators
// at 0x00C3DA60 / 0x00C3DB80 of the 0x00C3Cxxx combat-action module, whose owner object embeds two 20-byte
// pools at +0x1c (primary) and +0x30 (secondary). `delete` of the polymorphic node emits no null test.
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

struct CCombatActionManager_00c3d610 {
    unsigned char              _pad[0x1c];
    CCombatActionPool_00c3c9b0 m_primaryPool;      // +0x1c
    CCombatActionPool_00c3c9b0 m_secondaryPool;    // +0x30
    void RecycleAction(CPooledCombatAction* action);
};

void CCombatActionManager_00c3d610::RecycleAction(CPooledCombatAction* action)
{
    action->OnHandled();
    CCombatActionPool_00c3c9b0* pool;
    if (action->IsSecondaryAction())
        pool = &m_secondaryPool;
    else
        pool = &m_primaryPool;
    pool->RemoveActive(action);
    if (pool->m_freeCount >= 0)
        pool->PushFree(action);
    else
        delete action;
}
