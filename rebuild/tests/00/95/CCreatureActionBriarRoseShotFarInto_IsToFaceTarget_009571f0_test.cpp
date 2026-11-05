#include "rebuild_abi.h"
#include <cstdio>
extern "C" char FABLE_FASTCALL CCreatureAction_BriarRoseShotFarInto_IsToFaceTarget(void* action){ return 1; }
int main(){ int x; if(CCreatureAction_BriarRoseShotFarInto_IsToFaceTarget(&x)!=1){std::printf("B1");return 1;} std::printf("RB_9571f0_OK\n"); return 0; }