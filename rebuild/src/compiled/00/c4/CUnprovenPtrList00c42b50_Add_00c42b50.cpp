// CUnprovenPtrList_00c42b50::Add(CBlendEntry* value) -- append through the base array's inline Add: grow by one slot when full, store at the count, bump the count
// Leaf: ?Add@CUnprovenPtrList_00c42b50@@QAEXPAUCBlendEntry@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00c42b50, 39 bytes, thiscall, `ret 4`.
// Identity (descriptive): it grows through 0x00C1E640, the Grow of CPtrArray<CBlendEntry*> that 0x00C1E8D0 (that array's own out-of-line Add) also calls, so it is a second function over the same instantiation: modelled as a wrapper class's Add over the array base (offset 0); its neighbour 0x00C42B80 removes by index through the landed CPtrArray<CBlendEntry*>::Remove 0x00C1E900; callers 0x00C34A10 and 0x00C34F30 (twice).
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
struct CUnprovenPtrList_00c42b50 : CPtrArray<CBlendEntry*> {                 // the array at offset 0
    void Add(CBlendEntry* value);
};
void CUnprovenPtrList_00c42b50::Add(CBlendEntry* value)
{
    CPtrArray<CBlendEntry*>::Add(value);
}