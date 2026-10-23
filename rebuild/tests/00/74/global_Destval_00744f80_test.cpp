#include <cstdio>
static void* g1=0,*g2=0;
static void __fastcall dv_h1_00744db0(void* p){ g1=p; }
static void __fastcall dv_h2_00686900(void* p){ g2=p; }
static void D(void* self){ dv_h1_00744db0(self); dv_h2_00686900(self); }
int main(){ int m; D(&m); if(g1!=&m||g2!=&m){ std::printf("00744f80_TEST FAIL\n"); return 1;}
 std::printf("00744f80_TEST PASS\n"); return 0;}