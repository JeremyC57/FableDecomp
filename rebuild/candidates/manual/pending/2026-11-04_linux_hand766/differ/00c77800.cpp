// CEffectPart260::Stage_00c77800(CVoiceEffect* owner, SVoiceEffectParameters* parameters) -- recompute the reverb's decay filters
// Grade: DIFFER(725v725) -- same length; 55 lines differ by a uniform eax/ecx/edx rotation in each SetShelves argument setup (retail loads Context before the fmul)
// Leaf: ?Stage_00c77800@CEffectPart260@@QAEXPAUCVoiceEffect@@PAUSVoiceEffectParameters@@@Z
// Flags: none
extern "C" double __cdecl exp(double x);
#pragma intrinsic(exp)

struct SVoiceEffectParameters {
    unsigned long Flags;                           // +0x00
    unsigned char _pad04[0x10 - 0x04];
    float         DecayTime;                       // +0x10
    float         DecayHFRatio;                    // +0x14
    float         DecayLFRatio;                    // +0x18
    float         HighPassReference;               // +0x1c
    float         LowPassReference;                // +0x20
    unsigned char _pad24[0x3c - 0x24];
    float         AirAbsorption;                   // +0x3c
};
struct CMixContext {
    unsigned char _pad00[8];
    unsigned long SampleRate;                      // +0x08
    unsigned char _pad0c[4];
    unsigned long Predelay;                        // +0x10
    unsigned long Spread;                          // +0x14
    unsigned char _pad18[4];
    unsigned long Delays[4];                       // +0x1c
};
struct CEffectFilter {
    float Gain;                                    // +0x00
    float m_a0, m_a1, m_a2, m_b0, m_b1, m_b2;      // +0x04
    void SetShelves(float highGain, float highReference, float lowGain, float lowReference, unsigned long sampleRate);   // 0x00C74850
};
struct CDelayTap {                                 // 0x30 bytes
    unsigned char _pad00[0x14];
    CEffectFilter Filter;                          // +0x14
};
struct CAllPassTap {                               // 0x3c bytes
    CEffectFilter Filter;                          // +0x00
    unsigned long Delay;                           // +0x1c
    unsigned char _pad20[0x1c];
};
struct CVoiceEffect {
    unsigned char _pad00[0x84];
    CDelayTap     Taps[4];                         // +0x84 (filters +0x98)
    unsigned char _pad144[0x10];
    CEffectFilter Diffusers[3];                    // +0x154
    unsigned char _pad1a8[4];
    unsigned long Lengths[3];                      // +0x1ac
    unsigned char _pad1b8[8];
    union {                                        // +0x1c0 (the retail accesses overlap: layout from the code only)
        CAllPassTap AllPass[2];                    // +0x1c0, +0x1fc
        struct {
            unsigned char _pad1c0[0x28];
            unsigned long EndLengths[3];           // +0x1e8 (Lengths + 0x3c)
            unsigned char _pad1f4[0x22c - 0x1f4];
            float         AirDecay;                // +0x22c
        };
    };
};
struct CEffectPart260 {
    float         Values[10];                      // +0x00
    CMixContext*  Context;                         // +0x28
    void Stage_00c77800(CVoiceEffect* owner, SVoiceEffectParameters* parameters);
};

void CEffectPart260::Stage_00c77800(CVoiceEffect* owner, SVoiceEffectParameters* parameters)
{
    float decaySamples = (float)Context->SampleRate * parameters->DecayTime * 0.144764841f;
    float highRatio = (parameters->DecayHFRatio - 1.0f) / (decaySamples * parameters->DecayHFRatio);
    float lowRatio = (parameters->DecayLFRatio - 1.0f) / (decaySamples * parameters->DecayLFRatio);
    float rate = 1.0f / decaySamples;
    for (unsigned long i = 0; i < 4; ++i) {
        float delay = (float)Context->Delays[i];
        owner->Taps[i].Filter.Gain = (float)exp(-(rate * delay));
        owner->Taps[i].Filter.SetShelves((float)exp(highRatio * delay), parameters->HighPassReference,
                                         (float)exp(lowRatio * delay), parameters->LowPassReference, Context->SampleRate);
    }
    for (unsigned long j = 0; j < 2; ++j) {
        float delay = (float)owner->AllPass[j].Delay;
        owner->AllPass[j].Filter.Gain = (float)exp(-(rate * delay));
        owner->AllPass[j].Filter.SetShelves((float)exp(highRatio * delay), parameters->HighPassReference,
                                            (float)exp(lowRatio * delay), parameters->LowPassReference, Context->SampleRate);
    }
    unsigned long spread = Context->Spread >> 2;
    unsigned long offset = spread;
    for (unsigned long k = 0; k < 3; ++k) {
        float delay = (float)(((owner->Lengths[k] + owner->EndLengths[k]) >> 1) - Context->Predelay - offset);
        owner->Diffusers[k].Gain = (float)exp(-(rate * delay));
        owner->Diffusers[k].SetShelves((float)exp(highRatio * delay), parameters->HighPassReference,
                                       (float)exp(lowRatio * delay), parameters->LowPassReference, Context->SampleRate);
        offset = spread + offset;
    }
    owner->AirDecay = (float)exp(-((float)Context->SampleRate * parameters->AirAbsorption * rate));
}
