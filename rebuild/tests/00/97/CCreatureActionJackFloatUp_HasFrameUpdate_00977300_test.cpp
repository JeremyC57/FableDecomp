#include "rebuild_abi.h"
#include <cstdio>
extern "C" char FABLE_FASTCALL CCreatureAction_JackFloatUp_HasFrameUpdate(void* action){ return 1; }
int main(){ int x; if(CCreatureAction_JackFloatUp_HasFrameUpdate(&x)!=1){std::printf("B1");return 1;} std::printf("RB_977300_OK\n"); return 0; }