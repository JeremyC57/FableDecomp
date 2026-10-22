// Grade: DIFFER(376v681) -- lane gap: inline __asm SSE (the 0x013D2880 branch multiplies each bone matrix by the inverse rest matrix in a hand-written movaps/shufps/mulps/addps block reading pointers the C code stored to frame slots, on a 16-byte aligned ebp frame); the source covers the x87 branch's maths only
// NParticleEngine::CParticleProgram::ClingToMesh(CParticleSimulation* simulation, C3DAnimationBlendState* blendState, C3DAnimationManager* animation, const C3DVector* position, C3DMesh2* mesh) -- pull the simulated particles back onto the animated mesh: compute the mesh's bone matrices for this frame, premultiply each by the mesh's inverse rest-pose bone matrix, skin the program's vertices with them into a scratch buffer and blend the simulation positions toward the skinned positions by each particle's inverse mass
// Leaf: ClingToMesh
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE1320, 681 bytes (+ int3 padding), `ret 14h`, 16-byte aligned ebp frame.
// Callees: the scratch C4DVector buffer getter 0x00AE0540 and the scratch matrix buffer getter 0x00AE04E0 (both __fastcall
//   (count), manifest "GetGroupVectorBuffer"), C3DAnimationManager::CalcMeshTransformations 0x00A9F540, CParticleProgram::
//   TransformVertices 0x00AE0E60, GFInterpolateVectors 0x00A5BEF0 (__cdecl). Global: the SSE-enabled byte 0x013D2880.
// Layout: CParticleProgram NumVertices +0x20, Vertices (C4DVector*) +0x24, Groups (CArray<C3DGroup2>) +0xfc; C3DMesh2
//   NumBones +0x98, InverseBoneMatrices (CMatrix4x4*) +0xe0; CParticleSimulation Positions +0x1c, InverseMasses +0x2c.
// Note: the SSE branch (bone matrix times inverse matrix with movaps/shufps/mulps/addps on frame-held pointer copies) is an
//   inline __asm block -- lane gap: inline __asm SSE. This is the behavioural C++ (x87 branch maths only).
struct C3DVector { float X, Y, Z; };
struct C4DVector { float X, Y, Z, W; };
struct CMatrix4x4 { float M[4][4]; };
struct C3DAnimationBlendState;
struct C3DGroup2;
struct CGroupArray { void* Data; unsigned long Size; unsigned long Capacity; };
struct C3DMesh2 {
    unsigned char _pad00[0x98];
    unsigned long NumBones;                        // +0x98
    unsigned char _pad9c[0xe0 - 0x9c];
    CMatrix4x4*   InverseBoneMatrices;             // +0xe0
};
struct C3DAnimationManager {
    void CalcMeshTransformations(C3DAnimationBlendState* blendState, const C3DVector* position, C3DMesh2* mesh, CMatrix4x4* matrices, unsigned long count, bool world);   // 0x00A9F540
};
C4DVector* __fastcall GetGroupVectorBuffer(unsigned long count);   // 0x00AE0540
CMatrix4x4* __fastcall GetGroupMatrixBuffer(unsigned long count);  // 0x00AE04E0
void __cdecl GFInterpolateVectors(C4DVector* out, unsigned long outStride, const C4DVector* a, unsigned long aStride, const C4DVector* b, unsigned long bStride, const float* t, unsigned long tStride, unsigned long count);   // 0x00A5BEF0
namespace NParticleEngine {
struct CParticleSimulation {
    unsigned char _pad00[0x1c];
    C4DVector*    Positions;                       // +0x1c
    unsigned char _pad20[0xc];
    float*        InverseMasses;                   // +0x2c
};
class CParticleProgram {
public:
    unsigned char _pad00[0x20];
    unsigned long NumVertices;                     // +0x20
    C4DVector*    Vertices;                        // +0x24
    unsigned char _pad28[0xfc - 0x28];
    CGroupArray   Groups;                          // +0xfc
    void TransformVertices(C4DVector* out, const C4DVector* in, unsigned long count, const CMatrix4x4* matrices, const CGroupArray* groups);   // 0x00AE0E60
    void ClingToMesh(CParticleSimulation* simulation, C3DAnimationBlendState* blendState, C3DAnimationManager* animation, const C3DVector* position, C3DMesh2* mesh);
};

void CParticleProgram::ClingToMesh(CParticleSimulation* simulation, C3DAnimationBlendState* blendState, C3DAnimationManager* animation, const C3DVector* position, C3DMesh2* mesh)
{
    unsigned long count = NumVertices;
    C4DVector* skinned = GetGroupVectorBuffer(count);
    unsigned long numBones = mesh->NumBones;
    CMatrix4x4* matrices = GetGroupMatrixBuffer(numBones);
    animation->CalcMeshTransformations(blendState, position, mesh, matrices, numBones, true);
    for (unsigned long i = 0; i < numBones; ++i) {
        CMatrix4x4 bone = matrices[i];
        const CMatrix4x4& inverse = mesh->InverseBoneMatrices[i];
        for (int r = 0; r < 4; ++r)
            for (int c = 0; c < 4; ++c)
                matrices[i].M[r][c] = bone.M[r][3] * inverse.M[3][c] + inverse.M[2][c] * bone.M[r][2] +
                                      bone.M[r][1] * inverse.M[1][c] + inverse.M[0][c] * bone.M[r][0];
    }
    TransformVertices(skinned, Vertices, count, matrices, &Groups);
    GFInterpolateVectors(simulation->Positions, 0x10, skinned, 0x10, simulation->Positions, 0x10, simulation->InverseMasses, 4, count);
}
}
