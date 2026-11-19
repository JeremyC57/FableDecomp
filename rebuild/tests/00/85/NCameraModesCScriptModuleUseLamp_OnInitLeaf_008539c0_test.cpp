// Behaviour test for NCameraModes::CScriptModuleUseLamp::OnInitLeaf (0x008539C0): the row body verbatim over models of its callees.
// Kills: either alive test removed / inverted, the helper point ignored or the position fallback taken on a hit (or skipped on a
// miss), the wrong helper name / index / flags / time, Z not flattened, normalisation / perpendicular, radius factor 4, height 0.5,
// the seed step, the random side, the end values or the per-frame division.
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
namespace NGraphicAppearance { class CTCGraphicAppearance; }
class CThing { public: C3DVector Pos; bool Dead; const NGraphicAppearance::CTCGraphicAppearance* GetTCGraphicAppearance() const { return (const NGraphicAppearance::CTCGraphicAppearance*)App; } void* App; const C3DVector& GetPos() const { return Pos; } bool IsDead() const { return Dead; } };
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

extern const CCharString TARGETING_CURSOR_POSITION_NAME;
const CCharString TARGETING_CURSOR_POSITION_NAME = { 77 };
static bool helper_hit; static C3DVector helper_pos; static int helper_calls;
namespace NGraphicAppearance { class CTCGraphicAppearance { public:
    bool GetHelperPoint(const CCharString& name, long index, unsigned long flags, C3DVector& pos, float time) const
    { ++helper_calls; CHECK(&name == &TARGETING_CURSOR_POSITION_NAME && index == 0 && flags == 0 && time == 1.0f); if (helper_hit) pos = helper_pos; return helper_hit; } }; }
static NGraphicAppearance::CTCGraphicAppearance the_app;
namespace NCameraModes {
class CScriptModuleUseLamp { public:
    CBase* POwner; long WFDuration;
    CIntelligentPointer<const CThing> PCreature, PLamp;
    C3DVector Origin; float Theta, Rad, Height, DeltaTheta, DeltaRad, DeltaHeight; C3DVector AxisX, AxisY;
    CBase& GetOwner() { return *POwner; } long GetDuration() const { return WFDuration; }
    void OnInitLeaf();
};
// ---- row body (verbatim) ----
void CScriptModuleUseLamp::OnInitLeaf()
{
    if (IS_THING_ALIVE(PLamp) && IS_THING_ALIVE(PCreature)) {
        if (!PLamp->GetTCGraphicAppearance()->GetHelperPoint(TARGETING_CURSOR_POSITION_NAME, 0, 0, Origin, 1.0f))
            Origin = PLamp->GetPos();
        AxisY = Origin - PCreature->GetPos();
        AxisY.Z = 0.0f;
        float mag = AxisY.GetAccurateMagnitude();
        AxisY /= mag;
        AxisX = AxisY.CrossWithUp();
        Theta = 0.0f;
        Rad = mag * 4.0f;
        Height = 0.5f;
        float theta_end = ((float)GFRandom(2, GetOwner().GetPWorldSeed()) - 0.5f) * (2.0f / 3.0f);
        float rad_end = mag * 4.0f;
        float height_end = 0.5f;
        DeltaTheta = (theta_end - Theta) / GetDuration();
        DeltaRad = (rad_end - Rad) / GetDuration();
        DeltaHeight = (height_end - Height) / GetDuration();
    }
}
}

int main()
{
    using namespace NCameraModes;
    CThing lamp, creature; CBase owner; long cases = 0; lamp.App = &the_app; creature.App = 0;
    const float px[] = { 10.0f, -3.5f, 0.25f }, py[] = { 4.0f, 7.0f, -2.0f };
    for (int a = 0; a != 3; ++a) for (int b = 0; b != 3; ++b) for (int alive = 0; alive != 5; ++alive) for (int h = 0; h != 2; ++h) for (unsigned long s0 = 2; s0 < 40; s0 += 17) {
        lamp.Pos = C3DVector(px[a], py[b], 2.0f); creature.Pos = C3DVector(py[a], px[b], -1.0f + a);
        helper_hit = h != 0; helper_pos = C3DVector(px[b] + 1.0f, py[a] - 2.0f, 5.0f); helper_calls = 0;
        lamp.Dead = alive == 1; creature.Dead = alive == 2;
        CScriptModuleUseLamp m; memset(&m, 0, sizeof m);
        m.POwner = &owner; owner.Seed = s0; m.WFDuration = 30 + b;
        m.PLamp.Item = alive == 3 ? 0 : &lamp; m.PCreature.Item = alive == 4 ? 0 : &creature;
        m.Theta = 9.0f; m.Rad = 9.0f; m.Height = 9.0f;
        m.OnInitLeaf();
        if (alive != 0) { CHECK(m.Theta == 9.0f && m.Rad == 9.0f && owner.Seed == s0 && helper_calls == 0); ++cases; continue; }
        CHECK(helper_calls == 1);
        C3DVector o = h ? helper_pos : lamp.Pos;
        CHECK(m.Origin.X == o.X && m.Origin.Y == o.Y && m.Origin.Z == o.Z);
        float dx = o.X - creature.Pos.X, dy = o.Y - creature.Pos.Y, mag = (float)sqrt(dx * dx + dy * dy);
        CHECK(close(m.AxisY.X, dx / mag) && close(m.AxisY.Y, dy / mag) && m.AxisY.Z == 0.0f);
        CHECK(close(m.AxisX.X, dy / mag) && close(m.AxisX.Y, -dx / mag) && m.AxisX.Z == 0.0f);
        CHECK(m.Theta == 0.0f && close(m.Rad, mag * 4.0f) && m.Height == 0.5f);
        unsigned long r = ref_rand(s0); CHECK(owner.Seed == r);
        float d = (float)m.WFDuration;
        CHECK(close(m.DeltaTheta, ((float)(r % 2) - 0.5f) * (2.0f / 3.0f) / d));
        CHECK(fabs(m.DeltaRad) <= 1e-6f && m.DeltaHeight == 0.0f);
        ++cases;
    }
    CHECK(cases == 270);
    puts("008539c0_TEST PASS");
    return 0;
}