#include "engine/CCreatureActionBase.h"
#include <cstdio>
class CAnimationEntry; static int cat_probe(CAnimationEntry* p){ (void)p; return 2; }
static bool CanAnimationTransitionOut(CCreatureActionBase* action){ return (~cat_probe(action->PAnimEntry_Data)) & 1; }
int main(){ CCreatureActionBase obj; obj.PAnimEntry_Data=0; if(!CanAnimationTransitionOut(&obj)){ std::printf("006928b0_TEST FAIL\n"); return 1;}
 std::printf("006928b0_TEST PASS\n"); return 0;}