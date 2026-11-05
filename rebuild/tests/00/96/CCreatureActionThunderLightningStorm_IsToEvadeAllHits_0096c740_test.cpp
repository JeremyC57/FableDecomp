#include "rebuild_abi.h"
#include <cstdio>
extern "C" char FABLE_FASTCALL CCreatureAction_ThunderLightningStorm_IsToEvadeAllHits(void* action){ return 1; }
int main(){ int x; if(CCreatureAction_ThunderLightningStorm_IsToEvadeAllHits(&x)!=1){std::printf("B1");return 1;} std::printf("RB_96c740_OK\n"); return 0; }