#ifndef FABLE_LIBPK_CFADER_H
#define FABLE_LIBPK_CFADER_H
// CFader -- libpk's 0x50-byte fader: a ramped parameter plus a fade level and fade-in / fade-out state (landed rows 0x00C25E20..
//   0x00C26010: CFader::CFader 0x00C26010, SetFadeInTime 0x00C25E20, SetFadeOutTime 0x00C25E50, SetMaxRate 0x00C25E80,
//   GetOutput 0x00C25F00, Update 0x00C25F40, SetValue 0x00C25FF0). Descriptive member names (libpk has no PDB or RTTI); the
//   layout is the one those rows model. CSoundEventManager holds one at +0x7D8 (constructed by 0x00C11D00, destroyed by 0x00C13470).
#include "rebuild_abi.h"
#include <stddef.h>

#ifdef FABLE_RETAIL_CRAMPEDPARAM_GETVALUE
// Opt-in (p25N, row 0x00C3A7D0 CRampedParam::ApplyGainToFrame): the angle wrap and the wrapped value read, inline. Descriptive names
// (libpk has no PDB or RTTI). WrapAngle is the landed 0x00C0B4C0 (`float __stdcall`, `ret 4`; this inline text compiles to that copy,
// cmpcomdat SAME); GetValue / GetTarget compile to the retail out-of-line copies 0x00C0B730 / 0x00C0B770 (SAME), and expanded they give
// the TU-mates 0x00C3A8F0 / 0x00C38510 their retail bytes. Constants: pi 0x0128F604, 2*pi 0x0128F608, -pi 0x012B1EB8.
static const float kPi = 3.14159265f;           // 0x0128F604
static const float kTwoPi = 6.28318531f;        // 0x0128F608
static const float kMinusPi = -3.14159265f;     // 0x012B1EB8
inline float __stdcall WrapAngle(float angle)   // 0x00C0B4C0
{
    if (angle >= kPi)
        return angle - kTwoPi;
    if (angle < kMinusPi)
        return angle + kTwoPi;
    return angle;
}
#endif
#ifdef FABLE_RETAIL_CRAMPEDPARAM_APPLYGAINTOFRAME
class CCodaFrame;                               // libpk/CCodaFrameStreamer.h
#endif

#ifdef FABLE_RETAIL_RAMPED_PARAM_SET_TARGET_PASS29
// Descriptive: these three libpk names are used by landed caller and callee rows. No GameCODA or engine PDB owns libpk.
float FABLE_STDCALL WrapAngle(float angle);
class CMatrix3x4;
void FABLE_FASTCALL InvertMatrix3x4(CMatrix3x4* out, const CMatrix3x4& matrix);
#endif
struct CRampedParam {                           // 0x34-byte ramped audio parameter
#ifdef FABLE_RETAIL_RAMPED_PARAM_SET_TARGET_PASS29
    void SetImmediate(float value);
    void SetTarget(float target,float rate,float time);
#endif
    float m_value;                              // +0x00 current value
    float m_target;                             // +0x04 target value
    float m_targetRate;                         // +0x08 rate on arrival
    float m_rate;                               // +0x0c current rate
    float m_10;                                 // +0x10
    float m_time;                               // +0x14 elapsed ramp time
    float m_duration;                           // +0x18 ramp duration
    float m_coef[5];                            // +0x1c Taylor coefficients of the ramp
    bool  m_bAngle;                             // +0x30 wrap the value into [-pi, pi)
#ifdef FABLE_RETAIL_CRAMPEDPARAM_GETVALUE
    float GetValue() const { return m_bAngle ? WrapAngle(m_value) : m_value; }       // out of line 0x00C0B730
    float GetTarget() const { return m_bAngle ? WrapAngle(m_target) : m_target; }    // out of line 0x00C0B770
#endif
#ifdef FABLE_RETAIL_CRAMPEDPARAM_APPLYGAINTOFRAME
    // Nested opt-in (p25N, row 0x00C3A7D0): the ramp step the landed callers bind (?Update@CRampedParam@@QAEXM@Z: CFader::Update
    // 0x00C25F40, CAmbientCube::UpdateLevel 0x00C45980) and this row. Declarations only.
    void  Update(float timeStep);               // 0x00C0B7B0
    void  ApplyGainToFrame(CCodaFrame& frame, unsigned long sampleRate);   // 0x00C3A7D0
#endif
};

struct CFader {
    float        m_timeStep;                    // +0x00 update period
    float        m_level;                       // +0x04 fade level 0..1
    CRampedParam m_param;                       // +0x08
    float        m_silentTime;                  // +0x3C time spent at a zero output
    float        m_maxRate;                     // +0x40
    float        m_fadeInStep;                  // +0x44 level gained per update while fading in
    float        m_fadeOutStep;                 // +0x48 level lost per update while fading out
    long         m_state;                       // +0x4C 0 fading out, 1 fading in, 2 silent, 3 full
    float GetOutput() const;                    // 0x00C25F00
};
FABLE_STATIC_ASSERT_AT(33, sizeof(CRampedParam) == 0x34);
FABLE_STATIC_ASSERT_AT(34, offsetof(CFader, m_silentTime) == 0x3c);
FABLE_STATIC_ASSERT_AT(35, sizeof(CFader) == 0x50);
#endif
