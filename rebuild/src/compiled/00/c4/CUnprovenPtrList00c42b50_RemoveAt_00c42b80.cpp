// CUnprovenPtrList_00c42b50::RemoveAt(unsigned long index) -- remove the entry at index: read the slot, then remove that value through the base array's Remove
// Leaf: ?RemoveAt@CUnprovenPtrList_00c42b50@@QAEXK@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C42B80, 18 bytes, thiscall, tail jump to 0x00C1E900 (which returns `ret 4`).
// Identity (descriptive; libpk has no PDB or RTTI): the catalog's `_Cons_val<...CParticleMorphs::CEntry...>` label is
//   propagated. The body reads m_data[index] over its own argument slot and tail-jumps to 0x00C1E900, the landed
//   CPtrArray<CBlendEntry*>::Remove; its neighbour 0x00C42B50 (landed CUnprovenPtrList_00c42b50::Add) grows through
//   the same instantiation's Grow 0x00C1E640, so both are out-of-line members of one wrapper over that array.
//   Callers: 0x00C34A90 (`add ecx,4`) and CItemGridTable_012b501c::Remove 0x00C34FF0. The landed slot-8 RemoveAt
//   0x00C34C80 inlines the same body.
// Callees: CPtrArray<CBlendEntry*>::Remove(CBlendEntry*) 0x00C1E900.
struct CBlendEntry;

template <class T>
struct CPtrArray {                              // the libpk pointer array {data, capacity, count}
    T*            m_data;                       // +0
    unsigned long m_capacity;                   // +4
    unsigned long m_count;                      // +8
    void Remove(T value);                       // 0x00C1E900
};

struct CUnprovenPtrList_00c42b50 : CPtrArray<CBlendEntry*> {   // the array at offset 0
    void RemoveAt(unsigned long index);
};

void CUnprovenPtrList_00c42b50::RemoveAt(unsigned long index)
{
    Remove(m_data[index]);
}