// Behaviour test for 0x00E81B40 NScript::CV_FishingCompetitionScript::CFishCompOrganiser::GetBiggestFishWeight()
#include <stdio.h>
#include <math.h>
#include <string.h>

static int RoundNearestEven(float f)
{
    double fl = floor((double)f), diff = (double)f - fl;
    if (diff > 0.5 || (diff == 0.5 && fmod(fl, 2.0) != 0.0)) fl += 1.0;
    return (int)fl;
}
static float RoundToDecimals(float value, int decimals)
{
    float scale = 1.0f;
    for (int i = decimals; i; --i) scale *= 10.0f;
    return RoundNearestEven(value * scale) / scale;
}
static const char* g_defs[3] = { "FISH_A", "FISH_B", "FISH_C" };
static bool g_carried[3];
static float g_weights[3];
static float g_shown = 0.0f;
static int g_cutscenes = 0;
static float GetBiggestFishWeight(int count, const char*& entry)    // the row's body
{
    float biggest = -1.0f;
    for (int i = 0; i < count; ++i)
        if (g_carried[i]) {
            float weight = g_weights[i];
            if (weight > biggest) { entry = g_defs[i]; biggest = weight; }
        }
    g_shown = RoundToDecimals(biggest, 2);        // GFFloatToWideString -> TEXT_QST_B15_FISHSIZE parameter
    ++g_cutscenes;                                // PlayCutscene("CS_FISHCOMP_CHECKFISH_OUTPUTSIZE")
    return biggest;
}

int main()
{
    const char* entry = "NONE";
    int ok = 1;
    ok &= GetBiggestFishWeight(3, entry) == -1.0f && strcmp(entry, "NONE") == 0 && g_shown == -1.0f;
    g_carried[0] = g_carried[2] = true; g_weights[0] = 2.345f; g_weights[1] = 9.0f; g_weights[2] = 3.14159f;
    ok &= GetBiggestFishWeight(3, entry) == 3.14159f && strcmp(entry, "FISH_C") == 0;
    ok &= fabs(g_shown - 3.14f) < 1e-5 && g_cutscenes == 2;
    ok &= GetBiggestFishWeight(1, entry) == 2.345f && strcmp(entry, "FISH_A") == 0;
    printf(ok ? "00e81b40_TEST PASS\n" : "00e81b40_TEST FAIL\n");
    return ok ? 0 : 1;
}