// CNavQuadTree::SetUpRegions(long) -- build the navigation regions for the selected map.
// Leaf: ?SetUpRegions@CNavQuadTree@@AAEXJ@Z
// Grade: DIFFER(1270v1222) -- the second CNavRegion push expands its copy where retail calls the out-of-line copy constructor (VC7.1 RTM per-site inline budget), so map_index stays in another register and later list/index scheduling shifts (+48 bytes); 10 shapes (named region locals per site, pre-increment arguments, unsigned/zero-based counter, iterator reads, list placement, cached back pointer) leave or worsen it. Graded in Codex's header context
// Flags: /Gr
#define FABLE_RETAIL_NAV_REGIONS
#include "engine/CNavQuadTree.h"
void CNavQuadTree::SetUpRegions(long map_index)
{
 long region=1;
 Regions.push_back(CNavRegion(map_index,region));
 for(std::vector<CNavigationPosition>::const_iterator pos_it=RegionSeeds.begin();pos_it!=RegionSeeds.end();++pos_it){
  const CNavigationPosition& nav_pos=*pos_it;
  CANavQuadTreeNode* node=const_cast<CANavQuadTreeNode*>(GetNodeAt(nav_pos.Position,nav_pos.Layer));
  if(node){
   std::list<CANavQuadTreeNode*> nodes_for_region;
   if(node->MakeRegion(&Regions.back(),nodes_for_region)){
    for(std::list<CANavQuadTreeNode*>::iterator r_it=nodes_for_region.begin();r_it!=nodes_for_region.end();++r_it)(*r_it)->MakeRegion(&Regions.back(),nodes_for_region);
    ++region;Regions.push_back(CNavRegion(map_index,region));
   }
  }
 }
 for(long layer=0;layer<NavigationLayers.size();++layer){
  for(long current_node=0;current_node<NavigationLayers[layer].Nodes.size();++current_node){
   CANavQuadTreeNode* pnode=NavigationLayers[layer].Nodes[current_node];
   std::list<CANavQuadTreeNode*> nodes_for_region;
   while(pnode->AddNavigableNodeNeighbouringSwitchableNodeToRegion(&Regions.back(),nodes_for_region)){
    for(std::list<CANavQuadTreeNode*>::iterator r_it=nodes_for_region.begin();r_it!=nodes_for_region.end();++r_it)(*r_it)->MakeRegion(&Regions.back(),nodes_for_region);
    ++region;Regions.push_back(CNavRegion(map_index,region));nodes_for_region.clear();
   }
  }
 }
 for(long layer=0;layer<NavigationLayers.size();++layer){
  for(long current_node=0;current_node<NavigationLayers[layer].Nodes.size();++current_node){
   CANavQuadTreeNode* pnode=NavigationLayers[layer].Nodes[current_node];
   if(pnode->RemoveRegionZero()&&pnode!=&BlockedNode){delete pnode;NavigationLayers[layer].Nodes[current_node]=&BlockedNode;}
  }
 }
}
