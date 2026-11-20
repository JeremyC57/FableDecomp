// Behaviour test for 0x00B99300 CEnginePrimitiveManagerRepeatedStaticMeshes::ProcessLightingSW (links the REAL row object).
// Fixtures (raw native layout, offsets read by the row): the manager's SIMDVariableBlock +0x90 (0x00B99335), the block's Colour +0x700
// and MainLightColour +0x900 (16-byte CRGBFloatColour entries), the primitive's RenderInfo.Lights +0xA4 (0x00B9932F), the resource
// manager's lighting-result constant registers at +0x3018 (colour) / +0x3024 (main light) (0x00B99644 / 0x00B99529). The lighting
// manager and shader render manager members are provided here and log every call (receiver, arguments, the per-instance position
// and normal values). Globals: GEnableSIMD, the two debug statics bb (first switch) / aa (second switch).
// Reference model (independent): aligned = (count + 3) & ~3; shadowed: SSE -> SeparateMainLightVector(Colour, MainLightColour,
// offsets, nx, ny, nz, aligned, lights), else per instance obj < count SeparateMainLight(pos = offsets[obj].xyz, normal = (nx, ny,
// nz)[obj], lights, MainLightColour[obj], Colour[obj]); first switch -> count = 16; second switch -> MainLightColour[n] = (1, 0, 0, 0)
// for n < count; SetVSConstants(main-light register, MainLightColour, count). Not shadowed: SSE -> NoClipVector(Colour, offsets, nx,
// ny, nz, aligned, lights), else per instance NoClip(pos, normal, lights, Colour[obj]). Last: SetVSConstants(colour register, Colour,
// count).
#include <stdio.h>
#include <string.h>

class C3DVector { public: float X, Y, Z; };
class C4DVector { public: float X, Y, Z, W; };
class CRGBFloatColour { public: float R, G, B, A; };
class CEngineLocalLightArray { public: unsigned long LightCount; };
class CEngineInternalPrimitiveRepeatedStaticMesh;
class CEngineLightingManager { public:
    void CalcSWLightingNoClip(const C3DVector& pos, const C3DVector& normal, const CEngineLocalLightArray& light_array, CRGBFloatColour& light_result);
    void CalcSWLightingNoClipSeparateMainLight(const C3DVector& pos, const C3DVector& normal, const CEngineLocalLightArray& light_array,
                                               CRGBFloatColour& main_light_result, CRGBFloatColour& misc_light_result);
    void CalcSWLightingNoClipVector(CRGBFloatColour* results, const C4DVector* position, const float* vec_normal_x, const float* vec_normal_y,
                                    const float* vec_normal_z, long count, const CEngineLocalLightArray& light_array);
    void CalcSWLightingNoClipSeparateMainLightVector(CRGBFloatColour* results, CRGBFloatColour* results_main_light, const C4DVector* position,
                                                     const float* vec_normal_x, const float* vec_normal_y, const float* vec_normal_z, long count,
                                                     const CEngineLocalLightArray& light_array); };
class CShaderRenderManager { void SetVSConstants(long reg, const void* data, unsigned long count); };
class CEngineResourceManager;
class CEnginePrimitiveManagerRepeatedStaticMeshes {
    void ProcessLightingSW(CEngineInternalPrimitiveRepeatedStaticMesh& primitive, const float* normals_x, const float* normals_y,
                           const float* normals_z, const C4DVector* object_offsets, long count, bool shadowed);
public:
    static void Call(void* t, void* prim, const float* x, const float* y, const float* z, const C4DVector* o, long count, bool shadowed)
    { ((CEnginePrimitiveManagerRepeatedStaticMeshes*)t)->ProcessLightingSW(*(CEngineInternalPrimitiveRepeatedStaticMesh*)prim, x, y, z, o, count, shadowed); } };

bool GEnableSIMD;
int aa;   // 0x0143B954: red main light
int bb;   // 0x0143B958: 16-entry upload
static unsigned char LightingManagerObj[16], ShaderManagerObj[16];
static unsigned char ResourceManagerObj[0x3030];
namespace NEngineInternal {
    CEngineLightingManager* GPLightingManager = (CEngineLightingManager*)LightingManagerObj;
    CShaderRenderManager* GPShaderRenderManager = (CShaderRenderManager*)ShaderManagerObj;
    CEngineResourceManager* GPResourceManager = (CEngineResourceManager*)ResourceManagerObj;
}

