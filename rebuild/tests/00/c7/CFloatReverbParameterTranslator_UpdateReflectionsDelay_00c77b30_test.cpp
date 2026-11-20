// Behaviour test for CFloatReverbParameterTranslator::UpdateReflectionsDelay (0x00C77B30): links the row object. Input 0 of each of the
// four reflections gets the reflections delay in samples (rate * m_fReflectionsDelay, ds_ftoi = truncation toward zero, rounded up to a
// multiple of 4); input 1 gets that plus ds_ftoi((m_fReverbDelay / 0.05 + 1) * 0.1 * rate * sc_DefaultReflectionData[i].fInputDelay[0]),
// rounded up to a multiple of 4. Nothing else in the parameter block is written. Expected values from a double-precision model of the
// float arithmetic (round-to-nearest-even fistp, then +1 for negatives after the -0.5 bias).
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "fable_compiler.h"
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATREVERBPARAMETERTRANSLATOR_UPDATEDECAYTIMES
#define FABLE_RETAIL_CFLOATREVERBPARAMETERTRANSLATOR_TRANSLATE
#include "engine/CFloatReverbParameterTranslator.h"
#include "engine/CFloatReverbParameters.h"
#include "engine/CFloatReverbConstants.h"
#include "engine/CHLRvbParams.h"
#include "engine/ReflectionData.h"

// The retail table values (0x012BF0DC: fOutputGain, fFeedbackGain, fMinFeedbackDelay, fDelayOneSecond, fInputDelay[0]); only
// fInputDelay[0] is read here, the other fields are poisoned so a wrong member read shows.
extern "C" const ReflectionData sc_DefaultReflectionData[4] = {
    { -1000.0f, -1000.0f, -1000.0f, -1000.0f, 0.1052f },
    { -1000.0f, -1000.0f, -1000.0f, -1000.0f, 0.0673f },
    { -1000.0f, -1000.0f, -1000.0f, -1000.0f, 0.1737f },
    { -1000.0f, -1000.0f, -1000.0f, -1000.0f, 0.2404f },
};

// The method is private: call the row's symbol through a __fastcall stand-in (ecx = this, edx unused, the two stack arguments, callee
// pops 8 -- the thiscall frame).
extern "C" void __fastcall RowUpdateReflectionsDelay(void* self, int unused, void* pReverbParams, void* pSrcParams);
#pragma comment(linker, "/alternatename:@RowUpdateReflectionsDelay@16=?UpdateReflectionsDelay@CFloatReverbParameterTranslator@@AAEXPAVCFloatReverbParameters@@PAVCHLRvbParams@@@Z")

static long RoundNearestEven(float value)
{
    double d = value, down = floor(d), frac = d - down;
    long r = (long)down;
    if (frac > 0.5 || (frac == 0.5 && (r & 1)))
        ++r;
    return r;
}
static long ftoi(float f) { long i = RoundNearestEven(f - 0.5f); return i < 0 ? i + 1 : i; }

static unsigned char g_translator[64], g_constants[64], g_src[128], g_params[sizeof(CFloatReverbParameters) + 16];

static bool Run(unsigned int rate, float reflDelay, float reverbDelay)
{
    memset(g_translator, 0, sizeof g_translator); memset(g_constants, 0, sizeof g_constants);
    memset(g_src, 0, sizeof g_src); memset(g_params, 0xAB, sizeof g_params);
    *(unsigned int*)(g_constants + 8) = rate;                       // CReverbConstants::m_uiSamplesPerSecond +8
    *(void**)(g_translator + 0x28) = g_constants;                  // m_pReverbConstants +0x28
    *(float*)(g_src + 0x28) = reflDelay;                           // CHLRvbParams::m_fReflectionsDelay +0x28
    *(float*)(g_src + 0x30) = reverbDelay;                         // CHLRvbParams::m_fReverbDelay +0x30
    RowUpdateReflectionsDelay(g_translator, 0, g_params, g_src);
    unsigned int base = (unsigned int)((ftoi((float)rate * reflDelay) + 3) & ~3);
    bool ok = true;
    for (int i = 0; i < 4; ++i) {
        unsigned int* in = (unsigned int*)(g_params + 0x38 + 20 * i);   // m_Ref[i].m_auiInputDelay
        float x = (reverbDelay / 0.05f + 1.0f) * 0.1f * (float)rate * sc_DefaultReflectionData[i].fInputDelay[0];
        unsigned int want1 = (unsigned int)((ftoi(x) + base + 3) & ~3);
        if (in[0] != base || in[1] != want1) { printf("refl %d: %u %u want %u %u\n", i, in[0], in[1], base, want1); ok = false; }
        for (int k = 8; k < 20; ++k) if (g_params[0x38 + 20 * i + k] != 0xAB) { printf("refl %d byte %d written\n", i, k); ok = false; }
    }
    for (unsigned int k = 0; k < sizeof g_params; ++k)
        if ((k < 0x38 || k >= 0x38 + 80) && g_params[k] != 0xAB) { printf("byte %u written\n", k); ok = false; break; }
    return ok;
}

int main()
{
    bool ok = Run(44100, 0.007f, 0.011f);
    ok = Run(48000, 0.0f, 0.0f) && ok;
    ok = Run(22050, 0.3f, 0.1f) && ok;
    ok = Run(44100, 0.0201f, 0.0f) && ok;
    ok = Run(44100, -0.01f, -0.06f) && ok;                // negative products: ds_ftoi's +1 step
    // spot values (rate 44100, refl 0.007, reverb 0.011): base = (ftoi(308.7) + 3) & ~3 = 308; refl 0 input 1 = 308 + ftoi(1.22*4410*0.1052)
    unsigned int* r0 = (unsigned int*)(g_params + 0x38);
    Run(44100, 0.007f, 0.011f);
    ok = ok && r0[0] == 308 && r0[1] == ((308 + 565 + 3) & ~3u);
    printf(ok ? "00c77b30_TEST PASS\n" : "00c77b30_TEST FAIL\n");
    return ok ? 0 : 1;
}