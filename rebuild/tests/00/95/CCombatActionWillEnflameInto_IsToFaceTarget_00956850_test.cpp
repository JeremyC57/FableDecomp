#include "rebuild_abi.h"
#include <cstdio>
extern "C" char FABLE_FASTCALL CCombatAction_WillEnflameInto_IsToFaceTarget(void* action){ return 1; }
int main(){ int x; if(CCombatAction_WillEnflameInto_IsToFaceTarget(&x)!=1){std::printf("B1");return 1;} std::printf("RB_956850_OK\n"); return 0; }