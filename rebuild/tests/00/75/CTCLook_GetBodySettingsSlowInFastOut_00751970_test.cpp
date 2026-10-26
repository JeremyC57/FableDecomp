// Behaviour test for CTCLook::GetBodySettingsSlowInFastOut (0x00751970).
#include <cstdio>
#include <math.h>
static int g_calls = 0;
static float AngleDifference(float a, float b) { ++g_calls; float d = b - a; if (d > 0.5f) d -= 1.0f; if (d < -0.5f) d += 1.0f; return d; }
static float g_epsilon_0129ba3c = 1e-4f;
inline int Sign(float x) { return x < 0.0f ? -1 : 1; }

class CTCLook { public:
};

static void GetBodySettingsSlowInFastOut(float* angle, float* velocity, float acceleration, float maxSpeed, float targetAngle)
{
    float difference = AngleDifference(*angle, targetAngle);
    if ((float)fabs(difference) > g_epsilon_0129ba3c) {
        *velocity += Sign(difference) * acceleration;
        if (*velocity < -maxSpeed)
            *velocity = -maxSpeed;
        else if (*velocity > maxSpeed)
            *velocity = maxSpeed;
        float newAngle = *angle + *velocity;
        if (newAngle < 0.0f || newAngle >= 1.0f) {
            newAngle = (float)fmod((double)newAngle, 1.0);
            if (newAngle < 0.0f)
                newAngle += 1.0f;
        }
        AngleDifference(newAngle, targetAngle);
        *angle = targetAngle;
        *velocity = 0.0f;
    }
}

int main()
{
    bool ok = true;
    float angle = 0.25f, velocity = 0.0f;
    GetBodySettingsSlowInFastOut(&angle, &velocity, 0.01f, 0.05f, 0.75f);
    ok = ok && angle == 0.75f && velocity == 0.0f && g_calls == 2;
    angle = 0.5f; velocity = 0.3f;                 // already on target: untouched, one lookup
    GetBodySettingsSlowInFastOut(&angle, &velocity, 0.01f, 0.05f, 0.5f);
    ok = ok && angle == 0.5f && velocity == 0.3f && g_calls == 3;
    if (!ok) { std::printf("00751970_TEST FAIL\n"); return 1; }
    std::printf("00751970_TEST PASS\n");
    return 0;
}