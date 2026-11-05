#include "rebuild_abi.h"
#include <cstdio>
extern "C" char FABLE_FASTCALL CCreatureAction_JackDeathInto_IsToEvadeAllHits(void* action){ return 1; }
int main(){ int x; if(CCreatureAction_JackDeathInto_IsToEvadeAllHits(&x)!=1){std::printf("B1");return 1;} std::printf("RB_977270_OK\n"); return 0; }