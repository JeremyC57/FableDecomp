// Behaviour test for 0x00696060 (deleting destructor)
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
struct CCountedPointerHolder00696060 {
    long m_word00, m_word04, m_word08;
    CIVCountedPointer<CCountedObject> m_object;
};
int main()
{
    CCountedObject obj(3);
    CCountedPointerHolder00696060* a = new CCountedPointerHolder00696060; a->m_object.m_p = &obj;
    delete a;
    bool ok = obj.m_refCount == 2 && g_destroyed == 0;
    CCountedPointerHolder00696060 local; local.m_object.m_p = &obj; obj.m_refCount = 1;
    local.~CCountedPointerHolder00696060();
    if (obj.m_refCount != 0 || g_destroyed != 1 || local.m_object.m_p != 0) ok = false;
    local.m_object.m_p = 0;
    if (!ok) { std::printf("00696060_TEST FAIL\n"); return 1; }
    std::printf("00696060_TEST PASS\n");
    return 0;
}