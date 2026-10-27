// std::swap(_Tp& a, _Tp& b) -- STLport swap of two 12-byte trivially copied elements (type unproven): copy a to a temporary, assign b to a, the temporary to b
// Leaf: ??$swap@VCUnprovenElement12_00adaf60@@@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ADAF60, 45 bytes, `__fastcall` (/Gr: a in ecx, b in edx), `ret`.
// Identity (descriptive): std::swap<CUnprovenElement12_00adaf60>. No call, jump or data reference reaches it (an emitted COMDAT of a sort); it sits among 12-byte sort helpers of the same TU (0x00ADAF90 insertion sort keyed at -8, 0x00ADB000 __adjust_heap<CBlockInfo*>); a candidate shared with 0x00ADAA00, and one symbol cannot own both.
// Shape (family K112, representative 0x004C1E90): STLport 4.6 `_Tp __tmp = __a; __a = __b; __b = __tmp;` over a 3-dword
//   element (the landed J013 swap 0x00758350 recipe): the temporary loaded first, then dword moves for both assignments.
class CUnprovenElement12_00adaf60 {                                             // descriptive: a 12-byte trivially copied element
public:
    unsigned long Words[3];
};
namespace std {
template <class _Tp>
inline void __fastcall swap(_Tp& __a, _Tp& __b)
{
    _Tp __tmp = __a;
    __a = __b;
    __b = __tmp;
}
template void __fastcall swap<CUnprovenElement12_00adaf60>(CUnprovenElement12_00adaf60&, CUnprovenElement12_00adaf60&);
}