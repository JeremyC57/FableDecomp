#include <cstdio>
#include <cmath>
// behaviour test: CThingPlayerCreature::GetMovementBandTypeFromJoytickMagnitude -- 0 stationary, 1 slow, 2 walking, 3 running,
// 4 jogging from the def's joystick bands; stationary when the hero cannot run; the run option and sneaking change the bands
struct Def { float Stationary, Slow, Walking, Jogging; };
struct Hero {
    Def def; bool hasStealth, sneaking, hasSpecial, running; float maxRun; bool runOption;
    int Band(float m) const
    {
        bool sneak = hasStealth && sneaking;
        if (std::fabs(maxRun) < 0.01f) return 0;
        if (runOption) {
            if (m < def.Stationary) return 0;
            if (m < def.Walking) return 2;
            if (hasSpecial && running) return 3;
            return 4;
        }
        if (m < def.Slow && !sneak) return 1;
        else if (m < def.Walking) return 2;
        else if (m < 0.97f && sneak) return 2;
        else if (m < def.Jogging) return 4;
        else if (m < def.Stationary) return 0;
        else if (m < def.Slow) return 1;
        else if (m < def.Walking) return 2;
        else if (m < def.Jogging) return 4;
        else return 3;
    }
};
int main()
{
    Hero h; h.def.Stationary = 0.1f; h.def.Slow = 0.3f; h.def.Walking = 0.6f; h.def.Jogging = 0.9f;
    h.hasStealth = true; h.sneaking = false; h.hasSpecial = true; h.running = false; h.maxRun = 5.0f; h.runOption = false;
    bool ok = h.Band(0.2f) == 1 && h.Band(0.5f) == 2 && h.Band(0.8f) == 4 && h.Band(0.95f) == 3;
    h.sneaking = true;
    ok = ok && h.Band(0.2f) == 2 && h.Band(0.8f) == 2 && h.Band(0.98f) == 3;
    h.runOption = true;
    ok = ok && h.Band(0.05f) == 0 && h.Band(0.5f) == 2 && h.Band(0.8f) == 4;
    h.running = true;
    ok = ok && h.Band(0.8f) == 3;
    h.maxRun = 0.001f;
    ok = ok && h.Band(0.8f) == 0;
    std::printf(ok ? "006ab430_TEST PASS\n" : "006ab430_TEST FAIL\n");
    return ok ? 0 : 1;
}