// std::pair::pair(const pair& o) -- (pair<CCharString, CCharString>) STLport copy constructor: the first member through CCharString's copy constructor, then the second through CCharString's copy constructor; returns this
// Leaf: ??0?$pair@VCCharString@@V1@@std@@QAE@ABU01@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0059256C, 33 bytes, thiscall `ret 4` (a size-optimised /Os module: unaligned entry).
// Identity (proven): std::pair<CCharString, CCharString>::pair(const pair&); evidence: its callers are the list node factory 0x0059330E (0x00593324: malloc(0x10), placement copy at +8 after the next/prev links) and the `_Construct` COMDAT 0x00592D68; the node factory serves list::insert(iterator, const value_type&) 0x00593ADE, which CTCInventory::OpenPCOptionsMenu 0x0058AF10 calls on its local list ([ebp+58h]); both members copy through CCharString's copy constructor; Ego_r.pdb has list<pair<CCharString, CCharString> > and make_pair<CCharString, CCharString>, a non-const first (a list element must be assignable)
// Label: the manifest's "pair<CCharString,CCharString><CCharString,CCharString>" is propagated (family J041: five byte-similar pair bodies, each its own COMDAT; retail has
//   no identical-code folding, so each address is one pair type).
// Callees: CCharString::CCharString(const CCharString&) 0x0099EC30 (PDB ??0CCharString@@QAE@ABV0@@Z), twice.
// Shape: STLport 4.6 _pair.h `pair(const pair<_T1,_T2>& __o) : first(__o.first), second(__o.second) {}`: edi holds the source, esi `this`; the second member sits at +4.
#pragma optimize("s", on)                                 // size-optimised TU (unaligned start)
#include <stddef.h>
class CCharString {                                      // 4 bytes (Ego_r)
public:
    void* PStringData;                                     // +0x0
    CCharString(const CCharString& other);                 // 0x0099EC30
    ~CCharString();                                        // 0x0099EAE0
};
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                             // +0x0
    _T2 second;                                            // +0x4
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }   // STLport 4.6 _pair.h
};
template pair<CCharString, CCharString>::pair(const pair<CCharString, CCharString>&);
} // namespace std