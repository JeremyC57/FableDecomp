// std::swap(_Tp& a, _Tp& b) -- STLport swap of two 56-byte map entries (a 4-byte key and a 52-byte trivially copied value, types unproven): copy a to a temporary, assign b to a, the temporary to b
// Leaf: ??$swap@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A312E0, 57 bytes (+ int3 padding to 64), `__fastcall` (/Gr: a in ecx, b in edx), `ret`.
// Identity (descriptive): std::swap<std::pair<CUnprovenKey_00a312e0, CUnprovenValue_00a312e0 > >. No call, jump or data reference reaches it (an emitted COMDAT of a map sort), so neither the key nor the value type is provable: it sits in the C3DMeshFile TU between the 56-byte __unguarded_linear_insert bodies 0x00A31280 and 0x00A31320 (the latter over pair<unsigned long, C3DDummyObject>, the CPreAllocVectorMap<unsigned long, C3DDummyObject> of the mesh file; Ego_r.pdb C3DDummyObject is 52 bytes); pair<unsigned long, C3DDummyObject> is one candidate for one of 0x00A312E0 / 0x00A32390 and nothing tells the two apart. The first dword is copied on its own into the temporary (STLport pair's user copy constructor), the value by `rep movsd`.
// Label: the manifest's "swap<CDebugSphere>" is propagated (family J013: six unreferenced swap COMDATs, two per element size;
//   retail has no identical-code folding, so the two of each size are two distinct pair types).
// Shape: STLport 4.6 `_Tp __tmp = __a; __a = __b; __b = __tmp;` over STLport's pair, whose user copy constructor copies
//   `first` (one dword) and then `second` (`rep movsd` of 13 dwords) into the temporary, while the implicit
//   assignments copy the whole 56-byte entry (`rep movsd` of 14 dwords).
class CUnprovenKey_00a312e0 {                              // descriptive: a 4-byte key (enum / long / unsigned long)
public:
    unsigned long Value;
};
class CUnprovenValue_00a312e0 {                            // descriptive: a 52-byte trivially copied value
public:
    unsigned long Words[13];
};
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x04
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) {}
};
template <class _Tp>
inline void __fastcall swap(_Tp& __a, _Tp& __b)
{
    _Tp __tmp = __a;
    __a = __b;
    __b = __tmp;
}
template void __fastcall swap<pair<CUnprovenKey_00a312e0, CUnprovenValue_00a312e0 > >(pair<CUnprovenKey_00a312e0, CUnprovenValue_00a312e0 >&,
                                                                          pair<CUnprovenKey_00a312e0, CUnprovenValue_00a312e0 >&);
}