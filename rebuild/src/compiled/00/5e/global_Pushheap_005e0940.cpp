// _global::_Push_heap(pair<EAnimComponent,CIVCountedPointer<CAnimComponentBase> >*, long, long, pair<EAnimComponent,CIVCountedPointer<CAnimComponentBase> >, CKeyPairCompareLess<EAnimComponent,CIVCountedPointer<CAnimComponentBase> >) -- heap sift-up of the anim-component pairs by component key
// Leaf: _Push_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E0940, 184 bytes, `ret 0x10`.  No direct callees
// (virtual slot 1 of the pointee).
// Model of the landed TU neighbour __unguarded_linear_insert @ 0x005E07F0 (same pair, same comparator).
// Element: std::pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> > (8 bytes): a signed enum
// key (`cmp [parent],value.first ; jge`) and a one-word intrusive counted pointer whose pointee keeps
// its reference count at +4 and is freed through virtual slot 1.
// Link-time __fastcall (/Gr algorithm TU): first in ecx, hole in edx, then top, the 8-byte pair by
// value and the empty comparator (4 + 8 + 4 = 0x10).
// Shape: SGI/STLport `__push_heap`.  Each move is the pair's implicit operator=: the key store, then
// the inlined CIVCountedPointer::operator= (same-object skip; release the old pointee and clear the
// slot; adopt and add a reference).  After the final drop the by-value pair is destroyed here (the
// pointer's release with the pointee as `this`: `mov ecx,esi ; call [eax+4]`).
enum EAnimComponent { ANIM_COMPONENT_NONE = 0 };

class CAnimComponentBase { public:                  // CIVCountedPointeeBase-derived
    virtual void Destroy(int flags);         // slot 0
    virtual void Delete();                   // slot 1 (+0x04)
    unsigned long IVRefCount;                // +0x04
    void Release()
    {
        if (--IVRefCount == 0)
            Delete();
    }
};

struct CIVCountedPointer {                   // CIVCountedPointer<CAnimComponentBase>, 4 bytes
    CAnimComponentBase* Object;              // +0x00
    ~CIVCountedPointer()
    {
        if (Object)
            Object->Release();
    }
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        CAnimComponentBase* old = Object;
        if (old != other.Object) {
            if (old) {
                old->Release();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->IVRefCount;
        }
        return *this;
    }
};

struct PairAnimComponent {                   // std::pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >, 8 bytes
    EAnimComponent    first;                 // +0x00
    CIVCountedPointer second;                // +0x04
};

struct CKeyPairCompareLess {                 // CKeyPairCompareLess<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >
    bool operator()(const PairAnimComponent& a, const PairAnimComponent& b) const { return a.first < b.first; }
};

void __fastcall _Push_heap(PairAnimComponent* first, long hole, long top, PairAnimComponent value, CKeyPairCompareLess comp)
{
    long parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], value)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = value;
}