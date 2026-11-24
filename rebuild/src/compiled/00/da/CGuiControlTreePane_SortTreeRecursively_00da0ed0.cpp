// std::list<CScriptThing>::size() const -- STLport distance(begin(), end()): walk the ring from the sentinel's next node back to the sentinel, counting the nodes
// Leaf: ?size@?$list@VCScriptThing@@V?$allocator@VCScriptThing@@@std@@@std@@QBEIXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DA0ED0, 24 bytes, thiscall, plain `ret`; no callees.
// Identity (NF394, T3 nf391 q10): the only retail reference is `call 0x00DA0ED0` at 0x00DA0E5E inside NScript::CQ_MinionCampScript::
//   GetRandomSpawnPoint 0x00DA0E40 (landed; it binds the stand-in ?size@CRetailList_CScriptThing@@QBEIXZ here, queued). FableWin's
//   ?GetRandomSpawnPoint@CQ_MinionCampScript@NScript@@QAEXABV?$list@VCScriptThing@@V?$allocator@VCScriptThing@@@std@@@std@@AAVCScriptThing@@@Z
//   (register_scripts.obj) takes the points as list<CScriptThing> and calls exactly one list size on it,
//   ?size@?$list@VCScriptThing@@V?$allocator@VCScriptThing@@@std@@@std@@QBEIXZ (0x037EACC9).
// NF394: was the made-up `SortTreeRecursively(Tree*)` stand-in (catalog label "CGuiControlTreePane::SortTreeRecursively", propagated), shared
//   with 0x004B87D0 / 0x006CD240. Model: the 0x005143E0 form (NF391; 0x00ADB570, NF39); element type from the shared engine/CScriptThing.h
//   (FABLE_RETAIL_NSCRIPT, with engine/CCountedPointer.h first, as its contract and the caller 0x00DA0E40 have it).
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE
#define FABLE_RETAIL_NSCRIPT
class CCPPointerInfo;
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "stlport/fable_map.h"

template unsigned int std::list<CScriptThing, std::allocator<CScriptThing> >::size() const;
