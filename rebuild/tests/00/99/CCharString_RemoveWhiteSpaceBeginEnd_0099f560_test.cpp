#include <cstdio>
static void* g1=0,*g2=0;
static void __fastcall dv_h1_0099f450(void* p){ g1=p; }
static void __fastcall dv_h2_0099f4d0(void* p){ g2=p; }
static void D(void* self){ dv_h1_0099f450(self); dv_h2_0099f4d0(self); }
int main(){ int m; D(&m); if(g1!=&m||g2!=&m){ std::printf("0099f560_TEST FAIL\n"); return 1;}
 std::printf("0099f560_TEST PASS\n"); return 0;}