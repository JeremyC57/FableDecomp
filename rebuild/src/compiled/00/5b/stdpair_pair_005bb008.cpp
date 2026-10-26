// std::pair::pair(const first_type& a, const second_type& b) -- (pair<const long, CEnginePrimitiveHandle >) STLport member-wise constructor: the 4-byte first copied inline, the second through its out-of-line copy constructor; returns this
// Leaf: ??0?$pair@$$CBJVCEnginePrimitiveHandle@@@std@@QAE@ABJABVCEnginePrimitiveHandle@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BB008, 29 bytes, thiscall `ret 8`.
// Identity (proven): std::pair<const long, CEnginePrimitiveHandle>::pair(const first_type&, const second_type&); evidence: its only caller is the landed map<long, CEnginePrimitiveHandle>::operator[] 0x005BBFD6 (0x005BC015; CTCInventoryClothing::MorphPrimitiveHandles, Ego_r.pdb map<long, CEnginePrimitiveHandle>), whose default handle temporary it copies through 0x0043F450 (so 0x0043F450, catalogued as a C3DAnimationBlendState constructor, is CEnginePrimitiveHandle's copy constructor).
// Label: the manifest's "_Pair_base<NUISystem::CComponent*_const,std::pair<CWideString,long>_>" is propagated (family H046: six byte-identical bodies, each its own COMDAT; retail has
//   no identical-code folding, so each address is one pair type).
// Callee: 0x0043F450 (the second member's copy constructor, on `lea ecx,[esi+4]`).
// Shape: STLport 4.6 _pair.h `pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}`: the first word is loaded
//   through its reference before the second reference is pushed (as the landed pair<long, CCharString> 0x00695250).
#pragma optimize("s", on)                                 // size-optimised TU (unaligned start)
class CEnginePrimitiveHandle {                            // Ego_r.pdb, 8 bytes (vptr, counted pointer)
public:
    unsigned char _body[8];
    CEnginePrimitiveHandle(const CEnginePrimitiveHandle& other);   // 0x0043F450, PDB ??0CEnginePrimitiveHandle@@QAE@ABV0@@Z
};

namespace std {
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;                                             // +0x0
    _T2 second;                                            // +0x4
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }   // STLport 4.6 _pair.h
};
typedef const long _First;
typedef CEnginePrimitiveHandle _Second;
template pair<_First, _Second>::pair(const _First&, const _Second&);
} // namespace std