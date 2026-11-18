// Behaviour test for 0x00B45720 CEngineLightingManager::CalcSWLighting(pos, normal, light_array): the row's defines, includes and body verbatim
// over the shared headers (fixture of the e08 CalcSWLightingNoClip test 0x00B45CF0), run on a zeroed manager image with a mantissa table
// built here, checked against a double-precision reference: the front / back terms and their clamps, the light count bound, the Lambert
// clamp, lights behind / below the threshold skipped (the strict `dot > DELTA_NOTIONAL_ZERO` boundary made exact with table entry 64 = 256),
// the falloff and the product clamps at 1 and 0, then the per-channel clamp to 1 (each channel above 1 in the front-clamp run, below in
// others) and alpha 1 in the returned colour; unsaturated runs (audit e11) for the front term and its clamp, each light clamp, the normalise and
// dist2, and the final clamp just below / just above 1.
#include <stdio.h>
#include <string.h>
#include <math.h>
#define FABLE_RETAIL_CRGBFLOATCOLOUR_TO_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CRGBFLOATCOLOUR
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_MEMBERS
#define FABLE_RETAIL_CENGINELOCALLIGHTARRAY_PEEKLIGHT
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_CALCSWLIGHTINGNOCLIP
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_CALCSWLIGHTING_LOCALLIGHTS
#include "engine/C3DVector.h"
#include "engine/CRGBFloatColour.h"
#include "engine/GFLimit.h"
#include "engine/GFWorldDistance.h"
#include "engine/CEngineLightingManager.h"

// ---- row body ----
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

CRGBFloatColour CEngineLightingManager::CalcSWLighting(const C3DVector& pos, const C3DVector& normal, const CEngineLocalLightArray& light_array)
{
    float dp = -DiffuseVector.Dot(normal);
    float frontval = MIN(GFSqr(MAX(dp, 0.0f)), 1.0f);
    float backval = MAX(-dp, 0.0f);
    C3DVector col = PrimitiveDiffuseColour * frontval + PrimitiveAmbientColour + PrimitiveBacklightColour * backval;
    for (long l = 0; l < light_array.GetCount(); ++l) {
        const CEngineLocalLightElement& light = light_array.PeekLight(l);
        C3DVector light_vec = light.WorldPos - pos;
        light_vec.Normalise();
        float dot = light_vec.Dot(normal);
        GFLimit(dot, 0.0f, 1.0f);
        if (dot > DELTA_NOTIONAL_ZERO) {
            float dist2 = (light.WorldPos - pos).GetSquaredMagnitude();
            dot *= 1.0f / (dist2 * light.AttenuationDistSquared) + light.AttenuationConstant;
            GFLimit(dot, 0.0f, 1.0f);
            col += light.Colour * dot;
        }
    }
    if (col.X > 1.0f) col.X = 1.0f;
    if (col.Y > 1.0f) col.Y = 1.0f;
    if (col.Z > 1.0f) col.Z = 1.0f;
    return CRGBFloatColour(col.X, col.Y, col.Z, 1.0f);
}

