// Behaviour test for 0x004ae450 (deleting destructor)
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
static int g_vectorDtor = 0;
static int g_order = 0, g_vectorAt = 0, g_releaseAt = 0;
struct COrderedObject : CCountedObject {
    COrderedObject(long c) : CCountedObject(c) {}
    virtual void Destroy() { ++g_destroyed; g_releaseAt = ++g_order; }
};
struct CRecordVector { ~CRecordVector() { ++g_vectorDtor; g_vectorAt = ++g_order; } };
struct CRecordSetOwner004ae450 {
    long                              m_words[6];
    CIVCountedPointer<COrderedObject> m_object;
    CRecordVector                     m_records;
};
int main()
{
    COrderedObject obj(1);
    CRecordSetOwner004ae450* a = new CRecordSetOwner004ae450; a->m_object.m_p = &obj;
    delete a;
    bool ok = g_vectorDtor == 1 && g_destroyed == 1 && g_vectorAt < g_releaseAt;   // the vector dies first
    if (!ok) { std::printf("004ae450_TEST FAIL\n"); return 1; }
    std::printf("004ae450_TEST PASS\n");
    return 0;
}