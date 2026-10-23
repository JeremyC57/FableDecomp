// Behaviour test for CTCNymph::FrameUpdate (0x0080FAD0): the flying/fade state machine.
#include <cstdio>
#include <cstring>

static char g_log[512];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }

struct Thing { bool dead; };
static int g_worldFrame;
static Thing* g_targetingTarget;
static bool g_hasTargeting;

struct Nymph {
    bool flying, canAttack, readyToAttack;
    int fadeCounter, totalFade;
    unsigned int lastTick;
    Thing* target;
    Thing* oldEffect;
    static bool Alive(Thing* t) { return t && !t->dead; }
    void Alpha(int a) { char b[32]; std::sprintf(b, "alpha%d", a); Log(b); }
    void FrameUpdate()
    {
        if (flying) {
            if (fadeCounter == 0) {
                float t = (float)(unsigned int)(g_worldFrame - lastTick) / 30.0f;
                if (t < 1.0f) {
                    char b[32]; std::sprintf(b, "spline%d", (int)((t * 0.75f + 0.25f) * 100.0f + 0.5f)); Log(b);
                } else Log("pick");
                if (Alive(target)) {
                    if (canAttack) readyToAttack = true;
                } else {
                    readyToAttack = false;
                    if (g_hasTargeting && g_targetingTarget) target = g_targetingTarget;
                }
            } else if (--fadeCounter != 0)
                Alpha((int)((float)fadeCounter / totalFade * 255.0f));
            else { Log("pick"); Log("hide"); }
        } else if (fadeCounter != 0) {
            --fadeCounter;
            Alpha((int)((1.0f - (float)fadeCounter / totalFade) * 255.0f));
            if (fadeCounter == 0 && Alive(oldEffect)) { Log("kill"); oldEffect = 0; }
        }
        Log("base");
    }
};

int main()
{
    Thing t1 = { false }, old = { false };
    Nymph n;
    std::memset(&n, 0, sizeof(n));
    n.flying = true; n.lastTick = 0; g_worldFrame = 15; g_hasTargeting = true; g_targetingTarget = &t1; g_log[0] = 0;
    n.FrameUpdate();
    if (std::strcmp(g_log, "spline63;base;") != 0 || n.target != &t1) { std::printf("0080fad0_TEST FAIL\n"); return 1; }
    n.canAttack = true; g_worldFrame = 40; g_log[0] = 0;
    n.FrameUpdate();
    if (std::strcmp(g_log, "pick;base;") != 0 || !n.readyToAttack) { std::printf("0080fad0_TEST FAIL\n"); return 1; }
    n.fadeCounter = 2; n.totalFade = 4; g_log[0] = 0;
    n.FrameUpdate();
    n.FrameUpdate();
    if (std::strcmp(g_log, "alpha63;base;pick;hide;base;") != 0) { std::printf("0080fad0_TEST FAIL\n"); return 1; }
    n.flying = false; n.fadeCounter = 1; n.oldEffect = &old; g_log[0] = 0;
    n.FrameUpdate();
    if (std::strcmp(g_log, "alpha255;kill;base;") != 0 || n.oldEffect != 0) { std::printf("0080fad0_TEST FAIL\n"); return 1; }
    std::printf("0080fad0_TEST PASS\n");
    return 0;
}
