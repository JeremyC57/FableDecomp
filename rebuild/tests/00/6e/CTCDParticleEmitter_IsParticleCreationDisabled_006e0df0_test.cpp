#include <cstdio>
// Behaviour model of CTCDParticleEmitter::IsParticleCreationDisabled (0x006E0DF0).
struct Mode { int type; };
struct Thing { bool inLimbo; Mode* mode; bool busy; };
struct PE {
    bool asleep; Thing* attached;
    bool IsDisabled() const
    {
        if (asleep) return true;
        if (attached->inLimbo) return true;
        if (attached->mode) { if (attached->mode->type == 30 && !attached->busy) return true; }
        return false;
    }
};
int main()
{
    Mode m = { 30 }; Thing t = { false, &m, false }; PE pe = { false, &t };
    if (!pe.IsDisabled()) { std::printf("006e0df0_TEST FAIL\n"); return 1; }
    t.busy = true;
    if (pe.IsDisabled()) { std::printf("006e0df0_TEST FAIL\n"); return 1; }
    t.inLimbo = true;
    if (!pe.IsDisabled()) { std::printf("006e0df0_TEST FAIL\n"); return 1; }
    t.inLimbo = false; m.type = 3; t.busy = false;
    if (pe.IsDisabled()) { std::printf("006e0df0_TEST FAIL\n"); return 1; }
    pe.asleep = true;
    if (!pe.IsDisabled()) { std::printf("006e0df0_TEST FAIL\n"); return 1; }
    std::printf("006e0df0_TEST PASS\n");
    return 0;
}