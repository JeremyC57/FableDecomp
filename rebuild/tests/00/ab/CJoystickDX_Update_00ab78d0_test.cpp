#include <cstdio>
struct CJoystickDX;
static int g_seq=0;
struct CJoystickDX { int x; };
void __fastcall h1(CJoystickDX* joystickDX){ g_seq=g_seq*10+1; }
void __fastcall h2(CJoystickDX* joystickDX){ g_seq=g_seq*10+2; }
void __fastcall Close(CJoystickDX* joystickDX){ h1(joystickDX); h2(joystickDX); }
int main(){ CJoystickDX o; Close(&o); if(g_seq!=12){ std::printf("00ab78d0_TEST FAIL\n"); return 1;}
 std::printf("00ab78d0_TEST PASS\n"); return 0;}