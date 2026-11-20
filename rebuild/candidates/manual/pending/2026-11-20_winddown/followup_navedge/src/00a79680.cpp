// CNavQuadTree::ConnectVerticalMapEdge(float this_map_x, float adjacent_map_x, const CNavigatorManager& nav_manager) -- walk this map's vertical edge on every navigation layer: where a free node of this map faces a free node of the neighbouring map, record a connector between them under the neighbour's map index; the walk steps by the smaller (both free) / larger (both blocked) node size, the blocked side's size, or half a unit where the neighbour has no node
// Leaf: ?ConnectVerticalMapEdge@CNavQuadTree@@AAEXMMABVCNavigatorManager@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A79680, 678 bytes (the horizontal twin 0x00A793D0 with the axes swapped), `ret 0Ch`.
// Identity: Ego_r.pdb / FableWin.pdb ?ConnectVerticalMapEdge@CNavQuadTree@@AAEXMMABVCNavigatorManager@@@Z (private). Locals and scopes: Ego_d.pdb (debug length 1793): quad_tree_area; { nav_layer { current_y { neighbour_map_node
//   { neighbour_size, my_map_node { neighbour_map_index } } } } }.
// Callees: CNavigatorManager::GetNodeAt 0x00A14F70, CNavQuadTree::GetNodeAt 0x00A770B0, the node's slot 5 GetRegionID (+0x14),
//   operator new 0x00BFEA1A, CNavConnectorNode(const CANavQuadTreeNode*, const CANavQuadTreeNode*) 0x00A78A90, CCountedPointer
//   <CNavConnectorNode>::DeleteData 0x00A82FA0, vector<pair<long, CCountedPointer<CNavConnectorNode> > >::_M_insert_overflow
//   0x00A838F0, operator delete 0x00BFE9BC. Data: the node-size table 0x0129DCAC (CANavQuadTreeNode::GetSize), 0.25f 0x0123078C,
//   0.5f 0x0122F59C.
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_EXTENTS
#define FABLE_RETAIL_CANAVQUADTREENODE
#define FABLE_RETAIL_CNAVIGATORMANAGER
#define FABLE_RETAIL_CNAVIGATORMANAGER_GETNODEAT
#define FABLE_RETAIL_CNAVQUADTREE
#define FABLE_RETAIL_CNAVQUADTREE_CONNECTMAPEDGES
#define FABLE_RETAIL_CVECTORMAP_INSERT_KEYVALUE
#include "stlport/fable_containers.h"
#include "engine/C2DVector.h"
#include "engine/C2DExtentsF.h"
#include "engine/C2DBoxF.h"
#include "engine/CANavQuadTreeNode.h"
#include "engine/CNavigatorManager.h"
#include "engine/CNavQuadTree.h"
#include "engine/CNavConnectorNode.h"

void CNavQuadTree::ConnectVerticalMapEdge(float this_map_x, float adjacent_map_x, const CNavigatorManager& nav_manager)
{
    C2DBoxF quad_tree_area(MapPos, C2DExtentsF(MapWidth, MapHeight));
    for (long nav_layer = 0; nav_layer < NavigationLayers.size(); nav_layer++)
    {
        for (float current_y = quad_tree_area.TLY + 0.25f; current_y < quad_tree_area.BRY; )
        {
            const CANavQuadTreeNode* neighbour_map_node = nav_manager.GetNodeAt(C2DVector(adjacent_map_x, current_y), nav_layer);
            if (neighbour_map_node)
            {
                float neighbour_size = neighbour_map_node->GetSize();
                const CANavQuadTreeNode* my_map_node = GetNodeAt(C2DVector(this_map_x, current_y), nav_layer);
                if (!neighbour_map_node->Blocked && !my_map_node->Blocked)
                {
                    long neighbour_map_index = (neighbour_map_node->GetRegionID() >> 16) & 0xffff;
                    ConnectorNodes.Insert(neighbour_map_index, CCountedPointer<CNavConnectorNode>(new CNavConnectorNode(my_map_node, neighbour_map_node)));
                    current_y += std::min(my_map_node->GetSize(), neighbour_size);
                }
                else if (neighbour_map_node->Blocked && my_map_node->Blocked)
                    current_y += std::max(my_map_node->GetSize(), neighbour_size);
                // Inferred: FITTED -- the one-side-blocked step tested on the neighbour, my node's size first (retail `test al,al ; jne`
                //   0x00A798B8 with the neighbour-size block after it); `if (neighbour blocked) .. else ..` gives the blocks in the
                //   other order, DIFFER(678v678); testing my node instead DIFFER(681v678).
                else if (!neighbour_map_node->Blocked)
                    current_y += my_map_node->GetSize();
                else
                    current_y += neighbour_size;
            }
            else
                current_y += 0.5f;
        }
    }
}
