// Behaviour test for CCountedPointer<T>::DeleteData (0x00696560): null is ignored; a record is destroyed (shared member released) and freed.
#include <cstdio>
static int g_released = 0; static int g_freed = 0;
struct CIVCountedPointeeBase { virtual ~CIVCountedPointeeBase() {} virtual void DeleteThis() { ++g_released; } unsigned long IVRefCount; };
template<class T> class CIVCountedPointer { public: T* Object; ~CIVCountedPointer() { T* o = Object; if (o) { if (--o->IVRefCount == 0) o->DeleteThis(); Object = 0; } } };
struct CRecord { long a, b, c; CIVCountedPointer<CIVCountedPointeeBase> m_pShared; ~CRecord() { ++g_freed; } };
template<class T> struct CCountedPointer { static void DeleteData(void* data) { delete static_cast<T*>(data); } };
int main()
{
    CCountedPointer<CRecord>::DeleteData(0);
    if (g_freed != 0) { std::printf("00696560_TEST FAIL\n"); return 1; }
    CIVCountedPointeeBase shared; shared.IVRefCount = 2;
    CRecord* r = new CRecord; r->m_pShared.Object = &shared;
    CCountedPointer<CRecord>::DeleteData(r);
    if (g_freed != 1 || shared.IVRefCount != 1 || g_released != 0) { std::printf("00696560_TEST FAIL\n"); return 1; }
    r = new CRecord; r->m_pShared.Object = &shared;
    CCountedPointer<CRecord>::DeleteData(r);
    if (g_freed != 2 || shared.IVRefCount != 0 || g_released != 1) { std::printf("00696560_TEST FAIL\n"); return 1; }
    std::printf("00696560_TEST PASS\n");
    return 0;
}