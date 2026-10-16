#include <cstdio>
// Behaviour model of CTCDParticleEmitter::SetParticleCreationDisabled (0x006E0690).
struct Emitter { bool disabled, waitingToSleep, sleeping; };
struct Params { bool disabled; };
struct PE {
    Emitter* emitter; Params* params; bool waiting; bool disabled;
    void Set(bool d)
    {
        if (disabled != d) {
            disabled = d;
            if (emitter) { emitter->disabled = d; if (d) emitter->waitingToSleep = true; else { emitter->waitingToSleep = false; emitter->sleeping = false; } }
            else if (waiting) params->disabled = d;
        }
    }
};
int main()
{
    Emitter e = { false, false, true }; Params p = { false };
    PE pe = { &e, &p, true, false };
    pe.Set(true);
    if (!pe.disabled || !e.disabled || !e.waitingToSleep || !e.sleeping) { std::printf("006e0690_TEST FAIL\n"); return 1; }
    pe.Set(false);
    if (pe.disabled || e.disabled || e.waitingToSleep || e.sleeping) { std::printf("006e0690_TEST FAIL\n"); return 1; }
    pe.emitter = 0; pe.Set(true);
    if (!p.disabled) { std::printf("006e0690_TEST FAIL\n"); return 1; }
    p.disabled = false; pe.Set(true);
    if (p.disabled) { std::printf("006e0690_TEST FAIL\n"); return 1; }
    std::printf("006e0690_TEST PASS\n");
    return 0;
}