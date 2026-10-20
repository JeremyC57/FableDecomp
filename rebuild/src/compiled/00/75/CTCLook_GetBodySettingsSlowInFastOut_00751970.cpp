// CTCLook::GetBodySettingsSlowInFastOut(float& angle, float& velocity, float acceleration, float maxSpeed, float targetAngle) const -- when the body angle is off target, accelerate the velocity (clamped to +/- maxSpeed), compute the wrapped next angle, then snap the angle to the target and stop
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00751970, length 254 bytes (+ int3 padding), `ret 14h`; `this` is not read.
// The head twin GetHeadSettingsSlowInFastOut 0x00751820 keeps the overshoot test (sign of the old vs the new difference);
// in this body copy the second AngleDifference result is unused and the angle is always snapped to the target (retail
// stores the targetAngle argument, `mov ecx,[esp+20h]`, and zeroes the velocity).
// Callees: AngleDifference(float, float) @ 0x00A441E0 (`__stdcall`, float result; the second call's result is popped),
// _CIfmod @ 0x00BFEB28 (the fmod intrinsic on (double)angle, 1.0 from the double pool 0x01237F00).
// Constants: 1e-4f 0x0129BA3C (extern float, loaded not folded), 0.0f 0x0122DEDC, 1.0f 0x0122DED8.
// Shape: `(float)fabs(d) > eps` keeps d on the x87 stack; the int Sign (-1 stored, 1 when x >= 0) and the -maxSpeed temporary
// are homed in the dead `angle` argument slot, the wrapped angle in the dead `maxSpeed` slot; the clamp is an if / else-if
// pair through the velocity pointer.
#include <math.h>
#pragma intrinsic(fmod)
float __stdcall AngleDifference(float a, float b);   // 0x00A441E0
extern float g_epsilon_0129ba3c;                     // 0x0129BA3C (1e-4f)

inline int Sign(float x) { return x < 0.0f ? -1 : 1; }

struct CTCLook {
    void GetBodySettingsSlowInFastOut(float& angle, float& velocity, float acceleration, float maxSpeed, float targetAngle) const;   // FableWin.pdb ?GetBodySettingsSlowInFastOut@CTCLook@@ABEXAAM0MMM@Z
};

void CTCLook::GetBodySettingsSlowInFastOut(float& angle, float& velocity, float acceleration, float maxSpeed, float targetAngle) const
{
    float difference = AngleDifference(angle, targetAngle);
    if ((float)fabs(difference) > g_epsilon_0129ba3c) {
        velocity += Sign(difference) * acceleration;
        if (velocity < -maxSpeed)
            velocity = -maxSpeed;
        else if (velocity > maxSpeed)
            velocity = maxSpeed;
        float newAngle = angle + velocity;
        if (newAngle < 0.0f || newAngle >= 1.0f) {
            newAngle = (float)fmod((double)newAngle, 1.0);
            if (newAngle < 0.0f)
                newAngle += 1.0f;
        }
        AngleDifference(newAngle, targetAngle);
        angle = targetAngle;
        velocity = 0.0f;
    }
}