// _global::MultiplyMaskedParams(const CRampedParam* params, unsigned long mask, float value, unsigned long count) -- multiply value by the current value of every ramped parameter whose bit is set in mask (angle parameters wrapped into [-pi, pi)), walking at most count parameters and stopping early once the mask is exhausted or the product is zero
// Leaf: MultiplyMaskedParams
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C0B180, 123 bytes, `__fastcall` (params in ecx, mask in edx, value and count
//   on the stack, `ret 8`, result in st0). Descriptive name: the manifest's "Render_ApplyAlphaValueMask" is propagated.
//   The only caller is the parameter block's gain query 0x00C0B420 (the sixteen 0x34-byte ramped parameters at +4).
// Constants: 0.0 @ 0x0122DEDC, pi @ 0x0128F604, 2*pi @ 0x0128F608, -pi @ 0x012B1EB8.
// Shape: CRampedParam::GetValue (its out-of-line copy is 0x00C0B730) inlined with the register-form wrap; the product
//   stays on the x87 stack, the zero test is `fld 0 ; fld st1 ; fucompp` at the top and the bottom of the loop.
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
    float GetValue() const;
};

static const float kZero = 0.0f;                // 0x0122DEDC
static const float kPi = 3.14159265f;           // 0x0128F604
static const float kTwoPi = 6.28318531f;        // 0x0128F608
static const float kMinusPi = -3.14159265f;     // 0x012B1EB8

inline float CRampedParam::GetValue() const
{
    float value = m_value;
    if (m_bAngle) {
        if (value >= kPi)
            value -= kTwoPi;
        else if (value < kMinusPi)
            value += kTwoPi;
    }
    return value;
}

float __fastcall MultiplyMaskedParams(const CRampedParam* params, unsigned long mask, float value, unsigned long count)
{
    while (value != kZero && mask != 0 && count != 0) {
        if (mask & 1)
            value *= params->GetValue();
        --count;
        mask >>= 1;
        ++params;
    }
    return value;
}