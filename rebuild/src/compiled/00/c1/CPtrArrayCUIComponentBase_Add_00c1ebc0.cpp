// CPtrArray<CUIComponentBase*>::Add(CUIComponentBase* value) -- grow by one slot when full, store at the count, bump the count
// Leaf: ?Add@?$CPtrArray@PAUCUIComponentBase@@@@QAEXPAUCUIComponentBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00c1ebc0, 39 bytes, thiscall, `ret 4`.
// Identity (descriptive): it grows through 0x00C1EB30, bound ?Grow@?$CPtrArray@PAUCUIComponentBase@@@@QAEXK@Z (from the landed 0x00C1EAA0); the landed Remove next to it, 0x00C1EBF0, names the element CDisplayElement -- the Grow binding's spelling is kept for the callee; no direct callers. Descriptive names (libpk).
// Callees: CPtrArray<CUIComponentBase*>::Grow(unsigned long) 0x00C1EB30 (thiscall, slot count 1).
// Shape (family L123): the full test is `capacity - count == 0` (`mov ; sub ; jne`), the count re-read for the store and
//   the increment (as the landed AddBlend 0x00C1E780's inline Add).
struct CUIComponentBase;

template <class T>
struct CPtrArray {
    T* m_data;                              // +0
    unsigned long m_capacity;               // +4
    unsigned long m_count;                  // +8
    void Grow(unsigned long slots);         // 0x00C1EB30
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
template void CPtrArray<CUIComponentBase*>::Add(CUIComponentBase*);