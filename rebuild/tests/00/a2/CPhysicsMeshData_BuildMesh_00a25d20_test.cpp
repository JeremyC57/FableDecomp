// CPhysicsMeshData::BuildMesh() -- build the physics mesh from the loaded file data and register the resource
// Shape: the pool for the loader call is a named local read before the size and loader locals (retail loads pool, FileData, size, then the Owner->Manager->MeshFileLoader chain in ebx); the CPhysicsMesh ctor re-reads it inside the new-expression (`pool = manager->MemoryPool`) so it lands in the dying manager register edi
void operator delete(void* p);                     // 0x00BFE9BC
struct CCPPointerInfo {                            // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    T* operator->() const { return Data; }
    unsigned long Sizeof() const { return Data ? Data->DoSizeof() + 8 : 8; }
    void Reset()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Info = 0;
        Data = 0;
    }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
template <class T> struct CIVCountedPointer {      // intrusive counted pointer
    T* Data;
    void Clear() { if (Data) { Data->Release(); Data = 0; } }
    CIVCountedPointer& operator=(T* p) { Clear(); Data = p; return *this; }
    long Sizeof() const { return Data ? Data->Sizeof() + 4 : 4; }
};
struct CBankFileAsyncData {
    unsigned char _pad00[0x10];
    unsigned long Size;                            // +0x10
    void* GetData();                               // 0x009D5530
    unsigned long DoSizeof();                      // 0x009D5620
};
struct C3DMeshFile;
struct CMovableResourceMemoryPool;
struct CPhysicsMesh {
    virtual void v00();
    virtual void Destroy();                        // slot 1 (+0x04)
    long RefCount;                                 // +0x04
    unsigned char _pad08[0xc0 - 0x08];
    CPhysicsMesh(C3DMeshFile* file, CMovableResourceMemoryPool* pool);   // 0x00A6DA40
    long Sizeof();                                 // 0x00A6C900
    void Release() { if (--RefCount == 0) Destroy(); }
};
struct C3DMeshFileLoader {
    CCountedPointer<C3DMeshFile> LoadMeshFileCompressed(void* data, unsigned long size, CMovableResourceMemoryPool* pool);   // 0x00A9B740
};
struct CResource;
struct CResourceList {
    bool MakeFreeSpace(unsigned long size);        // 0x009FC4F0
    void AddResource(CResource* resource);         // 0x009FBFE0
};
struct CPhysicsMeshManager {
    unsigned char               _pad000[0x164];
    C3DMeshFileLoader*          MeshFileLoader;    // +0x164
    unsigned char               _pad168[0x2e8 - 0x168];
    CResourceList               ResourceList;      // +0x2e8
    unsigned char               _pad2e9[0x370 - 0x2e9];
    CMovableResourceMemoryPool* MemoryPool;        // +0x370
};
struct CPhysicsMeshInfo {
    void*                __vftable;
    CPhysicsMeshManager* Manager;                  // +0x04
};
struct CResource {                                 // 0x1c bytes
    void* __vftable;
    unsigned char _pad04[0x18];
    void SetResourceSize(unsigned long size);      // 0x009FBF20
    void SetBuilt();                               // 0x009FC440 (labelled CTCActivationTrigger::OnKill)
};
struct CPhysicsMeshData : CResource {
    CCountedPointer<CBankFileAsyncData> FileData;  // +0x1c
    CIVCountedPointer<CPhysicsMesh>     Mesh;      // +0x24
    CPhysicsMeshInfo*                   Owner;     // +0x28
    void BuildMesh();
};

