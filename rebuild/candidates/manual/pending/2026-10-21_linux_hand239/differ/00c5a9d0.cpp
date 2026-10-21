// CAudioLibVoice::SetRampParams(const SAudioRampParams*) -- derive the per-step exponential rate multipliers of an audio voice from a ramp description
// Grade: DIFFER(390v382) under /Op -- the int->float conversion of VolumeMilliBel gets a /Op rounding store/reload (fild ; fstp ; fld) that retail lacks; without /Op VC7.1 turns the repeated `/ steps` into reciprocal multiplies
// Flags: /Op
// Leaf: SetRampParams
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5A9D0, length 382 bytes (`ret 4`).
// Label: the manifest's "C3DMeshFileXBoundingBoxChunk::ReadFromFile" is wrong: the body touches +0x28..+0xB0 of a
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
struct SAudioRampParams {
    long  Reserved;                // +0x00
    float Time;                    // +0x04
    float PitchOctaves;            // +0x08
    float FilterOctaves;           // +0x0c
    long  VolumeMilliBel;          // +0x10
};

extern "C" long __cdecl AudioTimeToSteps(float seconds);   // 0x00C59B00
extern "C" double __cdecl pow(double, double);
#pragma intrinsic(pow)

struct CAudioLibVoice {
    unsigned char _00[0x28];
    unsigned long Flags;           // +0x28 (bits 0x8000 / 0x10000: ramps active)
    unsigned char _2c[0x88 - 0x2c];
    float PitchScale;              // +0x88
    float PitchUpRate;             // +0x8c
    float PitchDownRate;           // +0x90
    float VolumeScale;             // +0x94
    float VolumeUpRate;            // +0x98
    float VolumeDownRate;          // +0x9c
    float FilterScale;             // +0xa0
    float FilterUpRate;            // +0xa4
    float FilterDownRate;          // +0xa8
    long  RampStep;                // +0xac
    long  RampSteps;               // +0xb0
    void SetRampParams(const SAudioRampParams* params);
};

void CAudioLibVoice::SetRampParams(const SAudioRampParams* params)
{
    Flags &= ~0x18000;
    if (params) {
        if (params->Time > 0.0f)
            RampSteps = AudioTimeToSteps(0.5f / params->Time);
        else
            RampSteps = 0;
        if (RampSteps) {
            float steps = (float)RampSteps;
            PitchUpRate = (float)pow(2.0, (params->PitchOctaves + params->PitchOctaves) / steps);
            PitchDownRate = (float)pow(2.0, params->PitchOctaves * -2.0f / steps);
            FilterUpRate = (float)pow(2.0, (params->FilterOctaves + params->FilterOctaves) / steps);
            FilterDownRate = (float)pow(2.0, params->FilterOctaves * -2.0f / steps);
            float decibels = params->VolumeMilliBel * 0.001f;
            VolumeUpRate = (float)pow(10.0, (double)(decibels / steps));
            VolumeDownRate = (float)pow(10.0, (double)(-1.0f / steps * decibels));
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
        Flags &= ~0x18000;
    PitchScale = 1.0f;
    VolumeScale = 1.0f;
    FilterScale = 1.0f;
    RampStep = 0;
}
