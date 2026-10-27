// std::swap(_Tp& a, _Tp& b) -- STLport swap of two 12-byte trivially copied elements (type unproven): copy a to a temporary, assign b to a, the temporary to b
// Leaf: ??$swap@VCUnprovenElement12_004c1e90@@@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C1E90, 45 bytes, `__fastcall` (/Gr: a in ecx, b in edx), `ret`.
// Identity (descriptive): std::swap<CUnprovenElement12_004c1e90>. No call, jump or data reference reaches it (an emitted COMDAT of a sort); it sits among the NGraphicAppearance::CMeshInfo sort helpers (0x004C1EC0 / 0x004C1F10 __unguarded_linear_insert, 0x004C1F80 _Push_heap with CSortPrimaryMeshesFirst/Last); Ego_r.pdb CMeshInfo is 12 bytes, a candidate shared with 0x004C2B60.
// Shape (family K112, representative 0x004C1E90): STLport 4.6 `_Tp __tmp = __a; __a = __b; __b = __tmp;` over a 3-dword
//   element (the landed J013 swap 0x00758350 recipe): the temporary loaded first, then dword moves for both assignments.
class CUnprovenElement12_004c1e90 {                                             // descriptive: a 12-byte trivially copied element
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
template void __fastcall swap<CUnprovenElement12_004c1e90>(CUnprovenElement12_004c1e90&, CUnprovenElement12_004c1e90&);
}