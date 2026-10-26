// Behaviour test for 0x00A24930 C3DMeshInfo::~C3DMeshInfo
#include <cstdio>
static int g_destroyed = 0;
struct CCountedObject {
    long m_refCount;
    CCountedObject() : m_refCount(1) {}
    virtual ~CCountedObject() {}
    virtual void Destroy() { ++g_destroyed; }
    void Release() { if (--m_refCount == 0) Destroy(); }
};
template <class T> class CIVCountedPointer { public:
    T* Object;
    CIVCountedPointer() : Object(0) {}
    ~CIVCountedPointer() { Release(); }
    void Release() { if (Object) { Object->Release(); Object = 0; } }
    CIVCountedPointer& operator=(const CIVCountedPointer& o)
    { Release(); Object = o.Object; if (Object) ++Object->m_refCount; return *this; }
};
class C3DMeshLODInfo : public CCountedObject { public:};
struct CMeshData : CCountedObject { CIVCountedPointer<C3DMeshLODInfo> LODMeshes[4]; };
class C3DMeshInfo { public:
    CIVCountedPointer<CMeshData> MeshData;
    ~C3DMeshInfo()
    {
        if (MeshData.Object)
            for (unsigned long i = 0; i < 4; ++i)
                if (MeshData.Object->LODMeshes[i].Object)
                    MeshData.Object->LODMeshes[i] = CIVCountedPointer<C3DMeshLODInfo>();
    }
};
int main()
{
    C3DMeshLODInfo lod0, lod2; lod2.m_refCount = 2;
    CMeshData* data = new CMeshData; data->m_refCount = 2;     // shared: survives this info
    data->LODMeshes[0].Object = &lod0; data->LODMeshes[2].Object = &lod2;
    {
        C3DMeshInfo info; info.MeshData.Object = data;
    }
    bool ok = g_destroyed == 1 && lod0.m_refCount == 0 && lod2.m_refCount == 1 && data->m_refCount == 1
        && data->LODMeshes[0].Object == 0 && data->LODMeshes[2].Object == 0;
    { C3DMeshInfo empty; }
    ok = ok && g_destroyed == 1;
    if (!ok) { std::printf("00a24930_TEST FAIL\n"); return 1; }
    std::printf("00a24930_TEST PASS\n");
    return 0;
}