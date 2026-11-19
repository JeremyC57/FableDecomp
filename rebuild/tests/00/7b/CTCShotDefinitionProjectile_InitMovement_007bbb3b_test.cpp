// Behaviour test for NThingShot::CTCShotDefinitionProjectile::InitMovement (0x007BBB3B): a fixture models the physics component
// (position, RH set, acceleration, gravity), the world seed, the shot's aim point / accuracy / initial velocity and a deterministic
// RandomiseShotDirection, and runs the row's verbatim body over 12 cases against an independent double-precision reference.
#include <stdio.h>
#include <math.h>
static int failures;
static void check(bool ok, const char* what, int c) { if (!ok) { ++failures; printf("FAIL case %d: %s\n", c, what); } }
class C3DVector { public: float X, Y, Z;
    C3DVector() {} C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    const C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    const C3DVector operator*(float scale) const { return C3DVector((X * scale), Y * scale, Z * scale); }
    C3DVector CrossWithUp() const { return C3DVector(Y, -X, 0.0f); }
    const C3DVector Cross(const C3DVector& other) const { return C3DVector((Y * other.Z - Z * other.Y), (Z * other.X - X * other.Z), X * other.Y - Y * other.X); }
    C3DVector& AccurateNormalise() { float m = sqrtf(X * X + Y * Y + Z * Z); if (m > 0.0f) { X /= m; Y /= m; Z /= m; } return *this; } };
class CRightHandedSet { public: C3DVector Up, Forward;
    CRightHandedSet(const C3DVector& up, const C3DVector& forward) : Up(up), Forward(forward) {} };
class CTCPhysicsBase { public:
    C3DVector Position, Acceleration; CRightHandedSet RHSet; int resets, accels, gravity_calls; bool gravity;
    CTCPhysicsBase() : Position(0, 0, 0), Acceleration(0, 0, 0), RHSet(C3DVector(0, 0, 0), C3DVector(0, 0, 0)), resets(0), accels(0), gravity_calls(0), gravity(true) {}
    const C3DVector& GetPosition() const { return Position; }
    virtual void ResetRHSet(const CRightHandedSet& set) { RHSet = set; ++resets; check(accels == 0, "RH set reset before acceleration", -1); }
    void ApplyAcceleration(const C3DVector& a) { Acceleration = a; ++accels; check(resets == 1, "acceleration after RH set", -1); }
    virtual void EnableGravity(bool on) { gravity = on; ++gravity_calls; check(accels == 1, "gravity last", -1); } };
