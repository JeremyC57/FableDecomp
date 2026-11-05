#include "rebuild_abi.h"
#include <cstdio>
extern "C" char FABLE_FASTCALL CCreatureAction_SnapDragonFast_IsToFaceMovementDirection(void* action){ return 0; }
int main(){ int x; if(CCreatureAction_SnapDragonFast_IsToFaceMovementDirection(&x)!=0){std::printf("B1");return 1;} std::printf("RB_9532e0_OK\n"); return 0; }