// CNavNavigableLeafNode::RemoveRegionZero() -- true when the node's region ID is 0
// Leaf: ?RemoveRegionZero@CNavNavigableLeafNode@@MAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A75B50, 11 bytes: virtual slot 5 (`call [eax+14h]`), then `neg ; sbb ; inc`.
// Body port (NF321, 2026-11-23): defined on the real class of engine/CNavNavigableLeafNode.h (FABLE_RETAIL_CNAVNAVIGABLELEAFNODE; was
//   ?RemoveRegionZero@CNavNavigableLeafNode_Methods@@QAE_NXZ over a made-up slot run v0..v4 / RemoveRegion); the override is declared by
//   the add-only opt-in FABLE_RETAIL_CNAVNAVIGABLELEAFNODE_REMOVEREGIONZERO_DECL (both PDB name sets; retail vtable 0x0129DDA4 slot 14).
//   Slot 5 of that vtable is 0x00A75A70 ?GetRegionID@CNavNavigableLeafNode@@UBEJXZ (labels_rtti_port.tsv; the header's
//   CANavQuadTreeNode slot 5): a virtual call, no symbol binding.
#define FABLE_RETAIL_CNAVNAVIGABLELEAFNODE
#define FABLE_RETAIL_CNAVNAVIGABLELEAFNODE_REMOVEREGIONZERO_DECL
#include "engine/CNavNavigableLeafNode.h"

bool CNavNavigableLeafNode::RemoveRegionZero()
{
    return GetRegionID() == 0;
}
