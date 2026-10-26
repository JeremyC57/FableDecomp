#include <cstdio>
class CPhysicsMesh       { public: long m_size; long Sizeof() const { return m_size; } };
class CBankFileAsyncData { public: unsigned long m_size; unsigned long Sizeof() const { return m_size; } };
struct CMeshData {
    unsigned char _base[0x1c];
    CBankFileAsyncData* FileData_Data; void* FileData_Info; CPhysicsMesh* Mesh; void* Owner;
    unsigned long Sizeof() const
    {
        unsigned long meshSize = Mesh ? Mesh->Sizeof() + 4 : 4;
        unsigned long fileSize = FileData_Data ? FileData_Data->Sizeof() + 8 : 8;
        return 0x2c + meshSize + fileSize;
    }
};
class CPhysicsMeshInfo { public:
    CMeshData* MeshData;
    unsigned long Sizeof() const { return (MeshData ? MeshData->Sizeof() + 4 : 4) + 0xc0; }
};
int main()
{
    CPhysicsMeshInfo info; info.MeshData = 0;
    if (info.Sizeof() != 0xc4) { std::printf("00a26370_TEST FAIL\n"); return 1; }
    CMeshData d; d.FileData_Data = 0; d.Mesh = 0; info.MeshData = &d;
    if (info.Sizeof() != 0xf0 + 4 + 8) { std::printf("00a26370_TEST FAIL\n"); return 1; }
    CPhysicsMesh mesh; mesh.m_size = 100; CBankFileAsyncData file; file.m_size = 1000;
    d.Mesh = &mesh; d.FileData_Data = &file;
    if (info.Sizeof() != 0xf0 + 104 + 1008) { std::printf("00a26370_TEST FAIL\n"); return 1; }
    std::printf("00a26370_TEST PASS\n");
    return 0;
}