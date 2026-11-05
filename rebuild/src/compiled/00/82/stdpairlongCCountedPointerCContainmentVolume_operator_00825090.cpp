// std::pair<long, CCountedPointer<CContainmentVolume> >::operator=(const pair& other) -- the compiler-generated memberwise assignment of a vector-map entry: copy the key, then the counted pointer's operator= (release the old count block, freeing it and deleting the data at zero; adopt and add-ref the new one)
// Leaf: ??4?$pair@
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00825090, 78 bytes, thiscall, `ret 4`, returns this.
// Identity: called by std::__adjust_heap 0x00825B80 (callers: the landed std::__pop_heap 0x00826200 / std::__make_heap 0x008262B0 over pair<long, CCountedPointer<CContainmentVolume> >) and the vector-map insert at 0x00826DD0 (agent952 H047 swap 0x008250E0; CCachedVolumes::NavigationContainmentVolumes).
// Shape (family K012, representative 0x006B8E20): the implicit pair operator= emitted out of line (its address is taken
//   below, as retail's sort helpers call it); CCountedPointer::operator= reads the source data and block into locals
//   first, `if (Info != info) { DeleteData(); Data = data; Info = info; if (info) ++info->RefCount; }`, the release
//   re-reading Info after the decrement (`dec [eax] ; mov eax,[esi+N] ; cmp [eax],0`) and calling the block's
//   __fastcall deleter on its data.
#include "engine/CCountedPointer.h"
class CContainmentVolume;

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}
typedef std::pair<long, CCountedPointer<CContainmentVolume> > TEntry;
// Taking the implicit operator='s address makes VC7.1 emit it out of line.
TEntry& (TEntry::*g_pAssign_00825090)(const TEntry&) = &TEntry::operator=;