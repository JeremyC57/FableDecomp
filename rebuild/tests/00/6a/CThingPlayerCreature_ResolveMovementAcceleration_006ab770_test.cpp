// behaviour test for CThingPlayerCreature::ResolveMovementAcceleration (0x006AB770): clamp to the max speed, damp when not player-driven
#include <cstdio>
#include <cmath>
inline float Magnitude(float x, float y, float z) { return (float)std::sqrt(x * x + y * y + z * z); }
class C3DVector { public: float X, Y, Z; float Length() const { return Magnitude(X, Y, Z); } void operator*=(float s) { X *= s; Y *= s; Z *= s; } };
class CThingPlayerCreature { public:
    unsigned char MovedByPlayer : 1; C3DVector MovementAcceleration; float m_max;
    float GetCurrentMaxMovementSpeed() { return m_max; }
    void ResolveMovementAcceleration()
    {
        float maxSpeed = GetCurrentMaxMovementSpeed();
        float length = MovementAcceleration.Length();
        if (length > maxSpeed) MovementAcceleration *= maxSpeed / length;
        if (!MovedByPlayer) MovementAcceleration *= 0.49f;
    } };
static bool Near(float a, float b) { float d = a - b; return d < 1e-4f && d > -1e-4f; }
int main()
{
    CThingPlayerCreature p; p.m_max = 5.0f; p.MovedByPlayer = 1;
    p.MovementAcceleration.X = 6; p.MovementAcceleration.Y = 8; p.MovementAcceleration.Z = 0;   // length 10 -> 5
    p.ResolveMovementAcceleration();
    int bad = !Near(p.MovementAcceleration.X, 3.0f) || !Near(p.MovementAcceleration.Y, 4.0f) || p.MovementAcceleration.Z != 0.0f;
    p.MovedByPlayer = 0; p.ResolveMovementAcceleration();                                        // within max: damped only
    if (!Near(p.MovementAcceleration.X, 1.47f) || !Near(p.MovementAcceleration.Y, 1.96f)) bad = 1;
    if (bad) { std::printf("006ab770_TEST FAIL\n"); return 1; }
    std::printf("006ab770_TEST PASS\n");
    return 0;
}