// Behaviour test for NCameraModes::CScriptModuleOpenChest::OnInitLeaf (0x00853670): the row body verbatim over models of its callees.
// Kills: either alive test removed / inverted (dead or missing chest / creature leave the module untouched), Z not flattened, the
// magnitude / normalisation / perpendicular wrong, the radius factor, height, the seed step not written back, the random side, the
// 1.6 / 1.5 end values or the per-frame division.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
static void check(bool v, int line) { if (!v) { printf("FAIL line %d\n", line); exit(1); } }
#define CHECK(c) check((c), __LINE__)
static bool close(float a, float b) { return fabs(a - b) <= 1e-5f * (1.0f + fabs(b)); }
class C3DVector { public: float X, Y, Z; C3DVector() {} C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    const C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetAccurateMagnitude() const { float magnitude = X * X + Y * Y + Z * Z; return (float)sqrt(magnitude); }
    const C3DVector& operator/=(float v) { float reciprocal = (1.0f / v); X *= reciprocal; Y *= reciprocal; Z *= reciprocal; return *this; }
    C3DVector CrossWithUp() const { return C3DVector(Y, -X, 0.0f); } };
class CCharString { public: int id; };
static int get_pitem_calls;
class CThing { public: C3DVector Pos; bool Dead; void* App; const C3DVector& GetPos() const { return Pos; } bool IsDead() const { return Dead; } };
template<class T> class CIntelligentPointer { public: CThing* Item;
    CThing* GetPItem() const { ++get_pitem_calls; return Item; }
    const CThing* operator->() const { return GetPItem(); } };
template<class T> bool operator!=(const CIntelligentPointer<T>& p, int) { return p.GetPItem() != 0; }
template<class T> bool IS_THING_ALIVE(const T& thing) { if (thing != 0) { if (!thing->IsDead()) return true; } return false; }
static long GFROR13(unsigned long v) { return (long)((v >> 13) | (v << 19)); }
static unsigned long GFRandom(unsigned long range, unsigned long* seed)
{ *seed *= 0x24a1; *seed += 0x24df; *seed = GFROR13(*seed); if (range) return *seed % range; return 0; }
namespace NCameraModes {
class CBase { public: unsigned long Seed; unsigned long* GetPWorldSeed() const { return const_cast<unsigned long*>(&Seed); } };
}
static unsigned long ref_rand(unsigned long s) { s = s * 0x24a1 + 0x24df; return (s >> 13) | (s << 19); }
namespace NCameraModes {
class CScriptModuleOpenChest { public:
    CBase* POwner; long WFDuration;
    CIntelligentPointer<const CThing> PCreature, PChest;
    C3DVector Origin; float Theta, Rad, Height, DeltaTheta, DeltaRad, DeltaHeight; C3DVector AxisX, AxisY;
    CBase& GetOwner() { return *POwner; } long GetDuration() const { return WFDuration; }
    void OnInitLeaf();
};
// ---- row body (verbatim) ----
void CScriptModuleOpenChest::OnInitLeaf()
{
    if (IS_THING_ALIVE(PChest) && IS_THING_ALIVE(PCreature)) {
        Origin = PChest->GetPos();
        AxisY = Origin - PCreature->GetPos();
        AxisY.Z = 0.0f;
        float mag = AxisY.GetAccurateMagnitude();
        AxisY /= mag;
        AxisX = AxisY.CrossWithUp();
        Theta = 0.0f;
        Rad = mag * 2.0f;
        Height = 3.0f;
        float theta_end = ((float)GFRandom(2, GetOwner().GetPWorldSeed()) - 0.5f) * (2.0f / 3.0f);
        float rad_end = mag * 1.6f;
        float height_end = 1.5f;
        DeltaTheta = (theta_end - Theta) / GetDuration();
        DeltaRad = (rad_end - Rad) / GetDuration();
        DeltaHeight = (height_end - Height) / GetDuration();
    }
}
}

int main()
{
    using namespace NCameraModes;
    CThing chest, creature; CBase owner; long cases = 0;
    const float px[] = { 10.0f, -3.5f, 0.25f }, py[] = { 4.0f, 7.0f, -2.0f };
    for (int a = 0; a != 3; ++a) for (int b = 0; b != 3; ++b) for (int alive = 0; alive != 5; ++alive) for (unsigned long s0 = 1; s0 < 40; s0 += 13) {
        chest.Pos = C3DVector(px[a], py[b], 2.0f); creature.Pos = C3DVector(py[a], px[b], -1.0f + a);
        chest.Dead = alive == 1; creature.Dead = alive == 2;
        CScriptModuleOpenChest m; memset(&m, 0, sizeof m);
        m.POwner = &owner; owner.Seed = s0; m.WFDuration = 25 + a;
        m.PChest.Item = alive == 3 ? 0 : &chest; m.PCreature.Item = alive == 4 ? 0 : &creature;
        m.Theta = 9.0f; m.Rad = 9.0f; m.Height = 9.0f;
        m.OnInitLeaf();
        if (alive != 0) { CHECK(m.Theta == 9.0f && m.Rad == 9.0f && m.Height == 9.0f && owner.Seed == s0 && m.Origin.X == 0.0f); ++cases; continue; }
        float dx = chest.Pos.X - creature.Pos.X, dy = chest.Pos.Y - creature.Pos.Y, mag = (float)sqrt(dx * dx + dy * dy);
        CHECK(m.Origin.X == chest.Pos.X && m.Origin.Y == chest.Pos.Y && m.Origin.Z == chest.Pos.Z);
        CHECK(close(m.AxisY.X, dx / mag) && close(m.AxisY.Y, dy / mag) && m.AxisY.Z == 0.0f);
        CHECK(close(m.AxisX.X, dy / mag) && close(m.AxisX.Y, -dx / mag) && m.AxisX.Z == 0.0f);
        CHECK(m.Theta == 0.0f && close(m.Rad, mag * 2.0f) && m.Height == 3.0f);
        unsigned long r = ref_rand(s0); CHECK(owner.Seed == r);
        float d = (float)m.WFDuration;
        CHECK(close(m.DeltaTheta, ((float)(r % 2) - 0.5f) * (2.0f / 3.0f) / d));
        CHECK(close(m.DeltaRad, (mag * 1.6f - mag * 2.0f) / d) && close(m.DeltaHeight, (1.5f - 3.0f) / d));
        ++cases;
    }
    CHECK(cases == 135);
    puts("00853670_TEST PASS");
    return 0;
}