void CPhysicsMeshData::BuildMesh()
{
    CPhysicsMeshManager* manager = Owner->Manager;
    CResourceList& resources = manager->ResourceList;
    resources.MakeFreeSpace(FileData->Size);
    CMovableResourceMemoryPool* pool = manager->MemoryPool;
    unsigned long size = FileData->Size;
    C3DMeshFileLoader* loader = Owner->Manager->MeshFileLoader;
    CCountedPointer<C3DMeshFile> file = loader->LoadMeshFileCompressed(FileData->GetData(), size, pool);
    Mesh = new CPhysicsMesh(file.Data, pool = manager->MemoryPool);
    FileData.Reset();
    SetResourceSize(0x2c + Mesh.Sizeof() + FileData.Sizeof());
    SetBuilt();
    resources.AddResource(this);
    resources.MakeFreeSpace(0);
}
// ---- harness ----
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static char g_bankData[16];
static int g_destroyed = 0, g_fileDeletes = 0, g_bankDeletes = 0;
static unsigned long g_free[4]; static const void* g_freeThis[4]; static int g_nfree = 0;
static unsigned long g_resSize = 0; static int g_built = 0; static CResource* g_added = 0; static const void* g_addThis = 0;
static void* g_ldData = 0; static unsigned long g_ldSize = 0; static CMovableResourceMemoryPool* g_ldPool = 0; static const void* g_ldThis = 0;
static C3DMeshFile* g_ctorFile = 0; static CMovableResourceMemoryPool* g_ctorPool = 0;
static C3DMeshFile* const kFile = (C3DMeshFile*)0x1234;
void* CBankFileAsyncData::GetData() { return g_bankData; }
unsigned long CBankFileAsyncData::DoSizeof() { return 0x40; }
void CPhysicsMesh::v00() { }
void CPhysicsMesh::Destroy() { ++g_destroyed; }
CPhysicsMesh::CPhysicsMesh(C3DMeshFile* file, CMovableResourceMemoryPool* pool) { RefCount = 1; g_ctorFile = file; g_ctorPool = pool; }
long CPhysicsMesh::Sizeof() { return 0x100; }
static void __fastcall DeleteFile(void* data) { if (data == kFile) ++g_fileDeletes; }
static void __fastcall DeleteBank(void* data) { (void)data; ++g_bankDeletes; }
CCountedPointer<C3DMeshFile> C3DMeshFileLoader::LoadMeshFileCompressed(void* data, unsigned long size, CMovableResourceMemoryPool* pool)
{
    g_ldThis = this; g_ldData = data; g_ldSize = size; g_ldPool = pool;
    CCountedPointer<C3DMeshFile> r;
    r.Data = kFile;
    r.Info = new CCPPointerInfo;
    r.Info->RefCount = 1; r.Info->DeleteFunc = DeleteFile; r.Info->Data = kFile;
    return r;
}
bool CResourceList::MakeFreeSpace(unsigned long size) { if (g_nfree < 4) { g_freeThis[g_nfree] = this; g_free[g_nfree++] = size; } return true; }
void CResourceList::AddResource(CResource* resource) { g_added = resource; g_addThis = this; }
void CResource::SetResourceSize(unsigned long size) { g_resSize = size; }
void CResource::SetBuilt() { ++g_built; }

int main()
{
    static unsigned char mgrMem[0x380];
    memset(mgrMem, 0, sizeof mgrMem);
    CPhysicsMeshManager* mgr = (CPhysicsMeshManager*)mgrMem;
    C3DMeshFileLoader loaderObj;
    mgr->MeshFileLoader = &loaderObj;
    mgr->MemoryPool = (CMovableResourceMemoryPool*)0x5678;
    CPhysicsMeshInfo info; info.__vftable = 0; info.Manager = mgr;

    CBankFileAsyncData bank; memset(&bank, 0, sizeof bank); bank.Size = 0x777;
    CPhysicsMeshData d; memset(&d, 0, sizeof d);
    d.Owner = &info;
    d.FileData.Data = &bank;
    d.FileData.Info = new CCPPointerInfo;
    d.FileData.Info->RefCount = 1; d.FileData.Info->DeleteFunc = DeleteBank; d.FileData.Info->Data = &bank;
    CPhysicsMesh* old = new CPhysicsMesh(0, 0);
    d.Mesh.Data = old;

    d.BuildMesh();

    CHECK(g_nfree == 2);
    CHECK(g_free[0] == 0x777 && g_free[1] == 0);
    CHECK(g_freeThis[0] == &mgr->ResourceList && g_freeThis[1] == &mgr->ResourceList);
    CHECK(g_ldThis == &loaderObj && g_ldData == g_bankData && g_ldSize == 0x777 && g_ldPool == (CMovableResourceMemoryPool*)0x5678);
    CHECK(g_ctorFile == kFile && g_ctorPool == (CMovableResourceMemoryPool*)0x5678);
    CHECK(g_destroyed == 1);                    // the old mesh was released
    CHECK(d.Mesh.Data != 0 && d.Mesh.Data != old);
    CHECK(g_bankDeletes == 1);                  // FileData.Reset() dropped the last reference
    CHECK(d.FileData.Data == 0 && d.FileData.Info == 0);
    CHECK(g_resSize == 0x2c + 0x104 + 8);
    CHECK(g_built == 1);
    CHECK(g_added == &d && g_addThis == &mgr->ResourceList);
    CHECK(g_fileDeletes == 1);                  // the loaded file pointer died at scope end
    if (fails == 0) printf("00a25d20_TEST PASS\n");
    return fails;
}