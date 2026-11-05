#include "rebuild_abi.h"
#include <cstdio>
struct C { char p10[0x10]; int f10; char p14[0x1c-0x14]; int f1c; };
extern "C" char FABLE_FASTCALL CCreatureActionHeroJump_IsPreventingNormalMovement(C *action){ return (char)((action->f1c + 1) < action->f10); }
int main(){ C o; o.f10=5; o.f1c=2; if(CCreatureActionHeroJump_IsPreventingNormalMovement(&o)!=1){std::printf("B1");return 1;} o.f1c=10; if(CCreatureActionHeroJump_IsPreventingNormalMovement(&o)!=0){std::printf("B2");return 1;} std::printf("RB_85b8a0_OK\n"); return 0; }