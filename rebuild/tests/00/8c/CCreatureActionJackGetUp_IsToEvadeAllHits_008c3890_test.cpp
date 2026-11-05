#include "rebuild_abi.h"
#include <cstdio>
struct C { char d; };
extern "C" char FABLE_FASTCALL CCreatureAction_JackGetUp_IsToEvadeAllHits(C *action){ return 1; }
int main(){ C o; if(CCreatureAction_JackGetUp_IsToEvadeAllHits(&o)!=1){std::printf("B1\n");return 1;}
 std::printf("RB_8c3890_OK\n"); return 0; }