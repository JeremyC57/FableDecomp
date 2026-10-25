// CPanelFailedAllocationHandler::OnAllocationFailed(long size) -- the panel's pool ran out of memory: evict one stream buffer (the first idle one, else the first busy one unless it still covers the handler's key), detach it from the panel, release it and delete it; returns 1 when a buffer was evicted, else 0
// Leaf: ?OnAllocationFailed@CPanelFailedAllocationHandler@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C22080, 120 bytes (`ret 4`, the size argument is unused). Descriptive names
//   (no PDB covers this middleware library): the manifest's "CTCHero_Respawn" is propagated. The function is slot 1
//   of vtable 0x012B3850 (slot 0 the deleting destructor 0x00C22E30), whose two-argument constructor 0x00C22060
//   stores the panel at +4 and a key at +8; CPanelItemSet::Allocate 0x00C22100 builds one on the stack and hangs it in
//   the pool's failure-handler slot around the pool allocation, so this is the pool's CFailedAllocationHandler.
// Callees: the panel's buffer queues at +0x2B4 and +0x2E4 (virtual slot 11, +0x2C: first entry or null),
//   CStreamBuffer::Covers @ 0x00C26280 (`m_size >= key`), CPanel::DetachBuffer @ 0x00C12270,
//   CStreamBuffer::Release @ 0x00C26390, ~CStreamBuffer @ 0x00C26400 (an empty out-of-line body), operator delete
//   (MSVCR71 import thunk 0x00BFE9BC).
// Shape: the result is a full-register long (`mov eax,1` / `xor eax,eax`); the busy queue is only tried when the idle
//   queue is empty and its buffer is rejected when Covers() says so. `delete` keeps its null test and calls the
//   out-of-line destructor before operator delete.
// Retry pass 3: a rejected busy buffer is nulled (`if (buffer && buffer->Covers(m_key)) buffer = 0;`) and ONE
//   `if (buffer) { evict; return 1; } return 0;` follows: VC7.1 threads both failure paths straight to the trailing
//   `xor eax,eax` exit after the success epilogue, as retail (every early-return / `||` spelling put the zero exit
//   inline after the Covers() test).

struct CPanel;

struct CStreamBuffer {                          // the stream buffer record of the panel's queues
    unsigned char _pad00[0x10];
    long          m_size;                       // +0x10
    bool Covers(long key);                      // 0x00C26280
    void Release(CPanel* panel);                // 0x00C26390
    ~CStreamBuffer();                           // 0x00C26400 (empty, out of line)
};

struct CStreamBufferQueue {                     // polymorphic queue member of the panel
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10();
    virtual CStreamBuffer* GetFirst();          // slot 11 (+0x2C)
};

struct CPanel {
    unsigned char      _pad000[0x2b4];
    CStreamBufferQueue m_idleBuffers;           // +0x2B4
    unsigned char      _pad2b8[0x2e4 - 0x2b8];
    CStreamBufferQueue m_busyBuffers;           // +0x2E4
    void DetachBuffer(CStreamBuffer* buffer);   // 0x00C12270
};

struct CPanelFailedAllocationHandler {          // vtable 0x012B3850
    virtual void v00();                         // slot 0: deleting destructor 0x00C22E30
    virtual long OnAllocationFailed(long size); // slot 1
    CPanel* m_pPanel;                           // +0x04
    long    m_key;                              // +0x08
};

long CPanelFailedAllocationHandler::OnAllocationFailed(long)
{
    CStreamBuffer* buffer = m_pPanel->m_idleBuffers.GetFirst();
    if (!buffer) {
        buffer = m_pPanel->m_busyBuffers.GetFirst();
        if (buffer && buffer->Covers(m_key))
            buffer = 0;
    }
    if (buffer) {
        m_pPanel->DetachBuffer(buffer);
        buffer->Release(m_pPanel);
        delete buffer;
        return 1;
    }
    return 0;
}