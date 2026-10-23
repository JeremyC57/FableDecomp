#include <cstdio>
struct S;
static int g_seq=0;
struct S { int x; };
void __fastcall h1_00b948e0(S* self){ g_seq=g_seq*10+1; }
void __fastcall h2_00b91980(S* self){ g_seq=g_seq*10+2; }
void __fastcall Close(S* self){ h1_00b948e0(self); h2_00b91980(self); }
int main(){ S o; Close(&o); if(g_seq!=12){ std::printf("00b94a30_TEST FAIL\n"); return 1;}
 std::printf("00b94a30_TEST PASS\n"); return 0;}