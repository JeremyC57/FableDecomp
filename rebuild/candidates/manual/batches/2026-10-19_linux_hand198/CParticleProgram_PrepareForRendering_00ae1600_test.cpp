#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#define __declspec(x)
#endif
#define free test_free
#define malloc test_malloc
// Behaviour test for 00ae1600: the reconstructed source below (copied verbatim from src/) runs against stub callees.
// CParticleProgram::PrepareForRendering(C4DVector*, CParticleSimulation*, float, float) -- fill the render vertices: the fixed (non-simulated) positions first, then the simulated ones interpolated between the saved and current simulation states, blended towards the initial simulation when the blend is significant
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AE1600, length 181 bytes (`ret 10h`; the 4-argument overload).
// Leaf: ?PrepareForRendering@CParticleProgram@@QAEXPAUC4DVector@@PAUCParticleSimulation@@MM@Z
// Callees: the 8-argument vector interpolation @ 0x00A5BBA0 and GFInterpolateVectors @ 0x00A5B870 (__cdecl, labels
//          both "GFInterpolateVectors"; strides 16). Global: 1e-4f @ 0x0129BA3C (ordered greater-than test).
// Layout: engine/CParticleProgram.h is the PDB layout with 16-byte CArrays and a 0x14-byte vector map; the retail
// containers are 12 / 0x10 bytes, so NonSimCount/NonSimPositions sit at +0xbc/+0xc0 and SimGroups/NonSimGroups at
// +0xfc/+0x108 (modelled locally). The initial simulation's Positions is at +0x24.
// Shape: the non-sim copy is the inline SGI random-access copy (n = (last - first), 16-byte elements; the result
// pointer runs in a copy so the argument is re-read after the loop).
struct C4DVector { float X, Y, Z, W; };
struct CMatrix4x4;
struct C3DGroup2;
struct C3DAnimationBlendState;
struct C3DAnimationManager;
struct C3DVector;
struct C3DMesh2;
struct CMovableResourceMemoryPool;

template<class T> struct CArray {                  // retail CArray<T>: three pointers
    T* m_pData;
    T* m_pLast;
    T* m_pEnd;
};

// Interpolate `count` vectors from `from` towards `to` by `t` into `out` (strides in bytes).
void __cdecl GFInterpolateVectors_00a5bba0(C4DVector* out, unsigned long outStride, const C4DVector* from, unsigned long fromStride,
                                          const C4DVector* to, unsigned long toStride, unsigned long count, float t);   // 0x00A5BBA0
// Blend `count` vectors of `out` towards `to` by `t` in place.
void __cdecl GFInterpolateVectors(C4DVector* out, unsigned long outStride, const C4DVector* to, unsigned long toStride,
                                 unsigned long count, float t);                                                      // 0x00A5B870

extern const float g_MinimumMorphBlend;            // 0x0129BA3C (1e-4f)

struct CParticleSimulation {                       // Ego_r.pdb, retail 0x80 bytes
    void*         __vftable;                       // +0x00
    unsigned char RenderData[8];                   // +0x04 CCountedPointer<CParticleSimulationRenderData>
    float         Timestep;                        // +0x0c
    bool          TimestepChanged;                 // +0x10
    float         TimestepMultiplier;              // +0x14
    unsigned long Size;                            // +0x18
    C4DVector*    Positions;                       // +0x1c
    C4DVector*    LastPositions;                   // +0x20
    C4DVector*    SavedPositions;                  // +0x24
    C4DVector*    Accelerations;                   // +0x28
    float*        SimulationAlphas;                // +0x2c
    unsigned char _rest[0x50];                     // +0x30 .. +0x80
};

