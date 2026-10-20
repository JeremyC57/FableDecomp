// Behaviour test for CTCDParticleEmitter::CreateParticleWaitingFor (0x006E14E0).
#include <cstdio>

struct Params { bool interp, disabled, updSet, always; long start, end; float t; };
struct Emitter { bool disabled, waitingToSleep, always; int updates; long id; };
static Emitter g_e; static bool g_fail = false; static int g_adopted = 0;
static Emitter* Create(long id) { if (g_fail) return 0; g_e.id = id; return &g_e; }
struct Comp {
    long ParticleTypeID; Params* params; bool waiting, attached, alwaysUpdate, attachedAlive, bulletTime;
    void CreateParticleWaitingFor()
    {
        Params* p = params;
        Emitter* e = p->interp ? Create(p->start * 100 + p->end) : Create(ParticleTypeID);
        if (e) {
            if (params->updSet) ++e->updates;
            e->disabled = params->disabled;
            if (params->disabled) e->waitingToSleep = true;
            if (attached && attachedAlive && !bulletTime) { e->always = true; alwaysUpdate = true; }
            else e->always = params->always;
            ++g_adopted;
            waiting = false;
            delete params; params = 0;
        }
    }
};

int main()
{
    Comp c = { 5, new Params, true, false, false, false, false };
    Params init = { false, true, true, true, 1, 2, 0.5f }; *c.params = init;
    c.CreateParticleWaitingFor();
    if (g_e.id != 5 || !g_e.disabled || !g_e.waitingToSleep || !g_e.always || g_e.updates != 1 || c.waiting || c.params || g_adopted != 1) {
        std::printf("006e14e0_TEST FAIL\n"); return 1;
    }
    Params p2 = { true, false, false, false, 3, 4, 0.5f };
    c.params = new Params; *c.params = p2; c.waiting = true; c.attached = true; c.attachedAlive = true;
    g_e.always = false; c.CreateParticleWaitingFor();
    if (g_e.id != 304 || !g_e.always || !c.alwaysUpdate) { std::printf("006e14e0_TEST FAIL\n"); return 1; }
    c.params = new Params; *c.params = p2; g_fail = true; c.CreateParticleWaitingFor();
    if (!c.params || g_adopted != 2) { std::printf("006e14e0_TEST FAIL\n"); return 1; }
    delete c.params;
    std::printf("006e14e0_TEST PASS\n");
    return 0;
}
