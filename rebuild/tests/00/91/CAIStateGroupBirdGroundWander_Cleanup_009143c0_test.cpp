#include "engine/CAIStateGroup_BirdGroundWander.h"
#include <cstdio>
static CAIStateGroup_BirdGroundWander* gS3;
void __fastcall h1_9143c0(CAIStateGroup_BirdGroundWander* stateGroup){ gS3=stateGroup; }
int __cdecl h2_9143c0(){ return 88; }
void __fastcall fn_009143c0(CAIStateGroup_BirdGroundWander* stateGroup){ h1_9143c0(stateGroup); stateGroup->FrameLastFinishedWalking = h2_9143c0(); }
int main(){ CAIStateGroup_BirdGroundWander b; fn_009143c0(&b); if(gS3!=&b||b.FrameLastFinishedWalking!=88){ printf("009143c0_TEST FAIL\n"); return 1; }
 printf("009143c0_TEST PASS\n"); return 0; }