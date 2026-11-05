#include "rebuild_abi.h"
#include <cstdio>
struct C { char d; };
extern "C" int FABLE_FASTCALL CCreatureAction_UndeadIdleUnderground_GetAnimationTransitionInTime(C *action){ return 0; }
int main(){ C o; if(CCreatureAction_UndeadIdleUnderground_GetAnimationTransitionInTime(&o)!=0){std::printf("B1\n");return 1;}
 std::printf("RI_74dd70_OK\n"); return 0; }