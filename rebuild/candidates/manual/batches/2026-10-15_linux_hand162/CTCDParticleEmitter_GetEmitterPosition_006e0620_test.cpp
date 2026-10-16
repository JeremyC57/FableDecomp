#include <cstdio>
// Behaviour model of CTCDParticleEmitter::GetEmitterPosition (0x006E0620).
struct V { float X, Y, Z; };
struct Emitter { float m[12]; V GetPosition() const { V v = { m[9], m[10], m[11] }; return v; } };
struct PE {
    Emitter* emitter; V parentPos;
    V GetEmitterPosition() const { if (emitter) return emitter->GetPosition(); return parentPos; }
};
int main()
{
    Emitter e; for (int i = 0; i < 12; ++i) e.m[i] = (float)i;
    PE pe; pe.emitter = &e; pe.parentPos.X = 7; pe.parentPos.Y = 8; pe.parentPos.Z = 9;
    V a = pe.GetEmitterPosition();
    if (a.X != 9 || a.Y != 10 || a.Z != 11) { std::printf("006e0620_TEST FAIL\n"); return 1; }
    pe.emitter = 0; V b = pe.GetEmitterPosition();
    if (b.X != 7 || b.Z != 9) { std::printf("006e0620_TEST FAIL\n"); return 1; }
    std::printf("006e0620_TEST PASS\n");
    return 0;
}
