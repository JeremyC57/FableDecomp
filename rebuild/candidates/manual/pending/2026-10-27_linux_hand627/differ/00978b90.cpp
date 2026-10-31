// std::__adjust_heap(CThing** first, int holeIndex, int len, CThing* value, CThingCompare_InDirectionOfAndNearestToHeuristicBitch comp) -- STLport heap sift-down of things by the direction/distance comparator, then __push_heap 0x00978A00
// Grade: DIFFER(142v142) -- register rotation only in the 16-byte comparator copy into __push_heap's argument area: retail `mov eax,esp` with ecx/edx temps (two-deep pipelined loads), ours `mov ecx,esp` with edx/eax temps; a user C2DVector copy ctor (PDB ??0C2DVector@@QAE@ABV0@@Z) gives retail's eax pointer but loses the pipelining (one load ahead)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00978B90, 142 bytes, __fastcall (/Gr algorithm TU), `ret 18h`.
// Identity: reloc_bindings ??$__adjust_heap@PAPAVCThing@@HPAV1@VCThingCompare_InDirectionOfAndNearestToHeuristicBitch@?A0x...@@@std@@YIX
//   PAPAVCThing@@HHPAV1@V...@@Z (STLport 4.6 `std::__adjust_heap`; the anonymous-namespace hash is per-TU; the cutter label
//   `_Adjust_heap<...,_anon_FFAB493A...>` is the Dinkumware spelling of the Ego_r.pdb set). Comparator (Ego_r.pdb, 16 bytes):
//   C2DVector Pos +0, C2DVector Dir +8; const operator()(CThing*, CThing*) 0x00978790 (QBE_NPAVCThing@@0@Z).
// Callees: comparator operator() 0x00978790 (__thiscall on the by-value comparator), __push_heap 0x00978A00 (same instantiation).
// Tried: POD C2DVector (this file), C2DVector user copy ctor, non-POD C2DVector with only value ctors, a user comparator
//   copy ctor; QFE 4035 compiler. 0x00BEEDA0 (chunk 627) shows the same two-deep-copy residue. Next: lane gap: by-value
//   struct-argument copy scheduling (eax pointer + two-deep load pipeline) -- same with an extern or same-TU callee.
// Flags: /Gr
// Leaf: ??$__adjust_heap@PAPAVCThing@@
class CThing;
class C2DVector { public: float X, Y; };

namespace {
class CThingCompare_InDirectionOfAndNearestToHeuristicBitch { public:
    C2DVector Pos;                                   // +0x0
    C2DVector Dir;                                   // +0x8
    bool operator()(CThing* a, CThing* b) const;     // 0x00978790
};
}

namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __val, _Compare __comp);   // 0x00978A00

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp)
{
    _Distance __topIndex = __holeIndex;
    _Distance __secondChild = 2 * __holeIndex + 2;
    while (__secondChild < __len) {
        if (__comp(*(__first + __secondChild), *(__first + (__secondChild - 1))))
            __secondChild--;
        *(__first + __holeIndex) = *(__first + __secondChild);
        __holeIndex = __secondChild;
        __secondChild = 2 * (__secondChild + 1);
    }
    if (__secondChild == __len) {
        *(__first + __holeIndex) = *(__first + (__secondChild - 1));
        __holeIndex = __secondChild - 1;
    }
    __push_heap(__first, __holeIndex, __topIndex, __val, __comp);
}
template void __adjust_heap<CThing**, int, CThing*, CThingCompare_InDirectionOfAndNearestToHeuristicBitch>(
    CThing**, int, int, CThing*, CThingCompare_InDirectionOfAndNearestToHeuristicBitch);
}
