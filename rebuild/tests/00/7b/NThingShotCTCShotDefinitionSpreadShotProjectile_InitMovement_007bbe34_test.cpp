// Behaviour test for NThingShot::CTCShotDefinitionSpreadShotProjectile::InitMovement (0x007BBE34), through a copy of the model:
// the logic of the function over small stand-in engine classes (the out-of-line callees are modelled here: a quarter-turn
// RotateXY, a flat ground at Z = 2, an accuracy-100 RandomiseShotDirection that returns its input).
#include <cstdio>
#include <cmath>
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    const C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    const C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    const C3DVector operator^(const C3DVector& o) const { return C3DVector(Y * o.Z - Z * o.Y, Z * o.X - X * o.Z, X * o.Y - Y * o.X); }
    const C3DVector& operator+=(const C3DVector& o) { X += o.X; Y += o.Y; Z += o.Z; return *this; }
    // model of 0x00A14040: turn in the XY plane by `angle` revolutions
    void RotateXY(float angle) {
        double a = angle * 6.283185307179586;
        float c = (float)cos(a), s = (float)sin(a);
        float x = X * c - Y * s, y = X * s + Y * c;
        X = x; Y = y;
    }
    C3DVector& AccurateNormalise() {
        float m = (float)sqrt(X * X + Y * Y + Z * Z);
        if (m > 0.0f) { X /= m; Y /= m; Z /= m; }
        return *this;
    }
};
class CRightHandedSet { public: C3DVector Up, Forward; CRightHandedSet() {} CRightHandedSet(const C3DVector& u, const C3DVector& f) : Up(u), Forward(f) {} };
class CTCPhysicsBase { public:
    C3DVector Position, Facing, Acceleration;
    CRightHandedSet RHSet;
    bool Gravity; int Resets, Accels;
    CTCPhysicsBase() : Position(0.0f, 0.0f, 0.0f), Facing(0.0f, 0.0f, 0.0f), Acceleration(0.0f, 0.0f, 0.0f), Gravity(true), Resets(0), Accels(0) {}
    C3DVector GetFacingVector() const { return Facing; }
    const C3DVector& GetPosition() const { return Position; }
    void ResetRHSet(const CRightHandedSet& s) { RHSet = s; ++Resets; }
    void ApplyAcceleration(const C3DVector& a) { Acceleration = a; ++Accels; }
    void EnableGravity(bool on) { Gravity = on; }
};
class CThing { public:
    CTCPhysicsBase* PhysicsTC; bool Alive;
    CTCPhysicsBase* GetPhysicsTC() const { return PhysicsTC; }
    const C3DVector& GetPos() const { return PhysicsTC->Position; }
};
static bool IS_THING_ALIVE(const CThing* const& thing) { return thing != 0 && thing->Alive; }
static float GetHeightZBelowPos(const C3DVector&) { return 2.0f; }
static int g_RandomiseCalls; static long g_LastAccuracy;
static C3DVector RandomiseShotDirection(const C3DVector& direction, long accuracy, unsigned long*) { ++g_RandomiseCalls; g_LastAccuracy = accuracy; return direction; }
static const float SPREAD_SHOT_ANGLE_STEP = 0.333f;
static const float SPREAD_SHOT_TARGET_OFFSET = 1.5f;
static float s_SpreadShotAngle;
static const long ConstantFPS = 25;
class CThingShotDef { public: float Speed; };
struct CShot {
    CThing* ParentThing; const CThing* PTargetThing; const CThingShotDef* PShotDef; float AccuracyPercentage; unsigned long Seed;
    void InitMovement() {
        CThing* target = const_cast<CThing*>(PTargetThing);
        if (IS_THING_ALIVE((const CThing*)target)) {
            CTCPhysicsBase* physics = ParentThing->GetPhysicsTC();
            C3DVector aim = target->GetPhysicsTC()->GetFacingVector() * SPREAD_SHOT_TARGET_OFFSET;
            aim.RotateXY(s_SpreadShotAngle);
            aim += target->GetPos();
            aim.Z = GetHeightZBelowPos(aim);
            s_SpreadShotAngle += SPREAD_SHOT_ANGLE_STEP;
            if (s_SpreadShotAngle > 1.0f)
                s_SpreadShotAngle -= 1.0f;
            C3DVector dir = aim - physics->GetPosition();
            dir = RandomiseShotDirection(dir, (long)AccuracyPercentage, &Seed);
            dir.AccurateNormalise();
            C3DVector side = C3DVector(dir.Y, -dir.X, 0.0f) ^ dir;
            side.AccurateNormalise();
            physics->ResetRHSet(CRightHandedSet(side, dir));
            C3DVector velocity = dir * (PShotDef->Speed / ConstantFPS);
            physics->ApplyAcceleration(velocity);
            physics->EnableGravity(false);
        }
    }
};
static bool Near(float a, float b) { float d = a - b; if (d < 0) d = -d; return d < 1.0e-4f; }
static int Fail() { std::printf("007bbe34_TEST FAIL\n"); return 1; }
int main()
{
    CTCPhysicsBase shotPhys; shotPhys.Position = C3DVector(0.0f, 0.0f, 2.0f); shotPhys.Gravity = true;
    CTCPhysicsBase targetPhys; targetPhys.Position = C3DVector(10.0f, 0.0f, 5.0f); targetPhys.Facing = C3DVector(1.0f, 0.0f, 0.0f);
    CThing shotThing = { &shotPhys, true }; CThing targetThing = { &targetPhys, false };
    CThingShotDef def = { 50.0f };
    CShot shot = { &shotThing, &targetThing, &def, 80.9f, 1 };
    // 1. dead target: nothing moves, the angle does not advance
    shot.InitMovement();
    if (shotPhys.Resets != 0 || shotPhys.Accels != 0 || !shotPhys.Gravity || g_RandomiseCalls != 0 || s_SpreadShotAngle != 0.0f) return Fail();
    // 2. live target, angle 0: aim = (10 + 1.5, 0, ground 2); dir = +X; speed 50/25 = 2
    targetThing.Alive = true;
    shot.InitMovement();
    if (g_RandomiseCalls != 1 || g_LastAccuracy != 80) return Fail();                 // (long) truncation of 80.9
    if (shotPhys.Resets != 1 || shotPhys.Accels != 1 || shotPhys.Gravity) return Fail();
    if (!Near(shotPhys.RHSet.Forward.X, 1.0f) || !Near(shotPhys.RHSet.Forward.Y, 0.0f) || !Near(shotPhys.RHSet.Forward.Z, 0.0f)) return Fail();
    // side = (dir.Y, -dir.X, 0) x dir = (0, -1, 0) x (1, 0, 0) = (0, 0, 1)
    if (!Near(shotPhys.RHSet.Up.X, 0.0f) || !Near(shotPhys.RHSet.Up.Y, 0.0f) || !Near(shotPhys.RHSet.Up.Z, 1.0f)) return Fail();
    if (!Near(shotPhys.Acceleration.X, 2.0f) || !Near(shotPhys.Acceleration.Y, 0.0f) || !Near(shotPhys.Acceleration.Z, 0.0f)) return Fail();
    if (!Near(s_SpreadShotAngle, 0.333f)) return Fail();
    // 3. second shot: the offset is turned by 0.333 of a revolution about Z before it is added to the target position
    shot.InitMovement();
    {
        double a = 0.333 * 6.283185307179586;
        float ax = 10.0f + 1.5f * (float)cos(a), ay = 1.5f * (float)sin(a);
        float m = (float)sqrt(ax * ax + ay * ay);
        if (!Near(shotPhys.RHSet.Forward.X, ax / m) || !Near(shotPhys.RHSet.Forward.Y, ay / m) || !Near(shotPhys.RHSet.Forward.Z, 0.0f)) return Fail();
        if (!Near(shotPhys.Acceleration.X, 2.0f * ax / m) || !Near(shotPhys.Acceleration.Y, 2.0f * ay / m)) return Fail();
    }
    if (!Near(s_SpreadShotAngle, 0.666f)) return Fail();
    // 4. third and fourth shots: 0.999 stays, 1.332 wraps to 0.332
    shot.InitMovement();
    if (!Near(s_SpreadShotAngle, 0.999f)) return Fail();
    shot.InitMovement();
    if (!Near(s_SpreadShotAngle, 0.332f)) return Fail();
    if (shotPhys.Resets != 4 || shotPhys.Accels != 4 || g_RandomiseCalls != 4) return Fail();
    std::printf("007bbe34_TEST PASS\n");
    return 0;
}