// Grade: DIFFER(333v333) -- flags store scheduled before the mesh stores (retail stores it after Alpha but loads it into ebx first; written in place VC keeps the constructor's zero in ebx for the whole function instead) and the local handle sits at frame +8 (retail +4, this spill at +0)
// CEnginePrimitiveManagerStaticMeshesBase::AddStaticMesh(const C3DMeshInfo* mesh, const C3DMeshInfo* lodMesh, const CMatrix3x4& transform, float, const CEngineFadeDistance& fade, long alpha, const CRGBColour& colour, long alphaRef, float alphaMipBias, bool persistent, unsigned long flags) -- create a static-mesh internal primitive, fill it from the arguments, register it with the manager and return a handle to it
// Leaf: AddStaticMesh
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BA0FC0, length 336 bytes (with int3 padding; body 0x14d), `ret 30h`.
// PDB: ?AddStaticMesh@CEnginePrimitiveManagerStaticMeshesBase@@QAE?AVCEnginePrimitiveHandle@@PBVC3DMeshInfo@@0ABVCMatrix3x4@@
//   MABVCEngineFadeDistance@@JABVCRGBColour@@JM_NK@Z.
// Callees: operator new @ 0x00BFEA1A (0xac bytes), CEngineInternalPrimitiveBase::CEngineInternalPrimitiveBase(long, ulong)
//   @ 0x00B8FF00, CBaseClass::CBaseClass @ 0x0099A2D0 / ~CBaseClass @ 0x0099A2E0 (the handle's root), the manager's primitive
//   registration @ 0x00BACDD0 (thiscall (primitive, flags & 1); manifest label "_Cons_val<...>" is propagated); the
//   primitive's virtual slots 1 (Destroy) and 3 (OnLastHandleReleased).
// Identity: vtable 0x012A425C = CEngineInternalPrimitiveStaticMesh (rtti-vtables.tsv); handle vtable 0x0122F598.
// Layout: CEngineInternalPrimitiveBase (PDB): RefCount +0x04, EnginePrimitiveFlags +0x0c, AlphaMipBias +0x10, FadeDistance
//   +0x14 (8 bytes), Alpha +0x27, AlphaRef +0x28, bit 1 of +0x34 (NonPersistent); the static mesh (0xac bytes): mesh
//   pointers +0x50/+0x54, the float +0x58, colour +0x5c, ObjectTransform +0x70 (PDB) and zero-initialised words
//   +0x48/+0x4c/+0x64/+0x68 and bytes +0x6c..+0x6e (names unknown). Modelled locally.
// Shape: the derived constructor is inline in the new-expression; the local handle is built from the raw pointer, copied
//   into the return slot (add-ref) and destroyed (unique-owner callback, long release, base dtor).
class CEnginePrimitiveHandle;

struct C3DMeshInfo;
struct CMatrix3x4 { float M[12]; };
struct CEngineFadeDistance { float Start, End; };
struct CRGBColour { unsigned char B, G, R, A; };

class CEngineInternalPrimitiveBase {
public:
    virtual void v000();
    virtual void Destroy();                                          // slot 1
    virtual void v002();
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle* handle); // slot 3
    long                RefCount;                   // +0x04
    unsigned long       _pad08;
    unsigned long       EnginePrimitiveFlags;       // +0x0c
    float               AlphaMipBias;               // +0x10
    CEngineFadeDistance FadeDistance;               // +0x14
    unsigned char       _pad1c[0x0b];
    unsigned char       Alpha;                      // +0x27
    unsigned char       AlphaRef;                   // +0x28
    unsigned char       _pad29[0x0b];
    unsigned char       Enable : 1;                 // +0x34
    unsigned char       NonPersistent : 1;
    unsigned char       _pad35[0x13];
    CEngineInternalPrimitiveBase(long type, unsigned long flags);   // 0x00B8FF00
    __forceinline void DecRef() { --RefCount; }
    __forceinline long GetRefCount() const { return RefCount; }
    __forceinline void Release()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};

