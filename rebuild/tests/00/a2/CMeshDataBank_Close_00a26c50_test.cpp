#include <cstdio>
class CMeshDataBank;
static int g_seq=0;
class CMeshDataBank { public: int x; };
void __fastcall h1(CMeshDataBank* bank){ g_seq=g_seq*10+1; }
void __fastcall h2(CMeshDataBank* bank){ g_seq=g_seq*10+2; }
void __fastcall Close(CMeshDataBank* bank){ h1(bank); h2(bank); }
int main(){ CMeshDataBank o; Close(&o);
 if(g_seq!=12){ std::printf("00a26c50_TEST FAIL\n"); return 1;}
 std::printf("00a26c50_TEST PASS\n"); return 0;}