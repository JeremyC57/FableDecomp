// Behaviour test for 0x00B16410 NParticleEngine::CPSCOrbit::COrbitState::Update (links the REAL row object).
// No class at all: the row is entered through a linker alias (thiscall = fastcall with a spare edx) and works on raw native-layout
// storage:
//   COrbitState     NextAngle +0, RotateSpeedRandomValue +4, FromPosition +8, ToPosition +0x14, NextPosition +0x20,
//                   FromTangent +0x2C, ToTangent +0x38 (17 floats)
//   COrbitSettings  Type +0 (long: bits 0..1 the plane, bit 2 "through the whole matrix"), Enabled +4 (bool), Radius +8,
//                   Expand +0xC, CycleFlag +0x10 (bool), CycleTime +0x14, SqueezeScale +0x18, SqueezeAngle +0x1C,
//                   RotateSpeed +0x20, three more floats the row does not read
//   CMatrix3x4      E11..E43, twelve floats, rows of three
// The four things the row object leaves undefined are defined here under their retail names: GFastCosTable (1025 floats),
// DELTA_NOTIONAL_ZERO, TWO_PI and NParticleEngine::GetOneOverTickRate (counted, returns the case's tick interval).
// An independent model (RunModel, double arithmetic, its own table lookups and its own rotations) computes the expected state
// and matrix. Fields the function copies (the position ring, the tangent shift, the matrix translation) must be bit-equal;
// computed floats must agree within 2e-6 of the case's magnitude bound (a float store rounds to 6e-8 of the value and at most
// four such stores are chained; the smallest effect a branch decision has in the threshold scenarios is 5e-5 of the bound).
// Everything round the three objects (guard words, the settings bytes including padding) must be unchanged.
// The stack under the call is filled with large floats first (a disabled orbit must give a zero point, not stack leftovers).
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern "C" __declspec(dllimport) void* __stdcall SetUnhandledExceptionFilter(void*);
extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned int);

extern "C" void __fastcall CallUpdate(void* self, int spare, const void* settings, void* parent_transform, float time_secs);
#pragma comment(linker, "/alternatename:@CallUpdate@20=?Update@COrbitState@CPSCOrbit@NParticleEngine@@QAEXABVCOrbitSettings@23@AAVCMatrix3x4@@M@Z")

static long __stdcall Crash(void*) { ExitProcess(3); return 0; }

// ---- what the row object needs from the rest of the program
extern float GFastCosTable[1025];
float GFastCosTable[1025];                                           // 0x013CD550: cos(2*pi*k/1024), k = 0..1024
extern const float DELTA_NOTIONAL_ZERO = 9.999999747378752e-05f;     // 0x0129BA3C (bits 0x38D1B717)
extern const double TWO_PI = 6.2831854820251465;                     // 0x0124F2B8 (the float two-pi as a double)
static float g_tick; static int g_tickCalls;
namespace NParticleEngine { float __fastcall GetOneOverTickRate() { g_tickCalls++; return g_tick; } }   // 0x00AEAA00

// ---- deterministic random numbers
static unsigned long g_seed = 0x0b16410u;
static unsigned long Rnd() { g_seed = g_seed * 1664525ul + 1013904223ul; return g_seed >> 8; }
static long RndRange(long lo, long hi) { return lo + (long)(Rnd() % (unsigned long)(hi - lo + 1)); }
static float RndFloat(float lo, float hi) { return lo + (hi - lo) * (float)(Rnd() & 0xffff) / 65535.0f; }
static float FromBits(unsigned long bits) { float f; memcpy(&f, &bits, 4); return f; }
static unsigned long Bits(float f) { unsigned long b; memcpy(&b, &f, 4); return b; }
// a value as the float the row would store (through memory: a cast alone is not rounded by this compiler)
static double Single(double value) { float f = (float)value; return (double)FromBits(Bits(f)); }

