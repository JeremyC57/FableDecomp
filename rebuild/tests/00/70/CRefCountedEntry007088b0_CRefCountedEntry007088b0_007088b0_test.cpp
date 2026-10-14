#include <cstdio>
static int g_deletes = 0;
struct CIVCountedPointee { virtual void v00() {} virtual void Delete() { ++g_deletes; } long IVRefCount; };
struct CIVCountedPointer { CIVCountedPointee* m_pObject; explicit CIVCountedPointer(CIVCountedPointee* p) : m_pObject(p) { if (m_pObject) ++m_pObject->IVRefCount; }
    CIVCountedPointer(const CIVCountedPointer& o) : m_pObject(o.m_pObject) { if (m_pObject) ++m_pObject->IVRefCount; }
    ~CIVCountedPointer() { if (m_pObject) { if (--m_pObject->IVRefCount == 0) m_pObject->Delete(); } } };
struct CRefCountedEntry { CIVCountedPointer m_pObject; long m_first; long m_second; CRefCountedEntry(long first, long second, CIVCountedPointer object) : m_pObject(object), m_first(first), m_second(second) {} };

int main()
{
    CIVCountedPointee obj; obj.IVRefCount = 0;
    {
        CIVCountedPointer p(&obj);
        CRefCountedEntry e(11, 22, p);
        if (e.m_pObject.m_pObject != &obj || e.m_first != 11 || e.m_second != 22 || obj.IVRefCount != 2 || sizeof(e) != 12) { std::printf("007088b0_TEST FAIL\n"); return 1; }
    }
    if (obj.IVRefCount != 0 || g_deletes != 1) { std::printf("007088b0_TEST FAIL\n"); return 1; }
    CRefCountedEntry n(1, 2, CIVCountedPointer(0)); if (n.m_pObject.m_pObject != 0 || n.m_first != 1 || n.m_second != 2) { std::printf("007088b0_TEST FAIL\n"); return 1; }
    std::printf("007088b0_TEST PASS\n");
    return 0;
}