// CVtable_012b4fbc::IsNotEmpty() -- whether the pointer array at +4 holds any item (its count is above zero)
// Leaf: ?IsNotEmpty@CVtable_012b4fbc@@UAEJXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C34A50, 17 bytes, thiscall, `ret`.
// Identity (descriptive): slot 5 of the non-RTTI vtable 0x012B4FBC (see 0x00C34A10); the sibling vtable 0x012B4FEC's
//   slot 5 is 0x00C34B50 (`mov eax,[ecx+0Ch]`, its count). libpk has no PDB or RTTI.
// Callees: the array's count getter @ 0x00C42AC0 (`mov eax,[ecx+8] ; ret`; the landed candidate 0x00C34A40 spells it
//   CBaseIntelligentPointer::GetPItem_00c42ac0, a propagated name).
// Shape: a long result of an unsigned `> 0` compare: `xor ecx,ecx ; cmp ecx,eax ; sbb eax,eax ; neg eax`.
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

long CVtable_012b4fbc::IsNotEmpty()
{
    return m_items.GetCount() > 0;
}