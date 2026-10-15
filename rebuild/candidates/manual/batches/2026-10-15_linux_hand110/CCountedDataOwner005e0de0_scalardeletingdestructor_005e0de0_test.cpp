// Behaviour test for 0x005e0de0 (deleting destructor)
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
struct CInstanceCountedBase { virtual ~CInstanceCountedBase() { ++g_baseDtor; } };
struct CCountedDataOwner005e0de0 {
    CInstanceCountedBase              m_base;
    CIVCountedPointer<CCountedObject> m_data;
};
int main()
{
    CCountedObject obj(1);
    CCountedDataOwner005e0de0* a = new CCountedDataOwner005e0de0; a->m_data.m_p = &obj;
    delete a;
    bool ok = obj.m_refCount == 0 && g_destroyed == 1 && g_baseDtor == 1;
    if (!ok) { std::printf("005e0de0_TEST FAIL\n"); return 1; }
    std::printf("005e0de0_TEST PASS\n");
    return 0;
}
