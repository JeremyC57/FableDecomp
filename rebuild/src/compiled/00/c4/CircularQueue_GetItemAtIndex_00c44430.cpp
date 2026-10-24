// CircularQueue::GetItemAtIndex(unsigned long index) -- whether slot `index` is in range and holds an item
// Leaf: GetItemAtIndex
// Retail: Fable.exe @ 0x00C44430, 39 bytes, `__fastcall`, `ret 4`, returns bool, no callees. Caller 0x00C72C5A.
// Layout: as DequeueItem 0x00C44270 (+0x00 item slots, +0x08 capacity).
// Shape: the range test is an inline `IsValidIndex` (capacity != 0 && index < capacity) negated in an early return:
// that puts the `xor al,al` fail block between the two tests (`je fail ; cmp ; jb ok`). The same test written
// directly as `if (cap == 0 || index >= cap) return false;` (or the positive `&&` form) moves the fail block to the end.
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
    bool IsValidIndex(unsigned long index) const { return m_capacity != 0 && index < m_capacity; }
    bool GetItemAtIndex(unsigned long index);
};

bool CircularQueue::GetItemAtIndex(unsigned long index)
{
    if (!IsValidIndex(index))
        return false;
    return m_items[index] != 0;
}