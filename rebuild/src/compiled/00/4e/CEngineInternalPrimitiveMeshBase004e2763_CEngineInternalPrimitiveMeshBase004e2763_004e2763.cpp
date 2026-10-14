#pragma optimize("s", on)
// CEngineInternalPrimitiveMeshBase_004e2763::~CEngineInternalPrimitiveMeshBase_004e2763() -- free the vertex block at +0x10, release the mesh resource's counted pointer at +0xc, then the base destructor (sibling jump)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E2763, length 46 bytes.
// Callees: free @ 0x00BFEA14 (`pop ecx` cleanup: size TU), the resource's virtual slot 1 (Destroy) at zero count, CTCBase::~CTCBase @ 0x00686830 (the base, a
// sibling jump with `this`). Shape: no vtable store (a non-polymorphic class); the members are destroyed in reverse declaration order (the buffer at +0x10 freed
// before the counted pointer at +0xc is released, the size TU's `and dword ptr [esi+0Ch],0`), the destructor body itself is empty.
// The chunk label names the class family; the 0x004Exxxx body is the size-optimised sibling of 0x00781CE0.
extern "C" void __cdecl free(void* block);        // 0x00BFEA14

struct CMeshResource {                             // counted object: vptr, then the reference count
    virtual void v00();
    virtual void Destroy();                        // slot 1
    long m_refCount;                               // +0x04
    void Release()
    {
        if (--m_refCount == 0)
            Destroy();
    }
};

struct CIVCountedPointer_Mesh {
    CMeshResource* m_p;
    __forceinline ~CIVCountedPointer_Mesh()        // the inlined member destructor: the release (forced: the size TU would call it)
    {
        if (m_p) {
            m_p->Release();
            m_p = 0;
        }
    }
};

struct CMallocBlock {                              // a malloc'd buffer member: its destructor frees it
    void* m_p;
    __forceinline ~CMallocBlock()
    {
        if (m_p != 0)
            free(m_p);
    }
};

struct CTCBase {
    unsigned char _pad[0x0c];                      // +0x00 .. +0x0c
    ~CTCBase();                                    // 0x00686830
};

struct CEngineInternalPrimitiveMeshBase_004e2763 : CTCBase {
    CIVCountedPointer_Mesh m_resource;             // +0x0c
    CMallocBlock           m_vertices;             // +0x10
    ~CEngineInternalPrimitiveMeshBase_004e2763();
};

CEngineInternalPrimitiveMeshBase_004e2763::~CEngineInternalPrimitiveMeshBase_004e2763()
{
}