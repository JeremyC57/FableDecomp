// Behaviour test for 0x004AE3E0: the record vector at +0x1c is destroyed first, then the counted component at +0x18 released.
#include <cstdio>
static int g_log[8]; static int g_n = 0; static int g_deleted = 0;
class CIVCountedPointeeBase { public: virtual ~CIVCountedPointeeBase() {} virtual void DeleteThis() { ++g_deleted; g_log[g_n++] = 2; } unsigned long IVRefCount; };
class CAnimComponentBase : public CIVCountedPointeeBase { public: };
template<class T> class CIVCountedPointer {
public:
    T* Object;
    ~CIVCountedPointer() { T* object = Object; if (object) { if (--object->IVRefCount == 0) object->DeleteThis(); Object = 0; } }
};
struct CWideStringRecordVector { void* m_pStart; void* m_pFinish; void* m_pEndOfStorage; ~CWideStringRecordVector() { g_log[g_n++] = 1; } };
struct CHolder { unsigned char _pad00[0x18]; CIVCountedPointer<CAnimComponentBase> m_pComponent; CWideStringRecordVector m_records; ~CHolder() {} };
int main()
{
    CAnimComponentBase c; c.IVRefCount = 1;
    { CHolder h; h.m_pComponent.Object = &c; }
    if (g_n != 2 || g_log[0] != 1 || g_log[1] != 2 || c.IVRefCount != 0 || g_deleted != 1) { std::printf("004ae3e0_TEST FAIL\n"); return 1; }
    g_n = 0;
    { CHolder h; h.m_pComponent.Object = 0; }
    if (g_n != 1 || g_deleted != 1) { std::printf("004ae3e0_TEST FAIL\n"); return 1; }
    std::printf("004ae3e0_TEST PASS\n");
    return 0;
}