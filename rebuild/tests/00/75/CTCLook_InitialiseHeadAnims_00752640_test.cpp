// Behaviour test for CTCLook::InitialiseHeadAnims (0x00752640): live creature path vs appearance-def path, each head
// animation (V then H) replaced only when the lookup finds it.
#include <cstdio>
#include <cstring>

static char g_log[256];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }

struct Creature { bool dead; bool hasV; bool hasH; };
struct Look {
    Creature* creature;
    bool defHasV, defHasH;
    int animV, animH;
    void InitialiseHeadAnims()
    {
        if (creature && !creature->dead) {
            if (creature->hasV) { animV = 1; Log("cV"); }
            if (creature->hasH) { animH = 1; Log("cH"); }
        } else {
            if (defHasV) { animV = 2; Log("dV"); }
            if (defHasH) { animH = 2; Log("dH"); }
        }
    }
};

int main()
{
    Creature c = { false, true, false };
    Look l = { &c, true, true, 0, 0 };
    g_log[0] = 0; l.InitialiseHeadAnims();
    if (std::strcmp(g_log, "cV;") != 0 || l.animV != 1 || l.animH != 0) { std::printf("00752640_TEST FAIL\n"); return 1; }
    c.dead = true; l.animV = l.animH = 0; g_log[0] = 0; l.InitialiseHeadAnims();
    if (std::strcmp(g_log, "dV;dH;") != 0 || l.animV != 2 || l.animH != 2) { std::printf("00752640_TEST FAIL\n"); return 1; }
    l.creature = 0; l.defHasV = false; l.animV = l.animH = 0; g_log[0] = 0; l.InitialiseHeadAnims();
    if (std::strcmp(g_log, "dH;") != 0 || l.animV != 0) { std::printf("00752640_TEST FAIL\n"); return 1; }
    std::printf("00752640_TEST PASS\n");
    return 0;
}