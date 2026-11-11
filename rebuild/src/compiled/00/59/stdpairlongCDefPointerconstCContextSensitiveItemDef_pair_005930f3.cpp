// std::pair<long, CDefPointer<const CContextSensitiveItemDef> >::~pair() -- destroy one (priority, context-sensitive item def) pair of the inventory's context-sensitive sort: release the counted def pointer
// Leaf: ??1?$pair@JV?$CDefPointer@$$CBVCContextSensitiveItemDef@@@@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005930F3, 26 bytes, thiscall, plain `ret`; size-optimised TU (unaligned, `and [esi+4],0`).
// Identity (agentFAM06): FableWin.pdb ??1?$pair@JV?$CDefPointer@$$CBVCContextSensitiveItemDef@@@@@std@@QAE@XZ (the implicit pair
//   destructor; Ego_r.pdb holds the same pair's vector / _Pair_base / operator= / _Move_backward). Callers: CTCInventory::
//   UpdateContextSensitiveItems 0x00591B7F (two local pairs), its scalar deleting helper 0x005938D5, std::swap 0x005938F1 and the
//   STLport sort / heap helpers over the 8-byte pair with CContextSensitivePriorityCompare (0x00593924 __unguarded_linear_insert,
//   0x00593965 __push_heap, 0x00593E3E __adjust_heap, 0x00594637 __unguarded_partition, 0x00594697 the range destroy,
//   0x005946B2 __pop_heap, 0x00594946 __linear_insert, jmp 0x00593E10). The landed binder 0x00593924 already spells this symbol.
//   The release reads `second` at +4 (`first` is the long priority). Catalog CIVCountedPointer_Release is a placeholder.
// Body: the implicit destructor of the shared STLport 4.6 std::pair (rebuild/include/stlport/fable_containers.h) over the shared
//   CDefPointer<T> (engine/CDefPointer.h: ~CDefPointer() { Release(); }, the release in place). The helper below only makes
//   VC7.1 emit the implicit ??1 COMDAT; it is not a retail function.
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
#include "engine/CDefPointer.h"
class CContextSensitiveItemDef;

typedef std::pair<long, CDefPointer<const CContextSensitiveItemDef> > TContextSensitiveItemPair;

void EmitContextSensitiveItemPairDestructor(TContextSensitiveItemPair* p)   // emits the implicit ??1; not a retail function
{
    p->TContextSensitiveItemPair::~TContextSensitiveItemPair();
}
