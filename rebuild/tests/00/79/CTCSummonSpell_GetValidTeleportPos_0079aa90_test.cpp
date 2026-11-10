// Behaviour test for 0x0079AA90: CTCSummonSpell::GetValidTeleportPos (own position if valid, else ring points around the caster,
// at most ten more tries, then the last rotated direction at the original distance)
#include <cstdio>
#include <cmath>

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    const C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    const C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    const C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    const C3DVector& operator/=(float v) { float r = 1.0f / v; X *= r; Y *= r; Z *= r; return *this; }
    float GetMagnitude() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
    void RotateXY(float angle) {                      // angle in turns about Z
        double a = angle * 6.283185307179586;
        float x = (float)(X * cos(a) - Y * sin(a)), y = (float)(X * sin(a) + Y * cos(a));
        X = x; Y = y;
    }
};

class CThing { public: C3DVector Pos; const C3DVector& GetPos() const { return Pos; } };
class CWorldMap { };

static int g_checks;
static int g_validAt;                                 // the check number that succeeds (0 = never)
static C3DVector g_checked[16];

class CTCSummonSpell {
public:
    CThing* ParentThing;
    CWorldMap Map;
    CThing& GetParentThing() { return *ParentThing; }
    const CWorldMap& PeekWorldMap() { return Map; }
    bool IsValidTeleportPos(const C3DVector& pos, const CWorldMap&, const CThing*) {
        if (g_checks < 16) g_checked[g_checks] = pos;
        return ++g_checks == g_validAt;
    }
    C3DVector GetValidTeleportPos(const CThing* thing);
};

C3DVector CTCSummonSpell::GetValidTeleportPos(const CThing* thing)
{
    C3DVector direction = thing->GetPos() - GetParentThing().GetPos();
    float radius = direction.GetMagnitude();
    float angle = 0.0f;
    direction /= radius;
    float distance = radius;
    const CWorldMap& world_map = PeekWorldMap();
    C3DVector pos = thing->GetPos();
    long tries = 0;
    while (!IsValidTeleportPos(pos, world_map, thing)) {
        angle += 1.0f / 36.0f;
        if (angle >= 1.0f) {
            angle = 0.0f;
            radius += distance;
        }
        C3DVector rotated = direction;
        rotated.RotateXY(angle);
        pos = GetParentThing().GetPos() + rotated * radius;
        if (++tries > 10) {
            pos = GetParentThing().GetPos() + rotated * distance;
            break;
        }
    }
    return pos;
}

static int Near(float a, float b) { return fabs(a - b) < 1e-3f; }

int main()
{
    int fails = 0;
    CThing caster, target;
    caster.Pos = C3DVector(10.0f, 20.0f, 1.0f);
    target.Pos = C3DVector(10.0f, 24.0f, 1.0f);       // 4 units along +Y
    CTCSummonSpell spell;
    spell.ParentThing = &caster;
    // valid where it stands
    g_checks = 0; g_validAt = 1;
    C3DVector r = spell.GetValidTeleportPos(&target);
    if (g_checks != 1 || r.X != 10.0f || r.Y != 24.0f || r.Z != 1.0f) { std::printf("own position\n"); ++fails; }
    // valid at the third check: the second ring point (2/36 turn), still at radius 4 around the caster
    g_checks = 0; g_validAt = 3;
    r = spell.GetValidTeleportPos(&target);
    C3DVector d = r - caster.Pos;
    if (g_checks != 3 || !Near(d.GetMagnitude(), 4.0f) || !Near(d.X, (float)(-4.0 * sin(2.0 / 36.0 * 6.283185307179586)))) { std::printf("ring point\n"); ++fails; }
    // never valid: 11 checks (own spot + 10 ring points), then the 11th rotation at the original distance, unchecked
    g_checks = 0; g_validAt = 0;
    r = spell.GetValidTeleportPos(&target);
    d = r - caster.Pos;
    if (g_checks != 11 || !Near(d.GetMagnitude(), 4.0f) || !Near(d.Y, (float)(4.0 * cos(11.0 / 36.0 * 6.283185307179586)))) { std::printf("give up\n"); ++fails; }
    if (fails) { std::printf("0079aa90_TEST FAIL\n"); return 1; }
    std::printf("0079aa90_TEST PASS\n");
    return 0;
}