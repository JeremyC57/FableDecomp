// Behaviour test for 0x0090D030 CAIStateGroup_MinionGossip::Update: models the row's decisions (answer a gossip request or post
// one and wait 4 seconds, greet only as initiator or on a 20% roll, near/far greeting by a 5-unit distance, the interaction index
// sex*3 + other sex with the money swap and the same-home veto, and the crouch-to-child branch for an adult talking to a child).
#include <stdio.h>
#include <math.h>

static unsigned long g_rolls[16]; static int g_roll;
static unsigned long Random(unsigned long range) { return g_rolls[g_roll++] % range; }

static bool ShouldGreet(bool initiator) { return initiator || Random(100) < 20; }
static int Greeting(float distance) { return distance < 5.0f ? (int)Random(2) : 2 + (int)Random(4); }
static int Interaction(long mySex, long otherSex, bool sameHome)
{
    int interaction = (int)(mySex * 3 + otherSex);
    if (mySex != 0 && otherSex != 0 && Random(7) == 0) interaction = 100;   // VILLAGER_INTERACTION_EXCHANGE_MONEY
    if (sameHome && Random(2) == 0) interaction = -1;                      // no interaction
    return interaction;
}
static bool CrouchToChild(long mySex, long otherSex) { return otherSex == 0 && mySex != 0; }
static bool TimedOut(long frame, long start, long fps) { return frame > start + fps * 4; }

int main()
{
    int ok = 1;
    g_roll = 0; g_rolls[0] = 19; ok &= ShouldGreet(false);
    g_roll = 0; g_rolls[0] = 20; ok &= !ShouldGreet(false) && ShouldGreet(true);
    g_roll = 0; g_rolls[0] = 1; ok &= Greeting(4.0f) == 1;
    g_roll = 0; g_rolls[0] = 3; ok &= Greeting(6.0f) == 5;
    g_roll = 0; g_rolls[0] = 1; ok &= Interaction(1, 2, false) == 5;
    g_roll = 0; g_rolls[0] = 7; ok &= Interaction(2, 1, false) == 100;
    g_roll = 0; g_rolls[0] = 1; g_rolls[1] = 4; ok &= Interaction(1, 1, true) == -1;
    g_roll = 0; ok &= Interaction(0, 2, false) == 2;
    ok &= CrouchToChild(1, 0) && !CrouchToChild(0, 0) && !CrouchToChild(1, 1);
    ok &= !TimedOut(120, 0, 30) && TimedOut(121, 0, 30);
    if (ok) printf("0090d030_TEST PASS\n"); else printf("0090d030_TEST FAIL\n");
    return ok ? 0 : 1;
}
