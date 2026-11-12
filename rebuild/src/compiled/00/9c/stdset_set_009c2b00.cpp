// std::set<CDisplayModeDesc >::set() -- STLport 4.6 container default constructor: the member _Rb_tree built with _Compare() and allocator_type() (header node allocated, red, self-linked)
// Leaf: ??0?$set@VCDisplayModeDesc@@U?$less@VCDisplayModeDesc@@@std@@V?$allocator@VCDisplayModeDesc@@@3@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009C2B00, 54 bytes; node 0x20.
// Identity (proven, agentFAM07): CDisplayManager::EnumerateDisplayModes 0x009C02D0 builds a local set of display modes and calls this tree's _M_erase 0x009C1DC0,
//   landed and bound as _Rb_tree<CDisplayModeDesc, CDisplayModeDesc, _Identity<..>, ...>; ??0 / ??1 of set<CDisplayModeDesc> are in the
//   PDB name sets; the ctor is the COMDAT right before the set's destructor 0x009C2B40; node 0x20 = 0x10 + CDisplayModeDesc 0x10.
//   Value layout: Ego_r.pdb member names and types at the retail sizes (STLport containers: vector 12, map 12), only its size matters here.
//   Was the placeholder CUnprovenContainer_009c1dc0::CUnprovenContainer_009c1dc0 (name-only change: the same STLport text, the per-instantiation placeholder key/value
//   types replaced by the proven ones).
// Model (agentNF24 port of agentFAM07's draft): the shared stlport/fable_map.h (STLport 4.6 _Rb_tree; FABLE_RETAIL_STLP_MAP) and
//   engine/CDisplayModeDesc.h + engine/C2DExtentsI.h (0x10 bytes) replace the draft's copied STLport text and local class model; the explicit instantiation emits the member as its own
//   COMDAT, as retail kept every used inline member (no /OPT:REF).
#define FABLE_RETAIL_STLP_MAP
#include "stlport/fable_map.h"
#include "engine/CDisplayModeDesc.h"

template std::set<CDisplayModeDesc>::set();
