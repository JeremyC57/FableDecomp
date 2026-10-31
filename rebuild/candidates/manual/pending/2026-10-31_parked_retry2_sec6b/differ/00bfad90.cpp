// CLocalDetailPrimitiveMeshZSpriteBatch::RenderUpdate(const CEngineCamera& camera, bool skipDistanceTest, unsigned long clipFlags, const CLocalDetailObjectCollectionType* type, float) -- per-frame update of a batch of local-detail meshes: when the batch is clipped away (ClipPrimitive false), drop every object's mesh handle that lies beyond the clamped fade-out distance (and remember whether any handle survives); otherwise, for each object, place the mesh's bounding-sphere centre in the world, fade its alpha over the clamped fade band, update the alpha of a live handle or (re)add the object as a static mesh, and drop the handle once the object is beyond the fade-out distance
// Grade: DIFFER(1140v1151) 317 diff lines (parked stub 18v1151, 383 lines; retail_asm candidate): first full reconstruction -- both loops, the clamped fade band, the inline-asm FloatToInt alpha (Retail-asm line), the UpdateStaticMeshAlpha / AddStaticMesh split and the inline handle Clear / operator= / dtor are in place and the block layout follows retail; left: register allocation (retail keeps this in ebx, the camera in ebp and the collection type in edi for the whole function, zero in edx, and spills the loop index/offset to the dead argument slots), the clear-path clamp reads both renderer limits from memory copies, and the world-centre arithmetic schedule (retail copies the scale and the mesh centre Z through the frame). Tried: const& Clamp (327), by-value mesh centre + scale local (this, 317 vs 319)
// Leaf: ?RenderUpdate@CLocalDetailPrimitiveMeshZSpriteBatch@@UAEXABVCEngineCamera@@_NKPBVCLocalDetailObjectCollectionType@@M@Z
// Retail-asm: 0x00BFAFE4..0x00BFAFF0 `fstp dword ptr [esp+24h] ; fld dword ptr [esp+24h] ; fistp dword ptr [esp+28h]` -- the engine's inline-assembly float-to-int helper (bare dword fistp into a named long) for the fade alpha; VC7.1 C++ `(long)x` calls __ftol2.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BFAD90, length 1151 bytes, `ret 14h`.
// Callees: CLocalDetailPrimitiveBase::ClipPrimitive 0x00BF9AA0 (Ego_r.pdb
//   ?ClipPrimitive@CLocalDetailPrimitiveBase@@QAE_NPBVCLocalDetailObjectCollectionType@@ABVCEngineCamera@@_NK@Z),
//   CEnginePrimitiveRenderer::AddStaticMesh 0x00B32F20, CEnginePrimitiveRenderer::UpdateStaticMeshAlpha 0x00B31FF0 (Ego_r.pdb
//   order: UpdateRepeatedStaticMesh 0x00B31FE0 / UpdateStaticMeshAlpha / UpdateRepeatedZSprite; the body forwards to the
//   static-mesh manager at renderer +0x280), CBaseClass::~CBaseClass 0x0099A2E0; the persistent primitive's slots 1 / 3.
// Globals: primitive renderer 0x01436E80 (local-detail fade clamp floats +0x90 / +0x94, re-add flag byte +0x98 -- descriptive),
//   local-detail generator 0x01436E88 (alpha bias +0x28 -- descriptive). Floats: 255.0f 0x01230014, 0.0f 0x0122DEDC.
// Layout: CLocalDetailPrimitiveMeshZSpriteBatch / CObject (0x4c) as the landed sibling AddObjectsToPrimitiveRenderer
//   0x00BFACC0 (Ego_r.pdb); the collection type record as the sibling CLocalDetailPrimitiveMesh::AddObjectsToPrimitiveRenderer
//   0x00BFA920 model (fade +0x20/+0x24, alpha ref +0x28, mip bias +0x2c, z-sprite fade +0x3c/+0x40, alpha-is-boolean +0x4c,
//   receives-shadows +0x50); C3DMeshInfo bounding-sphere centre +0x28; camera position +0x54.
#include <math.h>
#define CEnginePrimitiveRenderer CEnginePrimitiveRendererLayout
#include "engine/CEnginePrimitiveRenderer.h"
#undef CEnginePrimitiveRenderer
#include "engine/CEngineCamera.h"
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class C3DMeshInfo;
class C3DVector { public:
    float X, Y, Z;
    __forceinline C3DVector() { }
    __forceinline C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    __forceinline C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    __forceinline C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    __forceinline C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    __forceinline C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    __forceinline float Length() const { return (float)sqrt(Z * Z + Y * Y + X * X); }
};
class CMatrix3x4 { public: float M[12]; };
class CEngineFadeDistance { public:
    float FadeStart, FadeEnd;
    CEngineFadeDistance(float s, float e) : FadeStart(s), FadeEnd(e) { }
};
class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
class CEnginePrimitiveHandle;
class CIVCountedPointeeBase { public:
    virtual ~CIVCountedPointeeBase();
    virtual void Delete() const;                        // 0x0042D930
    unsigned long IVRefCount;
    void DecRef() { --IVRefCount; }
    long GetRefCount() const { return IVRefCount; }
    __forceinline void RemoveReference() { DecRef(); if (GetRefCount() == 0) Delete(); }
};
class CEnginePersistentPrimitiveBase : public CIVCountedPointeeBase { public:
    long Type;
    virtual void CollectStats() const = 0;
    virtual void NotifyReleased(const CEnginePrimitiveHandle&);   // slot 3 0x00B8FFC0
};
struct CIVPrimitivePointer {
    CEnginePersistentPrimitiveBase* Object;
    CIVPrimitivePointer() : Object(0) { }
    CIVPrimitivePointer(const CIVPrimitivePointer& r) : Object(r.Object) { if (Object) ++Object->IVRefCount; }
    unsigned long GetRefCount() const { return Object ? Object->GetRefCount() : 0; }
    CEnginePersistentPrimitiveBase* operator->() const { return Object; }
    __forceinline void Clear() { if (Object) { Object->RemoveReference(); Object = 0; } }
    __forceinline ~CIVPrimitivePointer() { Clear(); }
    __forceinline CIVPrimitivePointer& operator=(const CIVPrimitivePointer& r) { if (Object != r.Object) { Clear(); Object = r.Object; if (Object) ++Object->IVRefCount; } return *this; }
};
class CEnginePrimitiveHandle : public CBaseClass { public:   // vtable 0x0122F598
    CIVPrimitivePointer Data;
    __forceinline void Clear() { if (Data.GetRefCount() == 1) Data->NotifyReleased(*this); Data.Clear(); Data.Object = 0; }
    __forceinline CEnginePrimitiveHandle& operator=(const CEnginePrimitiveHandle& r) { Clear(); Data = r.Data; return *this; }
    __forceinline virtual ~CEnginePrimitiveHandle() { Clear(); }
};
class CEnginePrimitiveRenderer : public CEnginePrimitiveRendererLayout { public:
    CEnginePrimitiveHandle AddStaticMesh(const C3DMeshInfo* mesh, const C3DMeshInfo* shadowMesh, const CMatrix3x4& matrix, float scale,
                                         const CEngineFadeDistance& fade, long alpha, const CRGBColour& colour, long alphaRef,
                                         float alphaMipBias, bool persistent, unsigned long flags);   // 0x00B32F20
    void UpdateStaticMeshAlpha(const CEnginePrimitiveHandle& handle, long alpha);                   // 0x00B31FF0
    float LocalDetailFadeMin() const { return *(const float*)_pad_0x90; }      // +0x90 (descriptive)
    float LocalDetailFadeMax() const { return *(const float*)_pad_0x94; }      // +0x94 (descriptive)
    unsigned char ReAddStaticMeshes() const { return _pad_0x98[0]; }           // +0x98 (descriptive)
};
extern CEnginePrimitiveRenderer* g_PrimitiveRenderer;   // 0x01436E80
class CEngineLocalDetailGenerator { public:
    unsigned char _pad00[0x28];
    long          AlphaBias;                            // +0x28 (descriptive)
};
extern CEngineLocalDetailGenerator* g_pLocalDetailGenerator;   // 0x01436E88
class CLocalDetailObjectCollectionType { public:
    void*        _vptr;
    C3DMeshInfo* Mesh;                                  // +0x04
    C3DMeshInfo* ZSpriteMesh;                           // +0x08
    C3DMeshInfo* ShadowMesh;                            // +0x0c
    long         MeshPolyCount, MeshBankIndex, ZSpriteMeshBankIndex, ShadowMeshBankIndex;   // +0x10 .. +0x1c
    float        FadeStart, FadeEnd;                    // +0x20, +0x24
    long         AlphaRef;                              // +0x28
    float        AlphaMipBias, WindSkewConstantFactor, WindSkewRandomFactor, WindSkewSpeedFactor;   // +0x2c .. +0x38
    float        ZSpriteFadeStart, ZSpriteFadeEnd;      // +0x3c, +0x40
    long         PrimitiveType, CacheGroup;             // +0x44, +0x48
    bool         AlphaIsBoolean, LandscapeNormalLighting, HasWindSkew, CastsShadows, ReceivesShadows;   // +0x4c .. +0x50
};
class CMovableResource { public:                        // vtable slots 0-3 (the landed sibling's model)
    virtual ~CMovableResource() { }
    virtual void RelocateData(long, void*, void*, long) = 0;
    virtual bool CanRelocateData(long) const;
    virtual unsigned long Verify() const;
};
class CLocalDetailPrimitiveBase : public CMovableResource { public:
    virtual void RenderUpdate(const CEngineCamera&, bool, unsigned long, const CLocalDetailObjectCollectionType*, float);
    CLocalDetailPrimitiveBase* Next;
    float BoundingBox[6]; float BoundingSphere[4];
    void* Type; void* Map;
    bool ClipPrimitive(const CLocalDetailObjectCollectionType* type, const CEngineCamera& camera, bool skipDistanceTest,
                       unsigned long clipFlags);        // 0x00BF9AA0
};
class CLocalDetailPrimitiveMeshZSpriteBatch : public CLocalDetailPrimitiveBase { public:
    class CObject { public:
        CMatrix3x4 Matrix;                              // +0x00 (position row +0x24)
        float      Scale;                               // +0x30
        float      BoundingSphere[4];                   // +0x34
        CEnginePrimitiveHandle PrimitiveHandle;         // +0x44
    };
    CObject* Objects;                                   // +0x38
    void*    ZSpriteEntries;                            // +0x3c
    long     ObjectCount;                               // +0x40
    bool     HasMeshPrimitiveHandles;                   // +0x44
    CEnginePrimitiveHandle ZSpriteHandle;               // +0x48
    virtual void RenderUpdate(const CEngineCamera& camera, bool skipDistanceTest, unsigned long clipFlags,
                              const CLocalDetailObjectCollectionType* type, float);   // slot 4
};
template <class T> __forceinline T Clamp(T value, T low, T high)
{
    if (value < low)
        return low;
    if (value > high)
        return high;
    return value;
}
__forceinline long FloatToInt(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}
__forceinline const C3DVector& CameraPosition(const CEngineCamera& camera) { return *(const C3DVector*)(camera.CameraDesc + 0x40); }

