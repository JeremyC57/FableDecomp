// CircularQueue::DequeueItem(unsigned long index) -- empty one slot: remember its item as the last dequeued, release the item, clear the slot and its 64-bit stamp; false when the slot was already empty
// Leaf: DequeueItem
// Retail: Fable.exe @ 0x00C44270, 73 bytes, `__fastcall`, `ret 4`, returns bool (`xor al,al` / `mov al,1`).
// Callees: CItem::Release() 0x00C3C5F0 (`if (m_refCount) --m_refCount;` on +0x0C; its manifest label
// "CItem::SetQuestPersistentFlag" is a propagated byte-twin name). Callers: 0x00C35A6C and the queue's own pop at
// 0x00C44320 (which dequeues m_items[m_tail] and advances the tail modulo the capacity); the clear-all loop at
// 0x00C443D0 inlines the same body.
// Layout (from the queue's init at 0x00C44360, which allocates capacity*12 bytes and splits them into the CItem* array
// and the 8-byte stamp array that follows it): +0x00 item slots, +0x04 stamps, +0x08 capacity, +0x0C head, +0x10 tail,
// +0x14 last dequeued item.
// Shape: the early `return false` keeps the fail block first; the slot is re-read for the m_lastItem store and again for
// the Release call (written as three separate `m_items[index]` reads -- a named slot pointer/reference keeps the scaled
// index in ebx instead).
class CItem {
public:
    unsigned long  m_words[3];                     // +0x00
    unsigned long  m_refCount;                     // +0x0C
    void Release();                                // 0x00C3C5F0: if (m_refCount) --m_refCount
};

class CircularQueue {
public:
    CItem**           m_items;                     // +0x00 slot array (one CItem* per slot)
    unsigned __int64* m_times;                     // +0x04 per-slot 64-bit stamp
    unsigned long     m_capacity;                  // +0x08 slot count
    unsigned long     m_head;                      // +0x0C
    unsigned long     m_tail;                      // +0x10
    CItem*            m_lastItem;                  // +0x14 the item most recently dequeued
    unsigned long     m_stats[3];                  // +0x18
    bool DequeueItem(unsigned long index);
    bool GetItemAtIndex(unsigned long index);
};

bool CircularQueue::DequeueItem(unsigned long index)
{
    if (m_items[index] == 0)
        return false;
    m_lastItem = m_items[index];
    m_items[index]->Release();
    m_items[index] = 0;
    m_times[index] = 0;
    return true;
}