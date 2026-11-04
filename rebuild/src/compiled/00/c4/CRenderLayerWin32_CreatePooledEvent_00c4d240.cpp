// CRenderLayerWin32::CreatePooledEvent(long params, CNodePool* pool, long* budget, long* total, long type, CPooledEvent** out) -- take an event from the pool's free chain while the budget lasts (else create one of the four kinds through the factory slots), initialise it and file it in the active chain; -5 when no event could be made
// Leaf: ?CreatePooledEvent@CRenderLayerWin32@@
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C4D240, 256 bytes, thiscall `ret 18h`.
// Identity: libpk (no PDB): descriptive name; callers 0x00C4D440 / 0x00C4D4B0 (landed as CRenderLayerWin32::ApplySlotA/B, whose
//   "SStateSlot" is this 20-byte CNodePool). Pool calls: CNodePool::GetFree 0x00C3C9A0, PopFree 0x00C3CA30, PushActive 0x00C3C9B0.
// Shape: pooled path while the budget lasts, else factory slots 4..7 by type; -5 when nothing was made; one shared exit.
struct CPooledNode {                            // intrusive doubly-linked node; pooled libpk events derive from it
    virtual ~CPooledNode();                     // slot 0 (a discarded node is deleted through it)
    CPooledNode* m_pPrev;                       // +0x04
    CPooledNode* m_pNext;                       // +0x08
};
struct CPooledEvent : CPooledNode {
    virtual long Initialise(long params);       // slot 1
};
struct CNodePool {                              // 20 bytes: an active chain and a free chain
    void*        m_pOwner;                      // +0x00
    CPooledNode* m_pActiveHead;                 // +0x04
    CPooledNode* m_pFreeHead;                   // +0x08
    long         m_freeCount;                   // +0x0c
    long         m_activeCount;                 // +0x10
    CPooledNode* GetFree();                     // 0x00C3C9A0
    void PushActive(CPooledNode* node);         // 0x00C3C9B0
    void PopFree();                             // 0x00C3CA30
};
struct CRenderLayerWin32 {                      // vtable 0x012B5B80 (no RTTI)
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual CPooledEvent* CreateEvent0();       // slot 4
    virtual CPooledEvent* CreateEvent1();       // slot 5
    virtual CPooledEvent* CreateEvent2();       // slot 6
    virtual CPooledEvent* CreateEvent3();       // slot 7
    long CreatePooledEvent(long params, CNodePool* pool, long* budget, long* total, long type, CPooledEvent** out);
};

long CRenderLayerWin32::CreatePooledEvent(long params, CNodePool* pool, long* budget, long* total, long type, CPooledEvent** out)
{
    CPooledEvent* event = 0;
    long result;
    if (*budget > 0) {
        event = (CPooledEvent*)pool->GetFree();
        result = event->Initialise(params);
        if (result != 0) {
            event = 0;
        } else {
            pool->PopFree();
            pool->PushActive(event);
            --*budget;
            if (*total > 0)
                --*total;
        }
    } else {
        switch (type) {
        case 0: event = CreateEvent0(); break;
        case 1: event = CreateEvent1(); break;
        case 2: event = CreateEvent2(); break;
        case 3: event = CreateEvent3(); break;
        }
        if (event == 0) {
            result = -5;
        } else {
            result = event->Initialise(params);
            if (result == 0) {
                pool->PushActive(event);
            } else {
                delete event;
                event = 0;
            }
        }
    }
    *out = event;
    return result;
}