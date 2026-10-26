// std::__unguarded_partition(pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >* first, pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >* last, pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> > pivot, CKeyPairCompareLess<EAnimComponent, CIVCountedPointer<CAnimComponentBase> > comp) -- STLport quicksort partition of anim-component map entries around a by-value pivot (signed key order); returns the cut
// Leaf: __unguarded_partition
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E19D0, 94 bytes, `ret 0Ch` (__fastcall /Gr algorithm TU: first in ecx ->
//   esi, last in edx -> edi, the 8-byte pivot by value and the empty comparator on the stack).
// Callees: 0x005E0760 swap (out of line here, unlike the CCharString partition 0x005E0650), the pivot's pointee virtual
//   slot 1 on the last reference.
// Instantiation: the CVectorMap<EAnimComponent, CIVCountedPointer<CAnimComponentBase> > sort of the anim-component TU
//   (called by __introsort_loop 0x005E37D0).
// Shape: STLport `for (;;) { while (comp(*first, pivot)) ++first; --last; while (comp(pivot, *last)) --last;
//   if (!(first < last)) return first; iter_swap(first, last); ++first; }` -- the pivot key is loaded once into ebx
//   (`cmp [esi],ebx ; jge` = signed enum compare), the loops are rotated with the step folded into the reload, and the
//   by-value pivot's counted pointer is released before the return (no zero store: the slot dies).
// Label correction: catalogued as `QuickSort_Partition_8Byte`; the body is STLport `__unguarded_partition`.
enum EAnimComponent { ANIM_COMPONENT_NONE = 0 };

class CAnimComponentBase { public:                                // CIVCountedPointeeBase-derived: vptr, reference count at +4
    virtual void v00();
    virtual void Delete();                                 // slot 1 (+0x04): destroy on the last reference
    long IVRefCount;                                       // +0x04
};

template <class T> class CIVCountedPointer { public:              // one word: the intrusively counted object
    T* Object;
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->IVRefCount;
    }
    __forceinline void Release()
    {
        T* object = Object;
        if (object) {
            if (--object->IVRefCount == 0)
                object->Delete();
            Object = 0;
        }
    }
    ~CIVCountedPointer() { Release(); }
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        if (Object != other.Object) {
            Release();
            Object = other.Object;
            if (Object)
                ++Object->IVRefCount;
        }
        return *this;
    }
};

template <class T1, class T2> struct pair {                // STLport pair: explicit memberwise copy constructor
    T1 first;
    T2 second;
    pair(const pair& other) : first(other.first), second(other.second) { }
};
typedef pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> > SAnimComponentPair;   // 8 bytes

template <class K, class V> class CKeyPairCompareLess { public:   // empty, passed by value
    bool operator()(const pair<K, V>& a, const pair<K, V>& b) const { return a.first < b.first; }
};
typedef CKeyPairCompareLess<EAnimComponent, CIVCountedPointer<CAnimComponentBase> > TAnimComponentLess;

void __fastcall swap(SAnimComponentPair& a, SAnimComponentPair& b);   // 0x005E0760

inline void iter_swap(SAnimComponentPair* a, SAnimComponentPair* b)
{
    swap(*a, *b);
}

SAnimComponentPair* __fastcall __unguarded_partition(SAnimComponentPair* first, SAnimComponentPair* last, SAnimComponentPair pivot, TAnimComponentLess comp)
{
    for (;;) {
        while (comp(*first, pivot))
            ++first;
        --last;
        while (comp(pivot, *last))
            --last;
        if (!(first < last))
            return first;
        iter_swap(first, last);
        ++first;
    }
}