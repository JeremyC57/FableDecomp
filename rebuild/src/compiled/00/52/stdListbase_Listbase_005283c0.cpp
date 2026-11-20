// std::_List_base<CThingPhysical::CHealthOverTimeHelper>::~_List_base() -- free every node of the health-over-time list, reset the
//   sentinel, free it (STLport 4.6 _list.h: `clear(); _M_node.deallocate(_M_node._M_data, 1);`)
// Leaf: ??1?$_List_base@VCHealthOverTimeHelper@CThingPhysical@@V?$allocator@VCHealthOverTimeHelper@CThingPhysical@@@std@@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005283C0, thiscall, `ret`; callee free 0x00BFEA14 (node and sentinel; the element is trivial).
// Identity (two sources): ~CThingPhysical 0x006A5D50 calls it on HealthOverTimeEffects (+0x98), whose Ego_r.pdb type is
//   list<CThingPhysical::CHealthOverTimeHelper> (struct_layouts_egor.tsv; FableWin.pdb names the same list's ctor / dtor); the landed
//   0x006A5D50 already binds this spelling; the only other reference is the ~list forwarder 0x00528C40 (`jmp 0x005283C0`) beside that
//   list's constructor 0x00528C20 (an 8-byte element: node 0x10), so no other element type reaches it (not a fold). The body is STLport
//   4.6's ~_List_base with clear() expanded. Descriptive-by-library: STLport has no PDB copy; the instantiation is the one the caller proves.
// Was: the free stand-in SortTreeRecursively_00436900 over local TreeNode / TreeList models
//   (rebuild/src/compiled/00/52/CGuiControlTreePane_SortTreeRecursively_005283c0.cpp).
// Form: an explicit instantiation of the destructor over the shared STLport list; the element type from the engine/CThingPhysical.h
//   overlay opt-in FABLE_RETAIL_CTHINGPHYSICAL_HEALTHOVERTIMEHELPER (Ego_r.pdb nested class, sizeof 8).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGPHYSICAL_HEALTHOVERTIMEHELPER
#define FABLE_RETAIL_STLP_LIST
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CThingPhysical.h"
template std::_List_base<CThingPhysical::CHealthOverTimeHelper, std::allocator<CThingPhysical::CHealthOverTimeHelper> >::~_List_base();
