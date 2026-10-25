// CAudioEventManager_00c3d660::DiscardMorphEvent(CMorphEvent_00c3d660* event) -- give a 0x4c-byte morph event back: mark its handle invalid (0x80000000), take it off the active chain of the manager's pool at +0x44, then park it on the free chain when recycling is enabled or delete it
// Leaf: ?DiscardMorphEvent@CAudioEventManager_00c3d660@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C3D660, 64 bytes, __fastcall `ret 4`.
// Library: libpk audio (no PDB/RTTI); descriptive names. `this` is the audio system singleton 0x0143BB58 (the thunk
//   0x00C33820 `mov ecx,0143BB58h ; jmp`); the landed factory 0x00C3DCD0 builds the same event (vtable 0x012B5A80,
//   +0x48 = 0x80000000) from the same pool and calls this when the event's Init fails. The manifest's
//   "CParticleMorph_EnqueueCombatAction" is propagated.
// Callees: CNodePool::RemoveActive 0x00C3C9E0, CNodePool::PushFree 0x00C3CA00, the event's deleting destructor (slot 0).
// Shape: the pool address is formed once (ebx); the handle store is scheduled between the push and the call; no null
//   test before the delete (the event was written through).
struct CMorphEvent_00c3d660 {                   // 0x4c bytes, intrusive doubly-linked node
    virtual ~CMorphEvent_00c3d660();            // slot 0
    CMorphEvent_00c3d660* m_pPrev;              // +0x04
    CMorphEvent_00c3d660* m_pNext;              // +0x08
    unsigned char _pad_0x0c[0x3c];
    long m_handle;                              // +0x48 (0x80000000: none)
};

struct CNodePool_00c3d660 {                     // 20 bytes: an active chain and a free chain
    void*                 m_pOwner;             // +0x00
    CMorphEvent_00c3d660* m_pActiveHead;        // +0x04
    CMorphEvent_00c3d660* m_pFreeHead;          // +0x08
    long                  m_freeCount;          // +0x0c (negative: recycling disabled)
    long                  m_activeCount;        // +0x10
    void RemoveActive(CMorphEvent_00c3d660* node);  // 0x00C3C9E0
    void PushFree(CMorphEvent_00c3d660* node);      // 0x00C3CA00
};

struct CAudioEventManager_00c3d660 {
    unsigned char _pad_0x00[0x44];
    CNodePool_00c3d660 m_morphPool;             // +0x44
    void DiscardMorphEvent(CMorphEvent_00c3d660* event);
};

void CAudioEventManager_00c3d660::DiscardMorphEvent(CMorphEvent_00c3d660* event)
{
    event->m_handle = (long)0x80000000;
    m_morphPool.RemoveActive(event);
    if (m_morphPool.m_freeCount >= 0)
        m_morphPool.PushFree(event);
    else
        delete event;
}