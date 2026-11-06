// Behaviour test for CTCSummonSpell::CreateMinion (0x0079ACC0): the minion faces along the given direction or away from the caster
// (normalised), gets the on-die effect timers (ConvertSecondsToWF(TimeOfSummon[level]) - CurTimer and ConvertSecondsToWF(FadeOutTime)),
// the glowing eyes only on the dummy points it has, alpha 0 and the summon tint on itself and every carried thing.
#include <cstdio>
#include <cstring>
#include <cmath>
static long ConvertSecondsToWF(float s) { return (long)(30 * s); }
struct Minion {
    float vel[3]; long timer, fadeOut, effect; int eyes, tinted, alpha; char log[256];
    void Ev(const char* e) { std::strcat(log, e); std::strcat(log, ";"); }
};
static void CreateMinion(Minion& m, const float* teleport, const float* caster, const float* direction, bool hasEffects,
                         bool leftEye, bool rightEye, int carried, float timeOfSummon, long curTimer, float fadeOutTime)
{
    std::memset(&m, 0, sizeof(m)); m.alpha = 255;
    if (direction) { m.vel[0] = direction[0]; m.vel[1] = direction[1]; m.vel[2] = direction[2]; }
    else {
        for (int i = 0; i < 3; ++i) m.vel[i] = teleport[i] - caster[i];
        float f = 1.0f / (float)std::sqrt(m.vel[0] * m.vel[0] + m.vel[1] * m.vel[1] + m.vel[2] * m.vel[2]);
        for (int i = 0; i < 3; ++i) m.vel[i] *= f;
    }
    m.Ev("FACTION_HERO"); m.Ev("SetNewBrain");
    if (hasEffects) {
        m.Ev("CREATURE_APPEARING"); m.Ev("CREATURE_ALIVE_EFFECT");
        m.effect = 7; m.timer = ConvertSecondsToWF(timeOfSummon) - curTimer; m.fadeOut = ConvertSecondsToWF(fadeOutTime);
        if (leftEye) ++m.eyes;
        if (rightEye) ++m.eyes;
    }
    m.tinted = 1 + carried; m.alpha = 0;
}
int main()
{
    float tp[3] = { 3, 4, 0 }, caster[3] = { 0, 0, 0 }, dir[3] = { 0, 1, 0 };
    Minion a; CreateMinion(a, tp, caster, 0, true, true, false, 2, 10.0f, 20, 2.0f);
    bool ok = std::fabs(a.vel[0] - 0.6f) < 1e-5f && std::fabs(a.vel[1] - 0.8f) < 1e-5f && a.vel[2] == 0.0f
        && a.timer == 280 && a.fadeOut == 60 && a.effect == 7 && a.eyes == 1 && a.tinted == 3 && a.alpha == 0
        && std::strcmp(a.log, "FACTION_HERO;SetNewBrain;CREATURE_APPEARING;CREATURE_ALIVE_EFFECT;") == 0;
    Minion b; CreateMinion(b, tp, caster, dir, false, true, true, 0, 10.0f, 0, 1.0f);
    ok = ok && b.vel[0] == 0.0f && b.vel[1] == 1.0f && b.timer == 0 && b.eyes == 0 && b.tinted == 1
        && std::strcmp(b.log, "FACTION_HERO;SetNewBrain;") == 0;
    std::printf(ok ? "0079acc0_TEST PASS\n" : "0079acc0_TEST FAIL\n");
    return ok ? 0 : 1;
}