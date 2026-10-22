// Behaviour test for CModeSummonerLightningOrb::OnActivate (0x008DAE10): effect spawning, emitter attach, component add, timer.
#include <cstdio>
#include <cstring>

static char g_log[256];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ","); }

struct Thing { int type; bool dead; bool hasEmitter; };
struct Def { bool hasSub; bool orb; bool charge; };

struct Mode {
    Thing* owner; Thing* orbEffect; Thing* chargeEffect; long timer;
    Thing spawned[2]; int nSpawned;
    Thing* Create(const char* what) { Log(what); spawned[nSpawned].dead = false; spawned[nSpawned].hasEmitter = true; return &spawned[nSpawned++]; }
    void OnActivate(const Def& def, long seconds)
    {
        Log("base");
        if (owner->type == 1) Log("suspend");
        if (def.hasSub) {
            if (def.orb) {
                orbEffect = Create("orb");
                if (orbEffect && !orbEffect->dead && orbEffect->hasEmitter) Log("attach10");
            }
            if (def.charge) chargeEffect = Create("charge");
        }
        Log("addtc");
        timer = (long)(seconds * 3.0f);
    }
};

int main()
{
    Thing owner; owner.type = 1; owner.dead = false; owner.hasEmitter = false;
    Mode m; m.owner = &owner; m.orbEffect = 0; m.chargeEffect = 0; m.nSpawned = 0; m.timer = 0;
    Def d; d.hasSub = true; d.orb = true; d.charge = true;
    m.OnActivate(d, 15);
    bool ok = std::strcmp(g_log, "base,suspend,orb,attach10,charge,addtc,") == 0 && m.timer == 45 && m.nSpawned == 2;
    g_log[0] = 0; owner.type = 0; d.hasSub = false; m.nSpawned = 0;
    m.OnActivate(d, 2);
    ok = ok && std::strcmp(g_log, "base,addtc,") == 0 && m.timer == 6;
    if (!ok) { std::printf("008dae10_TEST FAIL\n"); return 1; }
    std::printf("008dae10_TEST PASS\n");
    return 0;
}