// ---- raw objects with guard words round them
enum { GUARD = 8, STATE = 17, SETTINGS = 12, MATRIX = 12 };
static unsigned long g_stateBuf[GUARD + STATE + GUARD];
static unsigned long g_settingsBuf[GUARD + SETTINGS + GUARD];
static unsigned long g_matrixBuf[GUARD + MATRIX + GUARD];
static float* State() { return (float*)(g_stateBuf + GUARD); }
static unsigned long* Settings() { return g_settingsBuf + GUARD; }
static float* Matrix() { return (float*)(g_matrixBuf + GUARD); }
static float SettingFloat(int index) { return FromBits(Settings()[index]); }

static int g_fail;
static long g_case;
static void Fail(const char* what, int index, double got, double want)
{
    if (g_fail < 12) printf("case %ld: %s[%d] got %.9g want %.9g\n", g_case, what, index, got, want);
    g_fail++;
}

// ---- the model
static double ModelFastCosIndexed(double angle, long shift)
{
    double scaled = angle * 1024.0;
    double pos = Single(scaled);                                     // the helper's `pos` is a float; its argument expression
    double shifted = Single(scaled - 0.5);                           // is not rounded before the half is taken off
    double fl = floor(shifted); double diff = shifted - fl; long index = (long)fl;
    if (diff > 0.5 || (diff == 0.5 && (index & 1))) index++;         // round to nearest, ties to even
    double interp = pos - (double)index;
    long slot = ((index - shift) % 1024 + 1024) % 1024;
    return (1.0 - interp) * (double)GFastCosTable[slot] + interp * (double)GFastCosTable[slot + 1];
}
static double ModelFastCos(double angle) { return ModelFastCosIndexed(angle, 0); }
static double ModelFastSin(double angle) { return ModelFastCosIndexed(angle, 256); }
static bool ModelNonZero(float value) { return fabs((double)value) > (double)DELTA_NOTIONAL_ZERO; }

struct Expect { double state[STATE]; double matrix[MATRIX]; bool stateExact[STATE]; bool matrixExact[MATRIX]; };
static long g_statDisabled, g_statSqueeze, g_statPlain, g_statNoExpand, g_statNoCycleTime, g_statCycle, g_statClamp, g_statNoClamp;
static long g_statPlane[4], g_statWhole, g_statTranslate, g_statEdge;

