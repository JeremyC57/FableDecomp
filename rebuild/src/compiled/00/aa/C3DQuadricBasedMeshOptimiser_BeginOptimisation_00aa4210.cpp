// C3DQuadricBasedMeshOptimiser::BeginOptimisation(C3DMeshFile&, float) -- build an optimised mesh for the file and start it with the quadric optimisation parameters, returned by counted pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AA4210, length 211 bytes (`ret 0Ch`: hidden result, file, quality).
// MESH LAYER (replacement work later): Ego_r.pdb ?BeginOptimisation@C3DQuadricBasedMeshOptimiser@@QAE?AV?$CCountedPointer@VC3DOptimisedMeshBase@@@@AAVC3DMeshFile@@M@Z.
// Callees: operator new @ 0x00BFEA1A (0x10 bytes), COptimisedMesh::COptimisedMesh(C3DMeshFile*) @ 0x00ACE600,
// CCountedPointer<C3DOptimisedMeshBase>::Reset(T*) @ 0x00AA4460 (catalog "CCountedPointer<CIPhysicsFacesSegmentBase const>::Reset",
// a byte twin), the mesh's start call taking the parameter block @ 0x00ACD500 (catalog "COptimisedMesh::CollapseSegments"),
// operator delete @ 0x00BFE9BC.
// Shape: a 0x28-byte parameter block on the stack (the quality argument, then 1000, 1e6, 2.5, 0.00075, 1e6, 25000, 50,
// 1e6 and the integer 3), a zeroed counted-pointer local reset to the new mesh, the start call, then `return mesh;`
// (no NRVO: merged inc/dec copy-out).
// Leaf: BeginOptimisation@C3DQuadricBasedMeshOptimiser
struct SCountBlock {
    long m_references;
    void (__fastcall* m_pfnDestroy)(void*);
    void* m_pObject;
    SCountBlock(void (__fastcall* destroy)(void*), void* object) : m_references(1), m_pfnDestroy(destroy), m_pObject(object) {}
};
template<class T> struct CCountedPointer {
    T* Object;                                      // +0
    SCountBlock* Count;                             // +4
    CCountedPointer() : Object(0), Count(0) {}
    CCountedPointer(const CCountedPointer& other) : Object(other.Object), Count(other.Count) { if (Count) ++Count->m_references; }
    ~CCountedPointer()
    {
        if (Count) {
            if (--Count->m_references == 0) {
                Count->m_pfnDestroy(Count->m_pObject);
                delete Count;
            }
        }
    }
    void Reset(T* object);                          // 0x00AA4460 (defined below, before the caller)
    static void __fastcall DeleteData(void* object);   // 0x00AA4310
    T* operator->() const { return Object; }
};

struct CQuadricOptimisationParams {                 // 0x28 bytes
    float Quality;                                  // +0x00 the caller's value
    float m_04;                                     // 1000.0
    float m_08;                                     // 1000000.0
    float m_0c;                                     // 2.5
    float m_10;                                     // 0.00075
    float m_14;                                     // 1000000.0
    float m_18;                                     // 25000.0
    float m_1c;                                     // 50.0
    float m_20;                                     // 1000000.0
    long  m_24;                                     // 3
};
struct C3DMeshFile;
struct C3DOptimisedMeshBase {
    void BeginOptimisation(const CQuadricOptimisationParams& params);   // 0x00ACD500
};
struct COptimisedMesh : C3DOptimisedMeshBase {
    COptimisedMesh(C3DMeshFile* file);              // 0x00ACE600
    unsigned char _body[0x10];
};

struct C3DQuadricBasedMeshOptimiser {
    CCountedPointer<C3DOptimisedMeshBase> BeginOptimisation(C3DMeshFile& file, float quality);
};

// Reset shares the TU (0x00AA4460, right after this function in retail) and is defined above the caller:
// VC7.1 then knows it leaves eax/esi/edi alone and keeps the new mesh's words in esi across the calls.
#pragma auto_inline(off)
template<class T> void CCountedPointer<T>::Reset(T* object)
{
    if (Count) {
        --Count->m_references;
        if (Count->m_references == 0) {
            Count->m_pfnDestroy(Count->m_pObject);
            delete Count;
        }
    }
    Count = 0;
    Object = object;
    if (object)
        Count = new SCountBlock(DeleteData, Object);
}
template void CCountedPointer<C3DOptimisedMeshBase>::Reset(C3DOptimisedMeshBase* object);
#pragma auto_inline(on)

CCountedPointer<C3DOptimisedMeshBase> C3DQuadricBasedMeshOptimiser::BeginOptimisation(C3DMeshFile& file, float quality)
{
    CQuadricOptimisationParams params;
    params.Quality = quality;
    params.m_04 = 1000.0f;
    params.m_08 = 1000000.0f;
    params.m_0c = 2.5f;
    params.m_10 = 0.00075f;
    params.m_14 = 1000000.0f;
    params.m_18 = 25000.0f;
    params.m_1c = 50.0f;
    params.m_20 = 1000000.0f;
    params.m_24 = 3;
    CCountedPointer<C3DOptimisedMeshBase> mesh;
    mesh.Reset(new COptimisedMesh(&file));
    mesh->BeginOptimisation(params);
    return mesh;
}