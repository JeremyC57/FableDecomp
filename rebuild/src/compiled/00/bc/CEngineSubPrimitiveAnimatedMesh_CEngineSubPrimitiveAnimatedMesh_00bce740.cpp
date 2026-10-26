// CEngineSubPrimitiveAnimatedMesh::CEngineSubPrimitiveAnimatedMesh(void* primitive, void* mesh, void* material, bool flag) -- the inlined CEngineSubPrimitiveBase (CBase, the primitive at +0xc, the cleared link at +0x10), the flag byte at +0x20, the mesh and material at +0x18 / +0x1c, this class's vtable last
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BCE740, length 55 bytes.
// Callees: CBase::CBase @ 0x0099A2F0. Vtable 0x012A6C5C = CEngineSubPrimitiveAnimatedMesh (rtti-vtables.tsv). Shape: the four arguments are loaded into eax/ecx/edx/al, every member store
// precedes the vtable store (a vtable-last constructor), `this` is returned, `ret 10h`.
struct CBase {
    virtual void v00();
    CBase();                                       // 0x0099A2F0
};

struct __declspec(novtable) CEngineSubPrimitiveBase : CBase {   // vtable 0x012A3E7C (stored only by its own constructor @ 0x00B98A50)
    unsigned char _pad04[8];                       // +0x04 .. +0x0c
    void*         m_pPrimitive;                    // +0x0c
    long          m_pNext;                         // +0x10
    CEngineSubPrimitiveBase(void* primitive) : CBase(), m_pPrimitive(primitive), m_pNext(0) { }
    CEngineSubPrimitiveBase(void* primitive, void* mesh, void* material, bool flag)   // the mesh sub-primitives' base constructor: every member store before the derived vtable
        : CBase(), m_pPrimitive(primitive), m_pNext(0), m_pMesh(mesh), m_pMaterial(material), m_flag(flag) { }
    unsigned char _pad14[4];                       // +0x14 (not initialised)
    void*         m_pMesh;                         // +0x18
    void*         m_pMaterial;                     // +0x1c
    bool          m_flag;                          // +0x20
};

class CEngineSubPrimitiveAnimatedMesh : public CEngineSubPrimitiveBase { public:   // vtable 0x012A6C5C
    virtual void v00();
    CEngineSubPrimitiveAnimatedMesh(void* primitive, void* mesh, void* material, bool flag);
};

CEngineSubPrimitiveAnimatedMesh::CEngineSubPrimitiveAnimatedMesh(void* primitive, void* mesh, void* material, bool flag)
    : CEngineSubPrimitiveBase(primitive, mesh, material, flag)
{
}