// std::pair::pair(const CWideString& __a, const bool& __b) -- (<CWideString, bool>) copy both members
// Leaf: ??0?$pair@VCWideString@@_N@std@@QAE@ABVCWideString@@AB_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004851B6, 27 bytes, thiscall, `ret 8`.
// Identity (probable): the only caller is make_pair 0x004855DB (dead COMDAT, `pair(a, b)` into the return slot); the module holds map<CWideString, bool> (its landed _Rb_tree<CWideString, pair<const CWideString, bool>, ...>::_M_find 0x004856CD); Ego_r.pdb has ??$make_pair@VCWideString@@AA_N@std@@ (pair<CWideString, bool>) in the matching module; the copy constructor 0x0099B720 is bound ??0CWideString@@QAE@ABV0@@Z; the byte store is the bool.
// Callees: CWideString copy constructor 0x0099B720.
// Shape (family M068, representative 0x004851B6): STLport `pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}`
//   in a size-optimised module (unaligned entry, `push [esp+8]` straight from the argument slot): `optimize("s")`.
#pragma optimize("s", on)
class CWideString {                                       // 4 bytes (PStringData)
public:
    CWideString(const CWideString& other);                // 0x0099B720
    void* PStringData;
};
namespace std {
template <class _T1, class _T2> struct pair {
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    _T1 first;
    _T2 second;
};
template struct pair<CWideString, bool >;
}