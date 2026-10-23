#include <cstdio>
static void* g1=0,*g2=0;
static void __fastcall dv_h1_008d58d0(void* p){ g1=p; }
static void __fastcall dv_h2_00694430(void* p){ g2=p; }
static void D(void* action){ dv_h1_008d58d0(action); dv_h2_00694430(action); }
int main(){ int m; D(&m); if(g1!=&m||g2!=&m){ std::printf("008d5960_TEST FAIL\n"); return 1;}
 std::printf("008d5960_TEST PASS\n"); return 0;}