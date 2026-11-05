#include "rebuild_abi.h"
#include <cstdio>
struct C { char d; };
extern "C" char FABLE_FASTCALL CCreatureAction_GetUpFromSitting_IsMovementUsingPhysics(C *action){ return 0; }
int main(){ C o; if(CCreatureAction_GetUpFromSitting_IsMovementUsingPhysics(&o)!=0){std::printf("B1\n");return 1;}
 std::printf("RB_8601d0_OK\n"); return 0; }