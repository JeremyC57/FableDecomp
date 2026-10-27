// std::pair::pair(const CWideString& __a, const long& __b) -- (<CWideString, long>) copy both members
// Leaf: ??0?$pair@VCWideString@@J@std@@QAE@ABVCWideString@@ABJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005D811A, 27 bytes, thiscall, `ret 8`.
// Identity (probable): the only caller is make_pair 0x005D837C (dead COMDAT) in the CTCInventoryStats module (0x005D64DD..0x005D7FAB OnSerialise); Ego_r.pdb has ??$make_pair@VCWideString@@J@std@@ (pair<CWideString, long>, a list element) directly after CTCInventoryStats::OnSerialise; the 4-byte second is the long.
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
template struct pair<CWideString, long >;
}