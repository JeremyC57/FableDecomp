#include <cstdio>
struct CStreamingFontDataBank;
static int g_seq=0;
struct CStreamingFontDataBank { int x; };
void __fastcall h1(CStreamingFontDataBank* bank){ g_seq=g_seq*10+1; }
void __fastcall h2(CStreamingFontDataBank* bank){ g_seq=g_seq*10+2; }
void __fastcall Close(CStreamingFontDataBank* bank){ h1(bank); h2(bank); }
int main(){ CStreamingFontDataBank o; Close(&o); if(g_seq!=12){ std::printf("00a5f420_TEST FAIL\n"); return 1;}
 std::printf("00a5f420_TEST PASS\n"); return 0;}