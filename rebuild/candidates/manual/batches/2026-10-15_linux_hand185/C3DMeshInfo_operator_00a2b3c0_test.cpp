#include <cstdio>
static int g_deleted = 0;
struct CPointee {
    unsigned long IVRefCount;
    virtual ~CPointee() {}
    virtual void DeleteThis() { ++g_deleted; }
    void AddRef() { ++IVRefCount; }
    void Release() { if (--IVRefCount == 0) DeleteThis(); }
};
struct CIVPtr {
    CPointee* Object;
    CIVPtr& operator=(const CIVPtr& o)
    {
        if (Object != o.Object) {
            if (Object) { Object->Release(); Object = 0; }
            Object = o.Object;
            if (Object) Object->AddRef();
        }
        return *this;
    }
};
struct C3DVector { float X, Y, Z; };
struct CBox { C3DVector MinCoord, MaxCoord; };
struct CSphere { C3DVector Center; float Radius; };
struct C3DMeshInfo {
    long m_f4, m_f8; CIVPtr MeshData; CBox BoundingBox; CSphere BoundingSphere;
    float SafeBoundingRadius; long PhysicsMeshIndex; float LODErrors[4]; unsigned long LODDataSizes[4];
    C3DMeshInfo& operator=(const C3DMeshInfo& o)
    {
        m_f4 = o.m_f4; m_f8 = o.m_f8; MeshData = o.MeshData;
        BoundingBox = o.BoundingBox; BoundingSphere = o.BoundingSphere;
        SafeBoundingRadius = o.SafeBoundingRadius; PhysicsMeshIndex = o.PhysicsMeshIndex;
        for (int i = 0; i < 4; ++i) { LODErrors[i] = o.LODErrors[i]; LODDataSizes[i] = o.LODDataSizes[i]; }
        return *this;
    }
};
int main()
{
    CPointee p1, p2; p1.IVRefCount = 1; p2.IVRefCount = 1;
    C3DMeshInfo a = C3DMeshInfo(), b = C3DMeshInfo();
    a.MeshData.Object = &p1; b.MeshData.Object = &p2;
    b.m_f4 = 3; b.m_f8 = 4; b.BoundingBox.MaxCoord.Z = 5; b.BoundingSphere.Radius = 6;
    b.SafeBoundingRadius = 7; b.PhysicsMeshIndex = 8; b.LODErrors[3] = 9; b.LODDataSizes[2] = 10;
    C3DMeshInfo* r = &(a = b);
    if (r != &a || g_deleted != 1 || p1.IVRefCount != 0 || p2.IVRefCount != 2 || a.MeshData.Object != &p2 ||
        a.m_f4 != 3 || a.m_f8 != 4 || a.BoundingBox.MaxCoord.Z != 5 || a.BoundingSphere.Radius != 6 ||
        a.SafeBoundingRadius != 7 || a.PhysicsMeshIndex != 8 || a.LODErrors[3] != 9 || a.LODDataSizes[2] != 10)
    { std::printf("00a2b3c0_TEST FAIL\n"); return 1; }
    a = b;   // same pointee: no release
    if (p2.IVRefCount != 2 || g_deleted != 1) { std::printf("00a2b3c0_TEST FAIL\n"); return 1; }
    std::printf("00a2b3c0_TEST PASS\n");
    return 0;
}