static char Log[16384]; static int LogLen;
static void L(const char* fmt, ...);
#include <stdarg.h>
static void L(const char* fmt, ...) { va_list a; va_start(a, fmt); LogLen += vsprintf(Log + LogLen, fmt, a); va_end(a); }
static const void* Base;   // log pointers relative to a fixture base
static long R(const void* p) { return (long)((const char*)p - (const char*)Base); }

void CEngineLightingManager::CalcSWLightingNoClip(const C3DVector& pos, const C3DVector& normal, const CEngineLocalLightArray& la, CRGBFloatColour& res)
{ L("NC %ld p(%g %g %g) n(%g %g %g) L%ld r%ld;", R(this), pos.X, pos.Y, pos.Z, normal.X, normal.Y, normal.Z, R(&la), R(&res)); }
void CEngineLightingManager::CalcSWLightingNoClipSeparateMainLight(const C3DVector& pos, const C3DVector& normal, const CEngineLocalLightArray& la,
                                                                    CRGBFloatColour& m, CRGBFloatColour& c)
{ L("SM %ld p(%g %g %g) n(%g %g %g) L%ld m%ld c%ld;", R(this), pos.X, pos.Y, pos.Z, normal.X, normal.Y, normal.Z, R(&la), R(&m), R(&c)); }
void CEngineLightingManager::CalcSWLightingNoClipVector(CRGBFloatColour* r, const C4DVector* p, const float* x, const float* y, const float* z,
                                                        long count, const CEngineLocalLightArray& la)
{ L("NV %ld r%ld p%ld x%ld y%ld z%ld c%ld L%ld;", R(this), R(r), R(p), R(x), R(y), R(z), count, R(&la)); }
void CEngineLightingManager::CalcSWLightingNoClipSeparateMainLightVector(CRGBFloatColour* r, CRGBFloatColour* m, const C4DVector* p, const float* x,
                                                                         const float* y, const float* z, long count, const CEngineLocalLightArray& la)
{ L("SV %ld r%ld m%ld p%ld x%ld y%ld z%ld c%ld L%ld;", R(this), R(r), R(m), R(p), R(x), R(y), R(z), count, R(&la)); }
void CShaderRenderManager::SetVSConstants(long reg, const void* data, unsigned long count)
{ L("VS %ld reg%ld d%ld c%lu;", R(this), reg, R(data), count); }

// one arena so every logged pointer is a stable offset
static struct Arena {
    unsigned char Manager[0x94];
    unsigned char Prim[0xB0];
    CEngineLocalLightArray Lights;
    float NX[40], NY[40], NZ[40];
    C4DVector Offsets[40];
    __declspec(align(16)) unsigned char Block[0xC00];
} A;
static char Want[16384]; static int WantLen;
static void W_(const char* fmt, ...) { va_list a; va_start(a, fmt); WantLen += vsprintf(Want + WantLen, fmt, a); va_end(a); }

