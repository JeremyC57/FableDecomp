#include "rebuild_abi.h"
#include <cstdio>
extern "C" int FABLE_FASTCALL CCreatureAction_PutGoodsDown_GetAnimExtraFlags(void* action){ return 8; }
int main(){ int x; if(CCreatureAction_PutGoodsDown_GetAnimExtraFlags(&x)!=8){std::printf("B1");return 1;} std::printf("RB_915860_OK\n"); return 0; }