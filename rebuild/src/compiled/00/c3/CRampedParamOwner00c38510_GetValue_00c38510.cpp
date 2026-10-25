// CRampedParamOwner_00c38510::GetValue() -- read the owner's ramped audio parameter (member at +0x0c): its current value, wrapped once into [-pi, pi) when the parameter is an angle
// Leaf: ?GetValue@CRampedParamOwner_00c38510@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C38510, 50 bytes (+ int3 padding), __fastcall, float result on the x87 stack.
// Library: libpk audio (no PDB/RTTI); descriptive names. The 0x34-byte ramped parameter is the landed CRampedParam
//   (0x00C0B510 SetImmediate / 0x00C0B570 SetTarget: +0x00 current value, +0x30 angle flag); here it is embedded at +0x0c,
//   so the flag reads at +0x3c. The manifest's "GetNormalizedAngleValue" is propagated.
// Constants: pi @ 0x0128F604, 2*pi @ 0x0128F608, -pi @ 0x012B1EB8.
// Shape: the parameter's inline GetValue() with early returns in register form (`fcom`, no reload; each return its own `ret`).
static const float kPi = 3.14159265f;           // 0x0128F604
static const float kTwoPi = 6.28318531f;        // 0x0128F608
static const float kMinusPi = -3.14159265f;     // 0x012B1EB8

struct CRampedParam_00c38510 {                  // 0x34 bytes
    float m_value;                              // +0x00 current value
    float m_target;                             // +0x04 target value
    float m_rates[10];                          // +0x08 rates, time, duration, coefficients
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
};

struct CRampedParamOwner_00c38510 {
    unsigned char _pad_0x00[0x0c];
    CRampedParam_00c38510 m_param;              // +0x0c
    float GetValue() const;
};

float CRampedParamOwner_00c38510::GetValue() const
{
    return m_param.GetValue();
}