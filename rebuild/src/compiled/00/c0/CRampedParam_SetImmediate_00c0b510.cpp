// CRampedParam::SetImmediate(float value) -- jump a ramped audio parameter straight to a value: wrap it into [-pi, pi) when the parameter is an angle, make it the current value, the target and the constant term, and zero the ramp (rates, time, duration, higher coefficients)
// Leaf: SetImmediate
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C0B510, 91 bytes (`ret 4`). Descriptive names (no PDB covers this library):
//   the manifest's "Matrix_SetIdentityRotation" is propagated. The 0x34-byte element is the slot of the landed
//   parameter block constructor 0x00C0B200 (sixteen at +4, named "G0".."G15"; its inline reset stores 1.0 in the same
//   three fields and zeros in the same order); SetTarget 0x00C0B570 calls this when the ramp time is not positive.
// Constants: pi @ 0x0128F604, 2*pi @ 0x0128F608, -pi @ 0x012B1EB8.
// Shape: the wrap is inlined in register form (`fcom`, no reload); the zero register edx (also the flag-compare
//   operand) feeds the nine zero stores, interleaved by the scheduler with the three `fst`.
struct CRampedParam {                           // 0x34 bytes
    float m_value;                              // +0x00 current value
    float m_target;                             // +0x04 target value
    float m_targetRate;                         // +0x08 rate on arrival
    float m_rate;                               // +0x0c current rate
    float m_10;                                 // +0x10
    float m_time;                               // +0x14 elapsed ramp time
    float m_duration;                           // +0x18 ramp duration
    float m_coef[5];                            // +0x1c Taylor coefficients of the ramp (value, rate, 2nd..4th derivative)
    bool  m_bAngle;                             // +0x30 wrap the value into [-pi, pi)
    void SetImmediate(float value);
};

static const float kPi = 3.14159265f;           // 0x0128F604
static const float kTwoPi = 6.28318531f;        // 0x0128F608
static const float kMinusPi = -3.14159265f;     // 0x012B1EB8

void CRampedParam::SetImmediate(float value)
{
    if (m_bAngle) {
        if (value >= kPi)
            value -= kTwoPi;
        else if (value < kMinusPi)
            value += kTwoPi;
    }
    m_target = value;
    m_value = value;
    m_coef[0] = value;
    m_duration = 0.0f;
    m_time = 0.0f;
    m_coef[4] = 0.0f;
    m_coef[3] = 0.0f;
    m_10 = 0.0f;
    m_coef[2] = 0.0f;
    m_rate = 0.0f;
    m_coef[1] = 0.0f;
    m_targetRate = 0.0f;
}