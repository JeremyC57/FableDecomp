// NParticleEngine::CParticleProgram::ClingToMesh(CParticleSimulation* simulation, C3DAnimationBlendState* blendState, C3DAnimationManager* animation, const C3DVector* position, C3DMesh2* mesh) -- pull the simulated particles back onto the animated mesh: compute the mesh's bone matrices for this frame, premultiply each by the mesh's inverse rest-pose bone matrix, skin the program's vertices with them into a scratch buffer and blend the simulation positions toward the skinned positions by each particle's inverse mass
// Grade: DIFFER(681v681) 49 diff lines (asm oracle; base 376v681, 171) -- the bone x inverse product as the 4x4 MultiplySSE __forceinline helper of 00989230 (pointers homed/reloaded), CMatrix4x4 __declspec(align(16)) (the and esp,-16 frame: 130->86), stepwise x87 sum (86->49). Residue: this/count in ebx/edi swapped at entry, one homing store order, and the x87 fallback's per-column term/operand order (VC canonicalises a single sum: 32 operand-order/parenthesisation variants identical; per-column stepwise 58)
// Retail-asm: 0x00AE13CA..0x00AE14B7 `mov eax,[esp+20h] ; mov edx,[esp+1Ch] ; mov edi,[esp+18h] ; movaps xmm0,[edx] ... shufps xmm7,xmm7,0FFh ... movaps [edi+30h],xmm4` -- the hand SSE 4x4 multiply (twin of 0x0098929D / 0x00988A50): pointers homed and reloaded, fixed xmm0..xmm7.
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
struct __declspec(align(16)) CMatrix4x4 { float M[4][4]; };
extern bool g_SSEAvailable;                        // 0x013D2880
static __forceinline void MultiplySSE(CMatrix4x4* out, const CMatrix4x4* left, const CMatrix4x4* right)
{
    __asm {
        mov     eax, left
        mov     edx, right
        mov     edi, out
        movaps  xmm0, [edx]
        movaps  xmm1, [edx+10h]
        movaps  xmm2, [edx+20h]
        movaps  xmm3, [edx+30h]
        movaps  xmm4, [eax]
        movaps  xmm5, xmm4
        movaps  xmm6, xmm4
        movaps  xmm7, xmm4
        shufps  xmm4, xmm4, 0
        shufps  xmm5, xmm5, 55h
        shufps  xmm6, xmm6, 0AAh
        shufps  xmm7, xmm7, 0FFh
        mulps   xmm4, xmm0
        mulps   xmm5, xmm1
        mulps   xmm6, xmm2
        mulps   xmm7, xmm3
        addps   xmm4, xmm5
        addps   xmm6, xmm7
        addps   xmm4, xmm6
        movaps  [edi], xmm4
        movaps  xmm4, [eax+10h]
        movaps  xmm5, xmm4
        movaps  xmm6, xmm4
        movaps  xmm7, xmm4
        shufps  xmm4, xmm4, 0
        shufps  xmm5, xmm5, 55h
        shufps  xmm6, xmm6, 0AAh
        shufps  xmm7, xmm7, 0FFh
        mulps   xmm4, xmm0
        mulps   xmm5, xmm1
        mulps   xmm6, xmm2
        mulps   xmm7, xmm3
        addps   xmm4, xmm5
        addps   xmm6, xmm7
        addps   xmm4, xmm6
        movaps  [edi+10h], xmm4
        movaps  xmm4, [eax+20h]
        movaps  xmm5, xmm4
        movaps  xmm6, xmm4
        movaps  xmm7, xmm4
        shufps  xmm4, xmm4, 0
        shufps  xmm5, xmm5, 55h
        shufps  xmm6, xmm6, 0AAh
        shufps  xmm7, xmm7, 0FFh
        mulps   xmm4, xmm0
        mulps   xmm5, xmm1
        mulps   xmm6, xmm2
        mulps   xmm7, xmm3
        addps   xmm4, xmm5
        addps   xmm6, xmm7
        addps   xmm4, xmm6
        movaps  [edi+20h], xmm4
        movaps  xmm4, [eax+30h]
        movaps  xmm5, xmm4
        movaps  xmm6, xmm4
        movaps  xmm7, xmm4
        shufps  xmm4, xmm4, 0
        shufps  xmm5, xmm5, 55h
        shufps  xmm6, xmm6, 0AAh
        shufps  xmm7, xmm7, 0FFh
        mulps   xmm4, xmm0
        mulps   xmm5, xmm1
        mulps   xmm6, xmm2
        mulps   xmm7, xmm3
        addps   xmm4, xmm5
        addps   xmm6, xmm7
        addps   xmm4, xmm6
        movaps  [edi+30h], xmm4
    }
}

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
        if (g_SSEAvailable) {
            MultiplySSE(&matrices[i], &bone, &inverse);
        } else {
            for (int r = 0; r < 4; ++r)
                for (int c = 0; c < 4; ++c)
                {
                    float v = bone.M[r][3] * inverse.M[3][c];
                    v += inverse.M[2][c] * bone.M[r][2];
                    v += bone.M[r][1] * inverse.M[1][c];
                    v += inverse.M[0][c] * bone.M[r][0];
                    matrices[i].M[r][c] = v;
                }
        }
    }
    TransformVertices(skinned, Vertices, count, matrices, &Groups);
    GFInterpolateVectors(simulation->Positions, 0x10, skinned, 0x10, simulation->Positions, 0x10, simulation->InverseMasses, 4, count);
}
}
