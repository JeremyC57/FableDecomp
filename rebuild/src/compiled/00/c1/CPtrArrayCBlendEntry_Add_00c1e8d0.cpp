// CPtrArray<CBlendEntry*>::Add(CBlendEntry* value) -- grow by one slot when full, store at the count, bump the count
// Leaf: ?Add@?$CPtrArray@PAUCBlendEntry@@@@QAEXPAUCBlendEntry@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00c1e8d0, 39 bytes, thiscall, `ret 4`.
// Identity (descriptive): it grows through 0x00C1E640, bound ?Grow@?$CPtrArray@PAUCBlendEntry@@@@QAEXK@Z (the landed AddBlend 0x00C1E780's array), and sits before the landed CPtrArray<CBlendEntry*>::Remove 0x00C1E900; no direct callers. The element and array names are the landed descriptive ones (libpk: no PDB, no RTTI).
// Callees: CPtrArray<CBlendEntry*>::Grow(unsigned long) 0x00C1E640 (thiscall, slot count 1).
// Shape (family L123): the full test is `capacity - count == 0` (`mov ; sub ; jne`), the count re-read for the store and
//   the increment (as the landed AddBlend 0x00C1E780's inline Add).
struct CBlendEntry;

template <class T>
struct CPtrArray {
    T* m_data;                              // +0
    unsigned long m_capacity;               // +4
    unsigned long m_count;                  // +8
    void Grow(unsigned long slots);         // 0x00C1E640
    void Add(T value);
};

template <class T>
void CPtrArray<T>::Add(T value)
{
    if (m_capacity - m_count == 0)
        Grow(1);
    m_data[m_count] = value;
    ++m_count;
}
template void CPtrArray<CBlendEntry*>::Add(CBlendEntry*);