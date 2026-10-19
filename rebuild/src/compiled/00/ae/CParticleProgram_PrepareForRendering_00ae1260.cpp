// CParticleProgram::PrepareForRendering(C4DVector*, CParticleSimulation*, float, float, ..., CMatrix4x4*, C3DMesh2*) -- the skinned variant: transform the fixed positions by the matrix into the render vertices, interpolate the simulated ones, and when the blend is significant blend them towards the transformed initial simulation (built in the group vector buffer)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AE1260, length 192 bytes (`ret 2Ch`; the 11-argument overload).
// Leaf: ?PrepareForRendering@CParticleProgram@@QAEXPAUC4DVector@@PAUCParticleSimulation@@MMPAUC3DAnimationBlendState
// Callees: CParticleProgram::TransformVertices(C4DVector*, const C4DVector*, ulong, const CMatrix4x4*, const
//          CArray<C3DGroup2>*) @ 0x00AE0E60 (x2), GetGroupVectorBuffer(ulong) @ 0x00AE0540 (__fastcall), the 8-argument
//          vector interpolation @ 0x00A5BBA0 and GFInterpolateVectors @ 0x00A5B870. Global: 1e-4f @ 0x0129BA3C.
// Layout: as the 4-argument overload 0x00AE1600 (retail 12-byte containers); only the matrix of the animation
// arguments is used.
// Shape: the buffer pointer from GetGroupVectorBuffer stays in ebx for the transform and the blend.
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
    void TransformVertices(C4DVector* out, const C4DVector* in, unsigned long count, const CMatrix4x4* matrix,
                           const CArray<C3DGroup2>* groups);                                                         // 0x00AE0E60
    void PrepareForRendering(C4DVector* vertices, CParticleSimulation* simulation, float simulationInterpolate, float initialBlend,
                             C3DAnimationBlendState* blendState, C3DAnimationBlendState* lastBlendState, float animationInterpolate,
                             C3DAnimationManager* animationManager, C3DVector* position, CMatrix4x4* matrix, C3DMesh2* mesh);
};

C4DVector* __fastcall GetGroupVectorBuffer(unsigned long count);   // 0x00AE0540

void CParticleProgram::PrepareForRendering(C4DVector* vertices, CParticleSimulation* simulation, float simulationInterpolate, float initialBlend,
                                           C3DAnimationBlendState* blendState, C3DAnimationBlendState* lastBlendState, float animationInterpolate,
                                           C3DAnimationManager* animationManager, C3DVector* position, CMatrix4x4* matrix, C3DMesh2* mesh)
{
    TransformVertices(vertices, NonSimPositions, NonSimCount, matrix, &NonSimGroups);
    GFInterpolateVectors_00a5bba0(vertices + NonSimCount, sizeof(C4DVector), simulation->SavedPositions, sizeof(C4DVector),
                                  simulation->Positions, sizeof(C4DVector), simulation->Size, simulationInterpolate);
    if (initialBlend > g_MinimumMorphBlend) {
        C4DVector* initial = GetGroupVectorBuffer(simulation->Size);
        TransformVertices(initial, InitialSimulation.Positions, simulation->Size, matrix, &SimGroups);
        GFInterpolateVectors(vertices + NonSimCount, sizeof(C4DVector), initial, sizeof(C4DVector), simulation->Size, initialBlend);
    }
}