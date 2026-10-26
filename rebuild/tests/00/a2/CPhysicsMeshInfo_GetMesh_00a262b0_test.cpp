// Behaviour test for CPhysicsMeshInfo::GetMesh (0x00A262B0): load/build on demand, touch the resource, return an add-ref'd mesh pointer.
#include <cstdio>
#include <cstring>
static char g_log[256];
static void LOG(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
class CCharString { public: const char* Text; CCharString(const char* t, long = -1) : Text(t) {} ~CCharString() {} };
void DisplayProgress(const CCharString& text, bool flag, float fraction, bool flag2) { LOG(std::strcmp(text.Text, "Asset Wait: Physics Mesh") == 0 && !flag && fraction == -1.0f && !flag2 ? "progress" : "progress?"); }
class CResourceList;
class CResource { public:
    CResourceList* ResourceList; unsigned long LastUsedFrame; CResource* PrevResource;
    void ListRemove() { LOG("remove"); }
    void ListInsert(CResource*, CResource*) { LOG("insert"); }
    void Touch();
};
class CResourceList { public: CResource Head; unsigned long CurrentFrame; };
inline void CResource::Touch()
{
    if (ResourceList && LastUsedFrame != ResourceList->CurrentFrame) {
        ListRemove();
        ListInsert(ResourceList->Head.PrevResource, &ResourceList->Head);
        LastUsedFrame = ResourceList->CurrentFrame;
    }
}
class CBankFileAsyncData { public: bool done; bool IsFinished() { return done; } bool WaitUntilFinished() { LOG("wait"); done = true; return true; } };
template <class T> class CCountedPointer { public: T* Data; T* operator->() const { return Data; } };
struct CIVCountedObject { mutable long RefCount; };
template <class T> class CIVCountedPointer { public:
    T* Object;
    CIVCountedPointer() : Object(0) {}
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object) { if (Object) ++Object->RefCount; }
};
class CPhysicsMesh : public CIVCountedObject { public:};
static CPhysicsMesh g_mesh;
static CBankFileAsyncData g_file;
struct CMeshData : CResource {
    CCountedPointer<CBankFileAsyncData> FileData; CIVCountedPointer<const CPhysicsMesh> Mesh;
    void BuildMesh() { LOG("build"); Mesh.Object = &g_mesh; }
};
static CMeshData g_data;
class CPhysicsMeshInfo { public:
    CMeshData* MeshData;
    void BeginLoading() { LOG("begin"); MeshData = &g_data; g_data.FileData.Data = &g_file; }
    CIVCountedPointer<const CPhysicsMesh> GetMesh()
    {
        if (MeshData == 0 || MeshData->Mesh.Object == 0) {
            if (MeshData == 0 || MeshData->FileData.Data == 0)
                BeginLoading();
            if (!MeshData->FileData->IsFinished()) {
                DisplayProgress("Asset Wait: Physics Mesh", false, -1.0f, false);
                MeshData->FileData->WaitUntilFinished();
            }
            MeshData->BuildMesh();
        }
        MeshData->Touch();
        return MeshData->Mesh;
    }
};
int main()
{
    CResourceList list; list.CurrentFrame = 5; list.Head.PrevResource = 0;
    g_data.ResourceList = &list; g_data.LastUsedFrame = 1; g_data.FileData.Data = 0; g_data.Mesh.Object = 0; g_file.done = false; g_mesh.RefCount = 1;
    CPhysicsMeshInfo info; info.MeshData = 0;
    g_log[0] = 0;
    {
        CIVCountedPointer<const CPhysicsMesh> m = info.GetMesh();
        if (m.Object != &g_mesh || g_mesh.RefCount != 2 || g_data.LastUsedFrame != 5) { std::printf("00a262b0_TEST FAIL\n"); return 1; }
    }
    if (std::strcmp(g_log, "begin;progress;wait;build;remove;insert;") != 0) { std::printf("00a262b0_TEST FAIL\n"); return 1; }
    g_log[0] = 0;
    CIVCountedPointer<const CPhysicsMesh> m2 = info.GetMesh();   /* built and touched this frame: nothing to do */
    if (g_log[0] != 0 || m2.Object != &g_mesh || g_mesh.RefCount != 3) { std::printf("00a262b0_TEST FAIL\n"); return 1; }
    std::printf("00a262b0_TEST PASS\n");
    return 0;
}