// C3DMeshFileGroupChunk::GetList() -- first element of the 8-byte vertex CArray at +0x30, or the array object itself when empty
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ABC370, length 32 bytes.
// Callees: (none)
// Same shape as the landed GetList @ 0x00ABC420 (stride 32); here the element is 8 bytes (sar 3).
//
#include "rebuild_abi.h"
struct Vec8 { void* m_begin; void* m_end; void* m_capacity; };
struct C3DMeshFileGroupChunk {
    unsigned char _base[0x30];     // C3DMeshFileChunk (0x2c) + BoneIndex (+0x2c)
    Vec8 VertexList;               // +0x30 CArray<C3DVertexBlend2>
    void* GetList();
};
void* C3DMeshFileGroupChunk::GetList()
{
    Vec8* pv = &VertexList;
    if ((unsigned)(((char*)pv->m_end - (char*)pv->m_begin) >> 3) > 0)
        return pv->m_begin;
    return pv;
}
