// CFader::CFader(float timeStep) -- set the update period, start silent (level 0, state 2) with an unwrapped parameter, then reset the parameter to 0 and give the max rate and both fade times their 250 default
// Leaf: ??0CFader@@QAE@M@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C26010, 72 bytes (`ret 4`, returns this). Descriptive names (no PDB covers the
//   libpk audio library): the manifest's "CWideString_ConstructFromResourceId" is propagated. The fader is the
//   0x50-byte member the panel keeps at +0x7D8.
// Callees: CFader::SetValue @ 0x00C25FF0 (`m_param.SetTarget(value, 0, time)`, landed as "ConstructString"),
//   SetMaxRate @ 0x00C25E80 (landed as "AddExclusiveObserver"), SetFadeOutTime @ 0x00C25E50, SetFadeInTime @ 0x00C25E20.
// Shape: the member stores follow declaration order (+0, +4, the parameter's angle flag +0x38, +0x3C, +0x4C) with the
//   two zero pushes for SetValue scheduled between them. SetValue gets ecx untouched; after it ecx is reloaded once
//   (`mov ecx, esi`) and the three 250 setters are called back to back WITHOUT a reload: VC7.1 knows those three
//   bodies preserve ecx because they are compiled earlier in the same TU (retail's TU holds them at 0x00C25E20..
//   0x00C25E80), so they sit above the constructor under `auto_inline(off)`.
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
    CRampedParam() : m_bAngle(false) {}
    void  Update(float timeStep);               // 0x00C0B7B0
    void  SetTarget(float target, float rate, float time);   // 0x00C0B570
};

struct CFader {                                 // 0x50-byte fader (the panel holds one at +0x7D8)
    float        m_timeStep;                    // +0x00 update period
    float        m_level;                       // +0x04 fade level 0..1
    CRampedParam m_param;                       // +0x08
    float        m_silentTime;                  // +0x3C time spent at a zero output
    float        m_maxRate;                     // +0x40
    float        m_fadeInStep;                  // +0x44 level gained per update while fading in
    float        m_fadeOutStep;                 // +0x48 level lost per update while fading out
    long         m_state;                       // +0x4C 0 fading out, 1 fading in, 2 silent, 3 full
    explicit CFader(float timeStep);
    void SetFadeInTime(float time);             // 0x00C25E20
    void SetFadeOutTime(float time);            // 0x00C25E50
    void SetMaxRate(float rate);                // 0x00C25E80
    void SetValue(float value, float time);     // 0x00C25FF0
};

#pragma auto_inline(off)
// Same-TU bodies (retail 0x00C25E20 / 0x00C25E50 / 0x00C25E80): compiled above the constructor so VC7.1 knows they
// leave ecx alone.
void CFader::SetFadeInTime(float time)
{
    if (time < m_timeStep)
        m_fadeInStep = 1.0f;
    else
        m_fadeInStep = m_timeStep / time;
}

void CFader::SetFadeOutTime(float time)
{
    if (time < m_timeStep)
        m_fadeOutStep = 1.0f;
    else
        m_fadeOutStep = m_timeStep / time;
}

void CFader::SetMaxRate(float rate)
{
    m_maxRate = rate;
}
#pragma auto_inline(on)

CFader::CFader(float timeStep)
    : m_timeStep(timeStep), m_level(0.0f), m_silentTime(0.0f), m_state(2)
{
    SetValue(0.0f, 0.0f);
    SetMaxRate(250.0f);
    SetFadeOutTime(250.0f);
    SetFadeInTime(250.0f);
}