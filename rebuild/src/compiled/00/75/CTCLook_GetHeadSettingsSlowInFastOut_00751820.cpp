// CTCLook::GetHeadSettingsSlowInFastOut(float& angle, float& velocity, float acceleration, float maxSpeed, float targetAngle, float maxLeft, float maxRight) const -- when the head angle is off target, accelerate the velocity (clamped to +/- maxSpeed); if the step would overshoot the target (the difference changes sign) snap to it and stop, else take the step; finally keep the angle inside (-maxLeft, maxRight), stopping at the limit
// Leaf: GetHeadSettingsSlowInFastOut
// Naming wave 18: the shared const float 1e-4f at 0x0129BA3C is ?DELTA_NOTIONAL_ZERO@@3MB (raw Ego_r public + typed data record; was EPSILON).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00751820, 324 bytes (+ int3 padding), `ret 1Ch`; `this` is not read.
//   Ego_r.pdb: `?GetHeadSettingsSlowInFastOut@CTCLook@@ABEXAAM0MMMMM@Z`.
// Callees: AngleDifference(float, float) @ 0x00A441E0 (`__stdcall`, float result).
// Constants: 1e-4f 0x0129BA3C (extern float, loaded not folded), 0.0f 0x0122DEDC.
// Shape: the new difference is a named local (kept on the x87 stack; the old one is re-read from its slot for the Sign
//   compare after the call); otherwise as the landed body twin 0x00751970 (Sign homed in the dead `angle` slot, the -maxSpeed temporary, the if /
//   else-if velocity clamp); the overshoot test compares Sign of the old and the new difference; the limit clamp
//   re-tests `angle > maxRight` inside the combined range test.
#include <math.h>
float __stdcall AngleDifference(float a, float b);   // 0x00A441E0
extern const float DELTA_NOTIONAL_ZERO;     // 0x0129BA3C (1e-4f)

inline int Sign(float x) { return x < 0.0f ? -1 : 1; }

class CTCLook { public:
    void GetHeadSettingsSlowInFastOut(float& angle, float& velocity, float acceleration, float maxSpeed, float targetAngle,
                                      float maxLeft, float maxRight) const;
};

void CTCLook::GetHeadSettingsSlowInFastOut(float& angle, float& velocity, float acceleration, float maxSpeed,
                                           float targetAngle, float maxLeft, float maxRight) const
{
    float difference = AngleDifference(angle, targetAngle);
    if ((float)fabs(difference) > DELTA_NOTIONAL_ZERO) {
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
            angle = maxRight - DELTA_NOTIONAL_ZERO;
        else
            angle = -(maxLeft - DELTA_NOTIONAL_ZERO);
        velocity = 0.0f;
    }
}