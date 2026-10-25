// CAudioEventManager_00c3d6a0::DiscardScreenEvent(CScreenEvent_00c3d6a0* event) -- give an event of the manager's second pool (+0x58) back: release the event's resources (0x00C40800), take it off the active chain, then park it on the free chain when recycling is enabled or delete it
// Leaf: ?DiscardScreenEvent@CAudioEventManager_00c3d6a0@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C3D6A0, 66 bytes, __fastcall `ret 4`.
// Library: libpk audio (no PDB/RTTI); descriptive names. `this` is the audio system singleton 0x0143BB58 (the thunk
//   0x00C33990 `mov ecx,0143BB58h ; jmp`); the landed factory 0x00C3DDE0 ("CreateScreenEffectAction") takes these events
//   from the same pool and calls this when Init fails. The twin of DiscardMorphEvent 0x00C3D660 on the +0x44 pool. The
//   manifest's "CLandscapeBackgroundPatch_EnqueueAndClear" is propagated.
// Callees: the event's ReleaseResource 0x00C40800 (landed as "CTextureOwner::ClearErrorMessage": frees the +0x7c handle
//   through 0x00C3E730 and sets a global dirty bit), CNodePool::RemoveActive 0x00C3C9E0, CNodePool::PushFree 0x00C3CA00,
//   the event's deleting destructor (slot 0).
// Shape: a thiscall on the event does not prove it non-null, so the `delete` keeps its null test.
struct CScreenEvent_00c3d6a0 {                  // intrusive doubly-linked node
    virtual ~CScreenEvent_00c3d6a0();           // slot 0
    CScreenEvent_00c3d6a0* m_pPrev;             // +0x04
    CScreenEvent_00c3d6a0* m_pNext;             // +0x08
    void ReleaseResource();                     // 0x00C40800
};

struct CNodePool_00c3d6a0 {                     // 20 bytes: an active chain and a free chain
    void*                  m_pOwner;            // +0x00
    CScreenEvent_00c3d6a0* m_pActiveHead;       // +0x04
    CScreenEvent_00c3d6a0* m_pFreeHead;         // +0x08
    long                   m_freeCount;         // +0x0c (negative: recycling disabled)
    long                   m_activeCount;       // +0x10
    void RemoveActive(CScreenEvent_00c3d6a0* node); // 0x00C3C9E0
    void PushFree(CScreenEvent_00c3d6a0* node);     // 0x00C3CA00
};

struct CAudioEventManager_00c3d6a0 {
    unsigned char _pad_0x00[0x58];
    CNodePool_00c3d6a0 m_screenPool;            // +0x58
    void DiscardScreenEvent(CScreenEvent_00c3d6a0* event);
};

void CAudioEventManager_00c3d6a0::DiscardScreenEvent(CScreenEvent_00c3d6a0* event)
{
    event->ReleaseResource();
    m_screenPool.RemoveActive(event);
    if (m_screenPool.m_freeCount >= 0)
        m_screenPool.PushFree(event);
    else
        delete event;
}