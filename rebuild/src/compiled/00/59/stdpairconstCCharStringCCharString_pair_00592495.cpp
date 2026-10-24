// std::pair<const CCharString, CCharString>::pair(const pair<const char*, const char*>& p) -- build a string-pair map entry from a pair of literals
// Leaf: ??$?0PBDPBD@?$pair@$$CBVCCharString
// Retail: Fable.exe (TLC, VC7.1) @ 0x00592495, 37 bytes, `ret 4` (returns `this`).
// Callee: CCharString::CCharString(const char*, long = -1) @ 0x0099EBF0 (twice: first at +0, second at +4).
// Callers: 23 sites 0x0058CC86 .. 0x0058D1F3 in one CTCInventory routine, each storing two string literals into a
// frame `pair<const char*, const char*>` (`mov [ebp+4Ch],lit ; mov [ebp+50h],lit`), converting it here and passing
// the result to a map insert (0x00593ADE) -- the inline `m.insert(make_pair("A", "B"))` of a string -> string table.
// Label: the catalog "CCharString_Construct_Pair" is descriptive: the body is STLport's converting member-template
// constructor `template <class _U1, class _U2> pair(const pair<_U1, _U2>& __p) : first(__p.first),
// second(__p.second) {}`, emitted out of line by the size TU; each member is the implicit `const char*` ->
// CCharString conversion with the defaulted length.
// Shape: size TU (unaligned entry): `#pragma optimize("s", on)`.
#pragma optimize("s", on)

class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    template <class _U1, class _U2> pair(const pair<_U1, _U2>& __p) : first(__p.first), second(__p.second) {}
};

template pair<const CCharString, CCharString>::pair(const pair<const char*, const char*>&);
}