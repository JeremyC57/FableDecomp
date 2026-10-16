#include <cstdio>
// Behaviour model of CTCDParticleEmitter::SetEmitterOrientation (0x006E0480).
struct V { float X, Y, Z; };
struct RHS { V Up, Forward; };
struct Interp { RHS Start, End; Interp(const RHS& a, const RHS& b) : Start(a), End(b) {} };
struct Emitter { RHS start, end; int calls; void SetOrientation(const Interp& i) { start = i.Start; end = i.End; ++calls; } };
struct Params { RHS RHSet; };
struct PE {
    Emitter* emitter; Params* params;
    void SetEmitterOrientation(const RHS& r) { if (emitter) emitter->SetOrientation(Interp(r, r)); else params->RHSet = r; }
};
int main()
{
    RHS r = { { 0, 0, 1 }, { 0, 1, 0 } };
    Emitter e; e.calls = 0; Params p; p.RHSet.Up.Z = 0;
    PE pe = { &e, &p };
    pe.SetEmitterOrientation(r);
    if (e.calls != 1 || e.end.Forward.Y != 1 || p.RHSet.Up.Z != 0) { std::printf("006e0480_TEST FAIL\n"); return 1; }
    pe.emitter = 0; pe.SetEmitterOrientation(r);
    if (p.RHSet.Up.Z != 1 || e.calls != 1) { std::printf("006e0480_TEST FAIL\n"); return 1; }
    std::printf("006e0480_TEST PASS\n");
    return 0;
}
