// Behaviour test for 0x006E2B70 NSpeechGainManager::GetTargetGain: 1 when management is off; layer 0 -> high; layer 1 ->
// high only for the layer head with no layer-0 speakers, else low; layer 2 -> high/low by layer-0 speakers, mid when
// that is not positive or layer 1 has speakers; any other layer -> 0.
#include <cstdio>

struct Info { int Layer; bool IsLayerHead; };
struct Def { float High, Mid, Low; };
static bool g_enabled = true;
static short g_counts[4];

static float GetTargetGain(const Info& info, const Def* defs)
{
    if (!g_enabled) return 1.0f;
    float gain = 0.0f;
    switch (info.Layer) {
    case 0: gain = defs[0].High; break;
    case 1: if (g_counts[0] == 0 && info.IsLayerHead) gain = defs[1].High; else gain = defs[1].Low; break;
    case 2:
        if (g_counts[0] == 0) gain = defs[2].High; else gain = defs[2].Low;
        if (!(gain > 0.0f) || g_counts[1] != 0) gain = defs[2].Mid;
        break;
    }
    return gain;
}

#define FAIL { std::printf("006e2b70_TEST FAIL\n"); return 1; }
int main()
{
    Def d[3] = { { 1.0f, 0.5f, 0.25f }, { 0.9f, 0.6f, 0.3f }, { 0.8f, 0.4f, 0.0f } };
    Info i; i.Layer = 0; i.IsLayerHead = false;
    if (GetTargetGain(i, d) != 1.0f) FAIL;
    i.Layer = 1; if (GetTargetGain(i, d) != 0.3f) FAIL;
    i.IsLayerHead = true; if (GetTargetGain(i, d) != 0.9f) FAIL;
    g_counts[0] = 1; if (GetTargetGain(i, d) != 0.3f) FAIL;
    i.Layer = 2; if (GetTargetGain(i, d) != 0.4f) FAIL;      // low is 0 -> mid
    g_counts[0] = 0; if (GetTargetGain(i, d) != 0.8f) FAIL;
    g_counts[1] = 2; if (GetTargetGain(i, d) != 0.4f) FAIL;
    i.Layer = 3; if (GetTargetGain(i, d) != 0.0f) FAIL;
    g_enabled = false; if (GetTargetGain(i, d) != 1.0f) FAIL;
    std::printf("006e2b70_TEST PASS\n");
    return 0;
}