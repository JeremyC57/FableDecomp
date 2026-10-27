// std::__lower_bound(first, last, const unsigned long& val, MapperPred comp, int*) -- STLport binary search over a sorted pair<unsigned long, unsigned long> map of the occlusion TU, comparing each entry's key with val
// Leaf: ??$__lower_bound@PAU?$pair@KK@std@@KUMapperPred@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B8F180, 54 bytes, __fastcall (first ecx, last edx; val, comp, distance-type
//   pointer on the stack), `ret 0xc`.
// Identity (proven): Ego_r.pdb (Dinkumware spelling) ??$_Lower_bound@PAU?$pair@KK@std@@KHUMapperPred@?A0x...@@@std@@YI...
//   -- the STLport spelling is __lower_bound<pair<unsigned long,unsigned long>*, unsigned long, MapperPred, int>. It is the
//   8-byte-entry twin of the landed 0x00B8F1C0 (12-byte pair<unsigned long, CCountedPointer<CVertexBufferWin32> >) and sits
//   right before it; MapperPred is the occlusion TU's anonymous-namespace functor comparing an entry's key with val.
// Shape: distance is `(last - first) >> 3` (the 8-byte element divides with a bare `sar 3`); val is read once before the
//   loop; `__len = __len - __half - 1` is `or edi,-1 ; sub edi,edx ; add esi,edi`; the comparator inlines to an unsigned
//   compare of the entry's first dword.
namespace std {

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};

template <class _ForwardIter, class _Tp, class _Compare, class _Distance>
_ForwardIter __fastcall __lower_bound(_ForwardIter __first, _ForwardIter __last, const _Tp& __val, _Compare __comp, _Distance*)
{
    _Distance __len = __last - __first;
    _Distance __half;
    _ForwardIter __middle;

    while (__len > 0) {
        __half = __len >> 1;
        __middle = __first;
        __middle += __half;
        if (__comp(*__middle, __val)) {
            __first = __middle;
            ++__first;
            __len = __len - __half - 1;
        }
        else
            __len = __half;
    }
    return __first;
}

}   // namespace std

typedef std::pair<unsigned long, unsigned long> TOcclusionMapEntry;

namespace {
struct MapperPred {
    MapperPred() { }
    bool operator()(const TOcclusionMapEntry& entry, unsigned long id) const { return entry.first < id; }
};
}

template TOcclusionMapEntry* __fastcall std::__lower_bound(TOcclusionMapEntry*, TOcclusionMapEntry*,
                                                           const unsigned long&, MapperPred, int*);