// CVtable_012b4fec::RemoveAt(unsigned long index, long unused) -- remove the item at index from the pointer array at +4 (by value, through the array's Remove)
// Leaf: ?RemoveAt@CVtable_012b4fec@@UAEXKJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C34C80, 22 bytes, thiscall, `ret 8`.
// Identity (descriptive): slot 8 of the non-RTTI vtable 0x012B4FEC (see 0x00C34C40); the sibling list's out-of-line
//   0x00C42B80 does the same (`m_data[index]` written over the argument slot, then a tail jump to 0x00C1E900).
//   libpk has no PDB or RTTI.
// Callees: CPtrArray<CBlendEntry*>::Remove @ 0x00C1E900 (landed: finds the last slot holding the value, shifts the
//   rest down, drops the count).
// Shape: the element is loaded, ecx advanced to the member (`add ecx,4`), one push and the call.
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

void CVtable_012b4fec::RemoveAt(unsigned long index, long unused)
{
    m_items.Remove(m_items.m_data[index]);
}