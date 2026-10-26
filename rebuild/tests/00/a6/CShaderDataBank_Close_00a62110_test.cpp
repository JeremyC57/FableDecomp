#include <cstdio>
class CShaderDataBank;
static int g_seq=0;
class CShaderDataBank { public: int x; };
void __fastcall h1(CShaderDataBank* bank){ g_seq=g_seq*10+1; }
void __fastcall h2(CShaderDataBank* bank){ g_seq=g_seq*10+2; }
void __fastcall Close(CShaderDataBank* bank){ h1(bank); h2(bank); }
int main(){ CShaderDataBank o; Close(&o); if(g_seq!=12){ std::printf("00a62110_TEST FAIL\n"); return 1;}
 std::printf("00a62110_TEST PASS\n"); return 0;}