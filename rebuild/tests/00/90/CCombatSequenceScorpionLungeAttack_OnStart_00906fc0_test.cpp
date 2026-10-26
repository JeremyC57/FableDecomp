// Layout: retail STLport 4.6 container sizes put the PDB member on these bytes; renamed `field28` -> `LastFrameUsed`.
#include "engine/CCombatSequence_ScorpionLungeAttack.h"
#include <cstdio>
static int re_call(CCombatSequence_ScorpionLungeAttack* sequence){ (void)sequence; return 0x1234; }
static void OnStart(CCombatSequence_ScorpionLungeAttack* sequence){ sequence->LastFrameUsed = re_call(sequence); }
int main(){ CCombatSequence_ScorpionLungeAttack obj; obj.LastFrameUsed=0; OnStart(&obj);
 if(obj.LastFrameUsed!=0x1234){ std::printf("00906fc0_TEST FAIL\n"); return 1;}
 std::printf("00906fc0_TEST PASS\n"); return 0;}