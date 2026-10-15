// Behaviour test for 0x00724de0 (deleting destructor)
#include <cstdio>
static int g_destroyed = 0;
static int g_baseDtor = 0;
struct CCountedObject {
    long m_refCount;
    CCountedObject(long c) : m_refCount(c) {}
    virtual void Destroy() { ++g_destroyed; }
};
template <class T> struct CIVCountedPointer {
    T* m_p;
    CIVCountedPointer() : m_p(0) {}
    ~CIVCountedPointer()
    {
        T* p = m_p;
        if (p) {
            if (--p->m_refCount == 0)
                p->Destroy();
            m_p = 0;
        }
    }
};
static int g_meshFreed = 0;
struct CThingPhysicsMeshInfo { long m_data; };
struct CTCBase { virtual ~CTCBase() { ++g_baseDtor; } };
struct CTCPhysicsBase : CTCBase {
    float Radius;
    CIVCountedPointer<CCountedObject> PhysicsDef;
};
struct CTCPhysicsStandard : CTCPhysicsBase {
    CThingPhysicsMeshInfo* PPhysicsMeshInfo;
    virtual ~CTCPhysicsStandard()
    {
        if (PPhysicsMeshInfo) {
            delete PPhysicsMeshInfo;
            ++g_meshFreed;
        }
    }
};
int main()
{
    CCountedObject def(1);
    CTCPhysicsStandard* a = new CTCPhysicsStandard; a->PhysicsDef.m_p = &def; a->PPhysicsMeshInfo = new CThingPhysicsMeshInfo;
    CTCPhysicsStandard* b = new CTCPhysicsStandard; b->PPhysicsMeshInfo = 0;
    CTCBase* pa = a; delete pa;
    bool ok = g_meshFreed == 1 && g_destroyed == 1 && g_baseDtor == 1;
    CTCBase* pb = b; delete pb;
    if (g_meshFreed != 1 || g_destroyed != 1 || g_baseDtor != 2) ok = false;
    if (!ok) { std::printf("00724de0_TEST FAIL\n"); return 1; }
    std::printf("00724de0_TEST PASS\n");
    return 0;
}
