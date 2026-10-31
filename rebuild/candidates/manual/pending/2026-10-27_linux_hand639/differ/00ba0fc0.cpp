// CEnginePrimitiveManagerStaticMeshesBase::AddStaticMesh(const C3DMeshInfo* mesh, const C3DMeshInfo* shadowMesh, const CMatrix3x4& transform, float scale, const CEngineFadeDistance& fade, long alpha, const CRGBColour& tint, long alphaRef, float alphaMipBias, bool persistent, unsigned long flags) -- create a static-mesh internal primitive, fill it from the arguments, register it with the manager and return its handle
// Grade: DIFFER(333v333) -- only the flags argument store `mov [ebp+0xc],ebx` is scheduled right after its load (retail stores it after the Alpha member); moving the flags statement after Alpha flips the zero register to ebx (326 bytes)
// Leaf: ?AddStaticMesh@CEnginePrimitiveManagerStaticMeshesBase@@QAE?AVCEnginePrimitiveHandle@@PBVC3DMeshInfo@@0ABVCMatrix3x4@@MABVCEngineFadeDistance@@JABVCRGBColour@@JM_NK@Z
// Tried (a-d): modern AddSprite 0x00BAE160 model, PDB hierarchy OcclusionBase->MeshBase->StaticMesh with ctor (8, 10), AddPrimitive(CEngineInternalPrimitiveBase&, unsigned long), handle.Data.Reset, SetNonPersistent setter, statement order.
class C3DMeshInfo;
class CMatrix3x4 { public: float M[12]; };
class CEngineFadeDistance { public: float FadeBeginDistance, MaxDrawDistance; };
class CRGBColour { public: unsigned char B, G, R, A; };
class CEnginePrimitiveHandle;
class CIVCountedPointeeBase { public:
    virtual ~CIVCountedPointeeBase();                  // slot 0
    virtual void Delete() const;                       // slot 1 (+0x04)
    mutable unsigned long IVRefCount;                  // +0x04
    void AddRef() const { ++IVRefCount; }
    long GetRefCount() const { return IVRefCount; }
    long Release() const { long count = --IVRefCount; if (count == 0) Delete(); return count; }
};
class CEnginePersistentPrimitiveBase : public CIVCountedPointeeBase { public:
    long Type;                                         // +0x08
    virtual void v002();                               // slot 2 (no recovered name)
    virtual void NotifyReleased(const CEnginePrimitiveHandle& handle);   // slot 3 (+0x0c)
    virtual bool WasRendered() const;                  // slot 4
};
template <class T> class CIVCountedPointer { public:
    T* Object;
    CIVCountedPointer() : Object(0) { }
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    ~CIVCountedPointer() { Release(); }
    T* operator->() const { return Object; }
    bool IsNull() const { return Object == 0; }
    unsigned long GetRefCount() const { return Object->GetRefCount(); }
    void Release()
    {
        if (Object)
            Object->Release();
        Object = 0;
    }
    void Reset(T* object) { Release(); Object = object; }
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
class CEnginePrimitiveHandle : public CBaseClass { public:   // vtable 0x0122F598
    CIVCountedPointer<CEnginePersistentPrimitiveBase> Data;  // +0x04
    CEnginePrimitiveHandle() { }
    CEnginePrimitiveHandle(const CEnginePrimitiveHandle& other) : CBaseClass(other), Data(other.Data) { }
    __forceinline ~CEnginePrimitiveHandle() { Clear(); }
    void Clear()
    {
        if (!Data.IsNull() && Data.GetRefCount() == 1)
            Data->NotifyReleased(*this);
        Data.Release();
    }
};
class CEngineInternalPrimitiveBase : public CEnginePersistentPrimitiveBase { public:
    unsigned long  EnginePrimitiveFlags;               // +0x0c
    float          AlphaMipBias;                       // +0x10
    CEngineFadeDistance FadeDistance;                  // +0x14
    unsigned long  OcclusionLayerBitMask;              // +0x1c
    void*          GridCell;                           // +0x20
    unsigned char  GridPos[3];                         // +0x24
    unsigned char  Alpha;                              // +0x27
    unsigned char  AlphaRef;                           // +0x28
    unsigned char  PreloadStat;                        // +0x29
    long           InterpolationSet;                   // +0x2c
    unsigned long  RenderLayerMask;                    // +0x30
    unsigned char  Enable : 1;                         // +0x34
    unsigned char  NonPersistent : 1;
    unsigned char  OverrideFadeDistance : 1;
    unsigned char  PostRenderEnable : 1;
    unsigned short LastFrameRendered;                  // +0x36
    void*          NextPrimitive;                      // +0x38
    void*          RefPrimitive;                       // +0x3c
    void*          NextLayerMask;                      // +0x40
    void*          RefLayerMask;                       // +0x44
    CEngineInternalPrimitiveBase(long type, unsigned long renderLayerMask);   // 0x00B8FF00
    void SetNonPersistent(bool nonPersistent) { NonPersistent = nonPersistent; }   // Ego_r.pdb ?SetNonPersistent@CEngineInternalPrimitiveBase@@QAEX_N@Z
};
class CEngineInternalPrimitiveOcclusionBase : public CEngineInternalPrimitiveBase { public:
    CIVCountedPointer<CEnginePersistentPrimitiveBase> OcclusionInfo[2];   // +0x48 CIVCountedPointer<CEnginePrimitiveOcclusionInfo>[2]
    CEngineInternalPrimitiveOcclusionBase(long type, unsigned long renderLayerMask) : CEngineInternalPrimitiveBase(type, renderLayerMask) { }
};
class CEngineInternalPrimitiveMeshBase : public CEngineInternalPrimitiveOcclusionBase { public:
    const C3DMeshInfo* MeshInfo;                       // +0x50
    const C3DMeshInfo* ShadowMeshInfo;                 // +0x54
    float              Scale;                          // +0x58
    CRGBColour         TintColour;                     // +0x5c
    bool               BulletTimeAffectsCloth;         // +0x60
    CIVCountedPointer<CEnginePersistentPrimitiveBase> SimulationInfo;   // +0x64 CIVCountedPointer<CEnginePrimitiveSimulationInfo>
    CIVCountedPointer<CEnginePersistentPrimitiveBase> MeshEffect;       // +0x68 CIVCountedPointer<CEngineMeshEffect>
    unsigned char      ThemeChannelDesc[3];            // +0x6c
    CEngineInternalPrimitiveMeshBase(long type, unsigned long renderLayerMask)
        : CEngineInternalPrimitiveOcclusionBase(type, renderLayerMask), MeshInfo(0)
    { ThemeChannelDesc[0] = 0; ThemeChannelDesc[1] = 0; ThemeChannelDesc[2] = 0; }
};
class CEngineInternalPrimitiveStaticMesh : public CEngineInternalPrimitiveMeshBase { public:   // vtable 0x012A425C, 0xac bytes
    CMatrix3x4    ObjectTransform;                     // +0x70
    unsigned char RenderInfo[0xac - 0xa0];             // +0xa0 CEngineRenderInfoStaticMesh
    CEngineInternalPrimitiveStaticMesh() : CEngineInternalPrimitiveMeshBase(8, 10) { }
    virtual ~CEngineInternalPrimitiveStaticMesh();
};
class CEnginePrimitiveManager { public:
    void AddPrimitive(CEngineInternalPrimitiveBase& primitive, unsigned long flags);   // 0x00BACDD0
};
class CEnginePrimitiveManagerStaticMeshesBase : public CEnginePrimitiveManager { public:
    CEnginePrimitiveHandle AddStaticMesh(const C3DMeshInfo* mesh, const C3DMeshInfo* shadowMesh, const CMatrix3x4& transform,
                                         float scale, const CEngineFadeDistance& fade, long alpha, const CRGBColour& tint,
                                         long alphaRef, float alphaMipBias, bool persistent, unsigned long flags);
};

CEnginePrimitiveHandle CEnginePrimitiveManagerStaticMeshesBase::AddStaticMesh(const C3DMeshInfo* mesh, const C3DMeshInfo* shadowMesh,
    const CMatrix3x4& transform, float scale, const CEngineFadeDistance& fade, long alpha, const CRGBColour& tint,
    long alphaRef, float alphaMipBias, bool persistent, unsigned long flags)
{
    CEngineInternalPrimitiveStaticMesh* primitive = new CEngineInternalPrimitiveStaticMesh;
    primitive->EnginePrimitiveFlags = flags;
    primitive->MeshInfo = mesh;
    primitive->ShadowMeshInfo = shadowMesh;
    primitive->ObjectTransform = transform;
    primitive->Scale = scale;
    primitive->FadeDistance = fade;
    primitive->Alpha = (unsigned char)alpha;
    primitive->TintColour = tint;
    primitive->AlphaRef = (unsigned char)alphaRef;
    primitive->AlphaMipBias = alphaMipBias;
    primitive->SetNonPersistent(!persistent);
    CEnginePrimitiveHandle handle;
    handle.Data.Reset(primitive);
    AddPrimitive(*primitive, flags & 1);
    return handle;
}
