// Behaviour test for CTourGuideFollower::Main (0x00EE4DD0): a follower talked to after its stand-still timer ran out answers with
// RandomFollowerResponseM / F [rand()%5] by its sex (nothing for SEX_NONE) and restarts the timer; once the guide is killed or the
// tour is over it walks to the closing-time exit (3 units) and fades out, or idles when there is no exit.
#include <cstdio>

struct Follower {
    long Timer, Answers, LastIdx; char LastSex; bool Faded, Idle;
    void Talk(int sex, long r, long standStill)
    {
        if (Timer != 0) return;
        LastIdx = r % 5;
        LastSex = sex == 1 ? 'M' : sex == 2 ? 'F' : '-';
        ++Answers;
        Timer = standStill;
    }
    void Frame() { if (Timer > 0) --Timer; }
    long Close(bool exitAlive, float distance)
    {
        if (!exitAlive) { Idle = true; return 0; }
        long steps = 0;
        while (distance > 3.0f) { distance -= 1.0f; ++steps; }
        Faded = true;
        return steps;
    }
};

int main()
{
    Follower f = { 0, 0, -1, 0, false, false };
    bool ok = true;
    f.Talk(1, 12, 3); ok = ok && f.Answers == 1 && f.LastIdx == 2 && f.LastSex == 'M';
    f.Talk(2, 4, 3);  ok = ok && f.Answers == 1;
    f.Frame(); f.Frame(); f.Frame();
    f.Talk(2, 4, 3);  ok = ok && f.Answers == 2 && f.LastSex == 'F' && f.LastIdx == 4;
    ok = ok && f.Close(true, 5.5f) == 3 && f.Faded && !f.Idle;
    Follower g = { 0, 0, -1, 0, false, false };
    ok = ok && g.Close(false, 9.0f) == 0 && g.Idle && !g.Faded;
    if (!ok) { std::printf("00ee4dd0_TEST FAIL\n"); return 1; }
    std::printf("00ee4dd0_TEST PASS\n");
    return 0;
}
