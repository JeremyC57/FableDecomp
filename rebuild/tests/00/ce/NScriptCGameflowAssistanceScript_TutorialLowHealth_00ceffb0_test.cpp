// Behaviour test for CGameflowAssistanceScript::Tutorial_LowHealth @ 0x00CEFFB0: a scripted health curve drives the
// critical/low warnings with independent cooldowns until the maximum number of warnings has been given.
#include <cstdio>
#include <cstring>
struct Def { long low, crit, lowCool, critCool, maxWarn; };
static Def def = { 50, 20, 3, 2, 4 };
static float health[64]; static int frame = 0; static int frames = 0;
static char sent[16][32]; static int nsent = 0;
struct Script {
    long given; bool disabled;
    bool Terminating() { return frame >= frames; }
    void Run()
    {
        long lowCooldown = 0, criticalCooldown = 0;
        while (given < def.maxWarn) {
            ++frame;
            if (Terminating()) return;
            if (disabled) break;
            float h = health[frame];
            if (h < def.crit) {
                if (criticalCooldown <= 0) { std::strcpy(sent[nsent++], "CRIT"); criticalCooldown = def.critCool; ++given; }
            } else if (h < def.low) {
                if (lowCooldown <= 0) { std::strcpy(sent[nsent++], "LOW"); lowCooldown = def.lowCool; ++given; }
            }
            --criticalCooldown; --lowCooldown;
        }
    }
};
int main()
{
    int i;
    for (i = 0; i < 64; ++i) health[i] = 100.0f;
    health[2] = 40.0f; health[3] = 40.0f; health[4] = 10.0f; health[5] = 10.0f; health[6] = 10.0f; health[7] = 45.0f;
    frames = 60;
    Script s; s.given = 0; s.disabled = false;
    s.Run();
    // frame 2 LOW (low cd 3), 3 low on cooldown, 4 CRIT (crit cd 2), 5 crit on cooldown, 6 CRIT again, 7 LOW (cd expired) -> 4 given
    bool ok = nsent == 4 && !std::strcmp(sent[0], "LOW") && !std::strcmp(sent[1], "CRIT") && !std::strcmp(sent[2], "CRIT")
        && !std::strcmp(sent[3], "LOW") && s.given == 4 && frame == 7;
    if (!ok) { std::printf("00ceffb0_TEST FAIL\n"); return 1; }
    std::printf("00ceffb0_TEST PASS\n");
    return 0;
}