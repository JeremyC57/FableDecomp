#include <cstdio>
struct S;
static int g_seq=0;
struct S { int x; };
void __fastcall h1_00cb7c40(S* self){ g_seq=g_seq*10+1; }
void __fastcall h2_00cb8170(S* self){ g_seq=g_seq*10+2; }
void __fastcall Close(S* self){ h1_00cb7c40(self); h2_00cb8170(self); }
int main(){ S o; Close(&o); if(g_seq!=12){ std::printf("00cb8220_TEST FAIL\n"); return 1;}
 std::printf("00cb8220_TEST PASS\n"); return 0;}