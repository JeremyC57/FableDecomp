#include "rebuild_abi.h"
#include <cstdio>
extern "C" char FABLE_FASTCALL CCreatureAction_SummonerDead_IsToEvadeAllHits(void* action){ return 1; }
int main(){ int x; if(CCreatureAction_SummonerDead_IsToEvadeAllHits(&x)!=1){std::printf("B1");return 1;} std::printf("RB_8e4bc0_OK\n"); return 0; }