// std::pair::pair(const pair& o) -- (pair<const CCharString, CCountedPointer<NUISystem::CComponent> >) STLport copy constructor: the first member through CCharString's copy constructor, then the second through CCountedPointer's copy constructor; returns this
// Leaf: ??0?$pair@$$CBVCCharString@@V?$CCountedPointer@VCComponent@NUISystem@@@@@std@@QAE@ABU01@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0042A961, 33 bytes, thiscall `ret 4` (a size-optimised /Os module: unaligned entry).
// Identity (proven): std::pair<const CCharString, CCountedPointer<NUISystem::CComponent> >::pair(const pair&); evidence: its callers are the tree's node factory _M_create_node 0x0042BE2F (0x0042BE45, placement copy into the 0x1C-byte node at +0x10) and the `_Construct` COMDAT 0x0042B619; the node factory serves _M_insert 0x0042CC03 <- insert_unique 0x0042CC98 / insert(hint) 0x0042CF69 <- 0x0042D2A9 <- the landed map<CCharString, CCountedPointer<NUISystem::CComponent> >::operator[] 0x0042D45D (NUISystem::CManager::CreateMenuEntryComponent; Ego_r.pdb CManager::BastardChildren), so the node value is that map's value_type; the second member's copy 0x00428928 copies Data and Info and bumps *Info (Ego_r CCountedPointer<NUISystem::CComponent> {Data, Info})
// Label: the manifest's "pair<CCharString,CCharString><CCharString,CCharString>" is propagated (family J041: five byte-similar pair bodies, each its own COMDAT; retail has
//   no identical-code folding, so each address is one pair type).
// Callees: CCharString::CCharString(const CCharString&) 0x0099EC30 (PDB ??0CCharString@@QAE@ABV0@@Z), CCountedPointer<NUISystem::CComponent>::CCountedPointer(const CCountedPointer&) 0x00428928 (PDB ??0?$CCountedPointer@VCComponent@NUISystem@@@@QAE@ABV0@@Z).
// Shape: STLport 4.6 _pair.h `pair(const pair<_T1,_T2>& __o) : first(__o.first), second(__o.second) {}`: edi holds the source, esi `this`; the second member sits at +4.
#pragma optimize("s", on)                                 // size-optimised TU (unaligned start)
#include <stddef.h>
class CCharString {                                      // 4 bytes (Ego_r)
public:
    void* PStringData;                                     // +0x0
    CCharString(const CCharString& other);                 // 0x0099EC30
    ~CCharString();                                        // 0x0099EAE0
};
class CCPPointerInfo;
namespace NUISystem { class CComponent; }
template <class T> class CCountedPointer {                 // Ego_r.pdb, 8 bytes
public:
    T*              Data;                                  // +0x0
    CCPPointerInfo* Info;                                  // +0x4 (its first dword is the count the copy bumps)
    CCountedPointer(const CCountedPointer& other);         // 0x00428928 for NUISystem::CComponent
};
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                             // +0x0
    _T2 second;                                            // +0x4
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }   // STLport 4.6 _pair.h
};
template pair<const CCharString, CCountedPointer<NUISystem::CComponent> >::pair(const pair<const CCharString, CCountedPointer<NUISystem::CComponent> >&);
} // namespace std