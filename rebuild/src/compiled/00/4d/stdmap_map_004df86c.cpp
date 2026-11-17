// std::map<long, long, std::less<long>, std::allocator<std::pair<long const, long> > >::map() -- STLport 4.6 _map.h no-argument
//   constructor: `_M_t(Compare(), allocator_type())`, the two empty temporaries in one reused frame byte, then the tree constructor
//   _Rb_tree(const Compare&, const allocator_type&) 0x004DD287 out of line; returns `this`
// Leaf: ??0?$map@JJU?$less@J@std@@V?$allocator@U?$pair@$$CBJJ@std@@@2@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DF86C, 25 bytes, size TU.
// NF64 (round 7, q10671): an explicit instantiation of stlport/fable_map.h's map() (FABLE_RETAIL_STLP_MAP) with the tree constructor
//   kept out of line (FABLE_RETAIL_STLP_TREE_CTOR_DTOR_OUTOFLINE, as retail calls 0x004DD287). The landed callers 0x004E0860 and NF61's
//   0x005907AB bind this spelling. Was the stand-in `std::map_T_004df86c::map` (leaf ?map@map_T_004df86c@std@@). The key / value
//   types come from those callers (map<long,long>). Descriptive-by-library (retail is STLport 4.6; both PDBs are Dinkumware builds).
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_CTOR_DTOR_OUTOFLINE
#include "stlport/fable_map.h"

template std::map<long, long, std::less<long>, std::allocator<std::pair<long const, long> > >::map();
