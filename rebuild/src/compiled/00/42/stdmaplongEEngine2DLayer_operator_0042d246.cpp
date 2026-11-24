// std::map<long, EEngine2DLayer>::operator[](const long& k) -- STLport 4.6 subscript of NUISystem::CManager's Layers +0x70: lower_bound, insert (k, EEngine2DLayer()) when absent, return the value
// Leaf: ??A?$map@JW4EEngine2DLayer@@U?$less@J@std@@V?$allocator@U?$pair@$$CBJW4EEngine2DLayer@@@std@@@3@@std@@QAEAAW4EEngine2DLayer@@ABJ@Z
// Retail: Fable.exe @ 0x0042D246, 69 bytes, `ret 4`, frame pointer (`push ebp / mov ebp,esp`, `leave`), unaligned entry; callers
//   SetMetaLayer 0x0041E1CD (18 calls) and GetLayer 0x0041E3B2 (`Layers[0]`). The catalog label map<EUsedKeys,long>::operator[] is the
//   byte-twin instance's name: this one calls the map<long,EEngine2DLayer> tree's _M_lower_bound 0x0042AE33 and map::insert 0x0042CE46.
// Body: i = lower_bound(k) (the tree's _M_lower_bound out of line, node pointer in eax); if (i == end() || k < (*i).first) (signed
//   `jge`) i = insert(i, value_type(k, EEngine2DLayer())) (map::insert(iterator, const value_type&) 0x0042CE46, out of line; the
//   value_type temporary in the frame, the iterator argument built in its slot, the returned iterator in the dead parameter slot);
//   return (*i).second (node +0x14).
// Name (NF228, 2026-11-22, q12591 / q12601): the STLport spelling the header produces (STL instances have no PDB names: EXTRA_RULES
//   2026-11-20 "descriptive-by-library"); the landed binders GetLayer 0x0041E3B2 and SetMetaLayer 0x0041E1CD (this hand-back) bind it.
//   Was a row-local 2-argument STLport map model whose leaf was ??A?$map@JW4EEngine2DLayer@@U?$less@J@std@@@std@@QAEAAW4EEngine2DLayer@@ABJ@Z.
// Model: engine/NUISystem_CManager.h (EEngine2DLayer, the member's map type) and stlport/fable_map.h (FABLE_RETAIL_STLP_MAP operator[]
//   body; FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE: lower_bound over the out-of-line _M_lower_bound); the explicit instantiation emits
//   the member as its own COMDAT (the 0x005692B0 pattern). Its map::insert pick-any copy equals 0x0042CE46 (pk.py).
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE
#pragma optimize("s", on)
#include "engine/NUISystem_CManager.h"
template EEngine2DLayer& std::map<long, EEngine2DLayer >::operator[](const long&);
