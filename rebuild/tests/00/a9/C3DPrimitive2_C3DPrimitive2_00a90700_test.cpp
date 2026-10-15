// Behaviour test for 0x00A90700 C3DPrimitive2::C3DPrimitive2
#include <cstdio>
#include <new>
struct C3DBoundingSphere { long m_x, m_y, m_z, m_r; C3DBoundingSphere() : m_x(0), m_y(0), m_z(0), m_r(0) {} };
template<class T> struct CArray { T* m_begin; T* m_end; T* m_capacity; CArray() : m_begin(0), m_end(0), m_capacity(0) {} };
template<class T> struct scoped_ptr { T* m_p; scoped_ptr() : m_p(0) {} };
template<class T> struct CCountedPointer { T* Data; void* Info; CCountedPointer() : Data(0), Info(0) {} };
struct C3DTextureStages { long D, B; bool U; C3DTextureStages() : D(-1), B(-1), U(false) {} };
struct C3DPrimitive2 {
    long MaterialIndex, RepeatingMeshReps; C3DBoundingSphere BoundingSphere; float AverageTextureStretch;
    CArray<int> ClothPrimitives; scoped_ptr<int> Data, VertexBuffer, IndexBuffer;
    unsigned long VertexBufferStatus, IndexBufferStatus, StaticBlockCount; void* StaticBlocks;
    unsigned long AnimatedBlockCount; void* AnimatedBlocks; unsigned long AnimatedBlocksStatus, RepeatedBlocksStatus;
    unsigned long VertexCount, TriangleCount, IndexCount; bool UploadedFlag; unsigned char VertexStreamFlags; unsigned long InitFlags;
    C3DTextureStages RecordedTextureStages; void* RecordedConstantData; long RecordedFrame;
    CArray<unsigned short> PartitionedMeshSourceIndexBuffer; CCountedPointer<int> PartitionTree; CArray<unsigned long> MirroredTextureVertexIndexList;
    C3DPrimitive2() : RepeatingMeshReps(0), VertexBufferStatus(0), IndexBufferStatus(0), StaticBlockCount(0), StaticBlocks(0),
        AnimatedBlockCount(0), AnimatedBlocks(0), AnimatedBlocksStatus(0), RepeatedBlocksStatus(0), UploadedFlag(false),
        RecordedConstantData(0), RecordedFrame(-1) {}
};
int main()
{
    static unsigned char storage[sizeof(C3DPrimitive2)];
    for (unsigned i = 0; i < sizeof(storage); ++i) storage[i] = 0xAB;
    C3DPrimitive2* p = new (storage) C3DPrimitive2;
    bool ok = p->RepeatingMeshReps == 0 && p->BoundingSphere.m_r == 0 && p->ClothPrimitives.m_capacity == 0 && p->Data.m_p == 0
        && p->IndexBuffer.m_p == 0 && p->StaticBlockCount == 0 && p->AnimatedBlocks == 0 && p->RepeatedBlocksStatus == 0
        && !p->UploadedFlag && p->RecordedTextureStages.D == -1 && p->RecordedTextureStages.B == -1 && !p->RecordedTextureStages.U
        && p->RecordedConstantData == 0 && p->RecordedFrame == -1 && p->PartitionTree.Data == 0 && p->PartitionTree.Info == 0
        && p->MirroredTextureVertexIndexList.m_end == 0 && p->PartitionedMeshSourceIndexBuffer.m_begin == 0
        && (p->InitFlags & 0xFF) == 0xAB && (p->MaterialIndex & 0xFF) == 0xAB && p->VertexStreamFlags == 0xAB;   // left uninitialised
    if (!ok) { std::printf("00a90700_TEST FAIL\n"); return 1; }
    std::printf("00a90700_TEST PASS\n");
    return 0;
}