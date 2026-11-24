// Leaf: ?GetNodeType@CNavSwitchableLeafNode@@UBE?AW4ENavQuadTreeNodeType@@XZ
#define FABLE_RETAIL_CNAVSWITCHABLELEAFNODE
#define FABLE_RETAIL_CNAVSWITCHABLELEAFNODE_RTTI_DECLS
#include "engine/CNavSwitchableLeafNode.h"
ENavQuadTreeNodeType CNavSwitchableLeafNode::GetNodeType() const
{
    return (ENavQuadTreeNodeType)3;
}
// Flatten route (flatten_pilot v4, NF324): CNavSwitchableLeafNode_GetNodeType -> CNavSwitchableLeafNode::GetNodeType, the RTTI-placed PDB member; class model: engine/CNavSwitchableLeafNode.h opt-in FABLE_RETAIL_CNAVSWITCHABLELEAFNODE, FABLE_RETAIL_CNAVSWITCHABLELEAFNODE_RTTI_DECLS.