static void RunModel(const float* state, const unsigned long* settings, const float* matrix, float time_secs, Expect& e)
{
    int i;
    long type = (long)settings[0];
    bool enabled = (settings[1] & 0xff) != 0;
    double radius_setting = FromBits(settings[2]), expand = FromBits(settings[3]);
    bool cycle_flag = (settings[4] & 0xff) != 0;
    double cycle_time = FromBits(settings[5]), squeeze_scale = FromBits(settings[6]), squeeze_angle = FromBits(settings[7]);
    double rotate_speed = FromBits(settings[8]);
    for (i = 0; i < STATE; i++) { e.state[i] = state[i]; e.stateExact[i] = true; }
    for (i = 0; i < MATRIX; i++) { e.matrix[i] = matrix[i]; e.matrixExact[i] = true; }

    double angle = Single((rotate_speed + (double)state[1]) * (double)g_tick + (double)state[0]);
    e.state[0] = angle; e.stateExact[0] = false;
    for (i = 0; i < 3; i++) { e.state[2 + i] = state[5 + i]; e.state[5 + i] = state[8 + i]; }      // from = to, to = next

    double x = 0.0, y = 0.0;
    if (enabled) {
        if (ModelNonZero(FromBits(settings[7]))) {
            g_statSqueeze++;
            double px = ModelFastCos(angle + squeeze_angle);
            double py = ModelFastSin(angle + squeeze_angle) * squeeze_scale;
            double c = ModelFastCos(-squeeze_angle), s = ModelFastSin(-squeeze_angle);
            x = px * c + py * s;
            y = py * c - px * s;
        } else {
            g_statPlain++;
            x = ModelFastCos(angle);
            y = ModelFastSin(angle) * squeeze_scale;
        }
        double radius = radius_setting;
        if (!ModelNonZero(FromBits(settings[3]))) g_statNoExpand++;
        else if (!ModelNonZero(FromBits(settings[5]))) g_statNoCycleTime++;
        else if (cycle_flag) {
            g_statCycle++;
            radius = Single(radius + (1.0 - ModelFastCos((double)time_secs / cycle_time)) * expand * 0.5);
        } else {
            double t = time_secs;
            if (t > cycle_time) { t = cycle_time; g_statClamp++; } else g_statNoClamp++;
            radius = Single(radius + ModelFastSin(t / cycle_time * 0.25) * expand);
        }
        x *= radius; y *= radius;
    } else g_statDisabled++;

    long plane = type & 3;
    g_statPlane[plane]++;
    double next[3];
    for (i = 0; i < 3; i++) next[i] = state[8 + i];
    if (plane == 0) { next[0] = x; next[1] = y; next[2] = 0.0; }
    else if (plane == 1) { next[0] = x; next[1] = 0.0; next[2] = y; }
    else if (plane == 2) { next[0] = 0.0; next[1] = x; next[2] = y; }
    if (plane != 3) for (i = 0; i < 3; i++) next[i] = Single(next[i]);

    double out[3];
    if (type & 4) {
        g_statWhole++;
        for (i = 0; i < 3; i++) out[i] = next[0] * matrix[i] + next[1] * matrix[3 + i] + next[2] * matrix[6 + i] + matrix[9 + i];
    } else {
        g_statTranslate++;
        for (i = 0; i < 3; i++) out[i] = next[i] + matrix[9 + i];
    }
    for (i = 0; i < 3; i++) { e.state[8 + i] = out[i]; e.stateExact[8 + i] = false; }

    if (plane != 3) {
        double turn = -angle * TWO_PI;
        double c = cos(turn), s = sin(turn);
        int a = (plane == 2) ? 3 : 0;                               // the two matrix rows that turn: plane 0 rows 1 / 2,
        int b = (plane == 0) ? 3 : 6;                               // plane 1 rows 1 / 3, plane 2 rows 2 / 3
        double sign = (plane == 1) ? 1.0 : -1.0;                    // the Y turn has the opposite sense
        for (i = 0; i < 3; i++) {
            double ra = matrix[a + i], rb = matrix[b + i];
            e.matrix[a + i] = ra * c + sign * rb * s;
            e.matrix[b + i] = rb * c - sign * ra * s;
            e.matrixExact[a + i] = e.matrixExact[b + i] = false;
        }
    }
    // the matrix translation is the new point (bit-equal to the state's NextPosition: checked separately)
    for (i = 0; i < 3; i++) { e.matrix[9 + i] = out[i]; e.matrixExact[9 + i] = false; }
    for (i = 0; i < 3; i++) {
        e.state[11 + i] = state[14 + i];                                                           // from tangent = to tangent
        e.state[14 + i] = (out[i] - (double)state[5 + i]) * 0.5; e.stateExact[14 + i] = false;     // (next - from) / 2
    }
}

// Fills the stack the row is about to use with conspicuous floats, so that a local the row forgets to set shows.
static void Sink(void*) { }
static void (*g_sink)(void*) = Sink;
static void DirtyStack()
{
    float junk[256];
    for (int i = 0; i < 256; i++) junk[i] = 1000.0f + (float)(Rnd() & 0xff);
    g_sink(junk);
}

static float PickThreshold(int kind, float lo, float hi)
{
    // 0: zero, 1 / 2: exactly the notional zero (+ / -), 3 / 4: one float above it, 5 / 6: one below, 7: half of it, else: far away
    switch (kind) {
    case 0: return 0.0f;
    case 1: return FromBits(0x38D1B717u);
    case 2: return FromBits(0xB8D1B717u);
    case 3: return FromBits(0x38D1B718u);
    case 4: return FromBits(0xB8D1B718u);
    case 5: return FromBits(0x38D1B716u);
    case 6: return FromBits(0xB8D1B716u);
    case 7: return (Rnd() & 1) ? 5e-5f : -5e-5f;
    }
    float value = RndFloat(lo, hi);
    return (Rnd() & 1) ? value : -value;
}

