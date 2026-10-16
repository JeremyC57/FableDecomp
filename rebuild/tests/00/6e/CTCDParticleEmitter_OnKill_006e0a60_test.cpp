#include <cstdio>
// Behaviour model of CTCDParticleEmitter::OnKill (0x006E0A60).
struct Emitter { bool killed; };
struct Dependents { void* added; };
struct Thing { bool dead; Dependents* dep; };
struct PE {
    void* parent; Emitter* emitter; Thing* attached;
    void OnKill()
    {
        if (emitter) emitter->killed = true;
        if (attached && !attached->dead) { if (attached->dep) attached->dep->added = parent; }
    }
};
int main()
{
    int owner; Emitter e = { false }; Dependents d = { 0 }; Thing t = { false, &d };
    PE pe = { &owner, &e, &t };
    pe.OnKill();
    if (!e.killed || d.added != &owner) { std::printf("006e0a60_TEST FAIL\n"); return 1; }
    d.added = 0; t.dead = true; pe.emitter = 0; pe.OnKill();
    if (d.added) { std::printf("006e0a60_TEST FAIL\n"); return 1; }
    std::printf("006e0a60_TEST PASS\n");
    return 0;
}