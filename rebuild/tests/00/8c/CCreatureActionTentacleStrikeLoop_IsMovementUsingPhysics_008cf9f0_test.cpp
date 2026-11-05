#include "rebuild_abi.h"
#include <cstdio>
struct C { char d; };
extern "C" char FABLE_FASTCALL CCreatureAction_TentacleStrikeLoop_IsMovementUsingPhysics(C *action){ return 0; }
int main(){ C o; if(CCreatureAction_TentacleStrikeLoop_IsMovementUsingPhysics(&o)!=0){std::printf("B1\n");return 1;}
 std::printf("RB_8cf9f0_OK\n"); return 0; }