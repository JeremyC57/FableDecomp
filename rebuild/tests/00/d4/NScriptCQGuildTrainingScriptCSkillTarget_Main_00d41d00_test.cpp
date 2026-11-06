// Behaviour test for CQ_GuildTrainingScript::CSkillTarget::Main (0x00D41D00): the archery dummy. Each slide step is the start
// position plus the per-frame delta (end - start) / Speed times the step index, keeping the start Z; a static-mode hit at or
// below 0.5 costs a point and one above 0.5 scores one and ends the wait; a moving hit inside the ring scores DummyWorth
// times 1 / 2 / 3 / 4 for damage below 0.25 / 0.5 / 0.75 / otherwise.
#include <cstdio>

struct V3 { float X, Y, Z; };
static V3 SlideStep(V3 from, V3 to, long Speed, long i)
{
    V3 d = { (to.X - from.X) / Speed, (to.Y - from.Y) / Speed, 0.0f };
    V3 step = { d.X * (float)i, d.Y * (float)i, 0.0f };
    V3 r = { from.X + step.X, from.Y + step.Y, from.Z };
    return r;
}
static long Score(float damage, long worth)
{
    if (damage < 0.25) return worth;
    else if (damage < 0.5) return worth * 2;
    else if (damage < 0.75) return worth * 3;
    return worth * 4;
}
static long StaticWait(const float* hits, int n, long score)
{
    for (int k = 0; k < n; ++k) {
        if (hits[k] > 0.5) return score + 1;
        --score;
    }
    return score;
}

int main()
{
    bool ok = true;
    V3 a = { 0.0f, 0.0f, 7.0f }, b = { 10.0f, -20.0f, 99.0f };
    V3 p = SlideStep(a, b, 10, 0); ok = ok && p.X == 0.0f && p.Y == 0.0f && p.Z == 7.0f;
    p = SlideStep(a, b, 10, 5);    ok = ok && p.X == 5.0f && p.Y == -10.0f && p.Z == 7.0f;
    p = SlideStep(a, b, 10, 10);   ok = ok && p.X == 10.0f && p.Y == -20.0f && p.Z == 7.0f;
    ok = ok && Score(0.25f, 3) == 6 && Score(0.1f, 3) == 3 && Score(0.5f, 3) == 9 && Score(0.75f, 3) == 12 && Score(1.0f, 3) == 12;
    const float hits[] = { 0.25f, 0.5f, 0.75f };
    ok = ok && StaticWait(hits, 3, 10) == 9;
    if (!ok) { std::printf("00d41d00_TEST FAIL\n"); return 1; }
    std::printf("00d41d00_TEST PASS\n");
    return 0;
}