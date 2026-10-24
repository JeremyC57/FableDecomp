// _STL::__partial_sort(pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >* first, pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >* middle, pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >* last, pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >*, CKeyPairCompareLess<EAnimComponent, CIVCountedPointer<CAnimComponentBase> > comp) -- STLport partial_sort core for anim-component map entries: heapify [first, middle), pop every smaller later entry into the heap, then sort the heap
// Leaf: __partial_sort
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E2F40, 99 bytes (+ padding to 112), `ret 0Ch` (__fastcall /Gr algorithm TU: first in ecx ->
//   ebx, middle in edx -> edi, then last, the null value-type pointer and the empty comparator on the stack).
// Callees: 0x005E1C30 __make_heap (first, middle, comp, (T*)0, (int*)0), 0x005E1B60 __pop_heap (out of line here; its
//   manifest label `pair<long,CDefPointer<..>>::operator=` is propagated), 0x005E2860 sort_heap (landed).
// Instantiation: the CVectorMap<EAnimComponent, CIVCountedPointer<CAnimComponentBase> > sort of the anim-component TU
//   (the depth-limit fallback of __introsort_loop 0x005E37D0).
// Shape: STLport `make_heap(first, middle, comp); for (i = middle; i < last; ++i) if (comp(*i, *first))
//   __pop_heap(first, middle, i, _Tp(*i), comp, _STLP_DISTANCE_TYPE(first, _RandomAccessIter)); sort_heap(first,
//   middle, comp);` -- the signed key test reuses the loaded key for the by-value copy built in the argument area; the
//   loop re-reads `last` from its argument slot.
// Label correction: catalogued as `STL_HeapSortPhase`; the body is STLport `__partial_sort`.
enum EAnimComponent { ANIM_COMPONENT_NONE = 0 };

struct CAnimComponentBase {                                // CIVCountedPointeeBase-derived: vptr, reference count at +4
    virtual void v00();
    virtual void Delete();                                 // slot 1 (+0x04): destroy on the last reference
    long IVRefCount;                                       // +0x04
};

template <class T> struct CIVCountedPointer {              // one word: the intrusively counted object
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

template <class K, class V> struct CKeyPairCompareLess {   // empty, passed by value
    bool operator()(const pair<K, V>& a, const pair<K, V>& b) const { return a.first < b.first; }
};
typedef CKeyPairCompareLess<EAnimComponent, CIVCountedPointer<CAnimComponentBase> > TAnimComponentLess;

void __fastcall __make_heap(SAnimComponentPair* first, SAnimComponentPair* last, TAnimComponentLess comp, SAnimComponentPair*, int*);   // 0x005E1C30
void __fastcall __pop_heap(SAnimComponentPair* first, SAnimComponentPair* last, SAnimComponentPair* result,
                           SAnimComponentPair val, TAnimComponentLess comp, int*);   // 0x005E1B60
void __fastcall sort_heap(SAnimComponentPair* first, SAnimComponentPair* last, TAnimComponentLess comp);   // 0x005E2860

inline void make_heap(SAnimComponentPair* first, SAnimComponentPair* last, TAnimComponentLess comp)
{
    __make_heap(first, last, comp, (SAnimComponentPair*)0, (int*)0);
}

void __fastcall __partial_sort(SAnimComponentPair* first, SAnimComponentPair* middle, SAnimComponentPair* last, SAnimComponentPair*, TAnimComponentLess comp)
{
    make_heap(first, middle, comp);
    for (SAnimComponentPair* i = middle; i < last; ++i)
        if (comp(*i, *first))
            __pop_heap(first, middle, i, SAnimComponentPair(*i), comp, (int*)0);
    sort_heap(first, middle, comp);
}