// Behaviour test for 0x00A2B630 implicit ~CPhysicsMeshInfo::CMeshData: Mesh released (intrusive count, Delete at zero, pointer
// cleared), then FileData released, then ~CResource.
#include <cstdio>
#include <cstring>
static char g_log[32];
static void Log(const char* s) { std::strcat(g_log, s); }
class CIVCountedPointeeBase { public: CIVCountedPointeeBase() : Refs(1) { } long Release() { return --Refs; } void Delete() { Log("D"); } long Refs; };
class CMeshPointer {
public:
    CMeshPointer() : Object(0) { }
    ~CMeshPointer() { CIVCountedPointeeBase* o = Object; if (o) { if (o->Release() == 0) o->Delete(); Object = 0; } Log("M"); }
    CIVCountedPointeeBase* Object;
};
class CFilePointer { public: CFilePointer() : Count(0) { } ~CFilePointer() { if (Count && --*Count == 0) Log("F"); } int* Count; };
class CResource { public: virtual ~CResource() { Log("R"); } };
class CPhysicsMeshInfo;
class CMeshData : public CResource {
public:
    CMeshData(const CPhysicsMeshInfo* owner) : Owner(owner) { }
    CFilePointer FileData;
    CMeshPointer Mesh;
    const CPhysicsMeshInfo* Owner;
};
int main()
{
    CIVCountedPointeeBase mesh, shared;
    shared.Refs = 2;
    int fileCount = 1;
    g_log[0] = 0;
    CMeshData* d = new CMeshData(0);
    d->Mesh.Object = &mesh;
    d->FileData.Count = &fileCount;
    delete (CResource*)d;
    bool ok = std::strcmp(g_log, "DMFR") == 0 && mesh.Refs == 0 && fileCount == 0;
    g_log[0] = 0;
    d = new CMeshData(0);
    d->Mesh.Object = &shared;
    delete (CResource*)d;
    ok = ok && std::strcmp(g_log, "MR") == 0 && shared.Refs == 1;
    std::printf(ok ? "00a2b630_TEST PASS\n" : "00a2b630_TEST FAIL\n");
    return ok ? 0 : 1;
}