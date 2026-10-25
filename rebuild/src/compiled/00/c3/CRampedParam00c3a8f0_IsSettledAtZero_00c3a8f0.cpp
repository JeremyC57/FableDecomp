// CRampedParam_00c3a8f0::IsSettledAtZero() -- report whether a ramped audio parameter has finished its ramp (elapsed time equal to the duration) and rests at zero (its current value, wrapped into [-pi, pi) when it is an angle, equals 0.0f): 1 if so, else 0
// Leaf: ?IsSettledAtZero@CRampedParam_00c3a8f0@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C3A8F0, 89 bytes (+ int3 padding), __fastcall, plain `ret`, long result.
// Library: libpk audio (no PDB/RTTI); descriptive names. The 0x34-byte ramped parameter of the landed 0x00C0B510
//   SetImmediate / 0x00C0B7B0 Update (+0x00 value, +0x14 elapsed time, +0x18 duration, +0x30 angle flag).
//   The manifest's "AnimationCurve_CheckBoundaryTransition" is propagated.
// Constants: pi @ 0x0128F604, 2*pi @ 0x0128F608, -pi @ 0x012B1EB8, 0.0f @ 0x0122DEDC.
// Shape: `m_time != m_duration` loads the right operand first (`fld [+18h] ; fld [+14h] ; fucompp`); the inline GetValue
//   wraps in register form and joins; `value == 0.0f` loads the constant then `fxch`; `if (== ) return 1; return 0;` keeps the 1 exit on the
//   fall-through and shares the trailing zero exit with the first test.
static const float kPi = 3.14159265f;           // 0x0128F604
static const float kTwoPi = 6.28318531f;        // 0x0128F608
static const float kMinusPi = -3.14159265f;     // 0x012B1EB8

struct CRampedParam_00c3a8f0 {                  // 0x34 bytes
    float m_value;                              // +0x00 current value
    float m_target;                             // +0x04 target value
    float m_targetRate;                         // +0x08 rate on arrival
    float m_rate;                               // +0x0c current rate
    float m_10;                                 // +0x10
    float m_time;                               // +0x14 elapsed ramp time
    float m_duration;                           // +0x18 ramp duration
    float m_coef[5];                            // +0x1c ramp coefficients
    bool  m_bAngle;                             // +0x30 wrap the value into [-pi, pi)
    float GetValue() const
    {
        float value = m_value;
        if (m_bAngle) {
            if (value >= kPi)
                return value - kTwoPi;
            if (value < kMinusPi)
                return value + kTwoPi;
        }
        return value;
    }
    long IsSettledAtZero() const;
};

long CRampedParam_00c3a8f0::IsSettledAtZero() const
{
    if (m_time != m_duration)
        return 0;
    if (GetValue() == 0.0f)
        return 1;
    return 0;
}