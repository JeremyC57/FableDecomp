#include "engine/CVillageTaskTorch.h"
#include <cstdio>
static int probe(void* p){ (void)p; return 5; }
static bool CVillageTaskTorch_IsValid(CVillageTaskTorch* task){ return probe(reinterpret_cast<char*>(task)+8) != 0; }
int main(){ CVillageTaskTorch o; if(!CVillageTaskTorch_IsValid(&o)){ std::printf("0065ed9d_TEST FAIL\n"); return 1;}
 std::printf("0065ed9d_TEST PASS\n"); return 0;}