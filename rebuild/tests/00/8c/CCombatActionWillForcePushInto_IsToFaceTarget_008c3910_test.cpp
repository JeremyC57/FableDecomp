#include "rebuild_abi.h"
#include <cstdio>
struct C { char d; };
extern "C" char FABLE_FASTCALL CCombatAction_WillForcePushInto_IsToFaceTarget(C *action){ return 1; }
int main(){ C o; if(CCombatAction_WillForcePushInto_IsToFaceTarget(&o)!=1){std::printf("B1\n");return 1;}
 std::printf("RB_8c3910_OK\n"); return 0; }