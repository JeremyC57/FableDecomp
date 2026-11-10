// std::vector<CTVertexSeaBackground>::_M_fill_insert(CTVertexSeaBackground* position, unsigned int n, const CTVertexSeaBackground& value) -- STLport 4.6: insert n copies of value at position, preserving the old tail
// Leaf: ?_M_fill_insert@?$vector@VCTVertexSeaBackground@@V?$allocator@VCTVertexSeaBackground@@@std@@@std@@QAEXPAVCTVertexSeaBackground@@IABV3@@Z
// Flags: /d2inlT185
// Inline-budget: /d2inlT185 -- expand real STLport copy/construct/fill helpers; baseline001/002 measured the call/inline split.
// Retail: 0x00b801f0, 523 bytes, thiscall.
#define FABLE_RETAIL_STLP_FILL_LOCAL_ITERATOR
#define FABLE_RETAIL_CTVERTEXSEABACKGROUND_HIERARCHY
#define FABLE_RETAIL_STLP_RANDOM_ACCESS_COPY_BACKWARD
#define FABLE_RETAIL_STLP_FALSE_TAG_POD
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY
#define FABLE_RETAIL_STLP_VECTOR
#include "engine/CTVertexSeaBackground.h"
#include "stlport/fable_containers.h"
template void std::vector<CTVertexSeaBackground>::_M_fill_insert(CTVertexSeaBackground*,unsigned int,const CTVertexSeaBackground&);