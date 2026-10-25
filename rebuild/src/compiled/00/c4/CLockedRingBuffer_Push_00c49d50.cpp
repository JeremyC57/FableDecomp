// CLockedRingBuffer::Push(CCodaFrame* frame) -- under the ring's own critical section append a frame pointer at the tail slot of the fixed-capacity FIFO (dropped silently when the ring is full), advancing the tail modulo the capacity and bumping the count
// Leaf: ?Push@CLockedRingBuffer@@QAEXPAUCCodaFrame@@@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C49D50, 107 bytes (+ int3 padding), __fastcall `ret 4`, EH-framed.
// Library: libpk audio (no PDB/RTTI): descriptive names. Only caller: 0x00C495AA in the CCodaFrameStreamer code (the
//   frame streamer's ring member at +0x48, pushed after the frame is handed to the source); its Pop is the landed
//   0x00C4A0E0 (head index 0x00C49DD0 = tail - count, wrapped). The manifest's "RingBuffer_PushLocked" is kept as
//   the description.
// Layout: +0x00 the critical section (the scoped lock takes `this`), +0x18 capacity, +0x1C slot array, +0x20 count,
//   +0x24 tail index.
// Callees: CScopedLock::CScopedLock(void*) 0x00C0FFC0 / ~CScopedLock 0x00C0FFE0 (8-byte lock: section + entry tick).
// Shape: the full test is an early `return` inside the lock's scope -- that alone gives the EH frame (state -1 stored
//   right after the constructor, scheduled above the `je`); the `if (!full) { ... }` form compiles with no frame at
//   all (67 bytes). The increment of the count lands between the `idiv` and the tail store.
struct CScopedLock {
    CScopedLock(void* section);                 // 0x00C0FFC0
    ~CScopedLock();                             // 0x00C0FFE0
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
    void Push(CCodaFrame* frame);
};

void CLockedRingBuffer::Push(CCodaFrame* frame)
{
    CScopedLock lock(this);
    if (m_count == m_capacity)
        return;
    m_pSlots[m_tail] = frame;
    m_tail = (m_tail + 1) % m_capacity;
    ++m_count;
}