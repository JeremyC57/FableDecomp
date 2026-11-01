// Test for C3DMeshLODInfo::~C3DMeshLODInfo (0x00A24470): the FileData counted pointer is released before Mesh (reverse
// member order), each deleter runs on its last reference, both pointers end cleared, then the CResource base runs.
#include <cstdio>
static char g_log[16]; static int g_n = 0;
struct Info { unsigned long RefCount; void (*DeleteFunc)(void*); void* Data; };
static void DeleteMesh(void*) { g_log[g_n++] = 'M'; }
static void DeleteFile(void*) { g_log[g_n++] = 'F'; }
template <class T> struct CountedPointer {
    T* Data; Info* Info_;
    ~CountedPointer() { if (Info_) { --Info_->RefCount; if (Info_->RefCount == 0) { Info_->DeleteFunc(Info_->Data); delete Info_; } } Data = 0; Info_ = 0; }
};
struct Resource { virtual ~Resource() { g_log[g_n++] = 'R'; } unsigned long pad[6]; };
struct Mesh2 { int x; }; struct AsyncData { int y; };
class C3DMeshLODInfo : public Resource { public:
    CountedPointer<Mesh2> Mesh;
    CountedPointer<AsyncData> FileData;
    void* MeshInfo; unsigned char LOD; bool UsedMeshStats;
    ~C3DMeshLODInfo() { }
};
static Info* MakeInfo(void (*f)(void*), unsigned long n) { Info* i = new Info; i->RefCount = n; i->DeleteFunc = f; i->Data = 0; return i; }
int main()
{
    Mesh2 m; AsyncData d;
    C3DMeshLODInfo* p = new C3DMeshLODInfo;
    p->Mesh.Data = &m; p->Mesh.Info_ = MakeInfo(DeleteMesh, 1);
    p->FileData.Data = &d; p->FileData.Info_ = MakeInfo(DeleteFile, 1);
    delete p;
    if (g_n != 3 || g_log[0] != 'F' || g_log[1] != 'M' || g_log[2] != 'R') { std::printf("00a24470_TEST FAIL\n"); return 1; }
    Info* shared = MakeInfo(DeleteMesh, 2);
    p = new C3DMeshLODInfo; p->Mesh.Data = &m; p->Mesh.Info_ = shared; p->FileData.Data = 0; p->FileData.Info_ = 0;
    delete p;
    if (g_n != 4 || g_log[3] != 'R' || shared->RefCount != 1) { std::printf("00a24470_TEST FAIL\n"); return 1; }
    delete shared;
    std::printf("00a24470_TEST PASS\n");
    return 0;
}