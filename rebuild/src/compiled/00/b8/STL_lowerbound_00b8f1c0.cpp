// std::__lower_bound(first, last, const unsigned long& val, MapperPred comp, int*) -- STLport binary search over the occlusion manager's sorted flare vertex-buffer map (pair<unsigned long, CCountedPointer<CVertexBufferWin32> >), comparing each entry's key with val
// Leaf: ??$__lower_bound@PAU?$pair@KV?$CCountedPointer@VCVertexBufferWin32
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B8F1C0, 75 bytes, __fastcall (first ecx, last edx; val, comp, distance-type
//   pointer on the stack), `ret 0xc`.
// Identity: Ego_r.pdb (Dinkumware spelling) ??$_Lower_bound@PAU?$pair@KV?$CCountedPointer@VCVertexBufferWin32@@@@@std@@KHUMapperPred@?A0x...@@@std@@YI...
//   -- the STLport spelling is __lower_bound<pair*, unsigned long, MapperPred, int>. Callers: the lower_bound wrapper
//   0x00B8F2B0 (pushes the null distance pointer) and CEngineOcclusionManager::SelectFlareTestVertexBuffer 0x00B8DD04 over
//   FlareVertexBuffers (retail +0x24054). MapperPred is an anonymous-namespace functor of the occlusion TU comparing an
//   entry's key with an unsigned long. Catalog "BinarySearch_VertexBuffer" is descriptive.
// Shape: distance is `(last - first) / 12` (`imul 0x2AAAAAAB ; sar 1` with the sign fix); val is read once before the
//   loop; `__len = __len - __half - 1` is `or edx,-1 ; sub edx,eax ; add esi,edx`; the comparator inlines to an unsigned
//   compare of the entry's first dword.
class CVertexBufferWin32;

template <class T> class CCountedPointer {
public:
    T*    Data;                                    // +0x00
    void* Info;                                    // +0x04 shared count block
};

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

typedef std::pair<unsigned long, CCountedPointer<CVertexBufferWin32> > TFlareVertexBufferEntry;

namespace {
struct MapperPred {
    MapperPred() { }
    bool operator()(const TFlareVertexBufferEntry& entry, unsigned long id) const { return entry.first < id; }
};
}

template TFlareVertexBufferEntry* __fastcall std::__lower_bound(TFlareVertexBufferEntry*, TFlareVertexBufferEntry*,
                                                                 const unsigned long&, MapperPred, int*);