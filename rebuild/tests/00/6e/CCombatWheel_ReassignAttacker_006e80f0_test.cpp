// Behaviour test for CCombatWheel::ReassignAttacker (0x006E80F0): the dolly is the flat, normalised direction from
// the defender to the position (or to the attacker, randomly rotated about Z); degenerate directions leave the
// attacker untouched; a stored dolly gets a five-second timer.
#include <cstdio>
#include <cmath>

struct C2DVector { float X, Y; };
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
    void Normalise() { float l = std::sqrt(GetLengthSquared()); X /= l; Y /= l; Z /= l; }
    void Rotate(const C3DVector&, float turns)       // about Z
    { float a = turns * 6.2831853f, c = std::cos(a), s = std::sin(a); float x = X * c - Y * s; Y = X * s + Y * c; X = x; }
};
static float g_Epsilon = 1e-4f;
static long g_framesPerSecond = 30;
static float GFFloatRandom(float range, unsigned long* seed) { (void)seed; return range * 0.5f; }   // fixed: 0.25
struct CThingCreatureBase { C3DVector Pos; const C3DVector& GetPosition() const { return Pos; } };
struct CWorld { unsigned long Seed; unsigned long* GetRandomSeed() { return &Seed; } };
struct CAttackingCombatantInfo { bool PreferredAttackDollySet; long PreferredAttackDollyTimer; C2DVector PreferredAttackDollyVector; };
struct CCombatantInfo { CThingCreatureBase* Parent; CAttackingCombatantInfo AttackerInfo; };
struct CCombatWheel { void ReassignAttacker(CWorld* world, CCombatantInfo* attacker, CCombatantInfo* defender, const C3DVector* position); };

void CCombatWheel::ReassignAttacker(CWorld* world, CCombatantInfo* attacker, CCombatantInfo* defender, const C3DVector* position)
{
    C3DVector dolly;
    if (position) {
        dolly = *position - defender->Parent->GetPosition();
        dolly.Z = 0.0f;
    } else {
        dolly = attacker->Parent->GetPosition() - defender->Parent->GetPosition();
        dolly.Z = 0.0f;
        if (dolly.GetLengthSquared() > g_Epsilon * g_Epsilon)
            dolly.Rotate(C3DVector(0.0f, 0.0f, 1.0f), GFFloatRandom(0.5f, world->GetRandomSeed()) + 0.25f);
    }
    if (dolly.GetLengthSquared() > g_Epsilon * g_Epsilon) {
        dolly.Normalise();
        attacker->AttackerInfo.PreferredAttackDollySet = true;
        attacker->AttackerInfo.PreferredAttackDollyTimer = (long)(g_framesPerSecond * 5.0f);
        attacker->AttackerInfo.PreferredAttackDollyVector.X = dolly.X;
        attacker->AttackerInfo.PreferredAttackDollyVector.Y = dolly.Y;
    }
}

int main()
{
    CThingCreatureBase a, d; a.Pos = C3DVector(4.0f, 0.0f, 9.0f); d.Pos = C3DVector(0.0f, 0.0f, 0.0f);
    CCombatantInfo att = { &a, { false, 0, { 0.0f, 0.0f } } }, def = { &d, { false, 0, { 0.0f, 0.0f } } };
    CWorld w; w.Seed = 1; CCombatWheel wheel;
    C3DVector p(0.0f, 3.0f, 5.0f);
    wheel.ReassignAttacker(&w, &att, &def, &p);
    bool ok = att.AttackerInfo.PreferredAttackDollySet && att.AttackerInfo.PreferredAttackDollyTimer == 150
        && std::fabs(att.AttackerInfo.PreferredAttackDollyVector.X) < 1e-6f && std::fabs(att.AttackerInfo.PreferredAttackDollyVector.Y - 1.0f) < 1e-6f;
    wheel.ReassignAttacker(&w, &att, &def, 0);       // (4,0) turned by half a turn -> (-1, 0)
    ok = ok && std::fabs(att.AttackerInfo.PreferredAttackDollyVector.X + 1.0f) < 1e-5f && std::fabs(att.AttackerInfo.PreferredAttackDollyVector.Y) < 1e-5f;
    att.AttackerInfo.PreferredAttackDollySet = false; a.Pos = d.Pos;
    wheel.ReassignAttacker(&w, &att, &def, 0);       // degenerate: untouched
    ok = ok && !att.AttackerInfo.PreferredAttackDollySet;
    if (!ok) { std::printf("006e80f0_TEST FAIL\n"); return 1; }
    std::printf("006e80f0_TEST PASS\n");
    return 0;
}