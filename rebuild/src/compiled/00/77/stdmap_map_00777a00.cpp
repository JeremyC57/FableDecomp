// std::map<long, CShopItem >::map() -- STLport 4.6 container default constructor: the member _Rb_tree built with _Compare() and allocator_type() (header node allocated, red, self-linked)
// Leaf: ??0?$map@JVCShopItem@@U?$less@J@std@@V?$allocator@U?$pair@$$CBJVCShopItem@@@std@@@3@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00777A00, 54 bytes; node 0x30.
// Identity (proven, agentFAM07): CTCShop::Stock (Ego_r.pdb map<long, CShopItem>): the landed ~CTCShop 0x007783F0 calls this tree's _M_erase 0x00776AA0 and binds it
//   as _Rb_tree<long, pair<const long, CShopItem>, ...>; ??0 / ??1 of map<long, CShopItem> are in the PDB name sets; the ctor is the
//   COMDAT right before the map's destructor 0x00777A40; node 0x30 = 0x10 + 4 + CShopItem 0x1C.
//   Value layout: Ego_r.pdb member names and types at the retail sizes (STLport containers: vector 12, map 12), only its size matters here.
//   Was the placeholder CUnprovenContainer_00776aa0::CUnprovenContainer_00776aa0 (name-only change: the same STLport text, the per-instantiation placeholder key/value
//   types replaced by the proven ones).
// Model (agentNF24 port of agentFAM07's draft): the shared stlport/fable_map.h (STLport 4.6 _Rb_tree; FABLE_RETAIL_STLP_MAP) and
//   engine/CShopItem.h (28 bytes) replace the draft's copied STLport text and local class model; the explicit instantiation emits the member as its own
//   COMDAT, as retail kept every used inline member (no /OPT:REF).
#define FABLE_RETAIL_STLP_MAP
#include "stlport/fable_map.h"
#include "engine/CShopItem.h"

template std::map<long, CShopItem>::map();
