#include "rebuild_abi.h"
#include <cstdio>
extern "C" char FABLE_FASTCALL CCreatureAction_JackShotNear_IsToFaceTarget(void* action){ return 1; }
int main(){ int x; if(CCreatureAction_JackShotNear_IsToFaceTarget(&x)!=1){std::printf("B1");return 1;} std::printf("RB_977040_OK\n"); return 0; }