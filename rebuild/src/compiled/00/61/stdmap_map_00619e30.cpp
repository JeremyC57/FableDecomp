// std::map<long, CRegion >::map() -- STLport 4.6 container default constructor: the member _Rb_tree built with _Compare() and allocator_type() (header node allocated, red, self-linked)
// Leaf: ??0?$map@JVCRegion@@U?$less@J@std@@V?$allocator@U?$pair@$$CBJVCRegion@@@std@@@3@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00619E30, 54 bytes; node 0x6c.
// Identity (proven, agentFAM07): CTCInventoryMap::MapRegions (Ego_r.pdb map<long, CRegion>): the landed ~CTCInventoryMap 0x00606F80 calls this tree's _M_erase
//   0x006196D0 and binds it as _Rb_tree<long, pair<const long, CRegion>, ...>; ??0 / ??1 of map<long, CRegion> are in the PDB name
//   sets; the ctor is the COMDAT right before the map's destructor 0x00619E70; node 0x6C = 0x10 + 4 + retail CRegion 0x58
//   (Ego_r 100 bytes less 4 for each of its two vectors and its map).
//   Value layout: Ego_r.pdb member names and types at the retail sizes (STLport containers: vector 12, map 12), only its size matters here.
//   Was the placeholder CUnprovenContainer_006196d0::CUnprovenContainer_006196d0 (name-only change: the same STLport text, the per-instantiation placeholder key/value
//   types replaced by the proven ones).
// Model (agentNF24 port of agentFAM07's draft): the shared stlport/fable_map.h (STLport 4.6 _Rb_tree; FABLE_RETAIL_STLP_MAP) and
//   engine/CRegion.h (retail CRegion 0x58, C2DVector members) replace the draft's copied STLport text and local class model; the explicit instantiation emits the member as its own
//   COMDAT, as retail kept every used inline member (no /OPT:REF).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CREGION
#include "stlport/fable_map.h"
#include "engine/CRegion.h"

template std::map<long, CRegion>::map();
