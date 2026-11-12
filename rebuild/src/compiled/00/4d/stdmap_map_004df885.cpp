// std::map<long, long>::~map() -- STLport 4.6 container destructor (implicit): a tail jump to the member _Rb_tree's destructor
// (0x004DD2B1, kept out of line in this size TU and folded with the other POD-pair tree destructors).
// Leaf: ??1?$map@JJU?$less@J@std@@V?$allocator@U?$pair@$$CBJJ@std@@@2@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DF885, 5 bytes: `jmp 0x004DD2B1` (was the forwarder stand-in _Forward / ForwardTarget_004dd2b1,
//   catalog _global::TailJmp_OnReadFinished_004df885).
// Name (agentNF32, 2026-11-12): named by its callers and its target, not by a label. Callers: the landed CGameScriptInterface scalar
//   deleting destructor 0x006E7D50 destroys ScriptTimers (Ego_r struct_layouts CGameScriptInterface +60 map<long,long,less<long>,
//   allocator<pair<long const,long> > >) through this address and binds this symbol; the landed CTCWife::FindAppropriateGift 0x006CF530
//   destroys its local map<long, long> through it. Target: 0x004DD2B1 is the tree destructor body bound by the landed map destructor
//   stand-ins ~CLongLongMap / ~CLongMap / ~CContainer_004dd2b1. FableWin.pdb carries the same mangled ~map<long, long> (0x01BA9926; the
//   class template arguments mangle alike in both libraries). Descriptive-by-library for the STLport body.
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_MAP
#include "stlport/fable_map.h"

void DestroyMapOfLongLong(std::map<long, long>* container) { container->~map(); }
