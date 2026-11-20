#ifndef FABLE_ENGINE_CNAVCONNECTORNODE_H
#define FABLE_ENGINE_CNAVCONNECTORNODE_H
// CNavConnectorNode (new header, hand3c2, rows 0x00A793D0 / 0x00A79680 CNavQuadTree::ConnectHorizantalMapEdge / ConnectVerticalMapEdge):
// Ego_r.pdb members (struct_layouts_egor.tsv, 8 bytes): Neighbour1 +0, Neighbour2 +4 (CNavNavigableLeafNode*); the rows allocate it
// with `new(8)` (0x00A794BD). Ego_r.pdb ??0CNavConnectorNode@@QAE@PBVCANavQuadTreeNode@@0@Z (retail 0x00A78A90, the rows' call after
// the new; parameter names Ego_d.pdb) and ??1CNavConnectorNode@@QAE@XZ (non-virtual; the counted pointer's DeleteData 0x00A82FA0
// destroys it). Declarations only.
#include "rebuild_abi.h"
class CANavQuadTreeNode;
class CNavNavigableLeafNode;
class CNavConnectorNode {
public:
    CNavNavigableLeafNode* Neighbour1;                                   // +0x00
    CNavNavigableLeafNode* Neighbour2;                                   // +0x04
    CNavConnectorNode(const CANavQuadTreeNode* neighbour1, const CANavQuadTreeNode* neighbour2);   // 0x00A78A90
    ~CNavConnectorNode();
};
FABLE_STATIC_ASSERT(sizeof(CNavConnectorNode) == 8);
#endif
