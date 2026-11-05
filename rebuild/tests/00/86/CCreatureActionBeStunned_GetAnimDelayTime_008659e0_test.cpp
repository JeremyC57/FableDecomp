#include "rebuild_abi.h"
#include <cstdio>
struct C { char d; };
extern "C" int FABLE_FASTCALL CCreatureAction_BeStunned_GetAnimDelayTime(C *action){ return 0; }
int main(){ C o; if(CCreatureAction_BeStunned_GetAnimDelayTime(&o)!=0){std::printf("B1\n");return 1;}
 std::printf("RI_8659e0_OK\n"); return 0; }