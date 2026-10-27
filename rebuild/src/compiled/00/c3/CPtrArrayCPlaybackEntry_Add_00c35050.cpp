// CPtrArray<CPlaybackEntry*>::Add(CPlaybackEntry* value) -- grow by one slot when full, store at the count, bump the count
// Leaf: ?Add@?$CPtrArray@PAVCPlaybackEntry@@@@QAEXPAVCPlaybackEntry@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00c35050, 39 bytes, thiscall, `ret 4`.
// Identity (descriptive): it grows through 0x00C34D40 (unbound, the third Grow body) and sits before the landed CPtrArray<CPlaybackEntry*>::Remove 0x00C35080 of the same array; 0x00C34F30 calls the same Grow; no direct callers. Descriptive names (libpk).
// Callees: CPtrArray<CPlaybackEntry*>::Grow(unsigned long) 0x00C34D40 (thiscall, slot count 1).
// Shape (family L123): the full test is `capacity - count == 0` (`mov ; sub ; jne`), the count re-read for the store and
//   the increment (as the landed AddBlend 0x00C1E780's inline Add).
class CPlaybackEntry;

template <class T>
struct CPtrArray {
    T* m_data;                              // +0
    unsigned long m_capacity;               // +4
    unsigned long m_count;                  // +8
    void Grow(unsigned long slots);         // 0x00C34D40
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
template void CPtrArray<CPlaybackEntry*>::Add(CPlaybackEntry*);