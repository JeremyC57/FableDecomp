// CEngineClothSimulationAnimatedMesh::Render(const C3DPrimitive2& primitive, const CEngineSubPrimitiveAnimatedMesh& subPrimitive, const C3DMesh2& mesh, const C3DAnimationBlendState& blendState1, const C3DAnimationBlendState& blendState2, float blend, const CMatrix3x4& transform, ERenderPassType passType) -- render every cloth simulation of an animated mesh: fade it out by the distance of the transformed mesh bounding-sphere centre, interpolate the particle program's animated render positions (both blend states, the owner's animation manager, bone scalings and bone transformations) into the cloth renderer's position buffer, then hand the simulation and its cloth primitive to the cloth renderer
// Leaf: ?Render@CEngineClothSimulationAnimatedMesh@@UAEXABVC3DPrimitive2@@ABVCEngineSubPrimitiveAnimatedMesh@@ABVC3DMesh2@@ABVC3DAnimationBlendState@@3MABVCMatrix3x4@@W4ERenderPassType@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B814B0, length 397 bytes, `ret 20h`; byte-identical to the 2D sibling 0x00B81640
//   except the final cloth-renderer call (RenderClothPrimitive here, Render2DClothPrimitive there).
// Name: FableWin.pdb + Ego_r.pdb, the catalog spelling (virtual, public).
// Callees: CEngineClothSimulationMeshBase::CalculateFadeOut 0x00B806E0 (same TU, defined first: retail keeps edx live across the
//   call, which VC7.1 does only for a callee compiled earlier in the translation unit; the body is the landed one, out of line),
//   CEngineClothRenderer::GetPositionBuffer 0x00BB1E30, CEngineInternalPrimitiveAnimatedMesh::GetBoneTransformations 0x00BD2D90
//   (FableWin `?GetBoneTransformations@CEngineInternalPrimitiveAnimatedMesh@@QAEPBVCMatrix4x4@@XZ`), ::GetBoneScalings 0x00B83750
//   (FableWin `?GetBoneScalings@CEngineInternalPrimitiveAnimatedMesh@@QBEPBVC3DVector@@ABVC3DMesh2@@@Z`), the 11-argument
//   CParticleProgram::PrepareForRendering 0x00AE1260 (FableWin, simulation by reference), CEngineClothRenderer::RenderClothPrimitive
//   0x00BB3E80 (FableWin/Ego_r).
// Inlines (FableWin.pdb names): CMatrix3x4::operator*(const C3DVector&) const for the centre (the operator form is what gives
//   retail's x87 term order; the same sum written out in a C3DVector constructor schedules the X term first),
//   CParticleProgram::GetVertexPositionCount() = InitialSimulation.GetParticleCount() + NonSimCount (Size read first),
//   CEngineClothSimulationAnimatedMesh::GetOwner().
// Globals: 0x01436E80 the engine primitive renderer (BlockOnLoad +0x04, ClothRenderer +0x2F0; spelling g_pPrimitiveRenderer_01436e80
//   is descriptive, see NOTES); the camera 0x01436EA0 and simulation manager 0x01436E68 inside CalculateFadeOut.
// Layout: CEngineClothSimulationMeshBase (Ego_r.pdb names, landed 0x00B81180 offsets): ParticleSims +0x0C (CArray of counted
//   pointers), MeshInfo +0xA8, Owner +0xB0. The animation manager is the owner's C3DMeshInfo::GetAnimationManager() (FableWin.pdb
//   inline: MeshInfo +0x50 -> CMeshDataBankEntry::Bank +0x04 -> CMeshDataBank::AnimationManager +0x3C0, retail offsets; Ego_r.pdb
//   +0x448 less the 4-byte-shorter retail CArrays), parked in the dead transform argument slot before the PrepareForRendering pushes.
#include <math.h>
#define CEngineSimulationManager CEngineSimulationManagerLayout
#include "engine/CEngineSimulationManager.h"
#undef CEngineSimulationManager
#include "engine/CEngineCamera.h"
#define CParticleProgram CParticleProgramLayout
#include "engine/CParticleProgram.h"
#undef CParticleProgram

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetMagnitude() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
};
class C4DVector { public: float X, Y, Z, W; };
class CMatrix4x4;
class C3DMesh2;
class C3DAnimationBlendState;
class C3DAnimationManager;
class CMatrix3x4 { public:
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;
    C3DVector operator*(const C3DVector& v) const    // FableWin.pdb ??DCMatrix3x4@@QBE?AVC3DVector@@ABV1@@Z (inline here)
    {
        return C3DVector(v.X * E11 + v.Y * E21 + v.Z * E31 + E41,
                         v.X * E12 + v.Y * E22 + v.Z * E32 + E42,
                         v.X * E13 + v.Y * E23 + v.Z * E33 + E43);
    }
};
class CCPPointerInfo;
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    T* Get() const { return Data; }
    T* operator->() const { return Data; }
};
template <class T> class CArray { public:                 // retail CArray<T>: three pointers
    T* m_pData;
    T* m_pLast;
    T* m_pEnd;
    unsigned long size() const { return m_pLast - m_pData; }
    const T& operator[](unsigned long i) const { return m_pData[i]; }
};
class CParticleSimulation { public:                        // Ego_r.pdb, retail 0x80 bytes (landed 0x00AE1600 model)
    void*         __vftable;                               // +0x00
    unsigned char RenderData[8];                           // +0x04 CCountedPointer<CParticleSimulationRenderData>
    float         Timestep;                                // +0x0c
    bool          TimestepChanged;                         // +0x10
    float         TimestepMultiplier;                      // +0x14
    unsigned long Size;                                    // +0x18
    unsigned char _rest[0x80 - 0x1c];
    unsigned long GetParticleCount() const { return Size; }   // FableWin.pdb ?GetParticleCount@CParticleSimulation@@QBEKXZ
};
class CParticleProgram : public CParticleProgramLayout {
public:
    // FableWin.pdb ?GetVertexPositionCount@CParticleProgram@@QBEKXZ
    unsigned long GetVertexPositionCount() const { return reinterpret_cast<const CParticleSimulation*>(InitialSimulation)->GetParticleCount() + NonSimCount; }
    void PrepareForRendering(C4DVector* vertices, CParticleSimulation& simulation, float simulationInterpolate, float initialBlend,
        const C3DAnimationBlendState& blendState1, const C3DAnimationBlendState& blendState2, float blend, C3DAnimationManager& animationManager,
        const C3DVector* boneScalings, const CMatrix4x4* boneTransformations, const C3DMesh2& mesh);   // 0x00AE1260
};
class C3DClothPrimitive { public:
    void*                             __vftable;           // +0x00
    CCountedPointer<CParticleProgram> ParticleProgram;     // +0x04
    unsigned long                     PrimitiveIndex;      // +0x0c
    unsigned long                     MaterialIndex;       // +0x10
};
class C3DBoundingSphere { public:
    C3DVector Center;                                      // +0x00
    float     Radius;                                      // +0x0c
};
class C3DPrimitive2 { public:
    unsigned char _pad00[0x20];
    CArray<CCountedPointer<C3DClothPrimitive> > ClothPrimitives;   // +0x20
};
class CMeshDataBank { public:                              // retail offsets (STLport 12-byte CArrays)
    unsigned char _pad000[0x3c0];
    CCountedPointer<C3DAnimationManager> AnimationManager; // +0x3c0 (Ego_r.pdb +0x448)
};
class C3DMeshInfo { public:                                // CMeshDataBankEntry base: vptr, Bank +0x04, Index +0x08
    void*             __vftable;                           // +0x00
    CMeshDataBank*    Bank;                                // +0x04
    unsigned long     Index;                               // +0x08
    unsigned char     _pad0c[0x28 - 0x0c];                 // MeshData, BoundingBox
    C3DBoundingSphere BoundingSphere;                      // +0x28
    // FableWin.pdb ?GetAnimationManager@C3DMeshInfo@@QBEAAVC3DAnimationManager@@XZ
    C3DAnimationManager& GetAnimationManager() const { return *Bank->AnimationManager.Get(); }
};
class CEngineInternalPrimitiveBase { public:
    unsigned char _pad00[0x48];
};
class CEngineInternalPrimitiveMeshBase : public CEngineInternalPrimitiveBase { public:
    unsigned char _pad48[0x50 - 0x48];
    const C3DMeshInfo* MeshInfo;                           // +0x50
    unsigned char _pad54[0x70 - 0x54];
};
class CEngineInternalPrimitiveAnimatedMesh : public CEngineInternalPrimitiveMeshBase { public:
    const CMatrix4x4* GetBoneTransformations();                       // 0x00BD2D90
    const C3DVector* GetBoneScalings(const C3DMesh2& mesh) const;     // 0x00B83750
};
class CEngineSubPrimitiveBase { public:
    void*                         __vftable;               // +0x00
    unsigned long                 RenderFlags;             // +0x04
    unsigned char                 Alpha;                   // +0x08
    CEngineInternalPrimitiveBase* Parent;                  // +0x0c
    CEngineSubPrimitiveBase*      NextSubPrimitive;        // +0x10
    void*                         Manager;                 // +0x14
};
class CEngineSubPrimitiveMeshBase : public CEngineSubPrimitiveBase { public:
    C3DPrimitive2* MeshPrimitive;                          // +0x18
    long           RenderMode;                             // +0x1c
    bool           IsCloth;                                // +0x20
};
class CEngineSubPrimitiveAnimatedMesh : public CEngineSubPrimitiveMeshBase { };
enum ERenderPassType { };
class CEngineClothRenderer { public:
    C4DVector* GetPositionBuffer(unsigned long size);                                    // 0x00BB1E30
    void RenderClothPrimitive(const CParticleSimulation& simulation, const C3DClothPrimitive& clothPrimitive, const C3DPrimitive2& primitive,
        const CEngineSubPrimitiveMeshBase& subPrimitive, CEngineInternalPrimitiveMeshBase& internalPrimitive, const CMatrix3x4& transform,
        ERenderPassType passType, bool blockOnLoad);                                      // 0x00BB3E80
    void Render2DClothPrimitive(const CParticleSimulation& simulation, const C3DClothPrimitive& clothPrimitive, const C3DPrimitive2& primitive,
        const CEngineSubPrimitiveMeshBase& subPrimitive, CEngineInternalPrimitiveMeshBase& internalPrimitive, const CMatrix3x4& transform,
        ERenderPassType passType, bool blockOnLoad);                                      // 0x00BB30A0
};
class CEnginePrimitiveRenderer { public:
    void*         __vftable;                               // +0x00
    bool          BlockOnLoad;                             // +0x04 CEnginePrimitiveRendererSharedData
    unsigned char _pad05[0x2f0 - 5];
    CCountedPointer<CEngineClothRenderer> ClothRenderer;   // +0x2f0
};
extern CEnginePrimitiveRenderer* g_pPrimitiveRenderer_01436e80;   // 0x01436E80

