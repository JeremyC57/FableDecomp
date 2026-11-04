// Behaviour test for NParticleEngine::CParticleEmitterManager::CreateInterpolatedEmitter (0x00AEB600): a model of the row --
// both emitters must exist and pass ShouldSpawn at the position; the source is cloned into the active list (which may
// refuse it), and only an inserted clone is interpolated toward the target and placed; otherwise the result is null.
#include <stdio.h>
struct Emitter { bool spawn; float interp; const Emitter* target; bool placed; };
struct Manager {
    Emitter pool[4]; int used; int capacity;
    Emitter* Clone(const Emitter& from) { pool[used] = from; pool[used].placed = false; return &pool[used++]; }
    Emitter* CreateInterpolatedEmitter(const Emitter* from, const Emitter* to, float interpolate)
    {
        Emitter* result = 0;
        if (from && from->spawn && to && to->spawn) {
            Emitter* clone = Clone(*from);
            if (used <= capacity) {
                clone->target = to; clone->interp = interpolate; clone->placed = true;
                result = clone;
            }
        }
        return result;
    }
};
int main()
{
    Emitter a = { true, 0, 0, false }, b = { true, 0, 0, false }, no = { false, 0, 0, false };
    Manager m; m.used = 0; m.capacity = 1;
    Emitter* r = m.CreateInterpolatedEmitter(&a, &b, 0.25f);
    int ok = r && r->placed && r->target == &b && r->interp == 0.25f;
    ok = ok && !m.CreateInterpolatedEmitter(&a, &b, 0.5f);            // list full: clone refused
    ok = ok && !m.CreateInterpolatedEmitter(&a, &no, 0.5f) && !m.CreateInterpolatedEmitter(0, &b, 0.5f);
    printf(ok ? "00aeb600_TEST PASS\n" : "00aeb600_TEST FAIL\n");
    return ok ? 0 : 1;
}