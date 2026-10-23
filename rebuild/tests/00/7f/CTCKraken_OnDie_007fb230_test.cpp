#include <cstdio>
static void* g1=0,*g2=0;
static void __fastcall dv_h1_007fb1c0(void* p){ g1=p; }
static void __fastcall dv_h2_007fb150(void* p){ g2=p; }
static void D(void* kraken){ dv_h1_007fb1c0(kraken); dv_h2_007fb150(kraken); }
int main(){ int m; D(&m); if(g1!=&m||g2!=&m){ std::printf("007fb230_TEST FAIL\n"); return 1;}
 std::printf("007fb230_TEST PASS\n"); return 0;}