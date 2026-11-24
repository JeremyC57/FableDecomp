// std::list<CVertexGroupBlock>::push_back(const CVertexGroupBlock& x) -- append a copy of a block to the optimiser's block list
// Leaf: ?push_back@?$list@VCVertexGroupBlock@@V?$allocator@VCVertexGroupBlock@@@std@@@std@@QAEXABVCVertexGroupBlock@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ADCE10, 0x45 bytes (+ int3 padding), thiscall, `ret 4`.
// Identity: 0x20-byte list node {next, prev, CVertexGroupBlock}; the element copy is CVertexGroupBlock's implicit copy
// constructor -- vector<unsigned long>(const vector&) @ 0x00535470 then the tree copy 0x00A96E40 on +0xC: Ego_r.pdb
// list<CVertexGroupBlock>::push_back (CVertexGroupBlockOptimiser::Blocks). Catalog "List_EmplaceBack_Pair" is
// descriptive.
// Callees: malloc @ 0x00BFEA0E (allocate(1)), vector<unsigned long>::vector(const vector&) @ 0x00535470,
// map<unsigned long, long>::map(const map&) @ 0x00A96E40.
// Shape: STLport 4.x `push_back(x) { insert(end(), x); }` with insert and _M_create_node inline (placement-new null test
// around the element copy, then tmp->next = n, tmp->prev = p, p->next = tmp, n->prev = tmp). Unlike the range insert
// 0x00ADCC80, this TU calls the vector copy constructor out of line.
// NF258 (2026-11-22, L37): ported off the row-local std::vector / map / list / CVertexGroupBlock models onto the shared headers
//   (stlport/fable_containers.h list, engine/CArray.h, stlport/fable_map.h, engine/CVertexGroupBlock.h with the 0x00AD9770 member
//   opt-in). The vector copy constructor is declaration-only (FABLE_RETAIL_STLP_VECTOR_COPY_DECL_ONLY) and the map copy constructor
//   out of line (FABLE_RETAIL_STLP_MAP_COPY_OUTOFLINE, as 0x00AD9770), so the element copy binds
//   ??0?$vector@KV?$allocator@K@std@@@std@@QAE@ABV01@@Z (0x00535470) and ??0?$map@KJ...@QAE@ABV01@@Z (0x00A96E40); the leaf is the
//   two-parameter STLport list spelling the table already binds (witness 0x00AD9F10).
#define FABLE_RETAIL_STLP_VECTOR_COPY_DECL_ONLY
#define FABLE_RETAIL_STLP_MAP_COPY_OUTOFLINE
#define FABLE_RETAIL_STLP_TREE_DTOR_DECL_ONLY   // NF178b opt-in: ~_Rb_tree declared only, so the implicit member destructors the
                                                // CVertexGroupBlock copy pulls in emit no unreferenced _M_erase / clear pick-any copies
#define FABLE_RETAIL_CVERTEXGROUPBLOCK_MEMBERS
#include "engine/CArray.h"
#include "stlport/fable_map.h"
#include "engine/CVertexGroupBlock.h"

template void std::list<CVertexGroupBlock>::push_back(const CVertexGroupBlock&);
