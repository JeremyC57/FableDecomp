#include "rebuild_abi.h"
#include <cstdio>
extern "C" char FABLE_FASTCALL CCreatureAction_TentacleStrikeInto_IsMovementUsingPhysics(void* action){ return 0; }
int main(){ int x; if(CCreatureAction_TentacleStrikeInto_IsMovementUsingPhysics(&x)!=0){std::printf("B1");return 1;} std::printf("RB_925310_OK\n"); return 0; }