#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#define __int64 long long
#endif
typedef int LONG32;   // the retail 4-byte long (the g++ host is LP64)
#define TEST_ADDR "00a24fc0"
#define CHECK(c) do { if (!(c)) { std::printf("%s_TEST FAIL (line %d)\n", TEST_ADDR, __LINE__); return 1; } } while (0)

static int g_destroyed = 0;
struct CIVCountedPointeeBase {
    virtual ~CIVCountedPointeeBase() { }
    virtual void Destroy() { ++g_destroyed; }
    LONG32 RefCount;
};
template <class T> struct CIVCountedPointer {
    T* Data;
    LONG32 GetRefCount() const { return Data ? Data->RefCount : 0; }
    void Release()
    {
        T* data = Data;
        if (data) {
            if (--data->RefCount == 0)
                data->Destroy();
            Data = 0;
        }
        Data = 0;
    }
};
struct CPhysicsMeshInfo { unsigned char pad[12]; CIVCountedPointer<CIVCountedPointeeBase> MeshData; };
struct CMeshData {
    CIVCountedPointer<CIVCountedPointeeBase> m_resource;
    CPhysicsMeshInfo* m_pOwner;
    bool UnloadResource()
    {
        if (m_resource.GetRefCount() == 1) {
            m_pOwner->MeshData.Release();
            return true;
        }
        return false;
    }
};
int main()
{
    CIVCountedPointeeBase res, mesh;
    CPhysicsMeshInfo owner; owner.MeshData.Data = &mesh; mesh.RefCount = 1;
    CMeshData d; d.m_pOwner = &owner; d.m_resource.Data = 0;
    CHECK(!d.UnloadResource() && owner.MeshData.Data == &mesh);          // no resource reference
    d.m_resource.Data = &res; res.RefCount = 2;
    CHECK(!d.UnloadResource() && owner.MeshData.Data == &mesh);          // still shared
    res.RefCount = 1;
    CHECK(d.UnloadResource() && owner.MeshData.Data == 0 && g_destroyed == 1 && mesh.RefCount == 0);
    CHECK(d.UnloadResource() && g_destroyed == 1);                        // already released: just clears
    std::printf("00a24fc0_TEST PASS\n");
    return 0;
}