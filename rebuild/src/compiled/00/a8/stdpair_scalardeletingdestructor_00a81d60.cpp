// std::pair::`scalar deleting destructor'(unsigned int flags) -- (std::pair<unsigned __int64, std::list<C2DLineF> >) the compiler-generated deleting destructor: the implicit ~pair runs second's destructor (std::list<C2DLineF> at +0x8), then operator delete when bit 0 of flags is set; returns this
// Leaf: ??_G?$pair@_KV?$list@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@std@@@std@@QAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A81D60, 33 bytes (+ int3 padding), thiscall, `ret 4`; unreferenced (no call or pointer to it in Fable.exe).
// Identity: std::pair<unsigned __int64, std::list<C2DLineF> >::`scalar deleting destructor' -- probable; evidence: the only destructible member is the list<C2DLineF> at +8 (destructor 0x005127A0 = ~_List_base<C2DLineF>: list's `~list() {}` inline); FableWin.pdb's ??_G name set holds ??_G?$pair@_KV?$list@VC2DLineF@@...@std@@@std@@QAEPAXI@Z, the element of Ego_r.pdb CNavigationLayer::SwitchableLineLists (CVectorMap<unsigned __int64, list<C2DLineF> >), and the body sits in the navigation-layer module (CNavQuadTree / CNavigationLayer 0x00A77480..0x00A85700); CNavigationAreaUpdate::CSwitchableBlockage has the same layout but its code lives at 0x0051xxxx.
//   Family G042 (8 deleting destructors that differ in the one member's offset and destructor; the propagated label
//   "CLandscapeBackgroundPatch::`vector deleting destructor'" is wrong for all eight).
// Callees: std::_List_base<C2DLineF>::~_List_base() 0x005127A0, operator delete 0x00BFE9BC.
// Shape: `delete p` on the class emits the non-virtual ??_G as its own COMDAT (the helper below is not graded).
// NF246 (2026-11-22, the 0x005127A0 family, nf243 NOTES section 2): the row-local C2DVector / C2DLineF / std::pair / allocator / list
//   stand-ins replaced by the shared headers (engine/C2DLineF.h, stlport/fable_containers.h with FABLE_RETAIL_STLP_LIST_BASE_OUTOFLINE:
//   the _List_base destructor declared only, as this TU calls it): the list destructor call binds
//   ??1?$_List_base@VC2DLineF@@..@std@@QAE@XZ (0x005127A0; list's own ~list is the 5-byte jmp 0x00514280).
#define FABLE_RETAIL_STLP_LIST_BASE_OUTOFLINE
#include "engine/C2DLineF.h"
#include "stlport/fable_containers.h"
void __cdecl operator delete(void* p);                         // 0x00BFE9BC
void DeleteOne(std::pair<unsigned __int64, std::list<C2DLineF> >* p) { delete p; }
