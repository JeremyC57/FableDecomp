#include <cstdio>
static void* g1=0,*g2=0;
static void __fastcall dv_h1(void* p){ g1=p; }
static void __fastcall dv_h2_005a4cdd(void* p){ g2=p; }
static void Dest_val(void* self){ dv_h1(self); dv_h2_005a4cdd(self); }
int main(){ int m; Dest_val(&m); if(g1!=&m||g2!=&m){ std::printf("005ac0ee_TEST FAIL\n"); return 1;}
 std::printf("005ac0ee_TEST PASS\n"); return 0;}