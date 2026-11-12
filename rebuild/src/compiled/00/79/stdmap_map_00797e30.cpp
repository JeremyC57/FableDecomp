// std::map<CThingCreatureBase*, CTCUnholyPower::PerCreatureInfo >::map() -- STLport 4.6 container default constructor: the member _Rb_tree built with _Compare() and allocator_type() (header node allocated, red, self-linked)
// Leaf: ??0?$map@PAVCThingCreatureBase@@UPerCreatureInfo@CTCUnholyPower@@U?$less@PAVCThingCreatureBase@@@std@@V?$allocator@U?$pair@QAVCThingCreatureBase@@UPerCreatureInfo@CTCUnholyPower@@@std@@@5@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00797E30, 54 bytes; node 0x2c.
// Identity (proven, agentFAM07): the next COMDAT 0x00797E70 is this container's destructor and calls _M_erase 0x00797840, landed as the map<CThingCreatureBase*, CTCUnholyPower::PerCreatureInfo>
//   _M_erase; this constructor's symbol is in the Ego_r / FableWin PDB name sets; node 0x2C. engine/CTCUnholyPower.h carries the same per-creature map.
//   Value layout: Ego_r.pdb member names and types at the retail sizes (only sizeof matters here).
//   Was the placeholder CUnprovenContainer_00797e30::CUnprovenContainer_00797e30 (name-only change: the same STLport text, the per-instantiation placeholder key/value
//   types replaced by the proven ones).
// Model (agentNF24 port of agentFAM07's draft): the shared stlport/fable_map.h (STLport 4.6 _Rb_tree; FABLE_RETAIL_STLP_MAP) and
//   engine/CTCUnholyPower.h (nested PerCreatureInfo; the define set of the landed 0x00794E60, whose inline StopSound needs CThing / CTCSoundPlayer) replace the draft's copied STLport text and local class model; the explicit instantiation emits the member as its own
//   COMDAT, as retail kept every used inline member (no /OPT:REF).
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_ADDANDCOPYTC
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_CTCUNHOLYPOWER_CANCELFORCUTSCENE
#include "stlport/fable_map.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCUnholyPower.h"

template std::map<CThingCreatureBase*, CTCUnholyPower::PerCreatureInfo>::map();