class CThing { public: CTCPhysicsBase* Physics; CTCPhysicsBase* GetPhysicsTC() { return Physics; } };
class CWorld { public: unsigned long Seed; unsigned long* GetWorldSeed() const { return const_cast<unsigned long*>(&Seed); } };
namespace NThingShot {
static long seen_accuracy; static unsigned long* seen_seed; static int randomise_calls;
C3DVector RandomiseShotDirection(const C3DVector& direction, long accuracy, unsigned long* seed)
{
    seen_accuracy = accuracy; seen_seed = seed; ++randomise_calls; ++*seed;
    return C3DVector(direction.X + accuracy * 0.01f, direction.Y - (float)(*seed % 7) * 0.25f, direction.Z * 0.5f + 1.0f);
}
class CTCShotDefinitionProjectile { public:
    CThing* ParentThing; CWorld* World; C3DVector TargetPos; float AccuracyPercentage; float Velocity;
    CThing& GetParentThing() { return *ParentThing; }
    C3DVector GetShootAtPos() const { return TargetPos; }
    long GetShotAccuracyPercentage() const { return (long)AccuracyPercentage; }
    CWorld& GetWorld() const { return *World; }
    float GetInitialVelocity() const { return Velocity; }
    void InitMovement();
};
void CTCShotDefinitionProjectile::InitMovement()
{
    CTCPhysicsBase* ptc_phys = GetParentThing().GetPhysicsTC();
    C3DVector fire_at_pos = GetShootAtPos();
    C3DVector line_to_target = fire_at_pos - ptc_phys->GetPosition();
    line_to_target = RandomiseShotDirection(line_to_target, GetShotAccuracyPercentage(), GetWorld().GetWorldSeed());
    line_to_target.AccurateNormalise();
    C3DVector up = line_to_target.CrossWithUp().Cross(line_to_target);
    up.AccurateNormalise();
    ptc_phys->ResetRHSet(CRightHandedSet(up, line_to_target));
    float speed = GetInitialVelocity();
    C3DVector applied_acceleration = line_to_target * speed;
    ptc_phys->ApplyAcceleration(applied_acceleration);
    ptc_phys->EnableGravity(false);
}
}
static bool near(double a, double b) { return fabs(a - b) <= 1e-4 * (1.0 + fabs(b)); }
int main()
{
    static const float cases[12][9] = {
        // pos xyz, target xyz, accuracy, velocity, seed
        {0, 0, 0, 10, 0, 0, 0, 1, 1}, {0, 0, 0, 0, 10, 0, 50, 2, 2}, {1, 2, 3, 4, 6, 3, 99.9f, 0.5f, 3},
        {-5, 4, 1, 7, -8, 2, 25, 3.25f, 4}, {0, 0, 0, 3, 4, 12, 10, 1, 5}, {2, 2, 2, -2, -2, 0, 0, 10, 6},
        {100, 50, 0, 90, 70, 5, 75.5f, 0.125f, 7}, {0, 0, 5, 1, 1, 0, 33, 4, 8}, {3, -3, 0, -3, 3, 0, 60, 2.5f, 9},
        {0, 0, 0, 0.5f, 0.25f, 0, 5, 6, 10}, {-1, -1, -1, 1, 1, 1, 80, 1.5f, 11}, {10, 0, 0, 10, 20, 3, 15, 0.75f, 12} };
    for (int c = 0; c < 12; ++c) {
        const float* k = cases[c];
        CTCPhysicsBase phys; phys.Position = C3DVector(k[0], k[1], k[2]);
        CThing thing; thing.Physics = &phys; CWorld world; world.Seed = (unsigned long)k[8];
        NThingShot::CTCShotDefinitionProjectile shot; shot.ParentThing = &thing; shot.World = &world;
        shot.TargetPos = C3DVector(k[3], k[4], k[5]); shot.AccuracyPercentage = k[6]; shot.Velocity = k[7];
        NThingShot::randomise_calls = 0;
        shot.InitMovement();
        // reference
        long acc = (long)k[6]; unsigned long seed = (unsigned long)k[8] + 1;
        double dx = (double)k[3] - k[0] + acc * 0.01, dy = (double)k[4] - k[1] - (double)(seed % 7) * 0.25, dz = ((double)k[5] - k[2]) * 0.5 + 1.0;
        double m = sqrt(dx * dx + dy * dy + dz * dz); dx /= m; dy /= m; dz /= m;
        double ux = dy, uy = -dx, uz = 0.0;                       // CrossWithUp
        double cx = uy * dz - uz * dy, cy = uz * dx - ux * dz, cz = ux * dy - uy * dx;
        double cm = sqrt(cx * cx + cy * cy + cz * cz); if (cm > 0) { cx /= cm; cy /= cm; cz /= cm; }
        check(NThingShot::randomise_calls == 1 && NThingShot::seen_accuracy == acc && NThingShot::seen_seed == &world.Seed, "randomise args", c);
        check(world.Seed == seed, "seed advanced through the world", c);
        check(phys.resets == 1 && phys.accels == 1 && phys.gravity_calls == 1 && !phys.gravity, "calls", c);
        check(near(phys.RHSet.Forward.X, dx) && near(phys.RHSet.Forward.Y, dy) && near(phys.RHSet.Forward.Z, dz), "forward", c);
        check(near(phys.RHSet.Up.X, cx) && near(phys.RHSet.Up.Y, cy) && near(phys.RHSet.Up.Z, cz), "up", c);
        check(near(phys.Acceleration.X, dx * k[7]) && near(phys.Acceleration.Y, dy * k[7]) && near(phys.Acceleration.Z, dz * k[7]), "acceleration", c);
        check(fabs(phys.RHSet.Up.X * dx + phys.RHSet.Up.Y * dy + phys.RHSet.Up.Z * dz) < 1e-4, "up orthogonal to forward", c);
    }
    if (failures) { printf("%d failures\n", failures); return 1; }
    puts("007bbb3b_TEST PASS");
    return 0;
}