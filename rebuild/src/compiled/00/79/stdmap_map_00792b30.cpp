// std::map<CThingCreatureBase*, CTCDivineWrath::PerCreatureInfo >::map() -- STLport 4.6 container default constructor: the member _Rb_tree built with _Compare() and allocator_type() (header node allocated, red, self-linked)
// Leaf: ??0?$map@PAVCThingCreatureBase@@UPerCreatureInfo@CTCDivineWrath@@U?$less@PAVCThingCreatureBase@@@std@@V?$allocator@U?$pair@QAVCThingCreatureBase@@UPerCreatureInfo@CTCDivineWrath@@@std@@@5@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00792B30, 54 bytes; node 0x2c.
// Identity (proven, agentFAM07): the next COMDAT 0x00792B70 is this container's destructor and calls _M_erase 0x00792300, landed as the map<CThingCreatureBase*, CTCDivineWrath::PerCreatureInfo>
//   _M_erase; this constructor's symbol is in the Ego_r / FableWin PDB name sets; node 0x2C. engine/CTCDivineWrath.h: typedef std::map<CThingCreatureBase*, PerCreatureInfo> MapOfCreaturesToPerCreatureInfo.
//   Value layout: Ego_r.pdb member names and types at the retail sizes (only sizeof matters here).
//   Was the placeholder CUnprovenContainer_00792b30::CUnprovenContainer_00792b30 (name-only change: the same STLport text, the per-instantiation placeholder key/value
//   types replaced by the proven ones).
// Model (agentNF24 port of agentFAM07's draft): the shared stlport/fable_map.h (STLport 4.6 _Rb_tree; FABLE_RETAIL_STLP_MAP) and
//   engine/CTCDivineWrath.h (nested PerCreatureInfo) replace the draft's copied STLport text and local class model; the explicit instantiation emits the member as its own
//   COMDAT, as retail kept every used inline member (no /OPT:REF).
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "stlport/fable_map.h"
#include "engine/CTCDivineWrath.h"

template std::map<CThingCreatureBase*, CTCDivineWrath::PerCreatureInfo>::map();
