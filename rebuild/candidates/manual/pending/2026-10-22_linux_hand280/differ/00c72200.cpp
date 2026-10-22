// CAudioSource::UpdateGainState(const CAudioGainEnvelope* envelope, bool restart, __int64 now) -- step the source's gain envelope state machine (wait, attack ramp, decay towards the sustain level, sustain, release) for the current audio time, rescheduling the next transition time and stopping the source once the release has faded out; true when the gain changed
// Leaf: UpdateGainState
// Shape: switch compiled to a two-level jump table (byte index table after the dword case table); no extra compiler flags
// Grade: DIFFER(737v770) -- catalog row over-captures the next function (int3 padding and a 0x14-byte getter at 0x00C724F0 after the jump tables); code left: the restart-off exit (retail xor al,al shared as the return), the case-2 and case-8 Gain stores cross-jumped into the release store, one 64-bit load order
#include <math.h>
#pragma intrinsic(log, exp)

// Layout (descriptive; no PDB class for the audio layer): CAudioSource Flags +0x28, Gain +0x30, Rate +0x34,
//   NextTime +0x68 (__int64), ReleaseBase +0x70 (__int64); the envelope block: Release +0, Attack +4, Hold +8 (float),
//   Decay +0xc, Sustain +0x10 (float).
struct CAudioGainEnvelope {
    long  Release;                              // +0x00
    long  Attack;                               // +0x04
    float Hold;                                 // +0x08
    long  Decay;                                // +0x0c
    float Sustain;                              // +0x10
};

extern __int64 g_AudioClockRate;                // 0x0139CA20
float __cdecl AudioExp(float x);                // 0x00C595E0

class CAudioSource {
public:
    unsigned char _pad00[0x28];
    unsigned long Flags;                        // +0x28
    unsigned char _pad2c[0x30 - 0x2c];
    float Gain;                                 // +0x30
    float Rate;                                 // +0x34
    unsigned char _pad38[0x68 - 0x38];
    __int64 NextTime;                           // +0x68
    __int64 ReleaseBase;                        // +0x70
    void StartAttack(__int64 now, long duration);   // 0x00C72130
    void StartDecay(__int64 now, long duration);    // 0x00C720C0
    bool UpdateGainState(const CAudioGainEnvelope* envelope, bool restart, __int64 now);
};

bool CAudioSource::UpdateGainState(const CAudioGainEnvelope* envelope, bool restart, __int64 now)
{
    bool changed = false;
    if (envelope == 0) {
        if (restart) {
            Gain = 1.0f;
            return true;
        }
    } else {
    unsigned long state = Flags & 0x5e;
    switch (state) {
    case 0:
        if (now < NextTime)
            break;
        state = 2;
        StartAttack(now, envelope->Attack);
    case 2:
        if (restart)
            StartAttack(now, envelope->Attack);
        if (now >= NextTime) {
            Gain = 1.0f;
            NextTime = now + (__int64)(g_AudioClockRate * envelope->Hold);
            state = 4;
            changed = true;
        } else {
            Gain = 1.0f - (NextTime - now) * Rate;
            changed = true;
        }
        break;
    case 4:
        if (now < NextTime)
            break;
        StartDecay(now, envelope->Decay);
        state = 8;
    case 8:
        if (restart) {
            StartDecay(now, envelope->Release);
            NextTime = now + (__int64)((ReleaseBase - log(Gain)) / Rate);
        }
        if (envelope->Sustain + 3.0517578e-05f > Gain) {
            Gain = envelope->Sustain;
            state = 0x10;
            changed = true;
        } else {
            Gain = AudioExp((ReleaseBase - NextTime + now) * Rate);
            changed = true;
        }
        break;
    case 0x10:
        if (restart) {
            if (Gain != envelope->Sustain) {
                Gain = envelope->Sustain;
                changed = true;
            }
        }
    case 0x20:
        if (restart) {
            StartDecay(now, envelope->Release);
            NextTime = now + (__int64)((ReleaseBase - log(Gain)) / Rate);
        }
        if (Gain < 3.0517578e-05f) {
            Gain = 0.0f;
            Flags = (Flags & ~0x20) | 0x40;
        } else {
            Gain = (float)exp((ReleaseBase - NextTime + now) * Rate);
        }
        break;
    }
    Flags = (Flags & ~0x5e) | state;
    }
    return changed;
}
