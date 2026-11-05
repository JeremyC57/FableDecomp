// std::__introsort_loop(SStringKeyedChunk* first, SStringKeyedChunk* last, SStringKeyedChunk*, int depth_limit, CStringKeyedChunkLess comp) -- STLport introsort recursion over the 16-byte (CCharString key, {name, counted chunk}) records: partition around the median of three, recurse on the right part, loop on the left; partial-sort fallback when the depth runs out
// Leaf: ??$__introsort_loop@
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members m_pObject->Data, m_pCount->Info are the header CCountedPointer's (same offsets); the local SCountBlock (= CCPPointerInfo) members m_references->RefCount, m_pfnDestroy->DeleteFunc, m_pObject->Data are the header CCPPointerInfo's (same offsets).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A73A00, 216 bytes (+ int3 padding to 224), `ret 0Ch` (first in ecx, last in edx;
//   the value-type pointer, the depth limit and the empty comparator's slot on the stack).
// Callers: sort 0x00A73BA0 (landed) and itself. Callees: __median 0x00A70020 (landed, out of line here: `ret 8`),
//   __unguarded_partition 0x00A71910 (`ret 14h`), __partial_sort 0x00A733B0 (this chunk), CCharString copy constructor
//   0x0099EC30 (x2, the pivot copy).
// Label: catalog "Vector_IntrosortDepth_StringPair" is descriptive; the body is STLport _algo.c `__introsort_loop`.
// Element: pair<CCharString, {CCharString, CCountedPointer}> (the landed 0x00A73530 push_back records).
// Shape: STLport verbatim, as 0x00A73910, but the string-keyed __median is an out-of-line call: the partition's comp
//   slot is pushed first (arguments right to left), then the median's comp and c, and the pivot is copied from the
//   returned reference by pair's explicit copy ctor (two string copies, inline counted copy).
class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};

#include "engine/CCountedPointer.h"


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
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const;   // inline CCharString::operator< (inside __median)
};

typedef std::pair<CCharString, SNamedChunk>          SStringKeyedChunk;      // 0x10 bytes
typedef CKeyPairCompareLess<CCharString, SNamedChunk> CStringKeyedChunkLess;

namespace std {
template <class _Tp, class _Compare>
const _Tp& __fastcall __median(const _Tp& __a, const _Tp& __b, const _Tp& __c, _Compare __comp);   // 0x00A70020

template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot,
                                                   _Compare __comp);   // 0x00A71910
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*,
                               _Compare __comp);   // 0x00A733B0
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

template void __fastcall __introsort_loop(SStringKeyedChunk*, SStringKeyedChunk*, SStringKeyedChunk*, int, CStringKeyedChunkLess);
}