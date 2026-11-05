#include "rebuild_abi.h"
#include <cstdio>
struct C { char d; };
extern "C" char FABLE_FASTCALL CCombatAction_GenericBreakOffDrain_IsAbleToBeAttacked(C *action){ return 0; }
int main(){ C o; if(CCombatAction_GenericBreakOffDrain_IsAbleToBeAttacked(&o)!=0){std::printf("B1\n");return 1;}
 std::printf("RB_690df0_OK\n"); return 0; }