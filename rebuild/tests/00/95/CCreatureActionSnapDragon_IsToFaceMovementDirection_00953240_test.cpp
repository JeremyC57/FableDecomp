#include "rebuild_abi.h"
#include <cstdio>
extern "C" char FABLE_FASTCALL CCreatureAction_SnapDragon_IsToFaceMovementDirection(void* action){ return 0; }
int main(){ int x; if(CCreatureAction_SnapDragon_IsToFaceMovementDirection(&x)!=0){std::printf("B1");return 1;} std::printf("RB_953240_OK\n"); return 0; }