class CEngineSimulationManager : public CEngineSimulationManagerLayout { };
extern CEngineSimulationManager* g_pEngineSimulationManager;       // 0x01436E68
extern CEngineCamera* g_pEngineCamera;                             // 0x01436EA0
inline const C3DVector& PeekCameraPos(const CEngineCamera* camera)
{
    return *(const C3DVector*)(camera->CameraDesc + 0x40);   // CameraDesc.WorldPosition
}

class CEngineSimulationBase { public:
    virtual ~CEngineSimulationBase();
    long          RunState;                                // +0x04
    unsigned char SimStepOffset;                           // +0x08
};
class CEngineClothSimulationMeshBase : public CEngineSimulationBase { public:
    CArray<CCountedPointer<CParticleSimulation> > ParticleSims;   // +0x0c
    CMatrix3x4    CurrentTransform;                        // +0x18
    CMatrix3x4    LastTransform;                           // +0x48
    CMatrix3x4    InverseTransform;                        // +0x78
    C3DMeshInfo*  MeshInfo;                                // +0xa8
    unsigned long PrimitiveIndex;                          // +0xac
    CEngineInternalPrimitiveMeshBase* Owner;               // +0xb0
protected:
    float CalculateFadeOut(const C3DVector& position) const;   // 0x00B806E0
};
class CEngineClothSimulationAnimatedMesh : public CEngineClothSimulationMeshBase { public:
    CEngineInternalPrimitiveAnimatedMesh* GetOwner() const { return static_cast<CEngineInternalPrimitiveAnimatedMesh*>(Owner); }
    virtual void Render(const C3DPrimitive2& primitive, const CEngineSubPrimitiveAnimatedMesh& subPrimitive, const C3DMesh2& mesh,
        const C3DAnimationBlendState& blendState1, const C3DAnimationBlendState& blendState2, float blend, const CMatrix3x4& transform, ERenderPassType passType);
};

