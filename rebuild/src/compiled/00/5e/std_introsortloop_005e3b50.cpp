// std::__introsort_loop(pair<unsigned long, CAnimationEntry>* first, pair<unsigned long, CAnimationEntry>* last, pair<unsigned long, CAnimationEntry>*, int depth_limit, CKeyPairCompareLess comp) -- STLport introsort recursion over CRC-keyed animation entries: median-of-three partition, recurse on the right part until ranges reach 16 entries; at depth zero fall back to partial_sort
// Leaf: ??$__introsort_loop@
// Retail: Fable.exe (TLC, VC7.1) @ 0x005E3B50, 265 bytes (+3 int3 padding), `ret 0Ch`. /Gr algorithm TU written as explicit
//   __fastcall: first in ecx, last in edx; the value-type pointer tag, the depth limit and the 4-byte empty comparator slot
//   on the stack.
// Callers: sort 0x005E3CD0 (landed; depth 2*lg(n)) and itself. Callees: __unguarded_partition 0x005E2100 (the pivot
//   T(__median(...)) passed by value: a 20-byte copy built in the argument area, add-ref inline), __partial_sort 0x005E3550
//   (behind the inline partial_sort). __median and the comparator are inlined here (unsigned `cmp`/`jae` on the keys).
// Element: CAnimationSet's Anims entry pair<unsigned long, CAnimationEntry> (Ego_r.pdb names
//   `CKeyPairCompareLess<unsigned long, CAnimationEntry>` sort helpers); CAnimationEntry as in the landed 0x005E36B0.
// Label: catalog "std_sort_IntrosortPartition20Byte" is descriptive; this is STLport 4.6 `__introsort_loop`.
// Shape: STLport 4.6 _algo.c verbatim, as the landed 0x00AF3AA0; the 20-byte divide is `imul 0x66666667 ; sar edx,3`.
inline void* operator new(unsigned int, void* __p) throw() { return __p; }

class CAnimComponentMap { public:                                 // intrusively counted (CIVCountedPointeeBase): vptr, count at +4
    virtual void v00();
    virtual void Delete();                                 // slot 1 (+0x04)
    long IVRefCount;                                       // +0x04
};

template <class T> class CIVCountedPointer { public:              // one word: the intrusively counted object
    T* Object;
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->IVRefCount;
    }
    ~CIVCountedPointer()
    {
        T* object = Object;
        if (object) {
            if (--object->IVRefCount == 0)
                object->Delete();
            Object = 0;
        }
    }
};

class CDefString { public:                                        // one word: the def string table position
    long TablePos;
};

class CAnimationEntry {                                    // Ego_r.pdb, 0x10 bytes
public:
    long                                 BankIndex;        // +0x00
    CDefString                           AnimName;         // +0x04
    CDefString                           GroupName;        // +0x08
    CIVCountedPointer<CAnimComponentMap> Components;       // +0x0c
};

namespace std {
template <class _T1, class _T2> struct pair {              // STLport 4.6 pair: explicit memberwise copy constructor
    _T1 first;
    _T2 second;
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

typedef std::pair<unsigned long, CAnimationEntry> CAnimPair;
typedef CKeyPairCompareLess<unsigned long, CAnimationEntry> CAnimLess;

namespace std {
template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp);   // 0x005E2100
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*, _Compare __comp);   // 0x005E3550

template <class _Tp>
inline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }

template <class _RandomAccessIter, class _Compare>
inline void partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Compare __comp)
{
    __partial_sort(__first, __middle, __last, __value_type(__first), __comp);
}

template <class _Tp, class _Compare>
inline const _Tp& __median(const _Tp& __a, const _Tp& __b, const _Tp& __c, _Compare __comp)
{
    if (__comp(__a, __b))
        if (__comp(__b, __c))
            return __b;
        else if (__comp(__a, __c))
            return __c;
        else
            return __a;
    else if (__comp(__a, __c))
        return __a;
    else if (__comp(__b, __c))
        return __c;
    else
        return __b;
}

template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit, _Compare __comp)
{
    while (__last - __first > 16) {
        if (__depth_limit == 0) {
            partial_sort(__first, __last, __last, __comp);
            return;
        }
        --__depth_limit;
        _RandomAccessIter __cut =
            __unguarded_partition(__first, __last,
                                  _Tp(__median(*__first, *(__first + (__last - __first) / 2), *(__last - 1), __comp)),
                                  __comp);
        __introsort_loop(__cut, __last, (_Tp*)0, __depth_limit, __comp);
        __last = __cut;
    }
}
}

template void __fastcall std::__introsort_loop<CAnimPair*, CAnimPair, int, CAnimLess>(CAnimPair*, CAnimPair*, CAnimPair*, int, CAnimLess);