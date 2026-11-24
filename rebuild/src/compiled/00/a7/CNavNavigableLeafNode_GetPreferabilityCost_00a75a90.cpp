// Leaf: ?GetPreferabilityCost@CNavNavigableLeafNode@@UBEEXZ
// Header route (NF319, 2026-11-23): defined on the real class of engine/CNavNavigableLeafNode.h (was ?GetPreferabilityCost@CNavNavigableLeafNode_Methods@@QAEEXZ); the retail RTTI vftable
//   placement (ghidra_out/labels_rtti_port.tsv) names this PDB member at this address; declared by the add-only opt-in
//   FABLE_RETAIL_CNAVNAVIGABLELEAFNODE_RTTI_DECLS. Bytes unchanged.
#define FABLE_RETAIL_CNAVNAVIGABLELEAFNODE
#define FABLE_RETAIL_CNAVNAVIGABLELEAFNODE_RTTI_DECLS
#include "engine/CNavNavigableLeafNode.h"  // retyped onto the PDB layout; byte parity re-verified
unsigned char CNavNavigableLeafNode::GetPreferabilityCost() const { return this->PreferabilityCost; }
