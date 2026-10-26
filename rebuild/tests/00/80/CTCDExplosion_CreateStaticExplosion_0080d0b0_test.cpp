// Behaviour test for CTCDExplosion::CreateStaticExplosion (0x0080D0B0): one emitter per damage radius along the line plus
// one, placed from the start along the normalised direction; dead/missing emitters are not remembered; no emitter def
// means no emitters; the camera shake goes to the middle of the line when asked.
#include <cstdio>
#include <cmath>
#include <vector>
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    float Magnitude() const { return (float)std::sqrt(X * X + Y * Y + Z * Z); }
    float MagnitudeSquared() const { return Z * Z + Y * Y + X * X; }
    void Normalise() { float m = Magnitude(); X /= m; Y /= m; Z /= m; }
};
static C3DVector operator+(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X + b.X, a.Y + b.Y, a.Z + b.Z); }
static C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
static C3DVector operator*(const C3DVector& a, float s) { return C3DVector(a.X * s, a.Y * s, a.Z * s); }
class CThing { public: bool Dead; };
static std::vector<C3DVector> g_created;
static CThing g_things[16];
static int g_failEvery = 0;
static CThing* CreateEmitter(const C3DVector& pos)
{
    g_created.push_back(pos);
    int n = (int)g_created.size();
    if (g_failEvery && n % g_failEvery == 0) return 0;
    g_things[n].Dead = false;
    return &g_things[n];
}
static int g_shakes = 0;
static C3DVector g_shakeAt;
class CExplosionDef { public: long EmitterDefIndex; bool ShakeCamera; };
class CTCDExplosion { public:
    float DamageRadius;
    C3DVector StartPos, EndPos;
    CExplosionDef* PExplosionDef;
    bool CreatedEmitter;
    std::vector<CThing*> Emitters;
    void CreateStaticExplosion()
    {
        float radius = DamageRadius;
        if (PExplosionDef->EmitterDefIndex >= 0) {
            long count = (long)((EndPos - StartPos).Magnitude() / radius + 1.0f);
            C3DVector direction = EndPos - StartPos;
            if (direction.MagnitudeSquared() > 0.0001f * 0.0001f)
                direction.Normalise();
            for (long i = 0; i != count; ++i) {
                C3DVector pos = StartPos + direction * (float)i * radius;
                CThing* emitter = CreateEmitter(pos);
                if (emitter && !emitter->Dead)
                    Emitters.push_back(emitter);
            }
            CreatedEmitter = true;
        }
        if (PExplosionDef->ShakeCamera) {
            g_shakeAt = (StartPos + EndPos) * 0.5f;
            ++g_shakes;
        }
    }
};
static int fail() { std::printf("0080d0b0_TEST FAIL\n"); return 1; }
int main()
{
    CExplosionDef def = { 3, true };
    CTCDExplosion e;
    e.DamageRadius = 2.0f; e.StartPos = C3DVector(0, 0, 0); e.EndPos = C3DVector(10, 0, 0);
    e.PExplosionDef = &def; e.CreatedEmitter = false;
    e.CreateStaticExplosion();
    if (g_created.size() != 6 || e.Emitters.size() != 6 || !e.CreatedEmitter) return fail();
    if (g_created[5].X != 10.0f || g_created[1].X != 2.0f) return fail();
    if (g_shakes != 1 || g_shakeAt.X != 5.0f) return fail();
    g_created.clear(); g_failEvery = 2;
    CTCDExplosion f;
    f.DamageRadius = 4.0f; f.StartPos = C3DVector(1, 1, 1); f.EndPos = C3DVector(1, 1, 1);
    def.ShakeCamera = false; f.PExplosionDef = &def; f.CreatedEmitter = false;
    f.CreateStaticExplosion();
    if (g_created.size() != 1 || f.Emitters.size() != 1 || g_shakes != 1) return fail();
    def.EmitterDefIndex = -1;
    CTCDExplosion g = f; g.Emitters.clear(); g.CreatedEmitter = false; g_created.clear();
    g.CreateStaticExplosion();
    if (!g_created.empty() || g.CreatedEmitter) return fail();
    std::printf("0080d0b0_TEST PASS\n");
    return 0;
}