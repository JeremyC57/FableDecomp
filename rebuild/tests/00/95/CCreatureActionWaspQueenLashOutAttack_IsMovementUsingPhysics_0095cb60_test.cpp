#include "rebuild_abi.h"
#include <cstdio>
extern "C" char FABLE_FASTCALL CCreatureAction_WaspQueenLashOutAttack_IsMovementUsingPhysics(void* action){ return 0; }
int main(){ int x; if(CCreatureAction_WaspQueenLashOutAttack_IsMovementUsingPhysics(&x)!=0){std::printf("B1");return 1;} std::printf("RB_95cb60_OK\n"); return 0; }