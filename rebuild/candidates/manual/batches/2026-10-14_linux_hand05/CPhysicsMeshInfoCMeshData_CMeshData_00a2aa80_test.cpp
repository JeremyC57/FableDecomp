#include <cstdio>
#include <cstring>
struct CResource { virtual void v00() {} unsigned char _body[0x18]; CResource() { std::memset(_body, 0xAA, sizeof(_body)); } };
struct SMeshBuffers { void* m_pVertices; void* m_pIndices; void* m_pMesh; SMeshBuffers() : m_pVertices(0), m_pIndices(0), m_pMesh(0) { } };
struct CPhysicsMeshInfo { struct CMeshData : CResource { SMeshBuffers m_buffers; unsigned long m_size; virtual void v00() {} CMeshData(unsigned long size); }; };
CPhysicsMeshInfo::CMeshData::CMeshData(unsigned long size) : CResource(), m_buffers() { m_size = size; }
int main()
{
    CPhysicsMeshInfo::CMeshData d(1234);
    if (d.m_buffers.m_pVertices != 0 || d.m_buffers.m_pIndices != 0 || d.m_buffers.m_pMesh != 0 || d.m_size != 1234 || d._body[0] != 0xAA) { std::printf("00a2aa80_TEST FAIL\n"); return 1; }
    std::printf("00a2aa80_TEST PASS\n");
    return 0;
}
