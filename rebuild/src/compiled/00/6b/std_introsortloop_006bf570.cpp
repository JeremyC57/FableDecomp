// std::__introsort_loop(pair<long, CMusicManager::CRegisteredMusicEntry>* first, pair* last, pair*, int depth_limit, CKeyPairCompareLess comp) -- STLport introsort loop of the registered-music vector map: median-of-three partition and recurse on the right part while more than 16 entries remain, falling back to partial_sort when the depth limit runs out
// Leaf: ??$__introsort_loop@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006BF570, 287 bytes (+ int3 padding to 304), `ret 0Ch`.
// Identity: the catalog's "Heap_SortWideStringElements" is a descriptive auto label. It recurses into itself
//   (0x006BF64F) and is called by the map's sort (0x006BF6DE). Element pair<long, CMusicManager::CRegisteredMusicEntry>
//   (Ego_r.pdb, 24 bytes: key +0, SrcFileName +4, DstFileName +8, Gain +0xC, Offset +0x10, CacheToHD +0x14) and the
//   empty signed key comparator CKeyPairCompareLess<long, CRegisteredMusicEntry>, as the landed partition 0x006BED50.
// Callees: __unguarded_partition(first, last, T pivot, comp) 0x006BED50, __partial_sort(first, middle, last, T*, comp)
//   0x006BF340, CWideString copy ctor 0x0099B720 (the pivot's two strings), itself.
// Shape: STLport 4.6 _algo.c `__introsort_loop` verbatim from a /Gr TU (`__fastcall`: first in ecx -> ebx, last in edx ->
//   ebp; the value-type pointer, the depth limit and the 4-byte comparator on the stack). `__median` is inlined (signed
//   key compares choosing a reference), the 24-byte pivot `_Tp(__median(..))` is built in the argument area by STLport
//   pair's memberwise copy constructor (key, then the entry's implicit copy through a second source/destination pair:
//   both strings out of line, Gain/Offset words, the flag byte) after the comparator push; partial_sort is the inline
//   front end that passes `(_Tp*)0`.
class CWideString {
public:
    void* PStringData;                                      // +0x00
    CWideString(const CWideString& other);                  // 0x0099B720
    ~CWideString();                                         // 0x0099B510 (not called here)
};

class CMusicManager {
public:
    class CRegisteredMusicEntry {                           // 20 bytes, implicit copy constructor
    public:
        CWideString SrcFileName;                            // +0x00
        CWideString DstFileName;                            // +0x04
        float       Gain;                                   // +0x08
        float       Offset;                                 // +0x0C
        bool        CacheToHD;                              // +0x10
    };
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair& __o) : first(__o.first), second(__o.second) { }
};
}

template <class K, class V>
class CKeyPairCompareLess {                                 // empty comparator, passed by value
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

namespace std {

const int __stl_threshold = 16;

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
                                                   _Compare __comp);                         // 0x006BED50
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*,
                               _Compare __comp);                                             // 0x006BF340

template <class _Tp, class _Compare>
inline void partial_sort(_Tp* __first, _Tp* __middle, _Tp* __last, _Compare __comp)
{
    __partial_sort(__first, __middle, __last, (_Tp*)0, __comp);
}

template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit,
                                 _Compare __comp)
{
    while (__last - __first > __stl_threshold) {
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

typedef pair<long, CMusicManager::CRegisteredMusicEntry> _MusicEntry;
template void __fastcall __introsort_loop<_MusicEntry*, _MusicEntry, int, CKeyPairCompareLess<long, CMusicManager::CRegisteredMusicEntry> >(
    _MusicEntry*, _MusicEntry*, _MusicEntry*, int, CKeyPairCompareLess<long, CMusicManager::CRegisteredMusicEntry>);

} // namespace std