unsigned long GFastOneOverSqrtTable[128];
extern const float DELTA_NOTIONAL_ZERO = 1e-4f;
static void BuildTable()
{
    for (int i = 0; i < 128; ++i) {
        double m = ((i & 63) + 0.5) / 64.0, v = (i & 64) ? (1.0 + m) : 2.0 * (1.0 + m);
        double f = 2.0 / sqrt(v) - 1.0;
        long t = (long)(f * 256.0); if (t > 255) t = 255; if (t < 0) t = 0;
        GFastOneOverSqrtTable[i] = (unsigned long)t;
    }
}
// ---- harness ----
static int g_fail = 0;
static float g_last[3];
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }
struct V { double x, y, z; };
struct L { float pos[3]; float colour[3]; float attdist, attconst; };
static void Reference(const float* diff, const float* cd, const float* ca, const float* cb, const V& p, const V& n, const L* lights, int count, double* out)
{
    double dp = -(n.x * diff[0] + n.y * diff[1] + n.z * diff[2]);
    double f = dp > 0 ? dp : 0; f = f * f; if (f > 1) f = 1;
    double b = -dp > 0 ? -dp : 0;
    for (int c = 0; c < 3; ++c) out[c] = cd[c] * f + ca[c] + cb[c] * b;
    for (int i = 0; i < count; ++i) {
        double lx = lights[i].pos[0] - p.x, ly = lights[i].pos[1] - p.y, lz = lights[i].pos[2] - p.z;
        double d2 = lx * lx + ly * ly + lz * lz, len = sqrt(d2);
        double dot = (lx * n.x + ly * n.y + lz * n.z) / len;
        if (dot < 0) dot = 0; else if (dot > 1) dot = 1;
        if (!(dot > DELTA_NOTIONAL_ZERO)) continue;
        double s = dot * (1.0 / (d2 * lights[i].attdist) + lights[i].attconst);
        if (s < 0) s = 0; else if (s > 1) s = 1;
        for (int c = 0; c < 3; ++c) out[c] += lights[i].colour[c] * s;
    }
    for (int c = 0; c < 3; ++c) if (out[c] > 1.0) out[c] = 1.0;
}
static void Run(const float* diff, const V& p, const V& n, const L* lights, int count, const char* what)
{
    static unsigned char mgr[0x200], arrimg[0x1a4];
    static const float cd[4] = { 0.5f, 0.25f, 0.125f, 9.0f }, ca[4] = { 0.0625f, 0.03125f, 0.25f, 9.0f }, cb[4] = { 0.75f, 0.375f, 0.5f, 9.0f };
    memset(mgr, 0, sizeof mgr); memset(arrimg, 0, sizeof arrimg);
    memcpy(mgr + 0x30, diff, 12); memcpy(mgr + 0x48, cd, 16); memcpy(mgr + 0x58, ca, 16); memcpy(mgr + 0x68, cb, 16);
    *(unsigned long*)arrimg = (unsigned long)count;
    for (int i = 0; i < 8; ++i) {
        unsigned char* e = arrimg + 4 + i * 0x34;
        memcpy(e, lights[i].pos, 12); memcpy(e + 0x18, lights[i].colour, 12);
        *(float*)(e + 0x24) = 7.0f;                           // colour A, never used
        *(float*)(e + 0x28) = lights[i].attdist; *(float*)(e + 0x2c) = lights[i].attconst;
    }
    C3DVector pos, normal; pos.X = (float)p.x; pos.Y = (float)p.y; pos.Z = (float)p.z; normal.X = (float)n.x; normal.Y = (float)n.y; normal.Z = (float)n.z;
    CRGBFloatColour result = ((CEngineLightingManager*)mgr)->CalcSWLighting(pos, normal, *(const CEngineLocalLightArray*)arrimg);
    double want[3]; Reference(diff, cd, ca, cb, p, n, lights, count, want);
    const float got[3] = { result.R, result.G, result.B };
    for (int c = 0; c < 3; ++c)
        if (!(fabs(got[c] - want[c]) <= 2e-3 * (1.0 + fabs(want[c])))) { printf("%s channel %d: %f v %f\n", what, c, got[c], want[c]); check(0, what); }
    check(result.A == 1.0f, "alpha 1");
    g_last[0] = result.R; g_last[1] = result.G; g_last[2] = result.B;
}
int main()
{
    BuildTable();
    check(sizeof(CEngineLocalLightElement) == 0x34, "layout");
    static const L lights[8] = {
        { { 3, 0, 4 }, { 1.0f, 0.5f, 0.25f }, 0.2f, 0.1f },      // in front, dot 1.6 clamps to 1, partial falloff
        { { 0, 0, 10 }, { 0.5f, 1.0f, 2.0f }, 0.0001f, 2.0f },   // straight above: dot clamps to 1 (normal length 2), product clamps to 1
        { { 0, 0, -6 }, { 9.0f, 9.0f, 9.0f }, 0.1f, 0.1f },      // behind: dot clamps to 0, skipped
        { { 5, 0, 0.0001f }, { 9.0f, 9.0f, 9.0f }, 1e-9f, 0.0f },// grazing: dot 4e-5 < threshold, skipped
        { { 2, 2, 2 }, { 1.0f, 1.0f, 1.0f }, 1.0f, -5.0f },      // negative falloff: product clamps to 0
        { { 1, 1, 1 }, { 50.0f, 50.0f, 50.0f }, 0.1f, 1.0f },    // past the count in the first runs
        { { 4, 0, 1 }, { 0.5f, 0.25f, 1.0f }, 0.1f, 0.1f },     // partial: dot 2/sqrt(17) = 0.485, falloff 0.688, product 0.334
    };
    static const float diff_front[3] = { 0.0f, 0.0f, -0.4f };   // dp = 0.8 (normal z = 2): front term 0.64
    static const float diff_strong[3] = { 0.0f, 0.0f, -1.0f };  // dp = 2: front term clamps to 1
    static const float diff_back[3] = { 0.0f, 0.0f, 0.25f };    // dp = -0.5: back term 0.5
    V p = { 0, 0, 0 }, n = { 0, 0, 2 };
    Run(diff_front, p, n, lights, 5, "five lights");
    Run(diff_strong, p, n, lights, 2, "two lights, front clamp");
    check(g_last[0] == 1.0f && g_last[1] == 1.0f && g_last[2] == 1.0f, "every channel clamped to exactly 1");
    Run(diff_back, p, n, lights, 7, "seven lights, back term");
    Run(diff_back, p, n, lights, 0, "no lights");
    check(g_last[0] < 1.0f && g_last[1] < 1.0f && g_last[2] < 1.0f, "channels below 1 kept");
    // audit e11: the runs above saturate every channel once lights are on, which hides the front term, its clamp, the Lambert clamp
    // at 1, the normalise, dist2, the product clamp at 1 and the final clamp threshold. These runs stay below 1.
    Run(diff_front, p, n, lights, 0, "front term alone (dp 0.8: 0.64, not 0.8)");
    Run(diff_strong, p, n, lights, 0, "front clamp alone (dp 2: 1, not 4)");
    check(g_last[0] > 0.56f && g_last[0] < 0.565f, "front clamp exactly 1: R = 0.5 + 0.0625");
    {
        static const L small[8] = {
            { { 3, 0, 4 }, { 0.2f, 0.2f, 0.2f }, 0.2f, 0.1f },      // dot 1.6 clamps to 1 (normal length 2), product 0.3
            { { 4, 0, 1 }, { 0.3f, 0.3f, 0.3f }, 0.1f, 0.1f },      // normalised dot 0.485 (unnormalised 2), dist2 17, product 0.334
            { { 0, 0, 10 }, { 0.1f, 0.1f, 0.1f }, 0.0001f, 2.0f },  // product 102 clamps to 1
            { { 2, 2, 2 }, { 0.5f, 0.5f, 0.5f }, 1.0f, -5.0f },     // negative product clamps to 0
        };
        Run(diff_back, p, n, small, 4, "four unsaturated lights");
        check(g_last[0] < 0.9f && g_last[1] < 0.9f && g_last[2] < 0.9f, "unsaturated run stays below 1");
        static const L below[8] = { { { 0, 0, 10 }, { 0.5575f, 0.77625f, 0.495f }, 0.0001f, 2.0f } };   // channels 0.995
        Run(diff_back, p, n, below, 1, "channels just below 1");
        check(g_last[0] > 0.99f && g_last[0] < 0.999f && g_last[1] > 0.99f && g_last[1] < 0.999f && g_last[2] > 0.99f && g_last[2] < 0.999f, "0.995 not clamped");
        static const L above[8] = { { { 0, 0, 10 }, { 0.563f, 0.7818f, 0.5008f }, 0.0001f, 2.0f } };    // channels 1.0005 .. 1.0008
        Run(diff_back, p, n, above, 1, "channels just above 1");
        check(g_last[0] == 1.0f && g_last[1] == 1.0f && g_last[2] == 1.0f, "just above 1 clamped to exactly 1");
    }
    {
        const unsigned long saved = GFastOneOverSqrtTable[64];
        GFastOneOverSqrtTable[64] = 256;
        C3DVector unit; unit.X = 0.0f; unit.Y = 0.0f; unit.Z = 1.0f; unit.Normalise();
        check(unit.X == 0.0f && unit.Y == 0.0f && unit.Z == 1.0f, "exact unit normalise");
        static const L edge[8] = { { { 0, 0, 1 }, { 1.0f, 1.0f, 1.0f }, 1.0f, 999.0f } };   // falloff 1000: dot * falloff = 0.1
        V on = { 0, 0, DELTA_NOTIONAL_ZERO };
        Run(diff_back, p, on, edge, 1, "light exactly on the threshold is skipped");
        float above = DELTA_NOTIONAL_ZERO; unsigned int bits; memcpy(&bits, &above, 4); ++bits; memcpy(&above, &bits, 4);
        V up = { 0, 0, above };
        Run(diff_back, p, up, edge, 1, "light one float above the threshold counts");
        GFastOneOverSqrtTable[64] = saved;
    }
    if (!g_fail) printf("00b45720_TEST PASS\n");
    return g_fail;
}