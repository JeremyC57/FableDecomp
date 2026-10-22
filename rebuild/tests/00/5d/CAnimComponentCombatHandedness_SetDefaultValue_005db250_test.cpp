#include <cstdio>
struct CAnimComponentCombatHandedness { char pad[8]; int StartHandedness; int EndHandedness; };
static void f(CAnimComponentCombatHandedness* component){ component->StartHandedness=2; component->EndHandedness=0; }
int main(){ CAnimComponentCombatHandedness o; o.StartHandedness=9;o.EndHandedness=9; f(&o); if(o.StartHandedness!=2||o.EndHandedness!=0){ std::printf("005db250_TEST FAIL\n"); return 1;}
 std::printf("005db250_TEST PASS\n"); return 0;}