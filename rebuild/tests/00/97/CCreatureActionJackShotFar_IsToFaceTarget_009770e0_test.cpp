#include "rebuild_abi.h"
#include <cstdio>
extern "C" char FABLE_FASTCALL CCreatureAction_JackShotFar_IsToFaceTarget(void* action){ return 1; }
int main(){ int x; if(CCreatureAction_JackShotFar_IsToFaceTarget(&x)!=1){std::printf("B1");return 1;} std::printf("RB_9770e0_OK\n"); return 0; }