#ifndef FABLE_LIBPK_CFADER_H
#define FABLE_LIBPK_CFADER_H
// CFader -- libpk's 0x50-byte fader: a ramped parameter plus a fade level and fade-in / fade-out state (landed rows 0x00C25E20..
//   0x00C26010: CFader::CFader 0x00C26010, SetFadeInTime 0x00C25E20, SetFadeOutTime 0x00C25E50, SetMaxRate 0x00C25E80,
//   GetOutput 0x00C25F00, Update 0x00C25F40, SetValue 0x00C25FF0). Descriptive member names (libpk has no PDB or RTTI); the
//   layout is the one those rows model. CSoundEventManager holds one at +0x7D8 (constructed by 0x00C11D00, destroyed by 0x00C13470).
#include "rebuild_abi.h"
#include <stddef.h>

struct CRampedParam {                           // 0x34-byte ramped audio parameter
    float m_value;                              // +0x00 current value
    float m_target;                             // +0x04 target value
    float m_targetRate;                         // +0x08 rate on arrival
    float m_rate;                               // +0x0c current rate
    float m_10;                                 // +0x10
    float m_time;                               // +0x14 elapsed ramp time
    float m_duration;                           // +0x18 ramp duration
    float m_coef[5];                            // +0x1c Taylor coefficients of the ramp
    bool  m_bAngle;                             // +0x30 wrap the value into [-pi, pi)
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
FABLE_STATIC_ASSERT(sizeof(CRampedParam) == 0x34);
FABLE_STATIC_ASSERT(offsetof(CFader, m_silentTime) == 0x3c);
FABLE_STATIC_ASSERT(sizeof(CFader) == 0x50);
#endif
