// std::map<unsigned long, unsigned long>::operator[](const unsigned long& k) -- STLport subscript: lower_bound, insert (k, 0) when absent, return the mapped word
// Leaf: ??A?$map@KKU?$less@K@std@@V?$allocator@U?$pair@$$CBKK@std@@@2@@std@@QAEAAKABK@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00553E60, 115 bytes, thiscall, `ret 4`, returns &(*it).second.
// Identity: the manifest's "std::map::insert" is propagated. The landed callers C3DMesh2::RemapBoneIndices 0x00A8B820
//   and C3DPrimitive2::RemapBoneIndices 0x00A90F00 declare `unsigned long& operator[](const unsigned long&)` @ 0x00553E60
//   (bone-index remap); NUISystem::CMorphingSprite::ProcessChangeState shares the folded body (16 callers).
// Callees: _Rb_tree::insert_unique(iterator, const value_type&) 0x005537F0 (the iterator argument built in its slot,
//   the iterator result returned through the dead `k` argument slot).
// Shape: STLport 4.6 map::operator[] verbatim with `_M_lower_bound` INLINE (the `jb` walk keeps the header in esi,
//   the candidate in edx and the key in ebx); `__default_constructed((_Tp*)0)` gives the late `mov [esp+1Ch],0`
//   store of the pair temporary's second word after the argument pushes -- only when insert_unique has a same-TU
//   out-of-line body that reads the value (declaration-only schedules the store one push earlier).
// and reading the value, so VC7.1 schedules the pair temporary's late second-word store as retail does.
// NF61 (round 7, q10571): moved onto stlport/fable_map.h (FABLE_RETAIL_STLP_MAP operator[] body, PASS22 __default_constructed,
//   FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_HINT_BODY + FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY: the visible out-of-line hinted
//   insert_unique that reads the value, which schedules the pair temporary's late second-word store as retail -- the 0x00A4A670
//   pattern); the local std::map model (3 template arguments, no allocator, its own insert_unique) is gone. The call now binds the
//   tree's insert_unique(iterator, const value_type&) 0x005537F0 (NF58's spelling) and the self leaf is the 4-argument STLport
//   spelling the landed caller 0x00ACA0C0 already binds (no landed caller bound the old 3-argument spelling).
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ
#define FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_HINT_BODY
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "stlport/fable_map.h"

namespace std {
template unsigned long& map<unsigned long, unsigned long>::operator[](const unsigned long&);
}
