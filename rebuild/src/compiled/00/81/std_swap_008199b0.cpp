// std::swap(_Tp& a, _Tp& b) -- STLport swap of two 40-byte map entries (a 4-byte key and a 36-byte trivially copied value, types unproven): copy a to a temporary, assign b to a, the temporary to b
// Leaf: ??$swap@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008199B0, 57 bytes (+ int3 padding to 64), `__fastcall` (/Gr: a in ecx, b in edx), `ret`.
// Identity (descriptive): std::swap<std::pair<CUnprovenKey_008199b0, CUnprovenValue_008199b0 > >. No call, jump or data reference reaches it (an emitted COMDAT of a map sort), so neither the key nor the value type is provable: it sits in the CSkeletalMorphs TU (0x00819280..0x0081AED0: the 36/40-byte fill / copy / heap helpers next to CSkeletalMorphs::TransferBinaryIn), whose CVectorMap<unsigned long, CSkeletalMorphs::CEntry> is one candidate for one of 0x008199B0 / 0x0081A900; nothing tells the two apart. The first dword is copied on its own into the temporary (STLport pair's user copy constructor), the value by `rep movsd`.
// Label: the manifest's "swap<CDebugSphere>" is propagated (family J013: six unreferenced swap COMDATs, two per element size;
//   retail has no identical-code folding, so the two of each size are two distinct pair types).
// Shape: STLport 4.6 `_Tp __tmp = __a; __a = __b; __b = __tmp;` over STLport's pair, whose user copy constructor copies
//   `first` (one dword) and then `second` (`rep movsd` of 9 dwords) into the temporary, while the implicit
//   assignments copy the whole 40-byte entry (`rep movsd` of 10 dwords).
class CUnprovenKey_008199b0 {                              // descriptive: a 4-byte key (enum / long / unsigned long)
public:
    unsigned long Value;
};
class CUnprovenValue_008199b0 {                            // descriptive: a 36-byte trivially copied value
public:
    unsigned long Words[9];
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
template void __fastcall swap<pair<CUnprovenKey_008199b0, CUnprovenValue_008199b0 > >(pair<CUnprovenKey_008199b0, CUnprovenValue_008199b0 >&,
                                                                          pair<CUnprovenKey_008199b0, CUnprovenValue_008199b0 >&);
}