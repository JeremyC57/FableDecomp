// _STL::fill(pair<EBodyLocation,CWoundMorphs::CEntry>* first, pair<EBodyLocation,CWoundMorphs::CEntry>* last, const pair<EBodyLocation,CWoundMorphs::CEntry>& val) -- assign val to every 0x2C-byte wound-morph entry in [first, last)
// Leaf: fill
// Retail: Fable.exe (TLC, VC7.1) @ 0x00758220, 40 bytes, `ret 4` (link-time __fastcall: first in ecx, last in edx, the
//   value reference on the stack). No callees.
// Callers: 0x00759336 and 0x00759387, both inside vector<pair<EBodyLocation,CEntry>>::_M_fill_insert (0x00759290), which
//   copies its value into a frame temporary and fills the opened gap from it -- so the element is the CWoundMorphs map
//   entry (stride 0x2C, the 0x2E8BA2E9 divide by 44 in that caller).
// Label: the manifest's `_Fill<CHeroMarriageInfo*,CHeroMarriageInfo>` is a propagation over the byte-identical skeleton
//   (STLport spells the algorithm `fill`); the element type comes from the caller.
// Shape: STLport `for (; first != last; ++first) *first = val;` -- the implicit assignment of the 11-dword POD-like pair
//   is one `rep movsd` (ecx = 0Bh) per element, the cursor kept in eax and copied to edi each pass.
enum EBodyLocation { BODY_LOCATION_NONE = 0 };

struct CWoundMorphsEntry {                       // CWoundMorphs::CEntry, 40 bytes of POD
    long Words[10];
};

namespace _STL {
template <class A, class B> struct pair {
    A first;                                     // +0x00
    B second;                                    // +0x04
};

typedef pair<EBodyLocation, CWoundMorphsEntry> TWoundPair;

void __fastcall fill(TWoundPair* first, TWoundPair* last, const TWoundPair& val)
{
    for (; first != last; ++first)
        *first = val;
}
}