// CNavQuadTree::SetUpRegions(long mapIndex) -- flood the navigation graph into regions: region 1 first, then one region per region seed whose node starts a region (MakeRegion over the growing node list), then regions grown from navigable nodes that border switchable ones, each region appended to Regions with the next internal index; last, every node left in region zero is deleted and replaced by the shared BlockedNode
// Grade: DIFFER(1270v1222) -- the second region push (seed loop) expands CNavRegion's implicit copy ctor where retail calls 0x00A83110; register / slot rotation follows (part 2 / 3 counters)
// Leaf: ?SetUpRegions@CNavQuadTree@@AAEXJ@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A7A0A0, 1222 bytes, thiscall `ret 4`.
// Identity: Ego_r.pdb ?SetUpRegions@CNavQuadTree@@AAEXJ@Z (donor 0x00BC0880, 1488 B, between InitialiseLines and ClearNodes);
//   the one caller is CNavQuadTree::Initialise 0x00A7A8D0 (0x00A7AED7, the last step after the action points). The body reads
//   RegionSeeds +0x68 (12-byte CNavigationPosition), appends CNavRegion(mapIndex, n) to Regions +0x74 and calls the node virtuals
//   MakeRegion / AddNavigableNodeNeighbouringSwitchableNodeToRegion / RemoveRegionZero (slots 12 / 13 / 14, Ego_r order), then
//   swaps region-zero nodes for ?BlockedNode@CNavQuadTree@@2VCNavBlockedQuadTreeNode@@A (0x013D2D2C). Label correction: the
//   manifest's `_global::CNavQuadTree_InitializeWithEvents` is propagated.
// Callees: malloc 0x00BFEA0E / free 0x00BFEA14 (STLport list nodes), CNavRegion copy ctor 0x00A83110 (implicit), list<
//   CCountedPointer<CNavRegionConnection> >::clear 0x00A80A20 (_List_base::clear, the temporary's destructor),
//   _M_insert_dispatch 0x00A80E90 (the list copy inside the expanded copy ctor), CNavQuadTree::GetNodeAt 0x00A770B0.
// Open: retail ORs the packed id (`or eax,1` / `and eax,0FFFFh ; or eax,esi`) where the out-of-line ctor 0x00A78680 XORs;
//   the `|` body here reproduces the expansions (the xor body gives `xor eax,1`). The copy-ctor split (call, call, expand) is
//   per-site; every statement shape tried (s3..s9 in LOG.tsv) expands sites 2 and 3.
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CANAVQUADTREENODE
#define FABLE_RETAIL_CNAVBLOCKEDQUADTREENODE
#define FABLE_RETAIL_CNAVQUADTREE
#include "engine/CNavQuadTree.h"
#include "engine/CNavBlockedQuadTreeNode.h"

CNavRegion::CNavRegion(long mapIndex, long internalIndex)
{
    RegionID = (mapIndex << 16) | (internalIndex & 0xFFFF);
    MostRecentSearchID = 0;
}

void CNavQuadTree::SetUpRegions(long mapIndex)
{
    long regionIndex = 1;
    Regions.push_back(CNavRegion(mapIndex, regionIndex));
    for (std::vector<CNavigationPosition>::iterator seed = RegionSeeds.begin(); seed != RegionSeeds.end(); ++seed) {
        CANavQuadTreeNode* node = const_cast<CANavQuadTreeNode*>(GetNodeAt(seed->Position, seed->Layer));
        if (node) {
            std::list<CANavQuadTreeNode*> nodes;
            if (node->MakeRegion(&Regions.back(), nodes)) {
                for (std::list<CANavQuadTreeNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it)
                    (*it)->MakeRegion(&Regions.back(), nodes);
                ++regionIndex;
                Regions.push_back(CNavRegion(mapIndex, regionIndex));
            }
        }
    }
    for (unsigned long layer = 0; layer < NavigationLayers.size(); ++layer) {
        for (unsigned long i = 0; i < NavigationLayers[layer].Nodes.size(); ++i) {
            CANavQuadTreeNode* node = NavigationLayers[layer].Nodes[i];
            std::list<CANavQuadTreeNode*> nodes;
            while (node->AddNavigableNodeNeighbouringSwitchableNodeToRegion(&Regions.back(), nodes)) {
                for (std::list<CANavQuadTreeNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it)
                    (*it)->MakeRegion(&Regions.back(), nodes);
                ++regionIndex;
                Regions.push_back(CNavRegion(mapIndex, regionIndex));
                nodes.clear();
            }
        }
    }
    for (unsigned long layer2 = 0; layer2 < NavigationLayers.size(); ++layer2) {
        for (unsigned long j = 0; j < NavigationLayers[layer2].Nodes.size(); ++j) {
            CANavQuadTreeNode* node = NavigationLayers[layer2].Nodes[j];
            if (node->RemoveRegionZero() && node != &BlockedNode) {
                delete node;
                NavigationLayers[layer2].Nodes[j] = &BlockedNode;
            }
        }
    }
}
