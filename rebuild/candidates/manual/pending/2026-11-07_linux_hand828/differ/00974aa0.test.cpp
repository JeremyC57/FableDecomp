// Behaviour test for CAIStateGroup_JackOfBladesStage2::Update (0x00974AA0): the facing angle is atan2(dx, dy) / 2pi wrapped into
// [0, 1); the attack loop starts its counter at 6 and picks by counter % 7: 0 counts a fireball round then falls into 1-2 (left
// fireball, FIREBALL_JACK_OF_BLADES), 3-4 combo fireball (FIREBALL_JACK_OF_BLADES2), 5 flash (then minions/idle once more than one
// round was fired), 6 idle. A model of the angle wrap and the schedule.
#include <cstdio>
#include <cmath>

static float WrapTurnFraction(float angle)
{
    if (angle < 0.0f || angle >= 1.0f) {
        angle = (float)std::fmod((double)angle, 1.0);
        if (angle < 0.0f) angle += 1.0f;
    }
    return angle;
}

static char Pick(long counter, long& rounds)
{
    switch (counter % 7) {
    case 0: ++rounds;
    case 1: case 2: return 'L';
    case 3: case 4: return 'C';
    case 5: return 'F';
    case 6: return 'I';
    }
    return '?';
}

int main()
{
    bool ok = WrapTurnFraction(-0.25f) == 0.75f && WrapTurnFraction(1.5f) == 0.5f && WrapTurnFraction(0.25f) == 0.25f;
    float a = WrapTurnFraction((float)std::atan2(0.0, -1.0) * 0.15915494f);
    ok = ok && a > 0.49f && a < 0.51f;
    long rounds = 0; char seq[16];
    for (long c = 6, i = 0; i < 9; ++c, ++i) seq[i] = Pick(c, rounds);
    seq[9] = 0;
    ok = ok && seq[0] == 'I' && seq[1] == 'L' && seq[3] == 'L' && seq[4] == 'C' && seq[6] == 'F' && seq[7] == 'I' && seq[8] == 'L' && rounds == 2;
    std::printf(ok ? "00974aa0_TEST PASS\n" : "00974aa0_TEST FAIL\n");
    return ok ? 0 : 1;
}
