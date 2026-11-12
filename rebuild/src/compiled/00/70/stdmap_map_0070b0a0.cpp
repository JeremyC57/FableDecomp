// std::map<long, CEngineTextureDecal >::map() -- STLport 4.6 container default constructor: the member _Rb_tree built with _Compare() and allocator_type() (header node allocated, red, self-linked)
// Leaf: ??0?$map@JVCEngineTextureDecal@@U?$less@J@std@@V?$allocator@U?$pair@$$CBJVCEngineTextureDecal@@@std@@@3@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0070B0A0, 54 bytes; node 0x28.
// Identity (proven, agentFAM07): the landed CTCTextureDecal::PeekEngineDecals 0x0070A570 calls this tree's _M_erase 0x0070ADB0 on a local map of decals and binds it
//   with long / CEngineTextureDecal (spelled there as ?_M_erase@?$map@..); ??0 / ??1 of map<long, CEngineTextureDecal> are in the PDB
//   name sets; the ctor is the COMDAT right before the map's destructor 0x0070B0E0; node 0x28 = 0x10 + 4 + retail CEngineTextureDecal
//   0x14 (Ego_r 24 less 4 for its vector).
//   Value layout: Ego_r.pdb member names and types at the retail sizes (STLport containers: vector 12, map 12), only its size matters here.
//   Was the placeholder CUnprovenContainer_0070adb0::CUnprovenContainer_0070adb0 (name-only change: the same STLport text, the per-instantiation placeholder key/value
//   types replaced by the proven ones).
// Model (agentNF24 port of agentFAM07's draft): the shared stlport/fable_map.h (STLport 4.6 _Rb_tree; FABLE_RETAIL_STLP_MAP) and
//   engine/CEngineTextureDecal.h (20 bytes) replace the draft's copied STLport text and local class model; the explicit instantiation emits the member as its own
//   COMDAT, as retail kept every used inline member (no /OPT:REF).
#define FABLE_RETAIL_STLP_MAP
#include "stlport/fable_map.h"
#include "engine/CEngineTextureDecal.h"

template std::map<long, CEngineTextureDecal>::map();
