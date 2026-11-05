#include "rebuild_abi.h"
#include <cstdio>
struct C { char d; };
extern "C" char FABLE_FASTCALL CCombatAction_RecoverFromDazed_IsOverrideableByActionOfSamePriority(C *action){ return 1; }
int main(){ C o; if(CCombatAction_RecoverFromDazed_IsOverrideableByActionOfSamePriority(&o)!=1){std::printf("B1\n");return 1;}
 std::printf("RB_8e4400_OK\n"); return 0; }