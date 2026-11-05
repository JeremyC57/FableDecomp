#include "rebuild_abi.h"
#include <cstdio>
extern "C" char FABLE_FASTCALL CCreatureAction_JackShotFloat_IsToFaceTarget(void* action){ return 1; }
int main(){ int x; if(CCreatureAction_JackShotFloat_IsToFaceTarget(&x)!=1){std::printf("B1");return 1;} std::printf("RB_976fa0_OK\n"); return 0; }