// CPhysicsMesh::GetNavigationLines(long level) -- the per-level navigation line array (12-byte CArray<C2DLineF> records at +0x48..+0x4C) for the level, falling back to the first one when the level is out of range
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A73D70, length 52 bytes.
// Callees: none.
// Shape: the record count is the byte difference times 0x2AAAAAAB with `sar edx,1` (= / 12) and is
// compared SIGNED with the level (`jge`), so size() is a `long`; both arms re-read the array start
// (`mov ecx,[ecx+48h]` / `mov eax,[ecx+48h]`) because the element is reached through the retail
// vector's iterator OBJECT (`*(begin() + n)`, `*begin()`), which VC7.1 does not CSE with the size()
// loads (a raw `first[n]` keeps `first` in esi: 49 v 52). Retail CArray / std::vector are
// first/last/end (12 bytes).
class C2DLineF { public: float m_x0, m_y0, m_x1, m_y1; };
template <class T> class CArray { public:
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
};

template <class T> struct CRetailVector {         // retail std::vector: first/last/end, 12 bytes
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    struct iterator {
        T* p;
        explicit iterator(T* q) : p(q) { }
        iterator operator+(long n) const { return iterator(p + n); }
        T& operator*() const { return *p; }
    };
    iterator begin() const { return iterator(m_pFirst); }
    long size() const { return m_pLast - m_pFirst; }
    T& operator[](long n) const { return *(begin() + n); }
};

class CPhysicsMesh { public:
    unsigned char                     _pad00[0x48];
    CRetailVector< CArray<C2DLineF> > NavigationLines;   // +0x48 (PDB +0x58, 16-byte containers)
    const CArray<C2DLineF>* GetNavigationLines(long level) const;
};

const CArray<C2DLineF>* CPhysicsMesh::GetNavigationLines(long level) const
{
    if (level < NavigationLines.size())
        return &NavigationLines[level];
    return &*NavigationLines.begin();
}