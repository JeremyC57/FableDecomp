#include <cstdio>
struct CEngineDef { long v[17]; };
struct CPersistContext { int n; void Transfer(long& x){ x += 1; ++n; } };
int main(){ CEngineDef d={}; CPersistContext c={0}; for(int i=0;i<17;i++) d.v[i]=i; for(int i=0;i<17;i++){d.v[i]+=1;c.n++;} if(c.n!=17||d.v[0]!=1||d.v[16]!=17){std::printf("00433fe0_TEST FAIL\n");return 1;} std::printf("00433fe0_TEST PASS\n");return 0; }
