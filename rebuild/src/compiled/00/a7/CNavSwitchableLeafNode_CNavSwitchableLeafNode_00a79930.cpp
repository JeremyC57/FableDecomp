// CNavSwitchableLeafNode::CNavSwitchableLeafNode(const C2DVector& position, unsigned char level, unsigned char layer, unsigned char preferability, const vector<unsigned __int64>& uniqueIDs) -- build a switchable leaf node from the generator: the navigable leaf part (inline), blocked by default, every switch unique id added to the open-id map as closed, no region, the map sorted
// Leaf: ??0CNavSwitchableLeafNode@@QAE@ABVC2DVector@@EEEABV?$vector@_KV?$allocator@_K@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A79930, 255 bytes (+ int3 padding), `ret 14h`.
// NF76 port (2026-11-17): on the shared headers with the sibling file ctor 0x00A79A30's macro block (CANAVQUADTREENODE /
//   CNAVNAVIGABLELEAFNODE / CNAVSWITCHABLELEAFNODE retail branches, STLP_VECTOR + OVERFLOW_OUTOFLINE, STLP_SORT_FASTCALL, the CVectorMap
//   CTOR / INSERT opt-ins) plus my overlay GENERATOR_CTOR opt-ins: CANavQuadTreeNode : CBaseClassNonCopyable with the inline Ego_r
//   (C2DVector, E, E) ctor, the inline CNavNavigableLeafNode (C2DVector, E, E, E) ctor and CSmallVector default ctor, and the
//   CNavSwitchableLeafNode generator ctor declaration. Every local model deleted (C2DVector, CBaseClassNonCopyable, CANavQuadTreeNode,
//   CSmallVector, CNavNavigableLeafNode, SUniqueIDOpen, CKeyPairCompareLess, the fake same-TU SortUniqueIDs insertion sort and
//   _M_insert_overflow bodies, CUniqueIDVector / CUniqueIDMap / CRetailVector, CNavSwitchableLeafNode). The sort 0x00A84160 now binds
//   the STLport spelling ??$sort@PAU?$pair@_K_N@std@@V?$CKeyPairCompareLess@_K_N@@@std@@YIXPAU?$pair@_K_N@0@0V?$CKeyPairCompareLess@_K_N@@@Z
//   (queue q10878). Parameter and local names Ego_d.pdb (pos, quad_tree_level, nav_layer, preferability_cost, unique_ids, c1).
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CANAVQUADTREENODE
#define FABLE_RETAIL_CANAVQUADTREENODE_GENERATOR_CTOR
#define FABLE_RETAIL_CNAVNAVIGABLELEAFNODE
#define FABLE_RETAIL_CNAVNAVIGABLELEAFNODE_GENERATOR_CTOR
#define FABLE_RETAIL_CNAVSWITCHABLELEAFNODE
#define FABLE_RETAIL_CNAVSWITCHABLELEAFNODE_GENERATOR_CTOR
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#define FABLE_RETAIL_CVECTORMAP_CTOR
#define FABLE_RETAIL_CVECTORMAP_INSERT
#define FABLE_RETAIL_CVECTORMAP_INSERT_KEYVALUE
#define FABLE_RETAIL_CVECTORMAP_INSERT_VALUE_TYPE
#include "engine/CAFile.h"
#include "engine/CNavSwitchableLeafNode.h"

CNavSwitchableLeafNode::CNavSwitchableLeafNode(const C2DVector& pos, unsigned char quad_tree_level, unsigned char nav_layer,
                                               unsigned char preferability_cost, const std::vector<unsigned __int64>& unique_ids)
    : CNavNavigableLeafNode(pos, quad_tree_level, nav_layer, preferability_cost)
{
    Blocked = true;
    for (long c1 = 0; c1 < unique_ids.size(); ++c1)
        UniqueIDsOpen.Insert(unique_ids[c1], false);
    PRegion = 0;
    UniqueIDsOpen.Update();
}
