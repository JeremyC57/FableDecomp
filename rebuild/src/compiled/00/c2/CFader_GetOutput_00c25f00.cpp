// CFader::GetOutput() const -- the fader's output: the ramped parameter's value (wrapped into [-pi, pi) for an angle parameter) times the fade level
// Leaf: GetOutput
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C25F00, 56 bytes (`ret`, result on the FPU stack). Descriptive names (no PDB
//   covers the libpk audio library): the manifest's "Radius_ApplyModifiers" is propagated. Callers: the fader's Update
//   0x00C25F40 (twice) and the panel's tail-jump forwarder 0x00C11700.
// Callees: none; CRampedParam::GetValue (out-of-line copy 0x00C0B730) is inlined in its register form (`fcom` on the
//   loaded value, no reload), and VC7.1 tail-duplicates the `fmul [ecx+4]` into the `>= pi` arm.
struct CRampedParam {                           // 0x34-byte ramped audio parameter (chunk 431 model)
    float m_value;                              // +0x00 current value
    float m_target;                             // +0x04 target value
    float m_targetRate;                         // +0x08 rate on arrival
    float m_rate;                               // +0x0c current rate
    float m_10;                                 // +0x10
    float m_time;                               // +0x14 elapsed ramp time
    float m_duration;                           // +0x18 ramp duration
    float m_coef[5];                            // +0x1c Taylor coefficients of the ramp
    bool  m_bAngle;                             // +0x30 wrap the value into [-pi, pi)
    void  Update(float timeStep);               // 0x00C0B7B0
    void  SetTarget(float target, float rate, float time);   // 0x00C0B570
    float GetValue() const;                     // out-of-line copy 0x00C0B730
};

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

struct CFader {                                 // 0x50-byte fader (the panel holds one at +0x7D8)
    float        m_timeStep;                    // +0x00 update period
    float        m_level;                       // +0x04 fade level 0..1
    CRampedParam m_param;                       // +0x08
    float        m_silentTime;                  // +0x3C time spent at a zero output
    float        m_maxRate;                     // +0x40
    float        m_fadeInStep;                  // +0x44 level gained per update while fading in
    float        m_fadeOutStep;                 // +0x48 level lost per update while fading out
    long         m_state;                       // +0x4C 0 fading out, 1 fading in, 2 silent, 3 full
    float GetOutput() const;
};

float CFader::GetOutput() const
{
    return m_param.GetValue() * m_level;
}