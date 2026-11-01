// CCodaVoice::SetModulation(const SModulationParams* mod) -- configure the second pitch/volume/filter LFO of a Coda voice: half-period length in mix blocks from 0.5 / period seconds (none for a period <= 0) and the per-block rising/falling factors (2^(+-2*octaves/steps) for pitch and filter, 10^(+-0.001*millibel/steps) for volume; 1.0 when there are no steps); scales reset to 1 and the counter to 0
// Grade: DIFFER(382v382) -- one x87 shape: retail computes the falling volume rate as fld -1.0; fdiv steps; fmul decibels (no reciprocal CSE) where VC7.1 without /Op reuses 1/steps or reassociates; /Op instead adds a rounding store after the VolumeMilliBel fild (390v382)
// Next: try the decibel scaling as a double temporary or the -1.0 division in a same-TU noinline float helper; otherwise lane gap: x87 reassociation without /Op.
// Leaf: ?SetModulation@CCodaVoice@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5A9D0, length 382 bytes (`ret 4`).
// Label: the manifest's "C3DMeshFileXBoundingBoxChunk::ReadFromFile" is wrong (landed callers bind CCodaVoice::SetModulation): the body touches +0x28..+0xB0 of a
//        larger object (the chunk is 0x28 bytes), takes no stream, and its three callers (0xC5AEC6, 0xC5B449,
//        0xC5BD9D) sit in the audio-library block 0xC5A3C0..0xC5BF00 (CAudioSource_* neighbours). Class and
//        member names below are descriptive.
// Callees: the library's time-to-steps conversion @ 0x00C59B00 (__cdecl float -> long; manifest
//          "Graphics_ConvertTimeToPixels" is a placeholder), _CIpow @ 0x00BFEB40 (pow intrinsic).
// Constants: 0.0f 0x122DEDC, 0.5f 0x122F59C, 2.0 (double) 0x1244480, -2.0f 0x1246050, 0.001f 0x1266300,
//          10.0 (double) 0x129A7B0, -1.0f 0x122DEE0; 1.0f is kept in ebx for the plain stores.
// Shape: both ramp flag bits (0x8000|0x10000) are cleared first; a null description clears them again.
//          The step count is stored, read back, and its float conversion lives in the dead argument slot; the
//          millibel volume scaled by 0.001 is a float local.
struct SModulationParams {
    long  Reserved;                // +0x00
    float Time;                    // +0x04
    float PitchOctaves;            // +0x08
    float FilterOctaves;           // +0x0c
    long  VolumeMilliBel;          // +0x10
};

long __cdecl SecondsToMixBlocks(float seconds);   // 0x00C59B00 (landed spelling, ?SecondsToMixBlocks@@YAJM@Z)
extern "C" double __cdecl pow(double, double);
#pragma intrinsic(pow)

struct CCodaVoice {
    unsigned char _00[0x28];
    unsigned long m_flags;         // +0x28 (bits 0x8000 / 0x10000: ramps active)
    unsigned char _2c[0x88 - 0x2c];
    float m_modulationScale;       // +0x88 (landed spelling)
    float PitchUpRate;             // +0x8c
    float PitchDownRate;           // +0x90
    float VolumeScale;             // +0x94
    float VolumeUpRate;            // +0x98
    float VolumeDownRate;          // +0x9c
    float FilterScale;             // +0xa0
    float FilterUpRate;            // +0xa4
    float FilterDownRate;          // +0xa8
    long  m_modulationCounter;     // +0xac (landed spelling)
    long  RampSteps;               // +0xb0
    void SetModulation(const SModulationParams* mod);
};

void CCodaVoice::SetModulation(const SModulationParams* mod)
{
    m_flags &= ~0x18000;
    if (mod) {
        if (mod->Time > 0.0f)
            RampSteps = SecondsToMixBlocks(0.5f / mod->Time);
        else
            RampSteps = 0;
        if (RampSteps) {
            
            PitchUpRate = (float)pow(2.0, (mod->PitchOctaves + mod->PitchOctaves) / (float)RampSteps);
            PitchDownRate = (float)pow(2.0, mod->PitchOctaves * -2.0f / (float)RampSteps);
            FilterUpRate = (float)pow(2.0, (mod->FilterOctaves + mod->FilterOctaves) / (float)RampSteps);
            FilterDownRate = (float)pow(2.0, mod->FilterOctaves * -2.0f / (float)RampSteps);
            float decibels = mod->VolumeMilliBel * 0.001f;
            VolumeUpRate = (float)pow(10.0, (double)(decibels / (float)RampSteps));
            VolumeDownRate = (float)pow(10.0, (double)(decibels * -1.0f / (float)RampSteps));
        }
        else {
            PitchUpRate = 1.0f;
            PitchDownRate = 1.0f;
            VolumeUpRate = 1.0f;
            VolumeDownRate = 1.0f;
            FilterUpRate = 1.0f;
            FilterDownRate = 1.0f;
        }
    }
    else
        m_flags &= ~0x18000;
    m_modulationScale = 1.0f;
    VolumeScale = 1.0f;
    FilterScale = 1.0f;
    m_modulationCounter = 0;
}
