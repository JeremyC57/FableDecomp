// CTCTavernGameSpotTheAddition::FindTarget(long unused) -- returns the target thing (+0x0C) of the current entry (index +0x144) of the 16-byte target vector at +0x110, or 0 when the index is out of range
// Retail: Fable.exe @ 0x007D7C40, 50 bytes (`ret 4`: one argument, never read), no callees.
// Shape: the range check is `index < size()` with size() an unsigned element count (`sar edx,4`
//        then `jae`) followed by `index >= 0` (`test ; jl`); the element is reached through the
//        retail vector's iterator OBJECT (`*(begin() + n)`, the landed CRetailVector model of
//        0x004AF3C0), which re-loads `first` after the checks -- a raw `first[n]` reuses the size
//        computation's load.
class CThing;

template<class T> struct CRetailVector {
    T* first; T* last; T* end_of_storage;
    struct iterator {
        T* p;
        explicit iterator(T* q) : p(q) {}
        iterator operator+(long n) const { return iterator(p + n); }
        T& operator*() const { return *p; }
    };
    iterator begin() { return iterator(first); }
    unsigned long size() const { return (unsigned long)(last - first); }
    T& operator[](unsigned long n) { return *(begin() + n); }
};

struct STargetEntry {                   // 16 bytes
    unsigned char _pad[0xc];
    CThing* m_pThing;                   // +0x0c
};

struct CTCTavernGameSpotTheAddition {
    unsigned char _pad[0x110];
    CRetailVector<STargetEntry> m_targets;          // +0x110
    unsigned char _pad_0x11c[0x28];
    long m_currentTarget;                           // +0x144
    CThing* FindTarget(long unused);
};

CThing* CTCTavernGameSpotTheAddition::FindTarget(long)
{
    if (m_currentTarget < m_targets.size() && m_currentTarget >= 0)
        return m_targets[m_currentTarget].m_pThing;
    return 0;
}
