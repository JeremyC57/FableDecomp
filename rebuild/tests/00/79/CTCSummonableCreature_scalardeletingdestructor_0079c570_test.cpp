// Behaviour test for 0x0079c570 (deleting destructor)
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
static int g_ipDtor = 0;
struct CIntelligentPointer { void* PItem; CIntelligentPointer() : PItem(0) {} ~CIntelligentPointer() { ++g_ipDtor; } };
struct CTCBase { virtual ~CTCBase() { ++g_baseDtor; } void* ParentThing; };
struct CTCSummonableCreature : CTCBase {
    CIVCountedPointer<CCountedObject> PDef;
    CIntelligentPointer               Summoner;
    virtual ~CTCSummonableCreature() {}
};
int main()
{
    CCountedObject def(2);
    CTCSummonableCreature* a = new CTCSummonableCreature; a->PDef.m_p = &def;
    CTCBase* p = a; delete p;
    bool ok = def.m_refCount == 1 && g_destroyed == 0 && g_ipDtor == 1 && g_baseDtor == 1;
    CTCSummonableCreature* b = new CTCSummonableCreature; b->PDef.m_p = &def;
    p = b; delete p;
    if (def.m_refCount != 0 || g_destroyed != 1 || g_ipDtor != 2 || g_baseDtor != 2) ok = false;
    if (!ok) { std::printf("0079c570_TEST FAIL\n"); return 1; }
    std::printf("0079c570_TEST PASS\n");
    return 0;
}