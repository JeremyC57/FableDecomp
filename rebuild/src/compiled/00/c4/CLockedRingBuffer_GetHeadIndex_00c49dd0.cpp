// CLockedRingBuffer::GetHeadIndex() -- under the ring's own critical section return the slot index of the oldest entry of the fixed-capacity FIFO: the tail index minus the count, wrapped by the capacity when negative
// Leaf: ?GetHeadIndex@CLockedRingBuffer@@QAEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C49DD0, 45 bytes (+ int3 padding), __fastcall, plain `ret`, no EH frame.
// Library: libpk audio (no PDB/RTTI): descriptive names. Only caller: the ring's Pop, landed 0x00C4A0E0 (it reads
//   m_pSlots[GetHeadIndex()] and decrements the count while holding the same recursive lock); the Push twin is
//   0x00C49D50. The manifest's "RingBuffer_GetCountLocked" is propagated (the count is +0x20; this returns an index).
// Layout: +0x00 the critical section, +0x18 capacity, +0x1C slot array, +0x20 count, +0x24 tail index.
// Callees: PKCProtectionInstance::PKCProtectionInstance(void*) 0x00C0FFC0 / ~PKCProtectionInstance 0x00C0FFE0.
// Shape: a named `head` local survives the lock's destructor in esi; nothing in the lock's scope can throw, so no EH
//   frame is built (with or without /EHsc).
class PKCCriticalSection;
class PKCProtectionInstance { public:
    PKCProtectionInstance(PKCCriticalSection* section);                 // 0x00C0FFC0
    ~PKCProtectionInstance();                             // 0x00C0FFE0
    void* m_pSection;
    long  m_entryTick;
};

struct CCodaFrame;

struct CLockedRingBuffer {
    unsigned char m_section[0x18];              // +0x00 CRITICAL_SECTION
    long          m_capacity;                   // +0x18
    CCodaFrame**  m_pSlots;                     // +0x1c
    long          m_count;                      // +0x20
    long          m_tail;                       // +0x24
    long GetHeadIndex();
};

long CLockedRingBuffer::GetHeadIndex()
{
    PKCProtectionInstance lock((PKCCriticalSection*)this);
    long head = m_tail - m_count;
    if (head < 0)
        head += m_capacity;
    return head;
}