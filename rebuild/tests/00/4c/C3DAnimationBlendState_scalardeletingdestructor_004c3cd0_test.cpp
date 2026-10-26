// Behaviour test for 0x004c3cd0 (deleting destructor)
#include <cstdio>
static int g_destroyed = 0;
static int g_baseDtor = 0;
struct CCountedObject {
    long m_refCount;
    CCountedObject(long c) : m_refCount(c) {}
    virtual void Destroy() { ++g_destroyed; }
};
template <class T> class CIVCountedPointer { public:
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
class CBaseClass { public: virtual ~CBaseClass() { ++g_baseDtor; } };
class C3DAnimationBlendState { public:
    CBaseClass                        m_base;
    CIVCountedPointer<CCountedObject> Data;
};
int main()
{
    CCountedObject shared(2);
    C3DAnimationBlendState* a = new C3DAnimationBlendState; a->Data.m_p = &shared;
    C3DAnimationBlendState* b = new C3DAnimationBlendState; b->Data.m_p = &shared;
    C3DAnimationBlendState* c = new C3DAnimationBlendState;
    delete a;
    bool ok = shared.m_refCount == 1 && g_destroyed == 0 && g_baseDtor == 1;
    delete b;
    if (shared.m_refCount != 0 || g_destroyed != 1 || g_baseDtor != 2) ok = false;
    delete c;                                       // null pointer: nothing released
    if (g_destroyed != 1 || g_baseDtor != 3) ok = false;
    if (!ok) { std::printf("004c3cd0_TEST FAIL\n"); return 1; }
    std::printf("004c3cd0_TEST PASS\n");
    return 0;
}