#include <cstdio>
// Behaviour model of CTCDParticleEmitter::SetEmitterPosition (0x006E0500).
struct V { float X, Y, Z; };
struct Interp { V Start, End; Interp(const V& a, const V& b) : Start(a), End(b) {} };
struct Emitter { V start, end; int calls; void SetPosition(const Interp& i) { start = i.Start; end = i.End; ++calls; } };
struct Phys { V pos; int resets; void ResetPosition(const V& p) { pos = p; ++resets; } };
struct PE {
    Emitter* emitter; Phys* phys;
    void SetEmitterPosition(const V& p) { if (emitter) emitter->SetPosition(Interp(p, p)); else phys->ResetPosition(p); }
};
int main()
{
    V p = { 1, 2, 3 };
    Emitter e; e.calls = 0; Phys ph; ph.resets = 0;
    PE pe = { &e, &ph };
    pe.SetEmitterPosition(p);
    if (e.calls != 1 || e.end.Z != 3 || e.start.X != 1 || ph.resets) { std::printf("006e0500_TEST FAIL\n"); return 1; }
    pe.emitter = 0; pe.SetEmitterPosition(p);
    if (ph.resets != 1 || ph.pos.Y != 2) { std::printf("006e0500_TEST FAIL\n"); return 1; }
    std::printf("006e0500_TEST PASS\n");
    return 0;
}