void CLocalDetailPrimitiveMeshZSpriteBatch::RenderUpdate(const CEngineCamera& camera, bool skipDistanceTest, unsigned long clipFlags,
                                                         const CLocalDetailObjectCollectionType* type, float)
{
    if (!ClipPrimitive(type, camera, skipDistanceTest, clipFlags)) {
        if (HasMeshPrimitiveHandles) {
            HasMeshPrimitiveHandles = false;
            for (long i = 0; i < ObjectCount; ++i) {
                CObject& object = Objects[i];
                float distance = (CameraPosition(camera) - *(const C3DVector*)object.BoundingSphere).Length();
                if (distance > Clamp(type->ZSpriteFadeEnd, g_PrimitiveRenderer->LocalDetailFadeMin(), g_PrimitiveRenderer->LocalDetailFadeMax()))
                    object.PrimitiveHandle.Clear();
                else if (object.PrimitiveHandle.Data.Object)
                    HasMeshPrimitiveHandles = true;
            }
        }
        return;
    }
    for (long i = 0; i < ObjectCount; ++i) {
        CObject& object = Objects[i];
        C3DVector meshCentre = *(const C3DVector*)((const char*)type->Mesh + 0x28);
        float scale = object.Scale;
        C3DVector position = *(const C3DVector*)&object.Matrix.M[9];
        C3DVector centre = meshCentre * scale + position;
        float distance = (CameraPosition(camera) - centre).Length();
        float fadeStart = type->ZSpriteFadeStart;
        float fadeEnd = type->ZSpriteFadeEnd;
        float clampedEnd = Clamp(fadeEnd, g_PrimitiveRenderer->LocalDetailFadeMin(), g_PrimitiveRenderer->LocalDetailFadeMax());
        fadeStart -= fadeEnd - clampedEnd;
        long alpha = g_pLocalDetailGenerator->AlphaBias + FloatToInt((distance - clampedEnd) * 255.0f / (fadeStart - clampedEnd));
        if (alpha < 0)
            alpha = 0;
        else if (alpha > 255)
            alpha = 255;
        if (distance < clampedEnd) {
            if (object.PrimitiveHandle.Data.Object && (float)g_PrimitiveRenderer->ReAddStaticMeshes() == 0.0f)
                g_PrimitiveRenderer->UpdateStaticMeshAlpha(object.PrimitiveHandle, alpha);
            else {
                CEngineFadeDistance fade(type->FadeStart, type->FadeEnd);
                unsigned long flags = 0x211;
                if (type->AlphaIsBoolean)
                    flags = 0x311;
                if ((float)type->ReceivesShadows == 0.0f)
                    flags |= 0x1000;
                object.PrimitiveHandle = g_PrimitiveRenderer->AddStaticMesh(type->Mesh, type->ShadowMesh, object.Matrix, object.Scale, fade,
                                                                            alpha, CRGBColour(0xff, 0xff, 0xff, 0xff), type->AlphaRef,
                                                                            type->AlphaMipBias, true, flags);
                HasMeshPrimitiveHandles = true;
            }
        } else
            object.PrimitiveHandle.Clear();
    }
}
