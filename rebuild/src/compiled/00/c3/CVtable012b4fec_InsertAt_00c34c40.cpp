// CVtable_012b4fec::InsertAt(unsigned long index, CBlendEntry* item) -- -1 for an index inside the list, else the item is appended to the pointer array at +4 and 0 returned
// Leaf: ?InsertAt@CVtable_012b4fec@@UAEJKPAUCBlendEntry@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C34C40, 59 bytes, thiscall, `ret 8`.
// Identity (descriptive): slot 3 of the non-RTTI vtable 0x012B4FEC (stored by its constructor 0x00C34B30); the sibling
//   list vtable 0x012B4FBC has its InsertAt 0x00C34A10 in the same slot. libpk has no PDB or RTTI.
// Callees: the array's Grow(1) @ 0x00C1E640 (bound ?Grow@?$CPtrArray@PAUCBlendEntry@@@@QAEXK@Z). The element type is
//   the existing binding's (CBlendEntry*): the array code at 0x00C1E640 / 0x00C1E900 is shared by every pointer
//   instantiation, so the item type of this list is not proven.
// Shape: the inline CPtrArray::Add (`capacity - count == 0` as `mov ; sub ; jne`, the count re-read for the store and
//   the increment) over esi = &m_items.
struct CBlendEntry;

template <class T>
struct CPtrArray {                              // the libpk pointer array {data, capacity, count} (landed 0x00C1E8D0 model)
    T*            m_data;                       // +0
    unsigned long m_capacity;                   // +4
    unsigned long m_count;                      // +8
    void Grow(unsigned long slots);             // 0x00C1E640
    void Remove(T value);                       // 0x00C1E900
    void Add(T value)                           // out of line at 0x00C1E8D0, inlined here
    {
        if (m_capacity - m_count == 0)
            Grow(1);
        m_data[m_count] = value;
        ++m_count;
    }
};

class CVtable_012b4fec {                        // vtable 0x012B4FEC (constructor 0x00C34B30 zeroes the array)
public:
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual long InsertAt(unsigned long index, CBlendEntry* item);   // slot 3
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void RemoveAt(unsigned long index, long unused);         // slot 8
    CPtrArray<CBlendEntry*> m_items;            // +0x04
};

long CVtable_012b4fec::InsertAt(unsigned long index, CBlendEntry* item)
{
    if (index < m_items.m_count)
        return -1;
    m_items.Add(item);
    return 0;
}