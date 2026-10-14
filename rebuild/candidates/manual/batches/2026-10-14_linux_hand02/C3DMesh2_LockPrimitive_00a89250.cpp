// C3DMesh2::LockPrimitive(long index) -- lock the vertex, index and (if present) tangent pool handles, return the indexed 160-byte primitive
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A89250, length 80 bytes.
// Callees: 0x00A026F0 CHandleBasedMemoryPool::Lock x3
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

C3DPrimitive2* C3DMesh2::LockPrimitive(long index)
{
    m_pPool->Lock(m_vertexBufferHandle);
    m_pPool->Lock(m_indexBufferHandle);
    if (m_tangentBufferHandle != -1)
        m_pPool->Lock(m_tangentBufferHandle);
    return &m_pPrimitives[index];
}