static void OneCase()
{
    int i;
    for (i = 0; i < GUARD + STATE + GUARD; i++) g_stateBuf[i] = Rnd() * 2654435761ul;
    for (i = 0; i < GUARD + SETTINGS + GUARD; i++) g_settingsBuf[i] = Rnd() * 2246822519ul;
    for (i = 0; i < GUARD + MATRIX + GUARD; i++) g_matrixBuf[i] = Rnd() * 3266489917ul;

    int small = (int)(Rnd() % 3 == 0);                               // a small world: unit radius, no translation
    int edge = (int)(Rnd() % 3 == 0);                                // the threshold values of the three "non zero" tests
    if (edge) g_statEdge++;
    float* state = State(); unsigned long* settings = Settings(); float* matrix = Matrix();
    state[0] = RndFloat(-4.0f, 4.0f);
    state[1] = RndFloat(-1.0f, 1.0f);
    for (i = 2; i < STATE; i++) state[i] = small ? RndFloat(-1.0f, 1.0f) : RndFloat(-4.0f, 4.0f);
    for (i = 0; i < 9; i++) matrix[i] = small ? RndFloat(-1.0f, 1.0f) : RndFloat(-2.0f, 2.0f);
    for (i = 9; i < 12; i++) matrix[i] = small ? (Rnd() % 2 ? 0.0f : RndFloat(-1.0f, 1.0f)) : RndFloat(-4.0f, 4.0f);

    unsigned long type = (unsigned long)RndRange(0, 7);
    if (Rnd() % 4 == 0) type |= (Rnd() * 40503ul) & 0xfffffff8ul;                                  // bits the row must ignore
    settings[0] = type;
    settings[1] = (settings[1] & 0xffffff00ul) | (Rnd() % 6 == 0 ? 0u : (unsigned long)RndRange(1, 255));     // Enabled + padding
    float radius = small ? RndFloat(0.5f, 1.5f) : RndFloat(-3.0f, 3.0f);
    float expand = PickThreshold(edge ? (int)(Rnd() % 9) : (Rnd() % 5 == 0 ? 0 : 8), 0.25f, 2.0f);
    settings[4] = (settings[4] & 0xffffff00ul) | (Rnd() % 2 == 0 ? 0u : (unsigned long)RndRange(1, 255));     // CycleFlag + padding
    float cycle_time = PickThreshold(edge ? (int)(Rnd() % 9) : (Rnd() % 6 == 0 ? 0 : 8), 0.5f, 8.0f);
    float squeeze_scale = RndFloat(-2.0f, 2.0f);
    float squeeze_angle = PickThreshold(edge ? (int)(Rnd() % 9) : (Rnd() % 3 == 0 ? 0 : 8), 0.02f, 1.0f);
    float rotate_speed = RndFloat(-1.0f, 1.0f);
    memcpy(&settings[2], &radius, 4); memcpy(&settings[3], &expand, 4); memcpy(&settings[5], &cycle_time, 4);
    memcpy(&settings[6], &squeeze_scale, 4); memcpy(&settings[7], &squeeze_angle, 4); memcpy(&settings[8], &rotate_speed, 4);
    float time_secs = (Rnd() % 8 == 0) ? RndFloat(-3.0f, 0.0f) : RndFloat(0.0f, 12.0f);
    if (Rnd() % 16 == 0) time_secs = cycle_time;                                                   // the clamp's own boundary
    g_tick = (Rnd() % 3 == 0) ? (1.0f / 15.0f) : RndFloat(0.0f, 0.25f);
    g_tickCalls = 0;

    // the magnitude bound of everything the case computes
    double max_matrix = 0.0, max_translate = 0.0, max_state = 0.0;
    for (i = 0; i < 9; i++) if (fabs((double)matrix[i]) > max_matrix) max_matrix = fabs((double)matrix[i]);
    for (i = 9; i < 12; i++) if (fabs((double)matrix[i]) > max_translate) max_translate = fabs((double)matrix[i]);
    for (i = 2; i < STATE; i++) if (fabs((double)state[i]) > max_state) max_state = fabs((double)state[i]);
    double point = (fabs((double)radius) + fabs((double)expand)) * (1.0 + fabs((double)squeeze_scale));
    if (max_state > point) point = max_state;                        // plane 3 keeps the old point
    double bound = 1.0 + point * (1.0 + 3.0 * max_matrix) + max_translate + max_state + 2.0 * max_matrix;
    double tolerance = 2e-6 * bound;

    static unsigned long stateBefore[GUARD + STATE + GUARD], settingsBefore[GUARD + SETTINGS + GUARD], matrixBefore[GUARD + MATRIX + GUARD];
    memcpy(stateBefore, g_stateBuf, sizeof(stateBefore)); memcpy(settingsBefore, g_settingsBuf, sizeof(settingsBefore));
    memcpy(matrixBefore, g_matrixBuf, sizeof(matrixBefore));
    static Expect e;
    RunModel((const float*)(stateBefore + GUARD), settingsBefore + GUARD, (const float*)(matrixBefore + GUARD), time_secs, e);

    DirtyStack();
    CallUpdate(state, 0, settings, matrix, time_secs);

    if (g_tickCalls != 1) Fail("tick rate calls", 0, g_tickCalls, 1);
    if (memcmp(g_settingsBuf, settingsBefore, sizeof(settingsBefore))) Fail("settings changed", 0, 0, 0);
    for (i = 0; i < GUARD; i++) {
        if (g_stateBuf[i] != stateBefore[i] || g_stateBuf[GUARD + STATE + i] != stateBefore[GUARD + STATE + i]) Fail("state guard", i, 0, 0);
        if (g_matrixBuf[i] != matrixBefore[i] || g_matrixBuf[GUARD + MATRIX + i] != matrixBefore[GUARD + MATRIX + i]) Fail("matrix guard", i, 0, 0);
    }
    for (i = 0; i < STATE; i++) {
        double got = state[i];
        if (got != got) { Fail("state not a number", i, got, e.state[i]); continue; }
        if (e.stateExact[i]) { if (Bits(state[i]) != Bits((float)e.state[i])) Fail("state copy", i, got, e.state[i]); }
        else if (fabs(got - e.state[i]) > tolerance) Fail("state", i, got, e.state[i]);
    }
    for (i = 0; i < MATRIX; i++) {
        double got = matrix[i];
        if (got != got) { Fail("matrix not a number", i, got, e.matrix[i]); continue; }
        if (e.matrixExact[i]) { if (Bits(matrix[i]) != Bits((float)e.matrix[i])) Fail("matrix copy", i, got, e.matrix[i]); }
        else if (fabs(got - e.matrix[i]) > tolerance) Fail("matrix", i, got, e.matrix[i]);
    }
    for (i = 0; i < 3; i++) if (Bits(matrix[9 + i]) != Bits(state[8 + i])) Fail("translation is not the new point", i, matrix[9 + i], state[8 + i]);
}

