#include <cstdio>
struct CAIStateGroup_StandStill; static int g_n=0;
struct CAIStateGroup_StandStill { int x; void Update(); };
int __fastcall ss_sub(CAIStateGroup_StandStill* stateGroup){ g_n++; return g_n<3 ? 4 : 0; }
void CAIStateGroup_StandStill::Update(){ int r; do { r = ss_sub(this); } while(r==4); }
int main(){ CAIStateGroup_StandStill o; o.Update(); if(g_n!=3){ std::printf("0091bcf0_TEST FAIL %d\n",g_n); return 1;}
 std::printf("0091bcf0_TEST PASS\n"); return 0;}