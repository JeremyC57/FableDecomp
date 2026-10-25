// std::pair::pair(const CCharString& a, const CCharString& b) -- value constructor of pair<CCharString, CCharString>: copy-construct both strings
// Leaf: ??0?$pair@VCCharString@@V1@@std@@QAE@ABVCCharString@@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0076DD40, 32 bytes, __fastcall `ret 8`, returns `this`.
// Identity (label correction): the catalog's "pair<CCharString,CTCBase::CRegisteredTCInfo>" is propagated. The only caller,
//   0x00A3F2D0 (CSoundManagerLH, catalog vfunc_39), builds this pair from two CCharString references into a frame
//   temporary and then copies it (0x0099EC30 twice) plus a pointer into a 12-byte map value: the inner
//   `make_pair(a, b)` of a `insert(make_pair(make_pair(a, b), p))`. Both members use CCharString's copy constructor.
// Callee: CCharString::CCharString(const CCharString&) @ 0x0099EC30 (twice).
// Shape: STLport's `pair(const _T1& __a, const _T2& __b) : first(__a), second(__b)`; the first reference is loaded
//   before `push esi`, the second reloaded from the stack after the first call.
class CCharString {
public:
    void* PData;                                        // +0x00
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};

namespace std {
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x04
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) {}
};
template pair<CCharString, CCharString>::pair(const CCharString&, const CCharString&);
}