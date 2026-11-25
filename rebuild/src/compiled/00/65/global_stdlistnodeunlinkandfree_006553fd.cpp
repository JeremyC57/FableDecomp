// std::list<NPlayerGui::CAutoPickupBar::CAutoPickup>::erase(iterator) -- STLport 4.6 _list.h erase: unlink the node from its neighbours, destroy the (trivial) record, free the node, return the next node
// Leaf: ?erase@?$list@VCAutoPickup@CAutoPickupBar@NPlayerGui@@V?$allocator@VCAutoPickup@CAutoPickupBar@NPlayerGui@@@std@@@std@@QAE?AU?$_List_iterator@VCAutoPickup@CAutoPickupBar@NPlayerGui@@U?$_Nonconst_traits@VCAutoPickup@CAutoPickupBar@NPlayerGui@@@std@@@2@U32@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006553FD, 32 bytes, thiscall, iterator by value, result through the hidden pointer, `ret 8`; size TU
//   (`pop ecx` after free). Callee: free 0x00BFEA14 (MSVCR71).
// Identity (NF391 trace, NF396 re-check): its only reference is the call at 0x00655AB3 in 0x00655AA4, the list's pop_front
//   (`erase(begin())`: the head's next node passed by value, the result dropped); 0x00655AA4's only caller is 0x00646EA2 in
//   NPlayerGui::CAutoPickupBar::DrawStatBarOnLeft 0x00646DBD (RTTI: vftable 0x0125A768 slot 0, COL type .?AVCAutoPickupBar@NPlayerGui@@).
//   FableWin's CAutoPickupBar::DrawStatBarOnLeft calls list<NPlayerGui::CAutoPickupBar::CAutoPickup>::pop_front
//   (player_gui_draw_modules.obj) on Pickups (Ego_r.pdb +0x78, the class's only list). The byte-identical 0x006553D0 is CMultiStatBar's
//   list. Both PDBs are Dinkumware builds, so the spelling is the one the repo STLport emits (descriptive-by-library).
// NF396: was the free stand-in std_list_node_unlink_and_free(LinkedListNodeOverlay**, LinkedListNodeOverlay*) (NoDuplicates, shared with
//   0x006553D0 and 0x00655125). Model: the shared stlport/fable_containers.h list; no shared header defines CAutoPickupBar, so the
//   record is the Ego_r.pdb nested class (12 bytes: DefIndex, Count, TimeRemaining), as the landed _M_create_node 0x006554BD models it.
//   The explicit instantiation emits the member as its own pick-any COMDAT.
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_LIST
#include "stlport/fable_containers.h"
namespace NPlayerGui {
class CAutoPickupBar {
public:
    class CAutoPickup {                                   // Ego_r.pdb, 12 bytes
    public:
        long DefIndex;                                    // +0x0
        long Count;                                       // +0x4
        float TimeRemaining;                              // +0x8
    };
};
}

template std::list<NPlayerGui::CAutoPickupBar::CAutoPickup>::iterator std::list<NPlayerGui::CAutoPickupBar::CAutoPickup>::erase(std::list<NPlayerGui::CAutoPickupBar::CAutoPickup>::iterator);
