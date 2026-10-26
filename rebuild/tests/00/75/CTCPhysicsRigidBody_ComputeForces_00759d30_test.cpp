#include <cstdio>
#include <cmath>
// Behaviour test for CTCPhysicsRigidBody::ComputeForces (0x00759D30): the body of src/ over stand-ins -- forces zeroed,
// the debug kick on key 0x2e, gravity when affected, and linear/angular damping.
enum EInputEventType { EIET_NONE = 0 };
enum EInputKey { EIK_NONE = 0 };
class CInputEvent { public: int dummy; };
class CInputManager { public:
    long m_enabled;
    bool pressed;
    EInputEventType type; EInputKey key;
    bool IsKeyboardEventInQueue(EInputEventType t, EInputKey k, CInputEvent* event) { type = t; key = k; return pressed; }
};
static CInputManager g_input = { 0, false, EIET_NONE, EIK_NONE };
static CInputManager* g_pInputManager = &g_input;
static int g_randoms = 0;
static float GFFloatRandom(float range, unsigned long* seed) { ++*seed; ++g_randoms; return range * (float)g_randoms; }
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    void Zero() { X = Y = Z = 0.0f; }
    void operator+=(const C3DVector& v) { X += v.X; Y += v.Y; Z += v.Z; }
    void Normalise() { float l = sqrtf(X * X + Y * Y + Z * Z); X /= l; Y /= l; Z /= l; }
};
struct CRigidBodyConfig { C3DVector CMVelocity; C3DVector CMForce; C3DVector Torque; C3DVector AngularVelocity; };
class CWorld { public:
    unsigned long seed;
    unsigned long* GetRandomSeed() { return &seed; }
};
static CWorld g_world = { 0 };
class CTCPhysicsRigidBody {
public:
    bool gravity;
    bool IsAffectedByGravity() { return gravity; }
    float OneOverMass;
    CWorld* GetWorld() { return &g_world; }
    void ComputeForces(CRigidBodyConfig& config);
};

void CTCPhysicsRigidBody::ComputeForces(CRigidBodyConfig& config)
{
    config.Torque.Zero();
    config.CMForce.Zero();
    CInputEvent event;
    if (g_pInputManager->m_enabled && g_pInputManager->IsKeyboardEventInQueue((EInputEventType)2, (EInputKey)0x2e, &event)) {
        config.CMForce.Z += 25.0f / OneOverMass;
        config.Torque.X = GFFloatRandom(100.0f, GetWorld()->GetRandomSeed());
        config.Torque.Y = GFFloatRandom(100.0f, GetWorld()->GetRandomSeed());
        config.Torque.Z = GFFloatRandom(100.0f, GetWorld()->GetRandomSeed());
        config.Torque.Normalise();
    }
    if (IsAffectedByGravity()) {
        float mass = 1.0f / OneOverMass;
        config.CMForce += C3DVector(0.0f * mass, 0.0f * mass, mass * -10.0f);
    }
    config.CMForce += C3DVector(config.CMVelocity.X * -0.06f, config.CMVelocity.Y * -0.06f, config.CMVelocity.Z * -0.06f);
    config.Torque += C3DVector(config.AngularVelocity.X * -0.02f, config.AngularVelocity.Y * -0.02f, config.AngularVelocity.Z * -0.02f);
}

static bool near(float a, float b) { return fabsf(a - b) < 1e-4f; }
static int fail(int line) { printf("00759d30_TEST FAIL (line %d)\n", line); return 1; }
#define CHECK(c) do { if (!(c)) return fail(__LINE__); } while (0)

int main()
{
    CTCPhysicsRigidBody body; body.gravity = true; body.OneOverMass = 0.5f;
    CRigidBodyConfig c;
    c.CMForce = C3DVector(9, 9, 9); c.Torque = C3DVector(9, 9, 9);
    c.CMVelocity = C3DVector(10, 0, -100); c.AngularVelocity = C3DVector(0, 50, 0);
    body.ComputeForces(c);
    CHECK(near(c.CMForce.X, -0.6f) && near(c.CMForce.Y, 0.0f) && near(c.CMForce.Z, -20.0f + 6.0f));
    CHECK(near(c.Torque.X, 0.0f) && near(c.Torque.Y, -1.0f) && near(c.Torque.Z, 0.0f) && g_randoms == 0);
    // input disabled: queue not consulted
    CHECK(g_input.type == EIET_NONE);
    // kick: enabled and key pressed, no gravity, still body
    g_input.m_enabled = 1; g_input.pressed = true; body.gravity = false;
    c.CMVelocity = C3DVector(0, 0, 0); c.AngularVelocity = C3DVector(0, 0, 0);
    body.ComputeForces(c);
    CHECK(g_input.type == 2 && g_input.key == 0x2e && g_randoms == 3 && g_world.seed == 3);
    CHECK(near(c.CMForce.Z, 50.0f) && near(c.CMForce.X, 0.0f));
    float l = sqrtf(100.0f * 100.0f + 200.0f * 200.0f + 300.0f * 300.0f);
    CHECK(near(c.Torque.X, 100.0f / l) && near(c.Torque.Y, 200.0f / l) && near(c.Torque.Z, 300.0f / l));
    printf("00759d30_TEST PASS\n");
    return 0;
}