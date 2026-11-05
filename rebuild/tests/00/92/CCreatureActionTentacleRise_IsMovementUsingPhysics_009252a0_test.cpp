#include "rebuild_abi.h"
#include <cstdio>
extern "C" char FABLE_FASTCALL CCreatureAction_TentacleRise_IsMovementUsingPhysics(void* action){ return 0; }
int main(){ int x; if(CCreatureAction_TentacleRise_IsMovementUsingPhysics(&x)!=0){std::printf("B1");return 1;} std::printf("RB_9252a0_OK\n"); return 0; }