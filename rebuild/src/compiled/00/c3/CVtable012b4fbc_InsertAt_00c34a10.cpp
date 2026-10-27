// CVtable_012b4fbc::InsertAt(unsigned long index, CUnprovenItem_012b4fbc* item) -- this list only takes items at index 0: -2 for any other index, else the item is appended to the pointer array at +4 and 0 returned
// Leaf: ?InsertAt@CVtable_012b4fbc@@UAEJKPAUCUnprovenItem_012b4fbc@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C34A10, 34 bytes, thiscall, `ret 8`.
// Identity (descriptive): slot 3 of the non-RTTI vtable 0x012B4FBC (stored by its constructor 0x00C34AA0, which builds
//   the array at +4 out of line @ 0x00C42A60). Its sibling vtable 0x012B4FEC shares slots 1, 2, 9 and 10 and puts its
//   own InsertAt 0x00C34C40 in slot 3 (an index test against the count, then the inline append): two implementations
//   of one libpk list interface. libpk has no PDB or RTTI, so the class is named by its vtable.
// Callees: the array's Add @ 0x00C42B50 (thiscall: grow by one slot through 0x00C1E640 when full, store, count++).
// Shape: an unsigned index tested `> 0` (`test eax,eax ; jbe`), the early -2 return first.
struct CUnprovenItem_012b4fbc;

template <class T>
struct CPtrArray {                              // the libpk pointer array {data, capacity, count} (landed 0x00C1E8D0 model)
    T*            m_data;                       // +0
    unsigned long m_capacity;                   // +4
    unsigned long m_count;                      // +8
    unsigned long GetCount();                   // 0x00C42AC0 for this instantiation
    void Add(T value);                          // 0x00C42B50 for this instantiation
};

class CVtable_012b4fbc {                        // vtable 0x012B4FBC (constructor 0x00C34AA0)
public:
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual long InsertAt(unsigned long index, CUnprovenItem_012b4fbc* item);   // slot 3
    virtual unsigned long GetCount(long unused);    // slot 4 (0x00C34A40)
    virtual long IsNotEmpty();                  // slot 5
    CPtrArray<CUnprovenItem_012b4fbc*> m_items; // +0x04
};

long CVtable_012b4fbc::InsertAt(unsigned long index, CUnprovenItem_012b4fbc* item)
{
    if (index > 0)
        return -2;
    m_items.Add(item);
    return 0;
}