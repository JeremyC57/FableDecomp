#include <cstdio>
struct S; static int g_hit=0;
struct S { char pad[0x10]; int f10; void OnAnimationLoaded(); };
void __fastcall oal_sub(S* action){ g_hit=1; }
void S::OnAnimationLoaded(){ oal_sub(this); this->f10 = 7; }
int main(){ S o; o.f10=0; o.OnAnimationLoaded(); if(g_hit!=1||o.f10!=7){ std::printf("008bedc0_TEST FAIL\n"); return 1;}
 std::printf("008bedc0_TEST PASS\n"); return 0;}