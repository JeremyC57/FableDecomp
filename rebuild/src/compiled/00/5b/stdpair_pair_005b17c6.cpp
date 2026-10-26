// std::pair::pair(const first_type& a, const second_type& b) -- (pair<NUISystem::CComponent* const, std::pair<CWideString, long> >) STLport member-wise constructor: the 4-byte first copied inline, the second through its out-of-line copy constructor; returns this
// Leaf: ??0?$pair@QAVCComponent@NUISystem@@U?$pair@VCWideString@@J@std@@@std@@QAE@ABQAVCComponent@NUISystem@@ABU?$pair@VCWideString@@J@1@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005B17C6, 29 bytes, thiscall `ret 8`.
// Identity (proven): std::pair<NUISystem::CComponent* const, std::pair<CWideString, long>>::pair(const first_type&, const second_type&); evidence: its only caller is the landed map<NUISystem::CComponent*, pair<CWideString, long> >::operator[] 0x005B3100 (0x005B3135: CTCInventoryTrade NumBuyableThings / NumSellableThings / NumWantedThings, Ego_r.pdb), and its callee 0x005B12F4 is the landed pair<CWideString, long> copy constructor.
// Label: the manifest's "_Pair_base<NUISystem::CComponent*_const,std::pair<CWideString,long>_>" is propagated (family H046: six byte-identical bodies, each its own COMDAT; retail has
//   no identical-code folding, so each address is one pair type).
// Callee: 0x005B12F4 (the second member's copy constructor, on `lea ecx,[esi+4]`).
// Shape: STLport 4.6 _pair.h `pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}`: the first word is loaded
//   through its reference before the second reference is pushed (as the landed pair<long, CCharString> 0x00695250).
#pragma optimize("s", on)                                 // size-optimised TU (unaligned start)
namespace NUISystem { class CComponent; }
class CWideString {                                       // 4 bytes
public:
    void* PStringData;
    CWideString(const CWideString& other);                 // 0x0099B720
};
namespace std { template <class _T1, class _T2> struct pair; }

namespace std {
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;                                             // +0x0
    _T2 second;                                            // +0x4
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }   // STLport 4.6 _pair.h
};
typedef NUISystem::CComponent* const _First;
typedef std::pair<CWideString, long> _Second;
template pair<_First, _Second>::pair(const _First&, const _Second&);
} // namespace std