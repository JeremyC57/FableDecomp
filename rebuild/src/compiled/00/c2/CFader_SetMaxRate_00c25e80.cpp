// CFader::SetMaxRate(float rate) -- stores the fader's maximum rate at +0x40
// Leaf: ?SetMaxRate@CFader@@QAEXM@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C25E80, 10 bytes: mov eax,[esp+4] ; mov [ecx+40h],eax ; ret 4
// Naming (sweep1, 2026-10-25): was landed as the global module "AddExclusiveObserver" over a C_00c25e80::M stand-in
//   (catalog _global::SetMember_dword0x40_00c25e80, manifest CObservable::AddExclusiveObserver, propagated; its "// Leaf:
//   AddExclusiveObserver" named no symbol of its object); descriptive (no PDB covers the libpk library). The landed
//   CFader::CFader 0x00C26010 calls it (0x00C26039, SetMaxRate(250.0f)) and holds this very body as its same-TU
//   `void CFader::SetMaxRate(float rate) { m_maxRate = rate; }` (FINDINGS chunk 435, agent435 NOTES); the other caller
//   0x00C1174C runs it on the sound-event manager's +0x7D8 fader.
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
    void  SetTarget(float target, float rate, float time);   // 0x00C0B570
};

struct CFader {                                 // 0x50-byte fader (the sound-event manager holds one at +0x7D8)
    float        m_timeStep;                    // +0x00 update period
    float        m_level;                       // +0x04 fade level 0..1
    CRampedParam m_param;                       // +0x08
    float        m_silentTime;                  // +0x3C time spent at a zero output
    float        m_maxRate;                     // +0x40
    float        m_fadeInStep;                  // +0x44 level gained per update while fading in
    float        m_fadeOutStep;                 // +0x48 level lost per update while fading out
    long         m_state;                       // +0x4C 0 fading out, 1 fading in, 2 silent, 3 full
    void SetMaxRate(float rate);
};

void CFader::SetMaxRate(float rate)
{
    m_maxRate = rate;
}
