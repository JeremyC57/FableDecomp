#include <cstdio>
class CFontDataBank;
static int g_seq=0;
class CFontDataBank { public: int x; };
void __fastcall h1(CFontDataBank* bank){ g_seq=g_seq*10+1; }
void __fastcall h2(CFontDataBank* bank){ g_seq=g_seq*10+2; }
void __fastcall Close(CFontDataBank* bank){ h1(bank); h2(bank); }
int main(){ CFontDataBank o; Close(&o); if(g_seq!=12){ std::printf("00a635c0_TEST FAIL\n"); return 1;}
 std::printf("00a635c0_TEST PASS\n"); return 0;}