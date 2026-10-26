// CEngineInternalPrimitiveStaticMesh::Update(const CEnginePrimitive& primitive, unsigned long flags) -- applies a static-mesh descriptor: looks the mesh up in the engine mesh bank (also used as the shadow mesh), scale = size / 100, alpha / tint / cloth flag / mesh effect / theme channel, rebuilds the object matrix and the fade distances, and marks the fade distance as overridden
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B9D8E0, length 160 bytes (`ret 8`). Constant 0.01f @ 0x01230A5C.
// Callees: the mesh bank lookup @ 0x00A24280 (thiscall on [[0x01436E98]+0x10]; the manifest's "CTCInventoryAbilities::GetAbilityLevel" is
// a propagated label), the old mesh effect's virtual slot 1 (Destroy at zero count), CalcObjectMatrix @ 0x00BEBAA0 (`__fastcall`: matrix
// in ecx, position in edx, right-handed set and scale on the stack), CEngineInternalPrimitiveBase::SetFadeDistanceWithGamePrimitiveDefaults
// @ 0x00B8FD40.
// Layout: descriptor = PDB CEnginePrimitiveStaticMesh (76 bytes); target = PDB CEngineInternalPrimitiveBase (EnginePrimitiveFlags +0x0C,
// Alpha +0x27, flag byte +0x34 with OverrideFadeDistance bit 2) + CEngineInternalPrimitiveMeshBase (MeshInfo +0x50, ShadowMeshInfo
// +0x54, Scale +0x58, TintColour +0x5C, BulletTimeAffectsCloth +0x60, MeshEffect +0x68, ThemeChannelDesc +0x6C) + ObjectTransform +0x70
// (engine/CEngineInternalPrimitiveStaticMesh.h keeps the members opaque, so modelled locally with the PDB names).
// Shape: the engine record is reached through an inline accessor (`mov ecx,[0x01436E98]` before the argument load); the chained
// store is written `ShadowMeshInfo = MeshInfo = ...` (MeshInfo stored first); the counted copy keeps the old pointer in a local with
// the release on the pointee; the scale is re-read from the member as the matrix argument.
class C3DVector { public: float X, Y, Z; };
class CRightHandedSet { public: C3DVector Forward; C3DVector Up; };
class CRGBColour { public: unsigned char B, G, R, A; };
class CMatrix3x4 { public: float m[12]; };
class C3DMeshInfo;

class CEngineMeshEffect {
public:
    virtual void v000();
    virtual void Destroy();                        // slot 1 (+0x04)
    long RefCount;                                 // +0x04
    void Release()
    {
        if (--RefCount == 0)
            Destroy();
    }
};

class CIVCountedPointer {                          // CIVCountedPointer<CEngineMeshEffect>
public:
    CEngineMeshEffect* Object;
    __forceinline CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        CEngineMeshEffect* old = Object;
        if (old != other.Object) {
            if (old) {
                old->Release();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->RefCount;
        }
        return *this;
    }
};

struct CMeshBank {
    C3DMeshInfo* GetMesh(unsigned long index);     // 0x00A24280 (manifest "CTCInventoryAbilities::GetAbilityLevel", propagated)
};

struct SEngineResources {                          // engine resource record reached through the pointer at 0x01436E98
    unsigned char _pad[0x10];
    CMeshBank*    MeshBank;                        // +0x10
    CMeshBank* GetMeshBank() const { return MeshBank; }
};
extern SEngineResources* g_pEngineResources;       // 0x01436E98

void __fastcall CalcObjectMatrix(CMatrix3x4* matrix, const C3DVector* position, const CRightHandedSet* set, float scale);   // 0x00BEBAA0

class CEnginePrimitive {                           // PDB, 12 bytes
public:
    long          Type;
    long          InterpolationSet;
    unsigned long ThemeChannelDesc;
};

class CEnginePrimitiveStaticMesh : public CEnginePrimitive {   // PDB, 76 bytes
public:
    unsigned long     MeshIndex;                   // +0x0c
    C3DVector         WorldPos;                    // +0x10
    float             Size;                        // +0x1c
    float             FadeStart;                   // +0x20
    CRightHandedSet   RightHandedSet;              // +0x24
    CIVCountedPointer MeshEffect;                  // +0x3c
    unsigned long     ThemeChannelDesc;            // +0x40 CEnginePrimitiveThemeChannelDesc
    CRGBColour        TintColour;                  // +0x44
    unsigned char     Alpha;                       // +0x48
    unsigned char     Lit : 1;                     // +0x49
    bool              BulletTimeAffectsCloth;      // +0x4a
};

class CEngineInternalPrimitiveStaticMesh {         // PDB: CEngineInternalPrimitiveBase (0x48) + MeshBase (to 0x70) + StaticMesh (0xac)
public:
    virtual void v000();
    unsigned char     _pad04[8];
    unsigned long     EnginePrimitiveFlags;        // +0x0c
    unsigned char     _pad10[0x17];
    unsigned char     Alpha;                       // +0x27
    unsigned char     _pad28[0xc];
    unsigned char     Enable : 1;                  // +0x34
    unsigned char     NonPersistent : 1;
    unsigned char     OverrideFadeDistance : 1;
    unsigned char     PostRenderEnable : 1;
    unsigned char     _pad35[0x1b];
    C3DMeshInfo*      MeshInfo;                    // +0x50
    C3DMeshInfo*      ShadowMeshInfo;              // +0x54
    float             Scale;                       // +0x58
    CRGBColour        TintColour;                  // +0x5c
    bool              BulletTimeAffectsCloth;      // +0x60
    CIVCountedPointer SimulationInfo;              // +0x64
    CIVCountedPointer MeshEffect;                  // +0x68
    unsigned long     ThemeChannelDesc;            // +0x6c
    CMatrix3x4        ObjectTransform;             // +0x70
    void SetFadeDistanceWithGamePrimitiveDefaults(float fadeStart);   // 0x00B8FD40
    virtual void Update(const CEnginePrimitive& primitive, unsigned long flags);
};

void CEngineInternalPrimitiveStaticMesh::Update(const CEnginePrimitive& primitive, unsigned long flags)
{
    const CEnginePrimitiveStaticMesh& mesh = static_cast<const CEnginePrimitiveStaticMesh&>(primitive);
    EnginePrimitiveFlags = flags;
    ShadowMeshInfo = MeshInfo = g_pEngineResources->GetMeshBank()->GetMesh(mesh.MeshIndex);
    Scale = mesh.Size * 0.01f;
    Alpha = mesh.Alpha;
    TintColour = mesh.TintColour;
    BulletTimeAffectsCloth = mesh.BulletTimeAffectsCloth;
    MeshEffect = mesh.MeshEffect;
    ThemeChannelDesc = mesh.ThemeChannelDesc;
    CalcObjectMatrix(&ObjectTransform, &mesh.WorldPos, &mesh.RightHandedSet, Scale);
    SetFadeDistanceWithGamePrimitiveDefaults(mesh.FadeStart);
    OverrideFadeDistance = 1;
}