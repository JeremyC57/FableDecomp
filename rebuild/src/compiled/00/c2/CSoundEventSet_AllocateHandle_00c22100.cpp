// CSoundEventSet::AllocateHandle(CSoundEventManager* manager, long size, long key) -- allocate a block from the event set's handle pool with a stack failure handler (which may evict the manager's stream buffers) installed for the duration of the call; returns the new handle, or -1 when the pool could not satisfy the request
// Flags: /EHsc
// Leaf: ?AllocateHandle@CSoundEventSet@@QAEJPAUCSoundEventManager@@JJ@Z
// Naming (sweep1, 2026-10-25): libpk class set CSoundEventManager (vtable 0x012B2748) / CSoundEventSet (its +0x2C
//   member) / CSoundEvent (0xB4 bytes, vtable 0x012B3674): the describer 0x00C21040 called by TraceEvents/
//   TracePlaybacks prints "EventId(", "Priority=", "State='Waiting for data'" / "Playing using physical player";
//   chunk 431/435's CPanel/CPanelItem names came from propagated UI labels (renamed from 00/c2/CPanelItemSet_AllocateHandle_00c22100.cpp).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C22100, 139 bytes (`ret 0Ch`), built under /EHsc. Descriptive names (no PDB
//   covers this middleware library): the manifest's "NUISystem_CSteeringAgent_FindAgentInList" is propagated.
//   Callers 0x00C12FAD and 0x00C21874 call it on the manager's event set (`lea ecx,[edi+2Ch]`) with the manager as the
//   first argument and test the result with `jl`.
// Callees: CSoundEventFailedAllocationHandler::CSoundEventFailedAllocationHandler(manager, key) @ 0x00C22060 (vtable 0x012B3850,
//   slot 1 = OnAllocationFailed 0x00C22080), CHandleBasedMemoryPool::Allocate(long* handle, long size) @ 0x00C28C20.
// Layout: the event set's pool sits at +0x334 (manager +0x360, the pool the stream buffers Lock/Unlock); this
//   library's pool variant keeps an embedded default handler (one vptr) at +0x34 and the current handler pointer
//   at +0x38, one word above the engine's CHandleBasedMemoryPool (FailureHandler +0x34 in engine/
//   CHandleBasedMemoryPool.h), so the pool is modelled locally.
// Shape: the handler is a polymorphic stack object with an empty inline virtual destructor: EH state 0 before the
//   allocation and no destructor code (or -1 store) on the normal paths. The handle is a local homed in the dead
//   `key` argument slot (`lea ecx,[esp+2Ch]`); the default handler is restored right after the call (inline
//   SetFailureHandler(0)), between the `test eax,eax` and its `jne`. The setter must be the if/else form: the
//   ternary `handler ? handler : &m_defaultHandler` forms the default address in ecx (retail: edx), a direct
//   member store reschedules the whole call sequence.

struct CSoundEventManager;

class CFailedAllocationHandler { public:               // the pool's failure callback interface
    virtual ~CFailedAllocationHandler() {}      // slot 0
    virtual long OnAllocationFailed(long size) = 0;   // slot 1
};

struct CSoundEventFailedAllocationHandler : CFailedAllocationHandler {   // vtable 0x012B3850
    CSoundEventManager* m_pManager;                           // +0x04
    long    m_key;                              // +0x08
    CSoundEventFailedAllocationHandler(CSoundEventManager* manager, long key);   // 0x00C22060
    virtual long OnAllocationFailed(long size); // 0x00C22080
};

struct CDefaultFailedAllocationHandler : CFailedAllocationHandler {
    virtual long OnAllocationFailed(long size);
};

class CHandleBasedMemoryPool { public:                 // this library's pool variant (see Layout)
    unsigned char                   _pad00[0x34];
    CDefaultFailedAllocationHandler m_defaultHandler;    // +0x34
    CFailedAllocationHandler*       m_pFailureHandler;   // +0x38
    long Allocate(long* handle, long size);     // 0x00C28C20
    void SetFailureHandler(CFailedAllocationHandler* handler)
    {
        if (handler)
            m_pFailureHandler = handler;
        else
            m_pFailureHandler = &m_defaultHandler;
    }
};

struct CSoundEventSet {                          // +0x2C of the manager
    unsigned char          _pad000[0x334];
    CHandleBasedMemoryPool m_pool;              // +0x334
    long AllocateHandle(CSoundEventManager* manager, long size, long key);
};

long CSoundEventSet::AllocateHandle(CSoundEventManager* manager, long size, long key)
{
    CSoundEventFailedAllocationHandler handler(manager, key);
    m_pool.SetFailureHandler(&handler);
    long handle;
    long ok = m_pool.Allocate(&handle, size);
    m_pool.SetFailureHandler(0);
    if (!ok)
        return -1;
    return handle;
}