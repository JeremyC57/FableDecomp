#include <cstdio>
struct S { char pad[0xD8]; unsigned char f; };
static unsigned char PopDontPopulateNextLoadedRegion(S* world){ unsigned char v=world->f; world->f=0; return v; }
int main(){ S obj; obj.f=0x2A; unsigned char r=PopDontPopulateNextLoadedRegion(&obj);
 if(r!=0x2A||obj.f!=0){ std::printf("0049dbd0_TEST FAIL\n"); return 1;}
 std::printf("0049dbd0_TEST PASS\n"); return 0;}