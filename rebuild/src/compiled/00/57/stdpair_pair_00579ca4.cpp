// std::pair::pair(const first_type& a, const second_type& b) -- (pair<EHeroAbility, SExpSpendUndo >) STLport member-wise constructor: the 4-byte first copied inline, the second through its out-of-line copy constructor; returns this
// Leaf: ??0?$pair@W4EHeroAbility@@USExpSpendUndo@@@std@@QAE@ABW4EHeroAbility@@ABUSExpSpendUndo@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00579CA4, 29 bytes, thiscall `ret 8`.
// Identity (probable): std::pair<EHeroAbility, SExpSpendUndo>::pair(const first_type&, const second_type&); evidence: probable (which enum is layout evidence): its only caller is the unreferenced make_pair-shaped helper 0x00579E05 (cdecl, the pair returned through the hidden pointer) that sits next to the converting pair<const EHeroAbility, SExpSpendUndo> constructor 0x00579E1B used by CTCInventoryExperience::BuyAbility on UndoableAbilityBuys (+0x14C; Ego_r.pdb map<EHeroAbility, SExpSpendUndo>); the twin 0x00579CC1 / 0x00579E38 / 0x00579E4E chain is BuyStat's (EHeroTrainableStatType). Both are pair<enum, SExpSpendUndo> (callee: SExpSpendUndo's copy constructor 0x00579C35), and Ego_r.pdb has exactly these two SExpSpendUndo maps; the emission order (BuyAbility's instantiations first) assigns this one to EHeroAbility.
// Label: the manifest's "sub_00579ca4" is propagated (family H046: six byte-identical bodies, each its own COMDAT; retail has
//   no identical-code folding, so each address is one pair type).
// Callee: 0x00579C35 (the second member's copy constructor, on `lea ecx,[esi+4]`).
// Shape: STLport 4.6 _pair.h `pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}`: the first word is loaded
//   through its reference before the second reference is pushed (as the landed pair<long, CCharString> 0x00695250).
#pragma optimize("s", on)                                 // size-optimised TU (unaligned start)
enum EHeroAbility { EHeroAbility_Force32 = 0x7fffffff };                       // enumerators not needed
enum EHeroTrainableStatType { EHeroTrainableStatType_Force32 = 0x7fffffff };   // enumerators not needed
struct SExpSpendUndo {                                     // Ego_r.pdb struct (only its copy constructor matters here)
    unsigned char _body[0x38];
    SExpSpendUndo(const SExpSpendUndo& other);             // 0x00579C35, PDB ??0SExpSpendUndo@@QAE@ABU0@@Z
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
typedef EHeroAbility _First;
typedef SExpSpendUndo _Second;
template pair<_First, _Second>::pair(const _First&, const _Second&);
} // namespace std