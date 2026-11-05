#include "rebuild_abi.h"
#include <cstdio>
extern "C" char FABLE_FASTCALL CCreatureAction_ScorpionUnblockableRightTurnAttack_IsStrikeAction(void* action){ return 1; }
int main(){ int x; if(CCreatureAction_ScorpionUnblockableRightTurnAttack_IsStrikeAction(&x)!=1){std::printf("B1");return 1;} std::printf("RB_97c8e0_OK\n"); return 0; }