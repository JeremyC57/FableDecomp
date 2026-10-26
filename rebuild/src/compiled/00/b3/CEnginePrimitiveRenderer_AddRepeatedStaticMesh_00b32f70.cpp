// CEnginePrimitiveRenderer::AddRepeatedStaticMesh(const CAddMeshDesc& desc) -- create a repeated static mesh through the renderer's manager and return its handle
// Leaf: AddRepeatedStaticMesh
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B32F70, length 28 bytes (__fastcall, hidden result pointer).
// Identity: the catalog label GFPredicateAnd<...> is propagated.
// Callees: CEnginePrimitiveManagerRepeatedStaticMeshes::CreatePrimitive 0x00B9AB30.
// Model: LOCAL CEnginePrimitiveRenderer next to engine/CEnginePrimitiveRenderer.h: the header puts RepeatedStaticMeshManager 4 bytes
//   high (its PDB CArray is 0x10 bytes, retail's 0x0c), retail reads it at +0x290.
// Shape: the handle has a user copy ctor and dtor, so the result goes straight through the hidden pointer.
struct CAddMeshDesc;
class CEnginePrimitiveHandle { public:
    void* PPrimitive;                                       // the created primitive
    CEnginePrimitiveHandle(const CEnginePrimitiveHandle& other);
    ~CEnginePrimitiveHandle();
};
class CEnginePrimitiveManagerRepeatedStaticMeshes { public:
    CEnginePrimitiveHandle CreatePrimitive(const CAddMeshDesc& desc);   // 0x00B9AB30
};
class CEnginePrimitiveRenderer { public:
    unsigned char _pad[0x290];
    CEnginePrimitiveManagerRepeatedStaticMeshes* RepeatedStaticMeshManager;   // +0x290
    CEnginePrimitiveHandle AddRepeatedStaticMesh(const CAddMeshDesc& desc);
};
CEnginePrimitiveHandle CEnginePrimitiveRenderer::AddRepeatedStaticMesh(const CAddMeshDesc& desc)
{
    return RepeatedStaticMeshManager->CreatePrimitive(desc);
}