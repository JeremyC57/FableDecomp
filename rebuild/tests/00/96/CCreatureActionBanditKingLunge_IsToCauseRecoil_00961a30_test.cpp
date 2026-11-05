#include "rebuild_abi.h"
#include <cstdio>
extern "C" char FABLE_FASTCALL CCreatureAction_BanditKingLunge_IsToCauseRecoil(void* action){ return 1; }
int main(){ int x; if(CCreatureAction_BanditKingLunge_IsToCauseRecoil(&x)!=1){std::printf("B1");return 1;} std::printf("RB_961a30_OK\n"); return 0; }