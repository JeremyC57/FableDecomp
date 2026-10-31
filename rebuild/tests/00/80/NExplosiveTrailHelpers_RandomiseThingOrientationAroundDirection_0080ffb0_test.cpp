// Behaviour test for NExplosiveTrailHelpers::RandomiseThingOrientationAroundDirection @ 0x0080FFB0: the forward axis
// (1,0,0) is spun about up (0,0,1) by random(1.0), both axes are then tilted about the direction by 0.125 - random(0.25),
// the world seed is fetched once per random call, and the resulting set is handed to the physics component.
#include <cstdio>
#include <cmath>

class C3DVector {
public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    void Rotate(const C3DVector& axis, float angle)          // Rodrigues rotation about a unit axis
    {
        float c = (float)std::cos(angle), s = (float)std::sin(angle);
        float d = axis.X * X + axis.Y * Y + axis.Z * Z;
        float cx = axis.Y * Z - axis.Z * Y, cy = axis.Z * X - axis.X * Z, cz = axis.X * Y - axis.Y * X;
        float nx = X * c + cx * s + axis.X * d * (1 - c);
        float ny = Y * c + cy * s + axis.Y * d * (1 - c);
        float nz = Z * c + cz * s + axis.Z * d * (1 - c);
        X = nx; Y = ny; Z = nz;
    }
};
class CRightHandedSet {
public:
    C3DVector Up;
    C3DVector Forward;
    CRightHandedSet(const C3DVector& up, const C3DVector& forward) : Up(up), Forward(forward) { }
};

static int g_seedCalls = 0;
static unsigned long g_seed = 0;
class CWorld {
public:
    unsigned long* GetWorldSeed() const { ++g_seedCalls; return &g_seed; }
};
class CMainGameComponent {
public:
    CWorld* PWorld;
    CWorld& GetWorld() { return *PWorld; }
};
static CMainGameComponent* g_pMainGameComponent = 0;

static float g_randoms[2];
static float g_ranges[2];
static int g_randomCalls = 0;
static float GFFloatRandom(float range, unsigned long* seed)
{
    if (seed != &g_seed) return -100.0f;
    g_ranges[g_randomCalls] = range;
    return g_randoms[g_randomCalls++] * range;
}

class CTCPhysicsBase {
public:
    virtual ~CTCPhysicsBase() { }
    virtual void SetRHSet(const CRightHandedSet& rh_set) = 0;
};
class CRecordingPhysics : public CTCPhysicsBase {
public:
    CRecordingPhysics() : Calls(0), Got(C3DVector(0, 0, 0), C3DVector(0, 0, 0)) { }
    virtual void SetRHSet(const CRightHandedSet& rh_set) { ++Calls; Got.Up = rh_set.Up; Got.Forward = rh_set.Forward; }
    int Calls;
    CRightHandedSet Got;
};
class CThing {
public:
    CTCPhysicsBase* PhysicsTC;
};

static void RandomiseThingOrientationAroundDirection(CThing& thing, const C3DVector& direction)
{
    CWorld& world = g_pMainGameComponent->GetWorld();
    C3DVector up(0.0f, 0.0f, 1.0f);
    C3DVector forward(1.0f, 0.0f, 0.0f);
    forward.Rotate(up, GFFloatRandom(1.0f, world.GetWorldSeed()));
    const float tilt = 0.125f - GFFloatRandom(0.25f, world.GetWorldSeed());
    up.Rotate(direction, tilt);
    forward.Rotate(direction, tilt);
    CTCPhysicsBase* physics = thing.PhysicsTC;
    physics->SetRHSet(CRightHandedSet(up, forward));
}

static bool Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }

int main()
{
    CWorld world;
    CMainGameComponent component;
    component.PWorld = &world;
    g_pMainGameComponent = &component;
    CRecordingPhysics physics;
    CThing thing;
    thing.PhysicsTC = &physics;

    // spin pi/2 (random 0.5 * range 1.0 = 0.5 rad is used; choose values so the expected result is easy)
    g_randoms[0] = 0.5f;          // spin 0.5 rad about +Z
    g_randoms[1] = 0.5f;          // tilt = 0.125 - 0.125 = 0
    C3DVector direction(1.0f, 0.0f, 0.0f);
    RandomiseThingOrientationAroundDirection(thing, direction);
    if (physics.Calls != 1 || g_seedCalls != 2 || g_randomCalls != 2 || g_ranges[0] != 1.0f || g_ranges[1] != 0.25f) {
        std::printf("0080ffb0_TEST FAIL\n"); return 1;
    }
    if (!Near(physics.Got.Up.X, 0) || !Near(physics.Got.Up.Y, 0) || !Near(physics.Got.Up.Z, 1) ||
        !Near(physics.Got.Forward.X, (float)std::cos(0.5)) || !Near(physics.Got.Forward.Y, (float)std::sin(0.5)) ||
        !Near(physics.Got.Forward.Z, 0)) {
        std::printf("0080ffb0_TEST FAIL\n"); return 1;
    }

    // no spin, tilt 0.125 rad about +X: up (0,0,1) -> (0,-sin,cos), forward (1,0,0) unchanged
    g_randomCalls = 0; g_seedCalls = 0;
    g_randoms[0] = 0.0f;
    g_randoms[1] = 0.0f;          // tilt = 0.125
    RandomiseThingOrientationAroundDirection(thing, direction);
    if (physics.Calls != 2 || !Near(physics.Got.Up.Y, -(float)std::sin(0.125)) || !Near(physics.Got.Up.Z, (float)std::cos(0.125)) ||
        !Near(physics.Got.Forward.X, 1) || !Near(physics.Got.Forward.Y, 0)) {
        std::printf("0080ffb0_TEST FAIL\n"); return 1;
    }
    std::printf("0080ffb0_TEST PASS\n");
    return 0;
}