// std::pair<const CCharString, unsigned long>::pair(const pair&) -- copy constructor: the first member through its out-of-line copy constructor, then the 4-byte second; returns this
// Leaf: ??0?$pair@$$CBVCCharString@@K@std@@QAE@ABU01@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004297DB, 27 bytes, thiscall `ret 4`.
// Identity (proven): std::pair<const CCharString, unsigned long>::pair(const pair&); evidence: its callers are the tree's node factory _M_create_node 0x0042B587 (0x0042B59D, placement copy into the 0x18-byte node at +0x10) and the `_Construct` helper 0x0042A90C; the node factory serves _M_insert 0x0042BCB5 <- insert_unique <- insert(hint) 0x0042CDA6 <- the landed map<CCharString, unsigned long>::operator[] 0x0042D131, which the landed CCodeSectionManager::RegisterSection 0x009F05A0 calls on ShortNameToHandle / LongNameToHandle (Ego_r.pdb map<CCharString, unsigned long>), so the node value is pair<const CCharString, unsigned long>.
// Label: the manifest's "CComboMultiplierData" is propagated (family H044: six byte-identical bodies, each its own COMDAT; retail has
//   no identical-code folding, so each address is one record type).
// Callee: 0x0099EC30 (the first member's out-of-line copy).
// Shape: `first(o.first), second(o.second)` (or the matching assignments): edi holds the source, esi `this`, the 4-byte
//   second member is one eax move after the call.
#pragma optimize("s", on)                                 // size-optimised TU (unaligned start)
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
template pair<const CCharString, unsigned long>::pair(const pair<const CCharString, unsigned long>&);
} // namespace std
