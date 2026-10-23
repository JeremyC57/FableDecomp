#include <cstdio>
static void* g1=0,*g2=0;
static void __fastcall dv_h1_005fd0f0(void* p){ g1=p; }
static void __fastcall dv_h2_005fd1d0(void* p){ g2=p; }
static void Dest_val(void* self){ dv_h1_005fd0f0(self); dv_h2_005fd1d0(self); }
int main(){ int m; Dest_val(&m); if(g1!=&m||g2!=&m){ std::printf("005fe830_TEST FAIL\n"); return 1;}
 std::printf("005fe830_TEST PASS\n"); return 0;}