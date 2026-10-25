// std::__introsort_loop(SKeyedChunk* first, SKeyedChunk* last, SKeyedChunk*, int depth_limit, CKeyedChunkLess comp) -- STLport introsort recursion over the 16-byte (long key, {name, counted chunk}) records: partition around the median of three, recurse on the right part, loop on the left; partial-sort fallback when the depth runs out
// Leaf: ??$__introsort_loop@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A73910, 234 bytes (+ int3 padding to 240), `ret 0Ch` (first in ecx, last in edx;
//   the value-type pointer, the depth limit and the empty comparator's slot on the stack).
// Callers: sort 0x00A73B20 (landed) and itself. Callees: __unguarded_partition 0x00A71880 (`ret 14h`: the 16-byte pivot
//   and comp), __partial_sort 0x00A73330 (this chunk), CCharString copy constructor 0x0099EC30 (the pivot copy).
// Label: catalog "Introsort_QuickPartition_Recursive" is descriptive; the body is STLport _algo.c `__introsort_loop`.
// Element: pair<long, {CCharString, CCountedPointer}> ordered by the signed key (the landed 0x00A734D0 push_back records).
// Shape: STLport verbatim -- `while (last - first > __stl_threshold) { if (depth_limit == 0) { partial_sort(first, last,
//   last, comp); return; } --depth_limit; cut = __unguarded_partition(first, last, _Tp(__median(*first, *(first +
//   (last - first) / 2), *(last - 1), comp)), comp); __introsort_loop(cut, last, (_Tp*)0, depth_limit, comp); last = cut; }`
//   with the inline __median over the inline key compare (three keys in ecx/edx/ebp, the chosen element's address in
//   eax) and the pivot built by pair's explicit copy ctor in the argument area.
class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};

struct SCountBlock {
    long m_references;                                  // +0
    void (__fastcall* m_pfnDestroy)(void*);             // +4
    void* m_pObject;                                    // +8
};

template <class T>
class CCountedPointer {
public:
    T* m_pObject;                                       // +0
    SCountBlock* m_pCount;                              // +4
    CCountedPointer(const CCountedPointer& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount)
    {
        if (m_pCount) ++m_pCount->m_references;
    }
};

class C3DAnimFileRootChunk;

struct SNamedChunk {                                    // 12 bytes
    CCharString                             Name;       // +0x00
    CCountedPointer<C3DAnimFileRootChunk>   Chunk;      // +0x04
};

namespace std {
template <class _T1, class _T2> class pair {
public:
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

typedef std::pair<long, SNamedChunk>          SKeyedChunk;    // 0x10 bytes
typedef CKeyPairCompareLess<long, SNamedChunk> CKeyedChunkLess;

namespace std {
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

template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot,
                                                   _Compare __comp);   // 0x00A71880
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*,
                               _Compare __comp);   // 0x00A73330
template <class _Tp> inline _Tp* __value_type_of(_Tp*) { return (_Tp*)0; }
template <class _RandomAccessIter, class _Compare>
inline void partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Compare __comp)
{
    __partial_sort(__first, __middle, __last, __value_type_of(__first), __comp);
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

template void __fastcall __introsort_loop(SKeyedChunk*, SKeyedChunk*, SKeyedChunk*, int, CKeyedChunkLess);
}