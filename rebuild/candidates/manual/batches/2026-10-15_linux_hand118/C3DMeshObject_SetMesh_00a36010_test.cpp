// Behaviour test for 0x00A36010 C3DMeshObject::SetMesh
#include <cstdio>
#include <cstdlib>
struct CCPPointerInfo { long RefCount; void (*DeleteFunc)(void*); void* Data; };
static int g_deleted = 0;
static void DeleteData(void*) { ++g_deleted; }
template<class T> struct CCountedPointer {
    T* Data; CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer()
    {
        if (Info != 0) { --Info->RefCount; if (Info->RefCount == 0) { Info->DeleteFunc(Info->Data); std::free(Info); } }
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        T* data = other.Data; CCPPointerInfo* info = other.Info;
        if (Info != info) {
            if (Info != 0) { --Info->RefCount; if (Info->RefCount == 0) { Info->DeleteFunc(Info->Data); std::free(Info); } }
            Data = data; Info = info;
            if (info != 0) ++info->RefCount;
        }
        return *this;
    }
};
struct C3DMeshInfo {
    CCountedPointer<int> Stats;
    CCountedPointer<int> GetMeshStats() { return Stats; }
};
struct C3DMeshObject {
    C3DMeshInfo* MeshInfo; CCountedPointer<int> Mesh;
    void SetMesh(C3DMeshInfo* meshInfo) { MeshInfo = meshInfo; Mesh = meshInfo->GetMeshStats(); }
};
int main()
{
    int a = 1, b = 2;
    CCPPointerInfo* ia = (CCPPointerInfo*)std::malloc(sizeof(CCPPointerInfo)); ia->RefCount = 1; ia->DeleteFunc = DeleteData; ia->Data = &a;
    CCPPointerInfo* ib = (CCPPointerInfo*)std::malloc(sizeof(CCPPointerInfo)); ib->RefCount = 1; ib->DeleteFunc = DeleteData; ib->Data = &b;
    C3DMeshInfo info; info.Stats.Data = &b; info.Stats.Info = ib;
    C3DMeshObject o; o.MeshInfo = 0; o.Mesh.Data = &a; o.Mesh.Info = ia;
    o.SetMesh(&info);
    bool ok = o.MeshInfo == &info && o.Mesh.Data == &b && o.Mesh.Info == ib && ib->RefCount == 2 && g_deleted == 1;
    o.SetMesh(&info);
    ok = ok && ib->RefCount == 2 && g_deleted == 1;
    if (!ok) { std::printf("00a36010_TEST FAIL\n"); return 1; }
    std::printf("00a36010_TEST PASS\n");
    return 0;
}
