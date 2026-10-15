#include <cstdio>
#include <cstring>
#include <new>
struct C3DMeshInfoBase {
    virtual void v00() { }
    long m_4, m_8;
    C3DMeshInfoBase(long a4, long a8) : m_4(a4), m_8(a8) { }
};
template <class T> struct CIVCountedPointer { T* m_pObject; CIVCountedPointer() : m_pObject(0) { } };
struct C3DVector { float x, y, z; };
struct C3DBoundingBox { C3DVector MinCoord, MaxCoord; };
struct C3DBoundingSphere {
    long m_x, m_y, m_z;
    union { long m_radiusBits; float Radius; };
    C3DBoundingSphere() : m_x(0), m_y(0), m_z(0), m_radiusBits(0) { }
};
struct C3DMeshInfo : C3DMeshInfoBase {
    struct CMeshData;
    CIVCountedPointer<CMeshData> MeshData;
    C3DBoundingBox BoundingBox;
    C3DBoundingSphere BoundingSphere;
    float SafeBoundingRadius;
    long PhysicsMeshIndex;
    float LODErrors[4];
    unsigned long LODDataSizes[4];
    virtual void v00() { }
    C3DMeshInfo(void* unused, long a, long b) : C3DMeshInfoBase(b, a)
    {
        BoundingSphere.Radius = -1.0f;
        PhysicsMeshIndex = -1;
    }
};
int main()
{
    static unsigned char buf[sizeof(C3DMeshInfo)];
    std::memset(buf, 0xCD, sizeof(buf));
    C3DMeshInfo* m = new (buf) C3DMeshInfo(0, 0x22, 0x33);
    bool ok = m->m_4 == 0x33 && m->m_8 == 0x22 && m->MeshData.m_pObject == 0
        && m->BoundingSphere.m_x == 0 && m->BoundingSphere.m_y == 0 && m->BoundingSphere.m_z == 0
        && m->BoundingSphere.Radius == -1.0f && m->PhysicsMeshIndex == -1;
    if (!ok) { std::printf("00a248f0_TEST FAIL\n"); return 1; }
    std::printf("00a248f0_TEST PASS\n");
    return 0;
}