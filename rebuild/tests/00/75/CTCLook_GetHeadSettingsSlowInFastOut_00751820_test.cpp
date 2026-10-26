#include <cstdio>
#include <cmath>
// Behaviour test for CTCLook::GetHeadSettingsSlowInFastOut (0x00751820): same body as src/, with AngleDifference
// modelled as the wrapped (b - a) in [-0.5, 0.5) of a unit-turn angle.
float g_epsilon_0129ba3c = 1e-4f;
static float AngleDifference(float a, float b)
{
    float d = b - a;
    while (d >= 0.5f) d -= 1.0f;
    while (d < -0.5f) d += 1.0f;
    return d;
}
inline int Sign(float x) { return x < 0.0f ? -1 : 1; }
class CTCLook { public:
    void GetHeadSettingsSlowInFastOut(float& angle, float& velocity, float acceleration, float maxSpeed, float targetAngle,
                                      float maxLeft, float maxRight) const;
};
void CTCLook::GetHeadSettingsSlowInFastOut(float& angle, float& velocity, float acceleration, float maxSpeed,
                                           float targetAngle, float maxLeft, float maxRight) const
{
    float difference = AngleDifference(angle, targetAngle);
    if ((float)std::fabs(difference) > g_epsilon_0129ba3c) {
        velocity += Sign(difference) * acceleration;
        if (velocity < -maxSpeed)
            velocity = -maxSpeed;
        else if (velocity > maxSpeed)
            velocity = maxSpeed;
        float newAngle = angle + velocity;
        float newDifference = AngleDifference(newAngle, targetAngle);
        if (Sign(difference) != Sign(newDifference)) {
            angle = targetAngle;
            velocity = 0.0f;
        } else
            angle += velocity;
    }
    if (angle > maxRight || -maxLeft > angle) {
        if (angle > maxRight)
            angle = maxRight - g_epsilon_0129ba3c;
        else
            angle = -(maxLeft - g_epsilon_0129ba3c);
        velocity = 0.0f;
    }
}
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-5f; }
int main()
{
    int fails = 0;
    CTCLook look;
    // Accelerate towards a target ahead: velocity grows by the acceleration, angle takes the step.
    float angle = 0.0f, velocity = 0.0f;
    look.GetHeadSettingsSlowInFastOut(angle, velocity, 0.01f, 0.05f, 0.2f, 0.3f, 0.3f);
    if (!Near(velocity, 0.01f) || !Near(angle, 0.01f)) ++fails;
    // Velocity clamps at maxSpeed.
    velocity = 0.045f; angle = 0.0f;
    look.GetHeadSettingsSlowInFastOut(angle, velocity, 0.01f, 0.05f, 0.2f, 0.3f, 0.3f);
    if (!Near(velocity, 0.05f) || !Near(angle, 0.05f)) ++fails;
    // Negative direction clamps at -maxSpeed.
    velocity = -0.045f; angle = 0.0f;
    look.GetHeadSettingsSlowInFastOut(angle, velocity, 0.01f, 0.05f, -0.2f, 0.3f, 0.3f);
    if (!Near(velocity, -0.05f) || !Near(angle, -0.05f)) ++fails;
    // Overshoot: snap to target, stop.
    velocity = 0.04f; angle = 0.18f;
    look.GetHeadSettingsSlowInFastOut(angle, velocity, 0.01f, 0.05f, 0.2f, 0.3f, 0.3f);
    if (!Near(angle, 0.2f) || velocity != 0.0f) ++fails;
    // On target: nothing moves.
    velocity = 0.03f; angle = 0.1f;
    look.GetHeadSettingsSlowInFastOut(angle, velocity, 0.01f, 0.05f, 0.1f, 0.3f, 0.3f);
    if (!Near(angle, 0.1f) || !Near(velocity, 0.03f)) ++fails;
    // Right limit: past maxRight -> maxRight - eps, stopped.
    velocity = 0.05f; angle = 0.29f;
    look.GetHeadSettingsSlowInFastOut(angle, velocity, 0.01f, 0.05f, 0.45f, 0.3f, 0.3f);
    if (!Near(angle, 0.3f - 1e-4f) || velocity != 0.0f) ++fails;
    // Left limit: below -maxLeft -> -(maxLeft - eps), stopped.
    velocity = -0.05f; angle = -0.19f;
    look.GetHeadSettingsSlowInFastOut(angle, velocity, 0.01f, 0.05f, -0.45f, 0.2f, 0.3f);
    if (!Near(angle, -(0.2f - 1e-4f)) || velocity != 0.0f) ++fails;
    if (fails) { std::printf("00751820_TEST FAIL %d\n", fails); return 1; }
    std::printf("00751820_TEST PASS\n");
    return 0;
}