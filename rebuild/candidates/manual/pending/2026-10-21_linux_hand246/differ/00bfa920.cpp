// Grade: DIFFER(334v346) -- register allocation: retail holds 0 in ebp and the shared 0xFF constant in ebx (colour bytes and the pushed alpha), flags in edi, and spills the mip bias through a frame word; ours holds 0 in ebx, flags in edx, pushes 0xFF as an immediate. Tried: body-assigned colour (fixed the byte order), == compare (fixed jp), named mip-bias local
// CLocalDetailPrimitiveMesh::AddObjectsToPrimitiveRenderer() -- when the collection type has a mesh, add it to the primitive renderer as a static mesh (type mesh + shadow mesh, this object's matrix and scale, the type's fade distance, opaque white colour, alpha ref / mip bias, persistent, flags 1 / 0x101 for boolean alpha / +0x1000 for shadow receivers) and keep the returned primitive handle
// Leaf: AddObjectsToPrimitiveRenderer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BFA920, length 346 bytes.
// Callees: CEnginePrimitiveRenderer::AddStaticMesh(const C3DMeshInfo*, const C3DMeshInfo*, const CMatrix3x4&, float,
//   const CEngineFadeDistance&, long, const CRGBColour&, long, float, bool, unsigned long) @ 0x00B32F20 (Ego_r.pdb
//   signature; CEnginePrimitiveHandle by hidden pointer) on the renderer global @ 0x01436E80, the persistent primitive's
//   virtual slots 1 (Delete) and 3 (NotifyReleased), CBaseClass::~CBaseClass @ 0x0099A2E0 (the result temporary,
//   vtable 0x0122F598). Float pool: 0.0f @ 0x0122DEDC.
// Layout: as the landed sibling CLocalDetailPrimitiveMeshZSpriteBatch::AddObjectsToPrimitiveRenderer 0x00BFACC0 (the
//   local-detail base, the collection type record, CEnginePrimitiveHandle {CBaseClass vptr, IV pointer}); Ego_r.pdb
//   CLocalDetailPrimitiveMesh: Matrix +0x38, Scale +0x68, PrimitiveHandle +0x6C.
// Shape: `PrimitiveHandle = renderer->AddStaticMesh(...)` through the handle's inline operator= and destructor (both from
//   the sibling model); the colour is a CRGBColour(255,255,255,255) temporary stored in R,G,B,A order over a B,G,R,A
//   byte layout; the shadow flag is the receiver byte converted to float and compared with 0.
struct C3DMeshInfo;
struct CMatrix3x4 { float M[12]; };
struct CEngineFadeDistance {
    float FadeStart, FadeEnd;
    CEngineFadeDistance(float s, float e) : FadeStart(s), FadeEnd(e) { }
};
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
struct CEnginePrimitiveHandle;
struct CBaseClass { CBaseClass(); virtual ~CBaseClass(); };   // 0x0099A2E0
struct CIVCountedPointeeBase {
    virtual ~CIVCountedPointeeBase();
    virtual void Delete() const;
    unsigned long IVRefCount;
    void DecRef() { --IVRefCount; }
    long GetRefCount() const { return IVRefCount; }
    __forceinline void RemoveReference() { DecRef(); if (GetRefCount() == 0) Delete(); }
};
struct CEnginePersistentPrimitiveBase : CIVCountedPointeeBase {
    long Type;
    virtual void CollectStats() const = 0;
    virtual void NotifyReleased(const CEnginePrimitiveHandle&);   // slot 3
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
struct CEnginePrimitiveHandle : CBaseClass {      // vtable 0x0122F598
    CIVPrimitivePointer Data;
    __forceinline void Clear() { if (Data.GetRefCount() == 1) Data->NotifyReleased(*this); Data.Clear(); Data.Object = 0; }
    __forceinline CEnginePrimitiveHandle& operator=(const CEnginePrimitiveHandle& r) { Clear(); Data = r.Data; return *this; }
    __forceinline virtual ~CEnginePrimitiveHandle() { Clear(); }
};
struct CEnginePrimitiveRenderer {
    CEnginePrimitiveHandle AddStaticMesh(const C3DMeshInfo* mesh, const C3DMeshInfo* shadowMesh, const CMatrix3x4& matrix, float scale,
                                         const CEngineFadeDistance& fade, long alpha, const CRGBColour& colour, long alphaRef,
                                         float alphaMipBias, bool persistent, unsigned long flags);   // 0x00B32F20
};
extern CEnginePrimitiveRenderer* g_PrimitiveRenderer;   // 0x01436E80
struct CollectionType {
    void* vtable;
    C3DMeshInfo *Mesh, *ZSpriteMesh, *ShadowMesh;              // +0x04 .. +0x0c
    long MeshPolyCount, MeshBankIndex, ZSpriteMeshBankIndex, ShadowMeshBankIndex;   // +0x10 .. +0x1c
    float FadeStart, FadeEnd;                                   // +0x20, +0x24
    long AlphaRef;                                              // +0x28
    float AlphaMipBias, WindSkewConstantFactor, WindSkewRandomFactor, WindSkewSpeedFactor;   // +0x2c .. +0x38
    float ZSpriteFadeStart, ZSpriteFadeEnd;                     // +0x3c, +0x40
    long PrimitiveType, CacheGroup;                             // +0x44, +0x48
    bool AlphaIsBoolean, LandscapeNormalLighting, HasWindSkew, CastsShadows, ReceivesShadows;   // +0x4c .. +0x50
};
struct LocalDetailMeshView {
    void*                  vtable;                 // +0x00
    void*                  Next;                   // +0x04
    float                  BoundingBox[6];         // +0x08
    float                  BoundingSphere[4];      // +0x20
    CollectionType*        Type;                   // +0x30
    void*                  Map;                    // +0x34
    CMatrix3x4             Matrix;                 // +0x38
    float                  Scale;                  // +0x68
    CEnginePrimitiveHandle PrimitiveHandle;        // +0x6c
    void AddObjectsToPrimitiveRenderer();
};

void LocalDetailMeshView::AddObjectsToPrimitiveRenderer()
{
    if (!Type->Mesh)
        return;
    CEngineFadeDistance fade(Type->FadeStart, Type->FadeEnd);
    unsigned long flags = 1;
    if (Type->AlphaIsBoolean)
        flags = 0x101;
    if ((float)Type->ReceivesShadows == 0.0f)
        flags |= 0x1000;
    PrimitiveHandle = g_PrimitiveRenderer->AddStaticMesh(Type->Mesh, Type->ShadowMesh, Matrix, Scale, fade, 0xff,
                                                         CRGBColour(0xff, 0xff, 0xff, 0xff), Type->AlphaRef, Type->AlphaMipBias,
                                                         true, flags);
}
