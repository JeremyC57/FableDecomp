#include <cstdio>
class CThingTrackNode {public:CThingTrackNode* NextNode;CThingTrackNode* PreviousNode;bool IsStart,IsEnd;int guard;bool IsTrackStart()const{return IsStart;}bool IsTrackEnd()const{return IsEnd;}void FlipTrackDirection();};
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

int main(){int ok=1,cases=0;for(int n=1;n<=9;++n)for(int origin=0;origin<n;++origin)for(int pattern=0;pattern<8;++pattern){CThingTrackNode nodes[9];bool starts[9],ends[9];for(int i=0;i<n;++i){nodes[i].PreviousNode=i?nodes+i-1:0;nodes[i].NextNode=i+1<n?nodes+i+1:0;starts[i]=pattern<4?((pattern&1)!=0):((i+pattern)&1)!=0;ends[i]=pattern<4?((pattern&2)!=0):((i+pattern)&2)!=0;nodes[i].IsStart=starts[i];nodes[i].IsEnd=ends[i];nodes[i].guard=100+i;}nodes[origin].FlipTrackDirection();for(int i=0;i<n;++i){bool expect_start,expect_end;if(i==origin){expect_start=!starts[i]&&ends[i];expect_end=starts[i];}else{expect_start=ends[i];expect_end=!ends[i]&&starts[i];}ok &= nodes[i].NextNode==(i?nodes+i-1:0)&&nodes[i].PreviousNode==(i+1<n?nodes+i+1:0);ok &= nodes[i].IsStart==expect_start&&nodes[i].IsEnd==expect_end&&nodes[i].guard==100+i;}++cases;}ok &= cases==360;std::printf(ok?"00836de0_TEST PASS\n":"00836de0_TEST FAIL\n");return ok?0:1;}