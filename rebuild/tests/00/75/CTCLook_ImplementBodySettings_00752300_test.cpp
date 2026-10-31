// behaviour test for 0x00752300 CTCLook::ImplementBodySettings()
#include <cstdio>
#include <cmath>
struct Look { float maxTurn, combatMaxTurn, target; bool creature, dead, inCombat, isHeroThing, heroAlive, heroFlag, moved, incapacitated; float facing; };
static float AngleDiff(float a, float b) { float d = b - a; while (d > 0.5f) d -= 1.0f; while (d < -0.5f) d += 1.0f; return d; }
static void Implement(Look& l) {
    float speed = l.maxTurn;
    if (l.creature && !l.dead && l.inCombat) speed = l.combatMaxTurn;
    float step = speed * (1.0f / 360.0f);
    float d = AngleDiff(l.facing, l.target);
    if (d < -step) d = -step; else if (d > step) d = step;
    float a = l.facing + d;
    if (a < 0.0f || a >= 1.0f) { a = (float)std::fmod((double)a, 1.0); if (a < 0.0f) a += 1.0f; }
    if (l.isHeroThing && l.heroAlive && l.heroFlag && l.moved) return;
    if (!l.incapacitated) l.facing = a;
}
int main() {
    Look l = {36.0f, 72.0f, 0.5f, true, false, false, false, false, false, false, false, 0.0f};
    Implement(l);
    if (std::fabs(l.facing - 0.1f) > 1e-5f) { std::printf("00752300_TEST FAIL\n"); return 1; }
    l.facing = 0.0f; l.inCombat = true; Implement(l);
    if (std::fabs(l.facing - 0.2f) > 1e-5f) { std::printf("00752300_TEST FAIL\n"); return 1; }
    l.facing = 0.95f; l.target = 0.05f; l.inCombat = false; Implement(l);
    if (std::fabs(l.facing - 0.05f) > 1e-5f) { std::printf("00752300_TEST FAIL\n"); return 1; }
    l.facing = 0.0f; l.target = 0.5f; l.isHeroThing = l.heroAlive = l.heroFlag = l.moved = true; Implement(l);
    if (l.facing != 0.0f) { std::printf("00752300_TEST FAIL\n"); return 1; }
    l.moved = false; l.incapacitated = true; Implement(l);
    if (l.facing != 0.0f) { std::printf("00752300_TEST FAIL\n"); return 1; }
    std::printf("00752300_TEST PASS\n");
    return 0;
}