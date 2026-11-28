// Behaviour test for 0x00A24870 Fixture3DMeshLODInfo::ConvertToMeshStats
#include <cstdio>
static char g_log[16]; static int g_n = 0;
class Fixture3DMesh2 { public: unsigned long size; void ConvertToMeshStats() { g_log[g_n++] = 'C'; size = 100; } unsigned long Sizeof() const { return size; } };
class FixtureBankFileAsyncData { public: unsigned long Sizeof() const { return 0x48; } };
class Fixture3DMeshLODInfo;
class FixtureResourceList { public:
    Fixture3DMeshLODInfo* added; unsigned long freeArg;
    void AddResource(Fixture3DMeshLODInfo& r) { g_log[g_n++] = 'A'; added = &r; }
    bool MakeFreeSpace(unsigned long b) { g_log[g_n++] = 'F'; freeArg = b; return true; }
};
struct CMeshInfoOwner { FixtureResourceList ResourceList; };
class Fixture3DMeshInfo { public: CMeshInfoOwner* m_pOwner; };
class Fixture3DMeshLODInfo { public:
    unsigned long resourceSize;
    Fixture3DMesh2* Mesh_Data; FixtureBankFileAsyncData* FileData_Data; Fixture3DMeshInfo* MeshInfo; unsigned char LOD;
    void RemoveFromList() { g_log[g_n++] = 'R'; }
    void SetResourceSize(unsigned long s) { g_log[g_n++] = 'S'; resourceSize = s; }
    unsigned long Sizeof() const
    {
        unsigned long meshSize = Mesh_Data ? Mesh_Data->Sizeof() + 8 : 8;
        unsigned long fileSize = FileData_Data ? FileData_Data->Sizeof() + 8 : 8;
        return 0x34 + meshSize + fileSize;
    }
    void ConvertToMeshStats()
    {
        LOD = 4;
        Mesh_Data->ConvertToMeshStats();
        RemoveFromList();
        SetResourceSize(Sizeof());
        FixtureResourceList* list = &MeshInfo->m_pOwner->ResourceList;
        list->AddResource(*this);
        list->MakeFreeSpace(0);
    }
};
int main()
{
    Fixture3DMesh2 mesh; mesh.size = 5000;
    CMeshInfoOwner owner; owner.ResourceList.added = 0; owner.ResourceList.freeArg = 99;
    Fixture3DMeshInfo info; info.m_pOwner = &owner;
    Fixture3DMeshLODInfo lod; lod.Mesh_Data = &mesh; lod.FileData_Data = 0; lod.MeshInfo = &info; lod.LOD = 1; lod.resourceSize = 0;
    lod.ConvertToMeshStats();
    bool ok = lod.LOD == 4 && lod.resourceSize == 0x34 + 108 + 8 && owner.ResourceList.added == &lod && owner.ResourceList.freeArg == 0
        && g_n == 5 && g_log[0] == 'C' && g_log[1] == 'R' && g_log[2] == 'S' && g_log[3] == 'A' && g_log[4] == 'F';
    if (!ok) { std::printf("00a24870_TEST FAIL\n"); return 1; }
    std::printf("00a24870_TEST PASS\n");
    return 0;
}