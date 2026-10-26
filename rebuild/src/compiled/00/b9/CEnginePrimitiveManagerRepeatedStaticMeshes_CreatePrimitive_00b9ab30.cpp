// CEnginePrimitiveManagerRepeatedStaticMeshes::CreatePrimitive(const CAddMeshDesc&) -- allocate a repeated static mesh primitive, adopt it into a temporary handle, fill it from the description, mark it non-persistent unless the description asks for persistence, add it to the scene and return a counted copy of the handle
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B9AB30, length 256 bytes (`ret 8`: hidden result + the description).
// Callees: operator new @ 0x00BFEA1A, CEngineInternalPrimitiveBase::CEngineInternalPrimitiveBase(0x2a, 8) @ 0x00B8FF00, the
// primitive-handle base CBaseClass ctor/dtor @ 0x0099A2D0 / 0x0099A2E0, UpdateRepeatedStaticMesh(const CEnginePrimitiveHandle&,
// const CAddMeshDesc&) @ 0x00B991D0, CEngineScene::AddPrimitive(CEngineInternalPrimitiveBase*, unsigned long) @ 0x00B94D30.
// Vtables: 0x012A3F4C CEngineInternalPrimitiveRepeatedStaticMesh, 0x0122F598 the primitive handle.
// Layout: the primitive is engine/CEngineInternalPrimitiveRepeatedStaticMesh.h (0xb0 bytes; BoundingSphere +0x70); the
// primitive/handle classes are the native models of the landed sibling CEnginePrimitiveManagerRepeatedZSprites::CreatePrimitive
// 0x00BABA70 (occlusion base words +0x48/+0x4c before the vtable, the bounding sphere's inline zeroing ctor after it; the
// NonPersistent flag is bit 1 of the +0x34 bitfield byte). The scene is reached as manager +4 -> [0] -> +4 (descriptive names).
// Shape: identical to the sibling apart from the bitfield update (`NonPersistent = !desc.Persistent`, the description kept
// in ebp) and the scene registration through the context chain.
#include "engine/CEnginePrimitiveManagerRepeatedStaticMeshes.h"
#include <stddef.h>
void* __cdecl operator new(size_t);   // 0x00BFEA1A

