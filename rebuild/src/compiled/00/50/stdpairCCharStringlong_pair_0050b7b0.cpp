// std::pair<CCharString, long>::pair(const pair&) -- copy constructor: the first member through its out-of-line copy constructor, then the 4-byte second; returns this
// Leaf: ??0?$pair@VCCharString@@J@std@@QAE@ABU01@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0050B7B0, 27 bytes, thiscall `ret 4`.
// Identity (proven): std::pair<CCharString, long>::pair(const pair&); evidence: its callers are the landed deque<pair<CCharString, long> >::push_back 0x00656E0C (0x00656E24) and _M_push_back_aux_v 0x006569DA (0x006569E8, 0x00656A12), the queue NPlayerGui::CDrawGuildSeal::Messages (Ego_r.pdb queue<pair<CCharString, long> >) that the landed AddMessage 0x0064E0DA pushes, and the fill helper at 0x00655624.
// Label: the manifest's "CComboMultiplierData" is propagated (family H044: six byte-identical bodies, each its own COMDAT; retail has
//   no identical-code folding, so each address is one record type).
// Callee: 0x0099EC30 (the first member's out-of-line copy).
// Shape: `first(o.first), second(o.second)` (or the matching assignments): edi holds the source, esi `this`, the 4-byte
//   second member is one eax move after the call.
class CCharString {
public:
    void* PStringData;                                     // +0x0
    CCharString(const CCharString& other);                 // 0x0099EC30
    ~CCharString();                                        // 0x0099EAE0
};
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }   // STLport 4.6 _pair.h
};
template pair<CCharString, long>::pair(const pair<CCharString, long>&);
} // namespace std
