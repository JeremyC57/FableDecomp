#include "rebuild_abi.h"
#include <cstdio>
struct C { char d; };
extern "C" char FABLE_FASTCALL CCreatureAction_BlockBroken_IsToFaceTargetAtStart(C *action){ return 1; }
int main(){ C o; if(CCreatureAction_BlockBroken_IsToFaceTargetAtStart(&o)!=1){std::printf("B1\n");return 1;}
 std::printf("RB_739a50_OK\n"); return 0; }