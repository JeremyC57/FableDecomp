// Behaviour test for 0x007550C0: the holder's destructor releases the counted component at +0x3c (delete at zero) and clears it.
#include <cstdio>
static int g_deleted = 0;
struct CIVCountedPointeeBase { virtual ~CIVCountedPointeeBase() {} virtual void DeleteThis() { ++g_deleted; } unsigned long IVRefCount; };
struct CAnimComponentBase : CIVCountedPointeeBase { };
template<class T> class CIVCountedPointer {
public:
    T* Object;
    ~CIVCountedPointer() { T* object = Object; if (object) { if (--object->IVRefCount == 0) object->DeleteThis(); Object = 0; } }
};
struct CHolder { unsigned char _pad00[0x3c]; CIVCountedPointer<CAnimComponentBase> m_pComponent; ~CHolder() {} };
int main()
{
    CAnimComponentBase c; c.IVRefCount = 2;
    { CHolder h; h.m_pComponent.Object = &c; }
    if (c.IVRefCount != 1 || g_deleted) { std::printf("007550c0_TEST FAIL\n"); return 1; }
    { CHolder h; h.m_pComponent.Object = &c; }
    if (c.IVRefCount != 0 || g_deleted != 1) { std::printf("007550c0_TEST FAIL\n"); return 1; }
    { CHolder h; h.m_pComponent.Object = 0; }
    if (g_deleted != 1) { std::printf("007550c0_TEST FAIL\n"); return 1; }
    std::printf("007550c0_TEST PASS\n");
    return 0;
}