// CEnginePrimitiveManagerStaticMeshesBase::CreateInternalPrimitive(long type) -- allocate and construct the internal primitive for a static-mesh type: 8 = static mesh, 10 = morphed static mesh, 12 = static mesh from file; null for any other type or when the allocation fails
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BA7820, length 262 bytes (`ret 4`, `this` unused).
// Callees: operator new @ 0x00BFEA1A, CEngineInternalPrimitiveBase::CEngineInternalPrimitiveBase(long type, unsigned long flags)
// @ 0x00B8FF00 ((8, 0xa), (0xa, 8), (0xc, 8)), CEngineInternalPrimitiveMorphInfo::CEngineInternalPrimitiveMorphInfo @ 0x00BAA250
// (manifest label "CTCOpinionSource::CTCOpinionSource" is propagated; it is the morphed mesh's Morph member at +0xac).
// Vtables (rtti-vtables.tsv): 0x012A425C CEngineInternalPrimitiveStaticMesh, 0x012A51B4 CEngineInternalPrimitiveMorphedStaticMesh,
// 0x012A4374 CEngineInternalPrimitiveStaticMeshFromFile.
// Layout/shape: the zero stores belong to the intermediate `__declspec(novtable)` static-mesh base: three init-list words
// (+0x48/+0x4c/+0x50) and, as MEMBER OBJECTS with inline constructors (PDB CEngineInternalPrimitiveMeshBase), the two
// CIVCountedPointers SimulationInfo/MeshEffect (+0x64/+0x68) and the three-byte ThemeChannelDesc (+0x6c). Written as plain
// init-list scalars (the landed 0x00B9E8C0 model) VC7.1 hoists the vtable stores of the member-less classes above the zero
// run and cross-jumps the three bodies into one tail; the member objects keep retail's order (zeros, `lea` of Morph, vtable,
// Morph constructor). Retail sizes: 0xac, 0xe4 (PDB 0xf4: retail containers are narrower), 0xac. The switch lowers to a
// `sub eax,8 / sub eax,2 / sub eax,2` chain with the case-12 body first.
void* __cdecl operator new(unsigned int size);     // 0x00BFEA1A

struct CEngineInternalPrimitiveBase {              // engine/CEngineInternalPrimitiveBase.h (PDB, 0x48 bytes): the base constructor takes (type, flags)
    virtual void v00();
    unsigned char _pad04[0x44];                    // +0x04 .. +0x48
    CEngineInternalPrimitiveBase(long type, unsigned long flags);   // 0x00B8FF00
};

template <class T> struct CIVCountedPointer {     // intrusive counted pointer (one word)
    T* m_p;
    __forceinline CIVCountedPointer() : m_p(0) { }
};
struct CEnginePrimitiveSimulationInfo;
struct CEngineMeshEffect;

struct CEnginePrimitiveThemeChannelDesc {          // three flag bytes
    bool m_a, m_b, m_c;
    __forceinline CEnginePrimitiveThemeChannelDesc() : m_a(false), m_b(false), m_c(false) { }
};

struct __declspec(novtable) CEngineInternalPrimitiveStaticMeshBase : CEngineInternalPrimitiveBase {   // intermediate base
    long          m_mesh;                          // +0x48
    long          m_material;                      // +0x4C
    long          m_animation;                     // +0x50
    unsigned char _pad54[0x10];
    CIVCountedPointer<CEnginePrimitiveSimulationInfo> SimulationInfo;   // +0x64
    CIVCountedPointer<CEngineMeshEffect>              MeshEffect;       // +0x68
    CEnginePrimitiveThemeChannelDesc                  ThemeChannelDesc; // +0x6C
    unsigned char _pad6f[0x3d];                    // +0x6F .. +0xAC
    __forceinline CEngineInternalPrimitiveStaticMeshBase(long type, unsigned long flags)
        : CEngineInternalPrimitiveBase(type, flags), m_mesh(0), m_material(0), m_animation(0) { }
};

struct CEngineInternalPrimitiveStaticMesh : CEngineInternalPrimitiveStaticMeshBase {   // vtable 0x012A425C, 0xac bytes
    virtual void v00();
    __forceinline CEngineInternalPrimitiveStaticMesh() : CEngineInternalPrimitiveStaticMeshBase(8, 0xa) { }
};

struct CEngineInternalPrimitiveMorphInfo {
    unsigned char _body[0x38];
    CEngineInternalPrimitiveMorphInfo();           // 0x00BAA250
};

struct CEngineInternalPrimitiveMorphedStaticMesh : CEngineInternalPrimitiveStaticMeshBase {   // vtable 0x012A51B4, 0xe4 bytes
    CEngineInternalPrimitiveMorphInfo Morph;       // +0xAC
    virtual void v00();
    __forceinline CEngineInternalPrimitiveMorphedStaticMesh() : CEngineInternalPrimitiveStaticMeshBase(0xa, 8) { }
};

struct CEngineInternalPrimitiveStaticMeshFromFile : CEngineInternalPrimitiveStaticMeshBase {   // vtable 0x012A4374, 0xac bytes
    virtual void v00();
    __forceinline CEngineInternalPrimitiveStaticMeshFromFile() : CEngineInternalPrimitiveStaticMeshBase(0xc, 8) { }
};

struct CEnginePrimitiveManagerStaticMeshesBase {
    CEngineInternalPrimitiveBase* CreateInternalPrimitive(long type);
};

CEngineInternalPrimitiveBase* CEnginePrimitiveManagerStaticMeshesBase::CreateInternalPrimitive(long type)
{
    switch (type) {
    case 8:
        return new CEngineInternalPrimitiveStaticMesh;
    case 10:
        return new CEngineInternalPrimitiveMorphedStaticMesh;
    case 12:
        return new CEngineInternalPrimitiveStaticMeshFromFile;
    }
    return 0;
}
