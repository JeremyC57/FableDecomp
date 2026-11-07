// Behaviour test for 0x00931390 CAIStateGroup_MinionReactToCriminal::Update: models the row's decisions (herd the busy occupants
// when outside, hide inside when the house is quiet, lock the doors when the hero does not own the house, the speak timer
// (5 + rand(10) seconds), the cower line choice (4 in 5 cower, else grovel) and the loop exit on results 4 / 2).
#include <stdio.h>

enum { HERD = 1, HIDE = 2, INSIDE = 3, FLEE = 4, IDLE = 5 };
static int Branch(bool meInside, bool homeAlive, bool busyOccupants, bool heroInside, bool guardInside, bool hasOccupants,
                  bool heroAware)
{
    if (!meInside && homeAlive && busyOccupants) return HERD;
    if (!meInside && homeAlive && !busyOccupants) return HIDE;
    if (meInside && !heroInside && !guardInside && homeAlive && hasOccupants && !busyOccupants) return INSIDE;
    return heroAware ? FLEE : IDLE;
}
static bool LockDoors(bool heroOwnsHouse, bool busyOccupants) { return !heroOwnsHouse && !busyOccupants; }
static long NextSpeakFrame(long frame, float random10, long fps) { return (long)(frame + (random10 + 5.0f) * fps); }
static bool Cower(unsigned long roll) { return roll % 5 > 0; }
static bool KeepGoing(long result) { return result == 4 || result == 2; }

int main()
{
    int ok = 1;
    ok &= Branch(false, true, true, false, false, true, false) == HERD;
    ok &= Branch(false, true, false, false, false, true, false) == HIDE;
    ok &= Branch(true, true, false, false, false, true, false) == INSIDE;
    ok &= Branch(true, true, false, true, false, true, true) == FLEE;
    ok &= Branch(true, false, false, false, false, false, false) == IDLE;
    ok &= LockDoors(false, false) && !LockDoors(true, false) && !LockDoors(false, true);
    ok &= NextSpeakFrame(100, 2.5f, 30) == 325;
    ok &= Cower(6) && !Cower(10);
    ok &= KeepGoing(4) && KeepGoing(2) && !KeepGoing(1) && !KeepGoing(3);
    if (ok) printf("00931390_TEST PASS\n"); else printf("00931390_TEST FAIL\n");
    return ok ? 0 : 1;
}