struct CEnginePrimitiveHandleNative;
struct CEngineInternalPrimitiveNative {
    virtual ~CEngineInternalPrimitiveNative();
    virtual void Destroy();
    virtual void ReservedSlot2();
    virtual void OnLastHandleReleased(CEnginePrimitiveHandleNative*);
    long IVRefCount; long Type; unsigned long EnginePrimitiveFlags; float AlphaMipBias; float FadeBeginDistance, MaxDrawDistance;
    unsigned long OcclusionLayerBitMask; void* GridCell; signed char GridLevel; unsigned char GridX, GridY, Alpha, AlphaRef, PreloadStat;
    unsigned short Reserved2a; unsigned long InterpolationSet, RenderLayerMask;
    unsigned char Enable : 1; unsigned char NonPersistent : 1; unsigned char OverrideFadeDistance : 1; unsigned char PostRenderEnable : 1;
    unsigned char _bits35; unsigned short LastFrameRendered;
    void* NextPrimitive; void* RefPrimitive; void* NextLayerMask; void* RefLayerMask;
    CEngineInternalPrimitiveNative(long, unsigned long);   // 0x00B8FF00
    long RemoveReference() { return --IVRefCount; }
};
struct __declspec(novtable) CEngineInternalPrimitiveOcclusionNative : CEngineInternalPrimitiveNative {
    void* OcclusionInfo[2];
    CEngineInternalPrimitiveOcclusionNative(long type, unsigned long flags) : CEngineInternalPrimitiveNative(type, flags)
    { OcclusionInfo[0] = 0; OcclusionInfo[1] = 0; }
};
class C3DBoundingSphere { public: float X, Y, Z, Radius; C3DBoundingSphere() : X(0), Y(0), Z(0), Radius(0) {} };
class C3DBoundingBox { public: float MinX, MinY, MinZ, MaxX, MaxY, MaxZ; };
struct CEngineInternalRepeatedStaticMeshNative : CEngineInternalPrimitiveOcclusionNative {   // vtable 0x012A3F4C, 0xb0 bytes
    void* MeshInfo; long ObjectCount; C3DBoundingBox BoundingBox; C3DBoundingSphere BoundingSphere;
    float MaxScale, AlphaTransformMult, AlphaTransformOffset; void* VectorNormals; void* ObjectMatricies; void* ObjectOffsets;
    void* SubSections; void* WindDelayArray; float WindScaleFactor; unsigned char RenderInfo[0xc];
    virtual ~CEngineInternalRepeatedStaticMeshNative();
    CEngineInternalRepeatedStaticMeshNative() : CEngineInternalPrimitiveOcclusionNative(0x2a, 8), BoundingSphere() {}
};
struct CPrimitiveHandleBaseNative {
    virtual ~CPrimitiveHandleBaseNative();      // 0x0099A2E0
    CPrimitiveHandleBaseNative();               // 0x0099A2D0
    CPrimitiveHandleBaseNative(const CPrimitiveHandleBaseNative&) {}
};
struct CPrimitiveCountedPointerNative {
    CEngineInternalPrimitiveNative* Object;
    CPrimitiveCountedPointerNative(CEngineInternalPrimitiveNative* p) : Object(p) {}
    CPrimitiveCountedPointerNative(const CPrimitiveCountedPointerNative& o) : Object(o.Object) { if (Object) ++Object->IVRefCount; }
    __forceinline ~CPrimitiveCountedPointerNative()
    { if (Object) { CEngineInternalPrimitiveNative* p = Object; if (!p->RemoveReference()) p->Destroy(); } Object = 0; }
};
struct CEnginePrimitiveHandleNative : CPrimitiveHandleBaseNative {   // vtable 0x0122F598
    CPrimitiveCountedPointerNative Data;
    CEnginePrimitiveHandleNative(CEngineInternalPrimitiveNative* p) : Data(p) {}
    CEnginePrimitiveHandleNative(const CEnginePrimitiveHandleNative& o) : CPrimitiveHandleBaseNative(o), Data(o.Data) {}
    __forceinline virtual ~CEnginePrimitiveHandleNative()
    { if (Data.Object && Data.Object->IVRefCount == 1) Data.Object->OnLastHandleReleased(this); }
};

struct CAddMeshDesc {
    unsigned char _pad[0x58];
    bool          Persistent;                       // +0x58
};

class CEngineScene { public:
    void AddPrimitive(CEngineInternalPrimitiveNative* primitive, unsigned long flags);   // 0x00B94D30
};
struct SSceneHolder {
    void*         _pad00;
    CEngineScene* Scene;                            // +0x04
};
struct SEngineContext {
    SSceneHolder* SceneHolder;                      // +0x00
};

struct CEnginePrimitiveManagerRepeatedStaticMeshesMethods : CEnginePrimitiveManagerRepeatedStaticMeshes {
    SEngineContext* Context() { return *reinterpret_cast<SEngineContext**>(_pad_0x04); }   // +0x04
    void UpdateRepeatedStaticMesh(const CEnginePrimitiveHandleNative&, const CAddMeshDesc&);   // 0x00B991D0
    CEnginePrimitiveHandleNative CreatePrimitive(const CAddMeshDesc& desc);
};

CEnginePrimitiveHandleNative CEnginePrimitiveManagerRepeatedStaticMeshesMethods::CreatePrimitive(const CAddMeshDesc& desc)
{
    CEngineInternalRepeatedStaticMeshNative* primitive = new CEngineInternalRepeatedStaticMeshNative;
    CEnginePrimitiveHandleNative handle(primitive);
    UpdateRepeatedStaticMesh(handle, desc);
    primitive->NonPersistent = !desc.Persistent;
    Context()->SceneHolder->Scene->AddPrimitive(primitive, 1);
    return handle;
}