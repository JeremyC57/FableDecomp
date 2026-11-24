// std::list<CNavWayPoint>::size() const -- STLport distance(begin(), end()): walk the ring from the sentinel's next node back to the sentinel, counting the nodes
// Leaf: ?size@?$list@VCNavWayPoint@@V?$allocator@VCNavWayPoint@@@std@@@std@@QBEIXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006CD240, 24 bytes, thiscall, plain `ret`; no callees.
// Identity (NF394, T3 nf391 q10): retail calls it 6 times, all from CNavigatorAStar: 0x00A1E2FD in MoveTowardNextWayPoint 0x00A1E110 and
//   0x00A1EEEA / 0x00A1F2B4 / 0x00A1F43A / 0x00A1F488 / 0x00A1FA72 in CalculateAndMoveToPosProximity 0x00A1E5C0 (both landed; the table
//   already binds this spelling to 0x006CD240, witness 0x00A1E5C0). FableWin's MoveTowardNextWayPoint and CalculateAndMoveToPosProximity
//   (lib_navigator_a_star.obj) call exactly one list size, ?size@?$list@VCNavWayPoint@@V?$allocator@VCNavWayPoint@@@std@@@std@@QBEIXZ
//   (1 and 9 sites); the route is list<CNavWayPoint> (FableWin AddExtraSmoothingPoints' _List_iterator<_List_val<CNavWayPoint>> argument).
// NF394: was the made-up `SortTreeRecursively(Tree*)` stand-in (catalog label "CGuiControlTreePane::SortTreeRecursively", propagated), shared
//   with 0x004B87D0 / 0x00DA0ED0. Model: the 0x005143E0 form (NF391; 0x00ADB570, NF39); element type from the shared engine/CNavWayPoint.h
//   (FABLE_RETAIL_C2DVECTOR: the byte-packed C2DVector that gives the 10-byte record, as in the callers 0x00A1E110 / 0x00A1E5C0).
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE
#include "rebuild_abi.h"
#include "engine/CNavWayPoint.h"
#include "stlport/fable_map.h"

template unsigned int std::list<CNavWayPoint, std::allocator<CNavWayPoint> >::size() const;
