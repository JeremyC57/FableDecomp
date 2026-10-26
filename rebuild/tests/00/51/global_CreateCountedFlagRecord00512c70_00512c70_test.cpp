// Behaviour test for 0x00512c70 (counted-pointer factory of a 5-byte flag record)
#include <cstdio>
static int g_deleted = 0;
struct SFlagRecord { unsigned char m_data[4]; bool m_enabled; SFlagRecord() : m_enabled(true) {} };
static void DeleteFlagRecord(void* r) { delete static_cast<SFlagRecord*>(r); ++g_deleted; }
struct SCountBlock {
    long m_references; void (*m_pfnDestroy)(void*); void* m_pObject;
    SCountBlock(void* o, void (*d)(void*)) : m_references(1), m_pfnDestroy(d), m_pObject(o) {}
};
template <class T> class CCountedPointer { public:
    T* m_pObject; SCountBlock* m_pCount;
    CCountedPointer(T* o, void (*d)(void*)) : m_pObject(o) { if (o) m_pCount = new SCountBlock(o, d); else m_pCount = 0; }
    CCountedPointer(const CCountedPointer& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount) { if (m_pCount) ++m_pCount->m_references; }
    ~CCountedPointer() { if (m_pCount) { if (--m_pCount->m_references == 0) { m_pCount->m_pfnDestroy(m_pCount->m_pObject); delete m_pCount; } } }
};
static CCountedPointer<SFlagRecord> Create()
{
    CCountedPointer<SFlagRecord> record(new SFlagRecord, DeleteFlagRecord);
    return record;
}
int main()
{
    bool ok;
    {
        CCountedPointer<SFlagRecord> p = Create();
        ok = p.m_pObject && p.m_pObject->m_enabled && p.m_pCount && p.m_pCount->m_references == 1 && p.m_pCount->m_pObject == p.m_pObject && g_deleted == 0;
    }
    ok = ok && g_deleted == 1;
    if (!ok) { std::printf("00512c70_TEST FAIL\n"); return 1; }
    std::printf("00512c70_TEST PASS\n");
    return 0;
}