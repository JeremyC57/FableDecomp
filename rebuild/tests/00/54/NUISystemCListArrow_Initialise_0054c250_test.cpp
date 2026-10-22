#include <cstdio>
struct CListArrow;
static int g_hit=0;
struct CListArrow { char pad[0x18c]; char b18c; };
void __fastcall sub(CListArrow* arrow){ g_hit=1; }
void __fastcall ChangeCameraAngle(CListArrow* arrow){ sub(arrow); arrow->b18c = 0; }
int main(){ static CListArrow o; o.b18c=9; ChangeCameraAngle(&o);
 if(g_hit!=1||o.b18c!=0){ std::printf("0054c250_TEST FAIL\n"); return 1;}
 std::printf("0054c250_TEST PASS\n"); return 0;}