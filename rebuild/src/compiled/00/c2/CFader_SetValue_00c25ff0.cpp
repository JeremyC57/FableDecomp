// CFader::SetValue(float value, float time) -- retargets the fader's ramped parameter (+0x8) to the value over the time with a zero arrival rate
// Leaf: ?SetValue@CFader@@QAEXMM@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C25FF0, 23 bytes: mov eax,[esp+8] ; mov edx,[esp+4] ; push eax ; push 0 ;
//   push edx ; add ecx,8 ; call 0x00C0B570 ; ret 8
// Naming (sweep1, 2026-10-25): was landed as a fake `extern "C" __fastcall` ConstructString(CWideString* self, int edx,
//   long a, long b) stand-in (catalog NGameText::ConstructString, propagated); descriptive (no PDB covers the libpk
//   library). The landed CFader::CFader 0x00C26010 calls it first (0x00C2602D) and declares it
//   `void SetValue(float value, float time); // 0x00C25FF0` (`m_param.SetTarget(value, 0, time)`; FINDINGS chunk
//   435, agent435 NOTES); its callee 0x00C0B570 is CRampedParam::SetTarget (named by the landed
//   CRampedParam::SetImmediate 0x00C0B510). Now the plain thiscall member it is (ecx = this, two stack words, `ret 8`).
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
    void SetValue(float value, float time);
};

void CFader::SetValue(float value, float time)
{
    m_param.SetTarget(value, 0, time);
}
