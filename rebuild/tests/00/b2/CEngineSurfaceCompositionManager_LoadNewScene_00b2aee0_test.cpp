#include <cstdio>
struct Sub;
static char* g_p=0;
struct S { char pad[0x31c]; char M(int params); };
void __fastcall sub_009fd0c0(Sub* p){ g_p=(char*)p; }
char S::M(int params){ sub_009fd0c0((Sub*)((char*)this+0x31c)); return 0; }
int main(){ static S o; char r=o.M(1);
 if(r!=0||g_p!=(char*)&o+0x31c){ std::printf("00b2aee0_TEST FAIL\n"); return 1;}
 std::printf("00b2aee0_TEST PASS\n"); return 0;}