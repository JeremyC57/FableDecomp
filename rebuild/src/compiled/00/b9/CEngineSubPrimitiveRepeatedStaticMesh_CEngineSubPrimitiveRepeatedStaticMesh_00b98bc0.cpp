// CEngineSubPrimitiveRepeatedStaticMesh::CEngineSubPrimitiveRepeatedStaticMesh(void* mesh, void* material, void* instances) -- CBase, mesh at +0xc, cleared count and flag, material and instances at +0x18/+0x1c, own vtable last
// Leaf: ??0CEngineSubPrimitiveRepeatedStaticMesh@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B98BC0, length 49 bytes.
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0. Vtable 0x012A3E94 = CEngineSubPrimitiveRepeatedStaticMesh
// (rtti-vtables.tsv); the chunk label "CVertexShaderData::CVertexShaderData" is propagated.
// Shape: EVERY member store precedes the vtable store, so all of them belong to an argument-carrying
// `__declspec(novtable)` intermediate base (CEngineSubPrimitive-like, 0x21 bytes over CBase) whose
// init list runs +0xc = arg1, +0x10 = 0, then the flag byte at +0x20 is cleared from the same zero register
// before +0x18 = arg2 and +0x1c = arg3; the derived class only adds its vtable; `ret 0Ch`.
class CBaseClassNonCopyable {
protected:
    virtual ~CBaseClassNonCopyable();   // 0x0099A300
    CBaseClassNonCopyable();                                  // 0x0099A2F0
};

struct __declspec(novtable) CEngineSubPrimitiveBase : CBaseClassNonCopyable {
    unsigned char _pad04[8];                  // +0x04..+0x0b
    void*         m_pMesh;                    // +0x0c
    long          m_count;                    // +0x10
    unsigned char _pad14[4];                  // +0x14
    void*         m_pMaterial;                // +0x18
    void*         m_pInstances;               // +0x1c
    unsigned char m_flag;                     // +0x20
    CEngineSubPrimitiveBase(void* mesh, void* material, void* instances)
        : CBaseClassNonCopyable(), m_pMesh(mesh), m_count(0), m_pMaterial(material), m_pInstances(instances), m_flag(0) { }
};

struct CEngineSubPrimitiveRepeatedStaticMesh : CEngineSubPrimitiveBase {   // vtable 0x012A3E94
    CEngineSubPrimitiveRepeatedStaticMesh(void* mesh, void* material, void* instances);
};

CEngineSubPrimitiveRepeatedStaticMesh::CEngineSubPrimitiveRepeatedStaticMesh(void* mesh, void* material, void* instances)
    : CEngineSubPrimitiveBase(mesh, material, instances)
{
}