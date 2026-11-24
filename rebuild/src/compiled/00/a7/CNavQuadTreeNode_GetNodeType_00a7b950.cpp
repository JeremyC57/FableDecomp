// Leaf: ?GetNodeType@CNavQuadTreeNode@@UBE?AW4ENavQuadTreeNodeType@@XZ
#define FABLE_RETAIL_CNAVQUADTREENODE
#define FABLE_RETAIL_CNAVQUADTREENODE_RTTI_DECLS
#include "engine/CNavQuadTreeNode.h"
ENavQuadTreeNodeType CNavQuadTreeNode::GetNodeType() const
{
    return (ENavQuadTreeNodeType)0;
}
// Flatten route (flatten_pilot v4, NF324): CNavQuadTreeNode_GetNodeType -> CNavQuadTreeNode::GetNodeType, the RTTI-placed PDB member; class model: engine/CNavQuadTreeNode.h opt-in FABLE_RETAIL_CNAVQUADTREENODE, FABLE_RETAIL_CNAVQUADTREENODE_RTTI_DECLS.
