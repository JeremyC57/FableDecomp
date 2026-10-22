#include "engine/CCombatSequence_ScorpionLungeAttack.h"
#include <cstdio>
static int re_call(CCombatSequence_ScorpionLungeAttack* sequence){ (void)sequence; return 0x1234; }
static void OnStart(CCombatSequence_ScorpionLungeAttack* sequence){ sequence->field28 = re_call(sequence); }
int main(){ CCombatSequence_ScorpionLungeAttack obj; obj.field28=0; OnStart(&obj);
 if(obj.field28!=0x1234){ std::printf("00906fc0_TEST FAIL\n"); return 1;}
 std::printf("00906fc0_TEST PASS\n"); return 0;}