// CEngineClothSimulationStaticMesh::Render(const C3DPrimitive2& primitive, const CEngineSubPrimitiveStaticMesh& subPrimitive, ERenderPassType passType) -- render every cloth simulation of a static mesh: fade it out by the distance of the owner-transformed mesh bounding-sphere centre, interpolate the particle program's render positions into the cloth renderer's position buffer, then hand the simulation and its cloth primitive to the cloth renderer
// Leaf: ?Render@CEngineClothSimulationStaticMesh@@QAEXABVC3DPrimitive2@@ABVCEngineSubPrimitiveStaticMesh@@W4ERenderPassType@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B812F0, length 343 bytes, `ret 0Ch`.
// Name: Ego_r.pdb `?Render@CEngineClothSimulationStaticMesh@@QAEXABVC3DPrimitive2@@ABVCEngineSubPrimitiveStaticMesh@@W4ERenderPassType@@@Z`
//   (public, non-virtual; the two animated-mesh siblings 0x00B814B0 / 0x00B81640 are the virtual overloads).
// Callees: CEngineClothSimulationMeshBase::CalculateFadeOut 0x00B806E0 (same TU, defined first: retail keeps edx live across
//   the call, which VC7.1 does only for a callee compiled earlier in the translation unit -- the body below is the landed
//   0x00B806E0 one, kept out of line), CEngineClothRenderer::GetPositionBuffer 0x00BB1E30, CParticleProgram::PrepareForRendering
//   0x00AE1600, CEngineClothRenderer::RenderClothPrimitive 0x00BB3E80 (Ego_r.pdb
//   `?RenderClothPrimitive@CEngineClothRenderer@@QAEXABVCParticleSimulation@@ABVC3DClothPrimitive@@ABVC3DPrimitive2@@ABVCEngineSubPrimitiveMeshBase@@AAVCEngineInternalPrimitiveMeshBase@@ABVCMatrix3x4@@W4ERenderPassType@@_N@Z`).
// Globals: 0x01436E80 the engine primitive renderer (CEnginePrimitiveRenderer: BlockOnLoad +0x04 from
//   CEnginePrimitiveRendererSharedData, ClothRenderer +0x2F0 = PDB +0x2F4 less the 8-byte shorter retail CArray/map members);
//   spelling g_pPrimitiveRenderer_01436e80 is descriptive (the bound CEnginePrimitiveRenderer* spelling; see NOTES).
//   g_EngineInterpolationInfo 0x0143B8E0 (CEngineInterpolationInfo, the landed FrameStart 0x00B32660 spelling):
//   InterpolationSetList[set].WFInterpolate is the fixup 0x0143B934 (+0x4C list, +8 WFInterpolate, 12-byte sets).
// Layout: CEngineClothSimulationMeshBase (landed 0x00B81180 model): ParticleSims +0x0C (CArray of 8-byte counted
//   pointers), MeshInfo +0xA8, Owner +0xB0; C3DPrimitive2::ClothPrimitives +0x20 (CArray<CCountedPointer<C3DClothPrimitive> >,
//   PDB); C3DClothPrimitive::ParticleProgram +0x04 (PDB); CEngineInternalPrimitiveBase::InterpolationSet +0x2C (PDB);
//   CEngineInternalPrimitiveStaticMesh::ObjectTransform +0x70; CEngineSubPrimitiveBase::Parent +0x0C (PDB).
// Shape: the cloth primitive is fetched before the simulation; the owner pointer is one local; the centre is transformed in
//   one C3DVector constructor call with each component written X-term, Y-term, Z-term, translation (VC7.1 then evaluates the
//   Z term first as retail does); the vertex count is the FableWin inline CParticleProgram::GetVertexPositionCount().
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
class CMatrix3x4 { public:
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;
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
    const CParticleSimulation& GetInitialSimulation() const { return *reinterpret_cast<const CParticleSimulation*>(InitialSimulation); }
    // FableWin.pdb ?GetVertexPositionCount@CParticleProgram@@QBEKXZ (inline): InitialSimulation.GetParticleCount() + NonSimCount,
    //   which reads Size before NonSimCount as retail does (2026-10-28 retry: the fix, RELOCATION_MATCH)
    unsigned long GetVertexPositionCount() const { return GetInitialSimulation().GetParticleCount() + NonSimCount; }
    void PrepareForRendering(C4DVector* vertices, CParticleSimulation& simulation, float simulationInterpolate, float initialBlend);   // 0x00AE1600 (FableWin: by reference)
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
class C3DMeshInfo { public:
    unsigned char     _pad00[0x28];                        // vptr, MeshData, BoundingBox
    C3DBoundingSphere BoundingSphere;                      // +0x28
};
class CEngineInternalPrimitiveBase { public:
    unsigned char _pad00[0x2c];
    long          InterpolationSet;                        // +0x2c EEngineInterpolationSet
    unsigned char _pad30[0x48 - 0x30];
};
class CEngineInternalPrimitiveMeshBase : public CEngineInternalPrimitiveBase { public:
    unsigned char _pad48[0x70 - 0x48];
};
class CEngineInternalPrimitiveStaticMesh : public CEngineInternalPrimitiveMeshBase { public:
    CMatrix3x4    ObjectTransform;                         // +0x70
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
class CEngineSubPrimitiveStaticMesh : public CEngineSubPrimitiveMeshBase { public:
    CEngineInternalPrimitiveStaticMesh* GetOwner() const { return static_cast<CEngineInternalPrimitiveStaticMesh*>(Parent); }
};
enum ERenderPassType { };
class CEngineClothRenderer { public:
    C4DVector* GetPositionBuffer(unsigned long size);                                    // 0x00BB1E30
    void RenderClothPrimitive(const CParticleSimulation& simulation, const C3DClothPrimitive& clothPrimitive, const C3DPrimitive2& primitive,
        const CEngineSubPrimitiveMeshBase& subPrimitive, CEngineInternalPrimitiveMeshBase& internalPrimitive, const CMatrix3x4& transform,
        ERenderPassType passType, bool blockOnLoad);                                      // 0x00BB3E80
};
class CEnginePrimitiveRenderer { public:
    void*         __vftable;                               // +0x00
    bool          BlockOnLoad;                             // +0x04 CEnginePrimitiveRendererSharedData
    unsigned char _pad05[0x2f0 - 5];
    CCountedPointer<CEngineClothRenderer> ClothRenderer;   // +0x2f0
};
extern CEnginePrimitiveRenderer* g_pPrimitiveRenderer_01436e80;   // 0x01436E80
class CEngineInterpolationInfoSet { public:
    float GTPredictedRenderAt;
    float GTPredictedTimeSinceLastRenderFrame;
    float WFInterpolate;
};
class CEngineInterpolationInfo { public:
    CEngineInterpolationInfoSet Current, Last, PausedCurrent, PausedLast, BulletTimeCurrent, BulletTimeLast;
    long                        WFServerCurrentTime;       // +0x48
    CEngineInterpolationInfoSet InterpolationSetList[3];   // +0x4c
};
extern CEngineInterpolationInfo g_EngineInterpolationInfo;         // 0x0143B8E0

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
    CArray<CCountedPointer<CParticleSimulation> > ParticleSims;   // +0x0c (Ego_r.pdb CArray)
    CMatrix3x4    CurrentTransform;                        // +0x18
    CMatrix3x4    LastTransform;                           // +0x48
    CMatrix3x4    InverseTransform;                        // +0x78
    C3DMeshInfo*  MeshInfo;                                // +0xa8
    unsigned long PrimitiveIndex;                          // +0xac
protected:
    float CalculateFadeOut(const C3DVector& position) const;   // 0x00B806E0
};
class CEngineClothSimulationStaticMesh : public CEngineClothSimulationMeshBase { public:
    CEngineInternalPrimitiveStaticMesh* Owner;             // +0xb0
    void Render(const C3DPrimitive2& primitive, const CEngineSubPrimitiveStaticMesh& subPrimitive, ERenderPassType passType);
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

void CEngineClothSimulationStaticMesh::Render(const C3DPrimitive2& primitive, const CEngineSubPrimitiveStaticMesh& subPrimitive, ERenderPassType passType)
{
    for (unsigned long i = 0; i < ParticleSims.size(); ++i) {
        const C3DClothPrimitive* clothPrimitive = primitive.ClothPrimitives[i].Get();
        CParticleSimulation* simulation = ParticleSims[i].Get();
        CEngineInternalPrimitiveStaticMesh* owner = Owner;
        C3DVector centre = MeshInfo->BoundingSphere.Center;
        float fade = CalculateFadeOut(C3DVector(
            centre.X * owner->ObjectTransform.E11 + centre.Y * owner->ObjectTransform.E21 + centre.Z * owner->ObjectTransform.E31 + owner->ObjectTransform.E41,
            centre.X * owner->ObjectTransform.E12 + centre.Y * owner->ObjectTransform.E22 + centre.Z * owner->ObjectTransform.E32 + owner->ObjectTransform.E42,
            centre.X * owner->ObjectTransform.E13 + centre.Y * owner->ObjectTransform.E23 + centre.Z * owner->ObjectTransform.E33 + owner->ObjectTransform.E43));
        CParticleProgram* program = clothPrimitive->ParticleProgram.Get();
        C4DVector* buffer = g_pPrimitiveRenderer_01436e80->ClothRenderer->GetPositionBuffer(program->GetVertexPositionCount());
        program->PrepareForRendering(buffer, *simulation, g_EngineInterpolationInfo.InterpolationSetList[owner->InterpolationSet].WFInterpolate, fade);
        g_pPrimitiveRenderer_01436e80->ClothRenderer->RenderClothPrimitive(*simulation, *clothPrimitive, primitive, subPrimitive, *owner,
            subPrimitive.GetOwner()->ObjectTransform, passType, g_pPrimitiveRenderer_01436e80->BlockOnLoad);
    }
}