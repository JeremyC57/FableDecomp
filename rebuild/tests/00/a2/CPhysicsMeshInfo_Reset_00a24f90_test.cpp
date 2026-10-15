#include <cstdio>
static int g_destroyed = 0;
static int g_entryDestroyed = 0;
struct CMeshData {
    long m_refCount;
    virtual void v00() { }
    virtual void DeleteThis() { ++g_destroyed; }
};
template <class T> struct CIVCountedPointer {
    T* m_pObject;
    CIVCountedPointer() : m_pObject(0) { }
    void Release()
    {
        T* object = m_pObject;
        if (object) {
            if (--object->m_refCount == 0)
                object->DeleteThis();
            m_pObject = 0;
        }
    }
    CIVCountedPointer& operator=(T* object) { Release(); m_pObject = object; return *this; }
    ~CIVCountedPointer() { Release(); }
};
struct CMeshDataBankEntry {
    void* Bank; unsigned long Index;
    CMeshDataBankEntry() : Bank(0), Index(0) { }
    virtual ~CMeshDataBankEntry() { ++g_entryDestroyed; }
};
struct CPhysicsMeshInfo : CMeshDataBankEntry {
    CIVCountedPointer<CMeshData> MeshData;
    virtual ~CPhysicsMeshInfo() { }
    void Reset() { MeshData = 0; }
};
int main()
{
    CMeshData shared; shared.m_refCount = 2;
    {
        CPhysicsMeshInfo* a = new CPhysicsMeshInfo; a->MeshData.m_pObject = &shared;
        CPhysicsMeshInfo* b = new CPhysicsMeshInfo; b->MeshData.m_pObject = &shared;
        delete a;
        if (g_destroyed != 0 || shared.m_refCount != 1 || g_entryDestroyed != 1) { std::printf("00a24f90_TEST FAIL\n"); return 1; }
        b->Reset();
        if (g_destroyed != 1 || b->MeshData.m_pObject != 0) { std::printf("00a24f90_TEST FAIL\n"); return 1; }
        b->Reset();
        if (g_destroyed != 1) { std::printf("00a24f90_TEST FAIL\n"); return 1; }
        delete b;
        if (g_destroyed != 1 || g_entryDestroyed != 2) { std::printf("00a24f90_TEST FAIL\n"); return 1; }
    }
    std::printf("00a24f90_TEST PASS\n");
    return 0;
}