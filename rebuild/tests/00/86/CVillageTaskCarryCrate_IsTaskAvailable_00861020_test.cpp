#include <cstdio>
struct S { char pad[8]; char sub[4]; };
static int ita_probe(void* p){ (void)p; return 0; }
static bool IsTaskAvailable(S* task){ return ita_probe(reinterpret_cast<char*>(task)+8)==0; }
int main(){ S o; if(!IsTaskAvailable(&o)){ std::printf("00861020_TEST FAIL\n"); return 1;}
 std::printf("00861020_TEST PASS\n"); return 0;}