#include <cstdio>
class CSystemManager; struct S { char pad[4]; void* f4; int Initialise(CSystemManager* manager); };
int S::Initialise(CSystemManager* manager){ this->f4 = manager; return 1; }
int main(){ int m; S o; if(o.Initialise((CSystemManager*)&m)!=1||o.f4!=&m){ std::printf("009e4ad0_TEST FAIL\n"); return 1;}
 std::printf("009e4ad0_TEST PASS\n"); return 0;}