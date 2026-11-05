// std::pair<long, CCountedPointer<CRegionConnectionNode> >::operator=(const pair&) -- the implicit copy assignment: the key, then the counted pointer (release old, share new)
// Leaf: ??4?$pair@
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe @ 0x00448380, 78 bytes, `ret 4`, returns this; callers 0x004489F6 / 0x00448EB4 in the heap/insertion sort
// code over this 12-byte element (with 0x00448910 / 0x004488B0). Catalog label NInventory::CItem::operator= is wrong.
// Body: first = o.first; CCountedPointer::operator=: if (Info != o.Info) { release ours (if Info, --RefCount, at zero
// DeleteFunc(Data) __fastcall + operator delete(Info) 0x00BFE9BC); Data = o.Data; Info = o.Info; if (Info) ++RefCount }.
// Layout: pair {long first +0, CCountedPointer second: Data +4, Info +8}; CCPPointerInfo {RefCount, DeleteFunc, Data}.
// Shape: /O2 (16-aligned entry); the source Data / Info read into locals first (retail loads both before the compare); the compiler-generated operator=, emitted out of line by an assignment in a helper.
#include "engine/CCountedPointer.h"
class CRegionConnectionNode;
namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
}
typedef std::pair<long, CCountedPointer<CRegionConnectionNode> > CRegionNodeEntry;
void AssignRegionNodeEntry(CRegionNodeEntry& to, const CRegionNodeEntry& from)      // emission helper, not a retail function
{
    to = from;
}