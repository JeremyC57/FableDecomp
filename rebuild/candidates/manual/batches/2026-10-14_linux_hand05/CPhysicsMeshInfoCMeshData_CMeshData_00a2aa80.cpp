// CPhysicsMeshInfo::CMeshData::CMeshData(unsigned long size) -- base CResource(), own vtable, three null buffer pointers, the size argument
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A2AA80, length 36 bytes.
// Callees: CResource::CResource @ 0x009FBEC0 (vtable 0x012354A4, 7 dwords: ref count 1, list links,
// size and frame zeroed). This class's vtable is 0x0129CD78 (rtti-vtables.tsv: CPhysicsMeshInfo::CMeshData).
// Shape: the base runs on `this` (esi), the vtable is stored BEFORE the three zero stores at
// +0x1c..+0x24 -- so those three pointers are an embedded member object with an inline constructor
// (an init-list member would sink the vtable store below it, GOTCHAS) -- and the argument is
// assigned to +0x28 in the body. CResource occupies +0..+0x1c (engine/CResource.h).
struct CResource {
    virtual void v00();
    unsigned char _body[0x18];              // +0x04..+0x1c (ref count, list, prev/next, size, frame)
    CResource();                            // 0x009FBEC0
};

struct SMeshBuffers {                       // three raw buffer pointers, nulled by the inline constructor
    void* m_pVertices;                      // +0x00
    void* m_pIndices;                       // +0x04
    void* m_pMesh;                          // +0x08
    SMeshBuffers() : m_pVertices(0), m_pIndices(0), m_pMesh(0) { }
};

struct CPhysicsMeshInfo {
    struct CMeshData : CResource {          // vtable 0x0129CD78
        SMeshBuffers  m_buffers;            // +0x1c
        unsigned long m_size;               // +0x28
        virtual void v00();
        CMeshData(unsigned long size);
    };
};

CPhysicsMeshInfo::CMeshData::CMeshData(unsigned long size)
    : CResource()
    , m_buffers()
{
    m_size = size;
}
