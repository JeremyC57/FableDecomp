// CFader::Update() -- advance the ramped parameter by one update period, count the time the output sits at zero, step a fade-in/fade-out by its per-update step (finishing in state 3/2), and reset the silent time once the output is non-zero
// Leaf: Update@CFader
// Grade: RELOCATION_MATCH (wave-2 sec6): the fade-in add as a stepwise local (float level = m_level; level +=
//   m_fadeInStep; m_level = level;) pins retail's m_level-first x87 load; += / a = b + a / a = a + b all canonicalise to
//   the step first
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C25F40, 163 bytes (`ret`). Descriptive names (no PDB covers the libpk audio
//   library): the manifest's "HeroWorksInBrothel_UpdateAnim" is propagated. Its caller 0x00C1204C updates the panel's
//   +0x7D8 fader.
// Callees: CRampedParam::Update(float) @ 0x00C0B7B0 (chunk 431), CFader::GetOutput @ 0x00C25F00 (twice).
// Shape: GetOutput's body is compiled earlier in the same TU (stand-in below, its retail body): VC7.1 knows it leaves
//   edx alone, so the zero register edx (the switch bias `sub eax,edx`, the `m_level = 0` store) survives the second
//   call for the final `m_silentTime = 0` store. The state switch has two cases (0 fade out, 1 fade in); the case-1
//   body is the fall-through of `dec eax ; jne`.
// Tried (2 grades, x87 sum-order lane gap rule): `m_level += m_fadeInStep`, `m_level = m_fadeInStep + m_level`.
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
    float GetOutput() const;                    // 0x00C25F00
    void  Update();
};

#pragma auto_inline(off)
// Same-TU body of 0x00C25F00 (landed with this chunk): its register use is what VC7.1 relies on above.
__declspec(noinline) float CFader::GetOutput() const
{
    return m_param.GetValue() * m_level;
}
#pragma auto_inline(on)

void CFader::Update()
{
    m_param.Update(m_timeStep);
    if (GetOutput() == 0.0f)
        m_silentTime += m_timeStep;
    switch (m_state) {
    case 0:
        if (m_level > m_fadeOutStep)
            m_level -= m_fadeOutStep;
        else {
            m_level = 0.0f;
            m_state = 2;
        }
        break;
    case 1:
        if (1.0f - m_level > m_fadeInStep)
            { float level = m_level; level += m_fadeInStep; m_level = level; }
        else {
            m_level = 1.0f;
            m_state = 3;
        }
        break;
    }
    if (GetOutput() != 0.0f)
        m_silentTime = 0.0f;
}