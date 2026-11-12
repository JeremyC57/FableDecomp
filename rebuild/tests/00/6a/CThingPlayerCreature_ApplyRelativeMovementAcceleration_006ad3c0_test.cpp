// Behaviour test for CThingPlayerCreature::ApplyRelativeMovementAcceleration (0x006AD3C0): a self-contained copy of the model.
#include <cstdio>
#include <cmath>
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector& operator+=(const C3DVector& v) { X += v.X; Y += v.Y; Z += v.Z; return *this; }
    void RotateXY(float a) { float c = (float)cos(a), s = (float)sin(a); float x = X * c - Y * s, y = X * s + Y * c; X = x; Y = y; }
    void AccurateNormalise() { float m = (float)sqrt(X * X + Y * Y + Z * Z); if (m > 0.0f) { X /= m; Y /= m; Z /= m; } }
};
static const float DELTA_NOTIONAL_ZERO = 0.0001f;
struct Entry { int key; C3DVector sum; long count; bool used; };
struct Map {
    Entry e[8];
    Map() { for (int i = 0; i < 8; ++i) e[i].used = false; }
    Entry* find(int key) { for (int i = 0; i < 8; ++i) if (e[i].used && e[i].key == key) return &e[i]; return 0; }
    void insert(int key, const C3DVector& v, long n) { for (int i = 0; i < 8; ++i) if (!e[i].used) { e[i].used = true; e[i].key = key; e[i].sum = v; e[i].count = n; return; } }
};
struct Creature {
    bool usingFreeCamera; float renderAngle, controlAngle;
    float modeSpeed; float maxAcceleration; long idleCounter; Map components;
    C3DVector facing; C3DVector seenDirection, seenFacing; int seenType; int maxAccelerationCalls;
    float GetMovementSpeed(const C3DVector& direction, const C3DVector& f, int type) { seenDirection = direction; seenFacing = f; seenType = type; return modeSpeed; }
    float GetMaxAcceleration() { ++maxAccelerationCalls; return maxAcceleration; }
    void ApplyRelativeMovementAcceleration(const C3DVector& acceleration, int action)
    {
        float scale = 1.0f;
        C3DVector direction = acceleration;
        float angle;
        if (usingFreeCamera) angle = renderAngle; else angle = controlAngle;
        direction.RotateXY(angle);
        direction.AccurateNormalise();
        float speed = GetMovementSpeed(direction, facing, 0);
        if (speed > DELTA_NOTIONAL_ZERO)
            scale = speed / GetMaxAcceleration();
        Entry* it = components.find(action);
        if (it != 0) { it->sum += acceleration * scale; ++it->count; }
        else components.insert(action, acceleration * scale, 1);
        idleCounter = 0;
    }
};
static bool Near(float a, float b) { float d = a - b; if (d < 0) d = -d; return d < 0.0005f; }
int main()
{
    bool ok = true;
    Creature c;
    c.usingFreeCamera = false; c.renderAngle = 0.0f; c.controlAngle = 1.5707963f; c.modeSpeed = 3.0f; c.maxAcceleration = 6.0f;
    c.idleCounter = 77; c.facing = C3DVector(0.0f, 1.0f, 0.0f); c.maxAccelerationCalls = 0;
    // first request for action 5: a new entry, scaled by 3/6, count 1; the direction handed to the mode is rotated a quarter turn and unit length
    c.ApplyRelativeMovementAcceleration(C3DVector(2.0f, 0.0f, 4.0f), 5);
    Entry* e = c.components.find(5);
    if (!e || e->count != 1 || !Near(e->sum.X, 1.0f) || !Near(e->sum.Y, 0.0f) || !Near(e->sum.Z, 2.0f)) ok = false;
    if (c.idleCounter != 0 || c.maxAccelerationCalls != 1 || c.seenType != 0) ok = false;
    float m = c.seenDirection.X * c.seenDirection.X + c.seenDirection.Y * c.seenDirection.Y + c.seenDirection.Z * c.seenDirection.Z;
    if (!Near(m, 1.0f) || !Near(c.seenDirection.X, 0.0f) || c.seenDirection.Y <= 0.0f || !Near(c.seenFacing.Y, 1.0f)) ok = false;
    // second request for the same action accumulates and counts
    c.idleCounter = 9;
    c.ApplyRelativeMovementAcceleration(C3DVector(2.0f, 2.0f, 0.0f), 5);
    e = c.components.find(5);
    if (!e || e->count != 2 || !Near(e->sum.X, 2.0f) || !Near(e->sum.Y, 1.0f) || !Near(e->sum.Z, 2.0f) || c.idleCounter != 0) ok = false;
    // a negligible speed leaves the scale at 1 and does not ask for the maximum acceleration; the free camera uses the render angle
    c.modeSpeed = 0.00005f; c.usingFreeCamera = true;
    c.ApplyRelativeMovementAcceleration(C3DVector(1.0f, 0.0f, 0.0f), 2);
    e = c.components.find(2);
    if (!e || e->count != 1 || !Near(e->sum.X, 1.0f) || !Near(e->sum.Y, 0.0f) || c.maxAccelerationCalls != 2) ok = false;
    if (!Near(c.seenDirection.X, 1.0f) || !Near(c.seenDirection.Y, 0.0f)) ok = false;
    if (c.components.find(5)->count != 2 || c.components.find(3) != 0) ok = false;
    if (!ok) { std::printf("006ad3c0_TEST FAIL\n"); return 1; }
    std::printf("006ad3c0_TEST PASS\n");
    return 0;
}