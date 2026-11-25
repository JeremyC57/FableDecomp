// std::list<NPlayerGui::CMultiStatBar::CStatUpdate>::erase(iterator) -- STLport 4.6 _list.h erase: unlink the node from its neighbours, destroy the (trivial) record, free the node, return the next node
// Leaf: ?erase@?$list@VCStatUpdate@CMultiStatBar@NPlayerGui@@V?$allocator@VCStatUpdate@CMultiStatBar@NPlayerGui@@@std@@@std@@QAE?AU?$_List_iterator@VCStatUpdate@CMultiStatBar@NPlayerGui@@U?$_Nonconst_traits@VCStatUpdate@CMultiStatBar@NPlayerGui@@@std@@@2@U32@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006553D0, 32 bytes, thiscall, iterator by value, result through the hidden pointer, `ret 8`; size TU
//   (`pop ecx` after free). Callee: free 0x00BFEA14 (MSVCR71).
// Identity (NF391 trace, NF396 re-check): its only reference is the call at 0x00655A95 in 0x00655A86, the list's pop_front
//   (`erase(begin())`: the head's next node passed by value, the result dropped); 0x00655A86's only caller is 0x00646D7E in
//   NPlayerGui::CMultiStatBar::DrawStatBarOnLeft 0x00646C44 (RTTI: vftable 0x0125A758 slot 0, COL type .?AVCMultiStatBar@NPlayerGui@@).
//   FableWin's CMultiStatBar::DrawStatBarOnLeft calls list<NPlayerGui::CMultiStatBar::CStatUpdate>::pop_front (player_gui_draw_modules.obj),
//   on Updates, the class's only list. The byte-identical 0x006553FD is CAutoPickupBar's list (its own pop_front / DrawStatBarOnLeft
//   chain). Both PDBs are Dinkumware builds, so the spelling is the one the repo STLport emits (descriptive-by-library).
// NF396: was the free stand-in std_list_node_unlink_and_free(LinkedListNodeOverlay**, LinkedListNodeOverlay*) (NoDuplicates, shared with
//   0x006553FD and 0x00655125). Model: the shared stlport/fable_containers.h list over engine/NPlayerGui_CMultiStatBar.h (CStatUpdate
//   nested; the header's opt-in and the few defines its include chain needs); the explicit instantiation emits the member as its own pick-any COMDAT.
#pragma optimize("s", on)
#define FABLE_RETAIL_NPLAYERGUI_CMULTISTATBAR_PASS23
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_GUIMODE
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS
#define FABLE_RETAIL_STLP_LIST
#include "fable_string.h"
#include "engine/CEnginePrimitive2DSprite.h"
#include "engine/NPlayerGui_CMultiStatBar.h"

template std::list<NPlayerGui::CMultiStatBar::CStatUpdate>::iterator std::list<NPlayerGui::CMultiStatBar::CStatUpdate>::erase(std::list<NPlayerGui::CMultiStatBar::CStatUpdate>::iterator);
