// std::__pop_heap(first, last, result, val, comp, int*) -- STLport heap pop for anim-component map entries: move the top entry to *result, then sift the by-value val down from the root of the shrunk heap
// Leaf: ??$__pop_heap@PAU?$pair@W4EAnimComponent@@V?$CIVCountedPointer@VCAnimComponentBase@@@@@std@@U12@V?$CKeyPairCompareLess@W4EAnimComponent@@V?$CIVCountedPointer@VCAnimComponentBase@@@@@@H@std@@YIXPAU?$pair@W4EAnimComponent@@V?$CIVCountedPointer@VCAnimComponentBase@@@@@0@00U10@V?$CKeyPairCompareLess@W4EAnimComponent@@V?$CIVCountedPointer@VCAnimComponentBase@@@@@@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005E1B60, 126 bytes (+ padding to 128), `ret 14h` (__fastcall /Gr algorithm TU:
//   first in ecx -> esi, last in edx -> ebx; result, the 8-byte by-value pair, the empty comparator and the null distance
//   pointer on the stack).
// Identity (label correction): the catalog's "pair<long,CDefPointer<..>>::operator=" is propagated. The landed
//   __partial_sort 0x005E2F40 calls this as `__pop_heap(first, middle, i, _Tp(*i), comp, (int*)0)`; the element is
//   pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> > (8 bytes, the CVectorMap of the anim-component TU).
//   Callers: 0x005E1E53, 0x005E2313, 0x005E289E (sort_heap), 0x005E2F82 (__partial_sort).
// Callee: __adjust_heap(first, 0, last - first, val, comp) @ 0x005E1210 (__fastcall: holeIndex 0 in edx).
// Shape: STLport 4.6 `*__result = *__first; __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val,
//   __comp);` -- the pair assignment and CIVCountedPointer's assignment inline (compare, release the old object through
//   slot 1, clear, copy, add-ref); the by-value copy of val is built in the outgoing argument area; val is destroyed on
//   exit (the callee owns its by-value parameter). Both pointers are read into locals before the compare (`cmp ecx,eax`;
//   `Object != other.Object` gives `cmp ecx,[esi+4]`), and the incoming one is re-read after the release.
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
        T* object = Object;
        T* incoming = other.Object;
        if (object != incoming) {
            Release();
            Object = other.Object;
            if (Object)
                ++Object->IVRefCount;
        }
        return *this;
    }
};

namespace std {
template <class _T1, class _T2> struct pair {              // STLport pair: explicit memberwise copy constructor
    _T1 first;
    _T2 second;
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}
typedef std::pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> > SAnimComponentPair;   // 8 bytes

template <class K, class V> class CKeyPairCompareLess { public:   // empty, passed by value
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};
typedef CKeyPairCompareLess<EAnimComponent, CIVCountedPointer<CAnimComponentBase> > TAnimComponentLess;

namespace std {
void __fastcall __adjust_heap(SAnimComponentPair* __first, int __holeIndex, int __len, SAnimComponentPair __val,
                              TAnimComponentLess __comp);   // 0x005E1210

template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
void __fastcall __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result,
                           _Tp __val, _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}

template void __fastcall __pop_heap(SAnimComponentPair*, SAnimComponentPair*, SAnimComponentPair*, SAnimComponentPair,
                                    TAnimComponentLess, int*);
}