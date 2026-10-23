// Behaviour test for CTCNymph::StopFlyingAround (0x0080F870): only while flying; kills the old effect, rotates the current
// effect into the old slot, spawns the HOVER effect, and if it is alive attaches it, stops flying, starts the fade-in
// counter (def fade-in time when -1), shows the owner and kills the old effect at once when the counter is zero.
#include <cstdio>
#include <cstring>

static char g_log[512];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }

struct Thing { bool dead; int id; };
static Thing g_spawn;
static int g_fps = 30;

struct Nymph {
    bool flying, beenHit;
    Thing* effect;
    Thing* oldEffect;
    int fadeCounter, totalFade;
    float fadeInTime;
    static bool Alive(Thing* t) { return t && !t->dead; }
    void Stop(int fadeTime)
    {
        if (!flying) return;
        if (Alive(oldEffect)) { char b[32]; std::sprintf(b, "kill%d", oldEffect->id); Log(b); }
        oldEffect = effect;
        effect = &g_spawn;
        Log("create");
        if (Alive(effect)) {
            Log("attach");
            flying = false;
            beenHit = false;
            if (fadeTime == -1) fadeTime = (int)(g_fps * fadeInTime);
            fadeCounter = totalFade = fadeTime;
            Log("show");
            if (fadeCounter == 0 && Alive(oldEffect)) {
                char b[32]; std::sprintf(b, "kill%d", oldEffect->id); Log(b);
                oldEffect = 0;
            }
        }
    }
};

int main()
{
    Thing a = { false, 1 }, b = { false, 2 };
    Nymph n;
    n.flying = true; n.beenHit = true; n.effect = &a; n.oldEffect = &b; n.fadeInTime = 0.5f;
    g_spawn.dead = false; g_spawn.id = 3; g_log[0] = 0;
    n.Stop(-1);
    if (std::strcmp(g_log, "kill2;create;attach;show;") != 0 || n.fadeCounter != 15 || n.totalFade != 15 ||
        n.oldEffect != &a || n.flying || n.beenHit) { std::printf("0080f870_TEST FAIL\n"); return 1; }
    n.flying = true; n.effect = &a; n.oldEffect = 0; g_log[0] = 0;
    n.Stop(0);
    if (std::strcmp(g_log, "create;attach;show;kill1;") != 0 || n.oldEffect != 0) { std::printf("0080f870_TEST FAIL\n"); return 1; }
    n.flying = false; g_log[0] = 0;
    n.Stop(0);
    if (g_log[0] != 0) { std::printf("0080f870_TEST FAIL\n"); return 1; }
    std::printf("0080f870_TEST PASS\n");
    return 0;
}