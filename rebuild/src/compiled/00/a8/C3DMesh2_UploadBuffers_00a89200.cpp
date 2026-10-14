// C3DMesh2::UploadBuffers() -- AND together UploadBuffers() of every primitive (160-byte stride), true when all succeed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A89200, length 80 bytes.
// Callees: 0x00A93380 C3DPrimitive2::UploadBuffers
// `ok &= ...` on a bool keeps the running result in bl (and bl,al); the primitive pointer and count are
// re-read from `this` every iteration because the callee may modify the mesh.
//
#include "rebuild_abi.h"
struct CHandleBasedMemoryPool {
    void Lock(long handle);                             // 0x00A026F0
};
struct C3DPrimitive2 {
    unsigned char _body[0xa0];                          // 160-byte primitive record
    bool UploadBuffers();                               // 0x00A93380
};
// Retail C3DMesh2 (no PDB header for this class): the pool pointer at +0x04, three pool handles at
// +0x7c/+0x80/+0x84 (the third optional, -1 when absent), the primitive count at +0x90 and the
// primitive array at +0x94.
struct C3DMesh2 {
    void*                   __vftable;                  // +0x00
    CHandleBasedMemoryPool* m_pPool;                    // +0x04
    unsigned char           _pad_0x08[0x74];            // +0x08
    long                    m_vertexBufferHandle;       // +0x7c
    long                    m_indexBufferHandle;        // +0x80
    long                    m_tangentBufferHandle;      // +0x84 (-1 = none)
    unsigned char           _pad_0x88[0x08];            // +0x88
    unsigned long           m_numPrimitives;            // +0x90
    C3DPrimitive2*          m_pPrimitives;              // +0x94
    bool UploadBuffers();
    C3DPrimitive2* LockPrimitive(long index);
};

bool C3DMesh2::UploadBuffers()
{
    bool ok = true;
    for (unsigned long i = 0; i < m_numPrimitives; ++i)
        ok &= m_pPrimitives[i].UploadBuffers();
    return ok;
}