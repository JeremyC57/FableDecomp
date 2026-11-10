// 00836de0 CThingTrackNode::FlipTrackDirection -- reverse the connected track in both directions.
// Retail: 0x00836DE0, 247 bytes, __fastcall
// Leaf: ?FlipTrackDirection@CThingTrackNode@@QAEXXZ
// Flags: none
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGTRACKNODE
#define FABLE_RETAIL_CTHINGTRACKNODE_FLIP
#include "fable_string.h"
#include "engine/CThingTrackNode.h"
void CThingTrackNode::FlipTrackDirection()
{
 CThingTrackNode* old_previous_node=PreviousNode;
 CThingTrackNode* old_next_node=NextNode;
 PreviousNode=old_next_node;
 NextNode=old_previous_node;
 if(IsStart){IsStart=false;IsEnd=true;}
 else if(IsEnd){IsEnd=false;IsStart=true;}
 CThingTrackNode* forward_node=old_next_node;
 CThingTrackNode* backward_node=old_previous_node;
 while(forward_node||backward_node){
  if(forward_node){
   old_previous_node=forward_node->PreviousNode;
   old_next_node=forward_node->NextNode;
   forward_node->PreviousNode=old_next_node;
   forward_node->NextNode=old_previous_node;
   if(forward_node->IsTrackEnd()){forward_node->IsEnd=false;forward_node->IsStart=true;}
   else if(forward_node->IsTrackStart()){forward_node->IsEnd=true;forward_node->IsStart=false;}
   forward_node=old_next_node;
  }
  if(backward_node){
   old_previous_node=backward_node->PreviousNode;
   old_next_node=backward_node->NextNode;
   backward_node->PreviousNode=old_next_node;
   backward_node->NextNode=old_previous_node;
   if(backward_node->IsTrackEnd()){backward_node->IsEnd=false;backward_node->IsStart=true;}
   else if(backward_node->IsTrackStart()){backward_node->IsEnd=true;backward_node->IsStart=false;}
   backward_node=old_previous_node;
  }
 }
}