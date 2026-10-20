// Behaviour test for PerformForceUpdates (008e5670): air resistance (0x008E5670 only) scales the velocity by 1 - resistance; gravity
// (-0.12 Z) applies unless the thing is on the ground, the ground normal's Z is at least 0.98 and the velocity is ~zero.
#include <cstdio>
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};
const float g_epsilon_0129ba3c = 1e-4f;
static float g_normalZ = 1.0f; static int g_normalCalls = 0; static const C3DVector* g_normalAt = 0;
struct CWorldMap {
    C3DVector GetGroundFaceNormalAt(const C3DVector& pos) const { ++g_normalCalls; g_normalAt = &pos; return C3DVector(0.0f, 0.0f, g_normalZ); }
};
static CWorldMap g_map;
static int g_accelCalls = 0; static C3DVector g_accel(0, 0, 0);
struct CTCPhysicsStandard {
    bool m_gravity, m_air, m_onGround; float m_resistance;
    C3DVector Position;
    C3DVector Velocity;
    bool GetFeelsGravity() const { return m_gravity; }
    bool GetFeelsAirResistance() const { return m_air; }
    bool IsOnGround() const { return m_onGround; }
    float GetAirResistance() { return m_resistance; }
    CWorldMap* GetWorldMap() { return &g_map; }
    void ApplyAcceleration(const C3DVector& a) { ++g_accelCalls; g_accel = a; }
    void PerformForceUpdates();
};

void CTCPhysicsStandard::PerformForceUpdates()
{
    if (GetFeelsAirResistance()) {
        float keep = 1.0f - GetAirResistance();
        Velocity.X *= keep;
        Velocity.Y *= keep;
        Velocity.Z *= keep;
    }
    if (GetFeelsGravity()) {
        if (!IsOnGround() || GetWorldMap()->GetGroundFaceNormalAt(Position).Z < 0.98f
            || Velocity.GetLengthSquared() > g_epsilon_0129ba3c * g_epsilon_0129ba3c)
            ApplyAcceleration(C3DVector(0.0f, 0.0f, -0.12f));
    }
}

static bool Near(float a, float b) { float d = a - b; return d < 1e-5f && d > -1e-5f; }

int main()
{
    int bad = 0;
    const bool hasAir = true;
    CTCPhysicsStandard p;
    p.m_gravity = true; p.m_air = true; p.m_onGround = false; p.m_resistance = 0.25f;
    p.Position = C3DVector(1, 2, 3); p.Velocity = C3DVector(4, 8, -2);
    p.PerformForceUpdates();                            // airborne: resistance + gravity
    float k = hasAir ? 0.75f : 1.0f;
    if (!Near(p.Velocity.X, 4 * k) || !Near(p.Velocity.Y, 8 * k) || !Near(p.Velocity.Z, -2 * k)) bad |= 1;
    if (g_accelCalls != 1 || !Near(g_accel.Z, -0.12f) || g_accel.X != 0.0f || g_accel.Y != 0.0f || g_normalCalls != 0) bad |= 2;
    p.m_air = false; p.m_onGround = true; p.Velocity = C3DVector(0, 0, 0); g_normalZ = 1.0f;
    p.PerformForceUpdates();                            // resting on flat ground: no gravity
    if (g_accelCalls != 1 || g_normalCalls != 1 || g_normalAt != &p.Position) bad |= 4;
    g_normalZ = 0.5f;
    p.PerformForceUpdates();                            // steep slope: gravity
    if (g_accelCalls != 2) bad |= 8;
    g_normalZ = 1.0f; p.Velocity = C3DVector(0.01f, 0, 0);
    p.PerformForceUpdates();                            // flat but moving: gravity
    if (g_accelCalls != 3) bad |= 16;
    p.m_gravity = false;
    p.PerformForceUpdates();
    if (g_accelCalls != 3 || g_normalCalls != 3) bad |= 32;
    if (bad) { std::printf("008e5670_TEST FAIL\n"); return 1; }
    std::printf("008e5670_TEST PASS\n");
    return 0;
}