static int failures, cases;
static void Case(long count, bool shadowed, bool simd, long dbg1, long dbg2)
{
    ++cases;
    memset(A.Block, 0x5A, sizeof A.Block);
    unsigned char before[0xC00]; memcpy(before, A.Block, sizeof before);
    GEnableSIMD = simd; bb = (int)dbg1; aa = (int)dbg2;
    LogLen = 0; Log[0] = 0; WantLen = 0; Want[0] = 0;
    CEnginePrimitiveManagerRepeatedStaticMeshes::Call(A.Manager, A.Prim, A.NX, A.NY, A.NZ, A.Offsets, count, shadowed);
    // model
    Base = &A;
    CRGBFloatColour* colour = (CRGBFloatColour*)(A.Block + 0x700);
    CRGBFloatColour* main = (CRGBFloatColour*)(A.Block + 0x900);
    long aligned = (count + 3) & ~3;
    long lm = R(LightingManagerObj), sm = R(ShaderManagerObj);
    if (shadowed) {
        if (simd) W_("SV %ld r%ld m%ld p%ld x%ld y%ld z%ld c%ld L%ld;", lm, R(colour), R(main), R(A.Offsets), R(A.NX), R(A.NY), R(A.NZ), aligned, R(&A.Lights));
        else for (long i = 0; i < count; ++i)
            W_("SM %ld p(%g %g %g) n(%g %g %g) L%ld m%ld c%ld;", lm, A.Offsets[i].X, A.Offsets[i].Y, A.Offsets[i].Z, A.NX[i], A.NY[i], A.NZ[i],
               R(&A.Lights), R(&main[i]), R(&colour[i]));
        if (dbg1) count = 16;
        if (dbg2) for (long n = 0; n < count; ++n) { float v[4] = { 1.0f, 0.0f, 0.0f, 0.0f }; memcpy(before + 0x900 + n * 16, v, 16); }
        W_("VS %ld reg%ld d%ld c%lu;", sm, 37L, R(main), (unsigned long)count);
    } else {
        if (simd) W_("NV %ld r%ld p%ld x%ld y%ld z%ld c%ld L%ld;", lm, R(colour), R(A.Offsets), R(A.NX), R(A.NY), R(A.NZ), aligned, R(&A.Lights));
        else for (long i = 0; i < count; ++i)
            W_("NC %ld p(%g %g %g) n(%g %g %g) L%ld r%ld;", lm, A.Offsets[i].X, A.Offsets[i].Y, A.Offsets[i].Z, A.NX[i], A.NY[i], A.NZ[i],
               R(&A.Lights), R(&colour[i]));
    }
    W_("VS %ld reg%ld d%ld c%lu;", sm, 21L, R(colour), (unsigned long)count);
    bool ok = strcmp(Log, Want) == 0 && memcmp(before, A.Block, sizeof before) == 0;
    if (!ok) {
        ++failures;
        if (failures < 10) printf("FAIL count %ld shadowed %d simd %d dbg %ld %ld\n  got  %s\n  want %s\n  block %s\n", count, shadowed, simd, dbg1, dbg2,
                                  Log, Want, memcmp(before, A.Block, sizeof before) ? "DIFFERS" : "same");
    }
}

int main()
{
    Base = &A;
    *(unsigned char**)(A.Manager + 0x90) = A.Block;
    *(CEngineLocalLightArray**)(A.Prim + 0xA4) = &A.Lights;
    *(long*)(ResourceManagerObj + 0x3018) = 21;
    *(long*)(ResourceManagerObj + 0x3024) = 37;
    for (int i = 0; i < 40; ++i) {
        A.NX[i] = 0.5f + i; A.NY[i] = -1.25f - i; A.NZ[i] = 100.0f + 2 * i;
        A.Offsets[i].X = 1000.0f + i; A.Offsets[i].Y = 2000.0f - i; A.Offsets[i].Z = 3000.0f + 3 * i; A.Offsets[i].W = -7.0f;
    }
    static const long counts[] = { -5, -1, 0, 1, 2, 3, 4, 5, 7, 8, 9, 13, 16, 17, 20, 32 };
    for (int c = 0; c < (int)(sizeof counts / sizeof counts[0]); ++c)
        for (int s = 0; s < 2; ++s)
            for (int m = 0; m < 2; ++m)
                for (int d = 0; d < 4; ++d)
                    Case(counts[c], s != 0, m != 0, (d & 1) ? (d == 3 ? -9 : 1) : 0, (d & 2) ? 5 : 0);
    for (int c = 0; c < (int)(sizeof counts / sizeof counts[0]); ++c)   // audit: a negative second switch, with and without the first
        for (int s = 0; s < 2; ++s)
            for (int m = 0; m < 2; ++m) { Case(counts[c], s != 0, m != 0, 0, -3); Case(counts[c], s != 0, m != 0, 2, -3); }
    printf("cases %d\n", cases);
    printf(failures ? "00b99300_TEST FAIL\n" : "00b99300_TEST PASS\n");
    return failures ? 1 : 0;
}