// the landed 0x00B806E0 body (a real retail callee of this TU, out of line)
__declspec(noinline) float CEngineClothSimulationMeshBase::CalculateFadeOut(const C3DVector& position) const
{
    float fade = 0.0f;
    float distance = (position - PeekCameraPos(g_pEngineCamera)).GetMagnitude();
    const CEngineSimulationManager* manager = g_pEngineSimulationManager;
    float fadeBegin = manager->SimFadeBegin * manager->Zoom;
    fadeBegin *= manager->SimRadius;
    if (distance > fadeBegin) {
        float radius = manager->Zoom * manager->SimRadius;
        fade = (distance - fadeBegin) / (radius - fadeBegin);
        if (fade > 1.0f)
            fade = 1.0f;
    }
    return fade;
}

void CEngineClothSimulationAnimatedMesh::Render(const C3DPrimitive2& primitive, const CEngineSubPrimitiveAnimatedMesh& subPrimitive, const C3DMesh2& mesh,
    const C3DAnimationBlendState& blendState1, const C3DAnimationBlendState& blendState2, float blend, const CMatrix3x4& transform, ERenderPassType passType)
{
    for (unsigned long i = 0; i < ParticleSims.size(); ++i) {
        const C3DClothPrimitive* clothPrimitive = primitive.ClothPrimitives[i].Get();
        CParticleSimulation* simulation = ParticleSims[i].Get();
        C3DVector centre = MeshInfo->BoundingSphere.Center;
        float fade = CalculateFadeOut(transform * centre);
        CParticleProgram* program = clothPrimitive->ParticleProgram.Get();
        C4DVector* buffer = g_pPrimitiveRenderer_01436e80->ClothRenderer->GetPositionBuffer(program->GetVertexPositionCount());
        program->PrepareForRendering(buffer, *simulation, blend, fade, blendState1, blendState2, blend, GetOwner()->MeshInfo->GetAnimationManager(),
            GetOwner()->GetBoneScalings(mesh), GetOwner()->GetBoneTransformations(), mesh);
        g_pPrimitiveRenderer_01436e80->ClothRenderer->RenderClothPrimitive(*simulation, *clothPrimitive, primitive, subPrimitive, *Owner,
            transform, passType, g_pPrimitiveRenderer_01436e80->BlockOnLoad);
    }
}