// CLandscapeTesselatorBase::BuildBridgeVertexAndIndexBuffers(long bridge, CMovableResource* owner) -- copy the tesselated bridge strip into landscape-memory index and vertex buffers owned by `owner` and record them (with the triangle count) in the tesselation data's slot `bridge`; false when an allocation fails
// Leaf: BuildBridgeVertexAndIndexBuffers
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BE3660, length 380 bytes, `ret 8`.
// Callees: CEngineResourceManager::GetIndexBufferUsage @ 0x00B3B060 / GetVertexBufferUsage @ 0x00B3B030 on the engine data
//   global @ 0x01436E98, CEngineLandscapeMemoryManager::AllocateIndexBuffer(CMovableResource*, long, unsigned long,
//   EIndexBufferFormat, EPrimitiveType) @ 0x00BDA360 / AllocateVertexBuffer(CMovableResource*, EVertexType, long, long,
//   unsigned long) @ 0x00BDA3D0 (the manager at +0x6B0 of the engine global @ 0x01436EA8), CIndexBuffer::Lock @ 0x00AC1FB0 /
//   Unlock @ 0x00AC1F00, CVertexBufferWin32::Lock @ 0x00AC20A0 / Unlock @ 0x00AC20B0, the tesselator's virtual slots 1
//   (vertex count), 2 (vertex data) and 3 (vertex size); memcpy intrinsic.
// Layout: Ego_r.pdb CLandscapeTesselatorBase with the retail 12-byte CArray: BridgeIndexBuffer (unsigned short) +0x04, Data
//   (CTesselationData*) +0x10 (PDB +0x14); CTesselationData bridge vertex buffers +0x2C, bridge index buffers +0x3C, bridge
//   triangle counts +0x4C. Engine-side manager names from the manifest labels.
// Shape: the managers are fetched before the argument pushes; the index copy source is the CArray's data pointer when
//   non-empty (else the array itself); each locked pointer is a named local taken before the copy source is evaluated
//   (memcpy evaluates its arguments right to left); the triangle count is size / 3.
#include <string.h>
#pragma intrinsic(memcpy)
struct CMovableResource;
struct CIndexBuffer {
    unsigned char* Lock(unsigned long offset, unsigned long size, unsigned long flags);   // 0x00AC1FB0
    void Unlock();                                 // 0x00AC1F00
};
struct CVertexBufferWin32 {
    unsigned char* Lock(unsigned long offset, unsigned long size, unsigned long flags);   // 0x00AC20A0
    void Unlock();                                 // 0x00AC20B0
};
enum EIndexBufferFormat { INDEX_FORMAT_16 = 0x65 };
enum EPrimitiveType { PRIMITIVE_TRIANGLE_LIST = 4 };
enum EVertexType { VERTEX_TYPE_DEFAULT = 0 };
struct CEngineLandscapeMemoryManager {
    CIndexBuffer* AllocateIndexBuffer(CMovableResource* owner, long size, unsigned long usage, EIndexBufferFormat format, EPrimitiveType type);   // 0x00BDA360
    CVertexBufferWin32* AllocateVertexBuffer(CMovableResource* owner, EVertexType type, long count, long size, unsigned long usage);   // 0x00BDA3D0
};
struct CEngineResourceManager {
    unsigned long GetIndexBufferUsage();           // 0x00B3B060
    unsigned long GetVertexBufferUsage();          // 0x00B3B030
};
struct CEngine {
    unsigned char                  _pad[0x6b0];
    CEngineLandscapeMemoryManager* LandscapeMemoryManager;   // +0x6b0
};
extern CEngine*                g_pEngine;          // 0x01436EA8
extern CEngineResourceManager* g_pResourceManager; // 0x01436E98
struct CTesselationData {
    unsigned char       _pad[0x2c];
    CVertexBufferWin32* BridgeVertexBuffers[4];    // +0x2c
    CIndexBuffer*       BridgeIndexBuffers[4];     // +0x3c
    long                BridgePolyCounts[4];       // +0x4c
};
template <class T> struct CArray {                 // retail 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    const T* GetData() const { return size() > 0 ? _M_start : (const T*)this; }
};
class CLandscapeTesselatorBase {
public:
    virtual void v00();
    virtual long GetVertexCount();                 // slot 1
    virtual const void* GetVertexData();           // slot 2
    virtual long GetVertexSize();                  // slot 3
    CArray<unsigned short> BridgeIndexBuffer;      // +0x04
    CTesselationData*      Data;                   // +0x10
    bool BuildBridgeVertexAndIndexBuffers(long bridge, CMovableResource* owner);
};

bool CLandscapeTesselatorBase::BuildBridgeVertexAndIndexBuffers(long bridge, CMovableResource* owner)
{
    if (BridgeIndexBuffer.size() != 0) {
        long size = BridgeIndexBuffer.size() * sizeof(unsigned short);
        CEngineLandscapeMemoryManager* memory = g_pEngine->LandscapeMemoryManager;
        CIndexBuffer* indexBuffer = memory->AllocateIndexBuffer(owner, size, g_pResourceManager->GetIndexBufferUsage(),
                                                                INDEX_FORMAT_16, PRIMITIVE_TRIANGLE_LIST);
        if (!indexBuffer)
            return false;
        unsigned char* indices = indexBuffer->Lock(0, 0, 0);
        memcpy(indices, BridgeIndexBuffer.GetData(), size);
        indexBuffer->Unlock();
        Data->BridgeIndexBuffers[bridge] = indexBuffer;
        Data->BridgePolyCounts[bridge] = (long)BridgeIndexBuffer.size() / 3;
    }
    if (GetVertexCount()) {
        CEngineLandscapeMemoryManager* memory = g_pEngine->LandscapeMemoryManager;
        CVertexBufferWin32* vertexBuffer = memory->AllocateVertexBuffer(owner, VERTEX_TYPE_DEFAULT, GetVertexCount(), GetVertexSize(),
                                                                         g_pResourceManager->GetVertexBufferUsage());
        if (!vertexBuffer)
            return false;
        unsigned char* vertices = vertexBuffer->Lock(0, 0, 0);
        memcpy(vertices, GetVertexData(), GetVertexSize() * GetVertexCount());
        vertexBuffer->Unlock();
        Data->BridgeVertexBuffers[bridge] = vertexBuffer;
    }
    return true;
}