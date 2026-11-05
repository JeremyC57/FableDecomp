#include "rebuild_abi.h"
#include <cstdio>
static int hit=0;
extern "C" void FABLE_STDCALL Helper_68c6d0(void* event){ hit=1; }
extern "C" void FABLE_STDCALL CInputProcessHeroInformationScreens_ProcessInput(void* event, void* input){ Helper_68c6d0(event); }
int main(){ int x,y; CInputProcessHeroInformationScreens_ProcessInput(&x,&y); if(!hit){std::printf("B1");return 1;} std::printf("RB_68c6d0_OK\n"); return 0; }