class __declspec(novtable) CEngineInternalPrimitiveStaticMeshBase : public CEngineInternalPrimitiveBase {
public:
    long               Word48;                      // +0x48
    long               Word4c;                      // +0x4c
    const C3DMeshInfo* Mesh;                        // +0x50
    const C3DMeshInfo* LODMesh;                     // +0x54
    float              Value58;                     // +0x58
    CRGBColour         Colour;                      // +0x5c
    long               Word60;                      // +0x60
    long               Word64;                      // +0x64
    long               Word68;                      // +0x68
    bool               Flag6c;                      // +0x6c
    bool               Flag6d;                      // +0x6d
    bool               Flag6e;                      // +0x6e
    CMatrix3x4         ObjectTransform;             // +0x70
    unsigned char      RenderInfo[0xac - 0xa0];     // +0xa0
    CEngineInternalPrimitiveStaticMeshBase()
        : CEngineInternalPrimitiveBase(8, 10), Word48(0), Word4c(0), Mesh(0), Word64(0), Word68(0), Flag6c(false),
          Flag6d(false), Flag6e(false) { }
};

class CEngineInternalPrimitiveStaticMesh : public CEngineInternalPrimitiveStaticMeshBase {   // vtable 0x012A425C, 0xac bytes
public:
    CEngineInternalPrimitiveStaticMesh() { }
    virtual void v000();
};

class CBaseClass {
public:
    CBaseClass();                                   // 0x0099A2D0
    ~CBaseClass();                                  // 0x0099A2E0
};

class CEnginePrimitiveHandle : public CBaseClass {  // vtable 0x0122F598
public:
    CEngineInternalPrimitiveBase* Data;             // +0x04
    virtual void v000();
    CEnginePrimitiveHandle(CEngineInternalPrimitiveBase* primitive) : Data(primitive) { }
    __forceinline CEnginePrimitiveHandle(const CEnginePrimitiveHandle& other) : CBaseClass(other), Data(other.Data)
    {
        if (Data)
            ++Data->RefCount;
    }
    __forceinline ~CEnginePrimitiveHandle()
    {
        if (Data && Data->RefCount == 1)
            Data->OnLastHandleReleased(this);
        if (Data)
            Data->Release();
        Data = 0;
    }
};

class CEnginePrimitiveManagerStaticMeshesBase {
public:
    void RegisterPrimitive(CEngineInternalPrimitiveBase* primitive, unsigned long persistent);   // 0x00BACDD0
    CEnginePrimitiveHandle AddStaticMesh(const C3DMeshInfo* mesh, const C3DMeshInfo* lodMesh, const CMatrix3x4& transform,
                                         float value, const CEngineFadeDistance& fade, long alpha, const CRGBColour& colour,
                                         long alphaRef, float alphaMipBias, bool persistent, unsigned long flags);
};

CEnginePrimitiveHandle CEnginePrimitiveManagerStaticMeshesBase::AddStaticMesh(const C3DMeshInfo* mesh, const C3DMeshInfo* lodMesh,
    const CMatrix3x4& transform, float value, const CEngineFadeDistance& fade, long alpha, const CRGBColour& colour,
    long alphaRef, float alphaMipBias, bool persistent, unsigned long flags)
{
    CEngineInternalPrimitiveStaticMesh* primitive = new CEngineInternalPrimitiveStaticMesh;
    primitive->EnginePrimitiveFlags = flags;
    primitive->Mesh = mesh;
    primitive->LODMesh = lodMesh;
    primitive->ObjectTransform = transform;
    primitive->Value58 = value;
    primitive->FadeDistance = fade;
    primitive->Alpha = (unsigned char)alpha;
    primitive->Colour = colour;
    primitive->AlphaRef = (unsigned char)alphaRef;
    primitive->AlphaMipBias = alphaMipBias;
    primitive->NonPersistent = !persistent;
    CEnginePrimitiveHandle handle(primitive);
    RegisterPrimitive(primitive, flags & 1);
    return handle;
}
