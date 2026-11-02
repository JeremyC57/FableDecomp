// CNavigatorManager::OutputWorld(C2DVector const &,unsigned char) -- output navigation data for the selected world area.
// Leaf: ?OutputWorld@CNavigatorManager@@QAEXABVC2DVector@@E@Z
// Grade: DIFFER(1983v1973) -- inline budget: retail calls vector<const CANavQuadTreeNode*>::push_back (0x00A17400) at all three start-node sites and blocked_list push_back 0x00A17440; this build inlines the first one
// Context: grades against the Codex row context $S/codex11_dl/x/rows/00a15890/selected_context/hdr (Codex per-feature macros; NOT rebased on rebuild/include); source = Codex selected; 00a15890 probes in probe/00a15890.log
// Flags: /Gr
#define FABLE_RETAIL_NAV_OUTPUT
#define FABLE_RETAIL_NAV_OUTPUT_FULL_GROWTH
#define FABLE_RETAIL_NAV_REGIONS
#define FABLE_RETAIL_C2DVECTOR_UI
#include <stdio.h>
#include "engine/CNavigatorManager.h"
#include "engine/IsSameNavQuadTreeNode.h"
#include "stlport46/nav_find.h"
void CNavigatorManager::OutputWorld(const C2DVector& pos,unsigned char nav_layer) {
 std::vector<const CANavQuadTreeNode*> open_list;
 std::vector<C2DVector> closed_list;
 std::vector<CNavBlockedQuadTreeNode*> blocked_list;
 const CANavQuadTreeNode* start_node=GetNodeAt(pos,nav_layer);
 if(!start_node)return;
 FILE* file=fopen("C:\\Documents and Settings\\mducker\\My Documents\\Visual Studio Projects\\FablePathVisualiser\\nav.data","wt");
 if(!file){file=fopen("nav.data","wt");if(!file)return;}
 fprintf(file,"0.0, 0.0, 0.0, 0.0\n");
 if(start_node->IsSwitchable()) {
  C2DBoxF box=start_node->GetAreaCovering();fprintf(file, "%f, %f, %f, %f, %f, %f, %f, %f, %d\n", box.TLX,box.TLY,box.BRX,box.TLY,box.BRX,box.BRY,box.TLX,box.BRY,4);
  open_list.push_back(start_node);
 }else if(start_node->IsNavigable()) {
  C2DBoxF box=start_node->GetAreaCovering();
  switch(start_node->GetPreferabilityCost()) {
  case 0:fprintf(file, "%f, %f, %f, %f, %f, %f, %f, %f, %d\n", box.TLX,box.TLY,box.BRX,box.TLY,box.BRX,box.BRY,box.TLX,box.BRY,0); break;
  case 128:fprintf(file, "%f, %f, %f, %f, %f, %f, %f, %f, %d\n", box.TLX,box.TLY,box.BRX,box.TLY,box.BRX,box.BRY,box.TLX,box.BRY,2); break;
  default:start_node->GetPreferabilityCost();fprintf(file, "%f, %f, %f, %f, %f, %f, %f, %f, %d\n", box.TLX,box.TLY,box.BRX,box.TLY,box.BRX,box.BRY,box.TLX,box.BRY,3); break;
  }
  open_list.push_back(start_node);
 }else {
  C2DBoxF box=start_node->GetAreaCovering();fprintf(file, "%f, %f, %f, %f, %f, %f, %f, %f, %d\n", box.TLX,box.TLY,box.BRX,box.TLY,box.BRX,box.BRY,box.TLX,box.BRY,1);
  const CNavBlockedQuadTreeNode* blocked_neighbour=static_cast<const CNavBlockedQuadTreeNode*>(start_node);
  CNavBlockedQuadTreeNode* blocked_node=new CNavBlockedQuadTreeNode(*blocked_neighbour);
  blocked_list.push_back(blocked_node);open_list.push_back(blocked_node);
 }
 while(open_list.size()) {
  const CANavQuadTreeNode* nearest_node=open_list.back();
  closed_list.push_back(nearest_node->GetPosition());open_list.pop_back();
  bool first_time=true;bool null_node=false;C2DVector first_neighbour;
  const CANavQuadTreeNode* current_neighbour=GetNextNeighbour(*nearest_node,0,first_neighbour,&null_node);
  while(current_neighbour) {
   if(first_time){first_time=false;first_neighbour=current_neighbour->GetPosition();}
   if(!current_neighbour->IsNavigable()&&!current_neighbour->IsSwitchable()) {
    const CNavBlockedQuadTreeNode* blocked_neighbour=static_cast<const CNavBlockedQuadTreeNode*>(current_neighbour);
    CNavBlockedQuadTreeNode* blocked_node=new CNavBlockedQuadTreeNode(*blocked_neighbour);
    current_neighbour=blocked_node;blocked_list.push_back(blocked_node);
   }
   if(std::find(closed_list.begin(),closed_list.end(),current_neighbour->GetPosition())==closed_list.end() && std::find_if(open_list.begin(),open_list.end(),IsSameNavQuadTreeNode(current_neighbour))==open_list.end()) {
    if(current_neighbour->IsSwitchable()) {
     C2DBoxF box=current_neighbour->GetAreaCovering();fprintf(file, "%f, %f, %f, %f, %f, %f, %f, %f, %d\n", box.TLX,box.TLY,box.BRX,box.TLY,box.BRX,box.BRY,box.TLX,box.BRY,4);
     open_list.push_back(current_neighbour);
    }else if(current_neighbour->IsNavigable()) {
     C2DBoxF box=current_neighbour->GetAreaCovering();
     switch(current_neighbour->GetPreferabilityCost()) {
     case 0:fprintf(file, "%f, %f, %f, %f, %f, %f, %f, %f, %d\n", box.TLX,box.TLY,box.BRX,box.TLY,box.BRX,box.BRY,box.TLX,box.BRY,0); break;
     case 128:fprintf(file, "%f, %f, %f, %f, %f, %f, %f, %f, %d\n", box.TLX,box.TLY,box.BRX,box.TLY,box.BRX,box.BRY,box.TLX,box.BRY,2); break;
     default:fprintf(file, "%f, %f, %f, %f, %f, %f, %f, %f, %d\n", box.TLX,box.TLY,box.BRX,box.TLY,box.BRX,box.BRY,box.TLX,box.BRY,3); break;
     }
     open_list.push_back(current_neighbour);
    }else if(null_node) {
     C2DBoxF box=current_neighbour->GetAreaCovering();fprintf(file, "%f, %f, %f, %f, %f, %f, %f, %f, %d\n", box.TLX,box.TLY,box.BRX,box.TLY,box.BRX,box.BRY,box.TLX,box.BRY,5);
     closed_list.push_back(current_neighbour->GetPosition());
    }else {
     C2DBoxF box=current_neighbour->GetAreaCovering();fprintf(file, "%f, %f, %f, %f, %f, %f, %f, %f, %d\n", box.TLX,box.TLY,box.BRX,box.TLY,box.BRX,box.BRY,box.TLX,box.BRY,1);
     open_list.push_back(current_neighbour);
    }
   }
   current_neighbour=GetNextNeighbour(*nearest_node,current_neighbour,first_neighbour,&null_node);
  }
 }
 if(file)fclose(file);
 while(blocked_list.size()) {CNavBlockedQuadTreeNode* blocked_node=blocked_list.back();blocked_list.pop_back();delete blocked_node;}
}
