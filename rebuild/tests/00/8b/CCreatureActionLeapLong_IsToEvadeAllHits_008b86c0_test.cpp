// Layout: retail STLport 4.6 container sizes put the PDB member on these bytes; renamed `b114` -> `Landed`.
#include "engine/CCreatureAction_LeapLong.h"
#include <cstdio>
static bool f(CCreatureAction_LeapLong* action){ return action->Landed==0; }
int main(){ CCreatureAction_LeapLong o; o.Landed=0; if(!f(&o)){ std::printf("008b86c0_TEST FAIL\n"); return 1;}
 o.Landed=5; if(f(&o)){ std::printf("008b86c0_TEST FAIL\n"); return 1;}
 std::printf("008b86c0_TEST PASS\n"); return 0;}