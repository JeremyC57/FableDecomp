#include <stdio.h>
struct Self { char pad[0x28]; int field28; };
static int g_mult; static int g_cur;
static int GetCur() { return g_cur; }
static bool CCombatSequence_ScorpionLungeAttack_IsValid(Self* self){ int mult=g_mult; int cur=GetCur(); return (cur-self->field28)>(mult*10); }
int main(){ Self s; s.field28=100;
 g_mult=5; g_cur=200; bool r1=CCombatSequence_ScorpionLungeAttack_IsValid(&s);
 g_mult=10; g_cur=200; bool r2=CCombatSequence_ScorpionLungeAttack_IsValid(&s);
 g_mult=10; g_cur=201; bool r3=CCombatSequence_ScorpionLungeAttack_IsValid(&s);
 if(r1&&!r2&&r3) printf("BEHAVIOR_OK\n"); else printf("BEHAVIOR_FAIL r1=%d r2=%d r3=%d\n",r1,r2,r3);
 return 0; }