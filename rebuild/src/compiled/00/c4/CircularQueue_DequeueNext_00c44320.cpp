// CircularQueue::DequeueNext() -- release the item in the queue's tail slot when there is one (DequeueItem), advance the tail with wrap-around at the capacity, and return the tail index
// Leaf: ?DequeueNext@CircularQueue@@QAEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C44320, 50 bytes (+ int3 padding), __fastcall, plain `ret`, unsigned long.
// Library: libpk audio (no PDB/RTTI): descriptive names (the landed CircularQueue::DequeueItem 0x00C44270 and
//   GetItemAtIndex 0x00C44430 model: +0x00 item slots, +0x04 64-bit stamps, +0x08 capacity, +0x0C head, +0x10 tail,
//   +0x14 last dequeued item). The queue is the audio object's buffer queue at +0x54. Callers 0x00C5B3B6 and
//   0x00C731A2.
// Callee: DequeueItem 0x00C44270 (__fastcall, `ret 4`, bool; it releases the slot's item 0x00C3C5F0 and clears it).
// Shape: `if (m_capacity == ++m_tail)` -- the incremented tail is copied to eax and compared as `cmp ecx(capacity),eax`
//   with the store scheduled before the `jne`; the `++m_tail == m_capacity` spelling swaps the compare operands.
class CItem;

class CircularQueue {
public:
    CItem**           m_items;                  // +0x00 slot array (one CItem* per slot)
    unsigned __int64* m_times;                  // +0x04 per-slot 64-bit stamp
    unsigned long     m_capacity;               // +0x08 slot count
    unsigned long     m_head;                   // +0x0C
    unsigned long     m_tail;                   // +0x10
    CItem*            m_lastItem;               // +0x14 the item most recently dequeued
    unsigned long     m_stats[3];               // +0x18
    bool DequeueItem(unsigned long index);      // 0x00C44270
    unsigned long DequeueNext();
};

unsigned long CircularQueue::DequeueNext()
{
    if (m_items[m_tail] != 0) {
        DequeueItem(m_tail);
        if (m_capacity == ++m_tail)
            m_tail = 0;
    }
    return m_tail;
}