int main()
{
    SetUnhandledExceptionFilter((void*)Crash);
    int k;
    for (k = 0; k <= 1024; k++) GFastCosTable[k] = (float)cos(2.0 * 3.14159265358979323846 * (double)k / 1024.0);
    const long cases = 60000;
    for (g_case = 0; g_case < cases; g_case++) OneCase();
    printf("cases %ld: disabled %ld, squeeze %ld, plain %ld, no expand %ld, no cycle time %ld, cycle %ld, clamp %ld, no clamp %ld,\n"
           "  plane %ld / %ld / %ld / %ld, whole matrix %ld, translate %ld, threshold cases %ld\n",
           cases, g_statDisabled, g_statSqueeze, g_statPlain, g_statNoExpand, g_statNoCycleTime, g_statCycle, g_statClamp, g_statNoClamp,
           g_statPlane[0], g_statPlane[1], g_statPlane[2], g_statPlane[3], g_statWhole, g_statTranslate, g_statEdge);
    if (!g_statDisabled || !g_statSqueeze || !g_statPlain || !g_statNoExpand || !g_statNoCycleTime || !g_statCycle || !g_statClamp
        || !g_statNoClamp || !g_statPlane[0] || !g_statPlane[1] || !g_statPlane[2] || !g_statPlane[3] || !g_statWhole
        || !g_statTranslate || !g_statEdge) { printf("a path was never taken\n"); g_fail++; }
    if (g_fail) { printf("00b16410_TEST FAIL (%d)\n", g_fail); return 1; }
    printf("00b16410_TEST PASS\n");
    return 0;
}