template<class In, class Out> __forceinline Out CopyVectors(In first, In last, Out result)
{
    for (long n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}

class CParticleProgram {
public:
    void*               __vftable;                 // +0x00
    unsigned char       _base04[4];                // +0x04
    CParticleSimulation InitialSimulation;         // +0x08
    unsigned char       ExportParticles[0x10];     // +0x88 CVectorMap<CCharString, unsigned long>
    CArray<void>        RenderTris;                // +0x98
    CArray<void>        RenderQuads;               // +0xa4
    CArray<void>        RenderVertices;            // +0xb0
    unsigned long       NonSimCount;               // +0xbc
    C4DVector*          NonSimPositions;           // +0xc0
    CArray<void>        IndexedTextureCoords;      // +0xc4
    CArray<C4DVector>   TempVertexPositions;       // +0xd0
    CArray<long>        ParticleIndices;           // +0xdc
    CArray<long>        VertexIndices;             // +0xe8
    bool                BezierEnable;              // +0xf4
    float               AveragePatchSize;          // +0xf8
    CArray<C3DGroup2>   SimGroups;                 // +0xfc
    CArray<C3DGroup2>   NonSimGroups;              // +0x108
    void PrepareForRendering(C4DVector* vertices, CParticleSimulation* simulation, float simulationInterpolate, float initialBlend);
};

void CParticleProgram::PrepareForRendering(C4DVector* vertices, CParticleSimulation* simulation, float simulationInterpolate, float initialBlend)
{
    CopyVectors(NonSimPositions, NonSimPositions + NonSimCount, vertices);
    GFInterpolateVectors_00a5bba0(vertices + NonSimCount, sizeof(C4DVector), simulation->SavedPositions, sizeof(C4DVector),
                                  simulation->Positions, sizeof(C4DVector), simulation->Size, simulationInterpolate);
    if (initialBlend > g_MinimumMorphBlend)
        GFInterpolateVectors(vertices + NonSimCount, sizeof(C4DVector), InitialSimulation.Positions, sizeof(C4DVector),
                             simulation->Size, initialBlend);
}

// ---- stubs and driver ----
static char g_log[512];
static char g_num[128];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ","); }
const float g_MinimumMorphBlend = 1e-4f;
void __cdecl GFInterpolateVectors_00a5bba0(C4DVector* out, unsigned long os, const C4DVector* from, unsigned long fs,
                                          const C4DVector* to, unsigned long ts, unsigned long count, float t)
{
    std::sprintf(g_num, "I8:%lu/%lu/%lu/%lu/%g", os, fs, ts, count, t); Log(g_num);
    for (unsigned long i = 0; i < count; ++i) { out[i].X = from[i].X + (to[i].X - from[i].X) * t; out[i].W = 9; }
}
void __cdecl GFInterpolateVectors(C4DVector* out, unsigned long os, const C4DVector* to, unsigned long ts, unsigned long count, float t)
{
    std::sprintf(g_num, "I6:%lu/%lu/%lu/%g", os, ts, count, t); Log(g_num);
    for (unsigned long i = 0; i < count; ++i) out[i].X = out[i].X + (to[i].X - out[i].X) * t;
}
static void Fill(C4DVector* v, int n, float base) { for (int i = 0; i < n; ++i) { v[i].X = base + i; v[i].Y = base; v[i].Z = -base; v[i].W = 1; } }
int main()
{
    static CParticleProgram prog;
    C4DVector fixed[2], saved[3], current[3], initial[3], out[8];
    Fill(fixed, 2, 100); Fill(saved, 3, 0); Fill(current, 3, 10); Fill(initial, 3, 50);
    std::memset(out, 0, sizeof(out));
    prog.NonSimCount = 2; prog.NonSimPositions = fixed; prog.InitialSimulation.Positions = initial;
    CParticleSimulation sim;
    std::memset(&sim, 0, sizeof(sim));
    sim.Size = 3; sim.SavedPositions = saved; sim.Positions = current;
    prog.PrepareForRendering(out, &sim, 0.5f, 0.0f);
    bool ok = out[0].X == 100 && out[1].X == 101 && out[1].Z == -100 && out[1].W == 1 && out[2].X == 5 && out[4].X == 7 && out[2].W == 9 &&
              std::strcmp(g_log, "I8:16/16/16/3/0.5,") == 0;
    g_log[0] = 0;
    prog.PrepareForRendering(out, &sim, 0.5f, 0.5f);
    ok = ok && out[2].X == 27.5f && std::strcmp(g_log, "I8:16/16/16/3/0.5,I6:16/16/3/0.5,") == 0;
    if (!ok) { std::printf("00ae1600_TEST FAIL\n"); return 1; }
    std::printf("00ae1600_TEST PASS\n");
    return 0;
}
