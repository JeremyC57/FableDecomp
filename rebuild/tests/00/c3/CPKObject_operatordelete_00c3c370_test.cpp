// Behaviour test for 00c3c370: CPKObject::operator new / delete -- class allocations go through the host's hooks
#include <cstdio>
#include <cstdlib>
static unsigned int g_size = 0; static void* g_freed = 0; static int g_allocs = 0, g_frees = 0;
static void* __stdcall HookAlloc(unsigned int size) { ++g_allocs; g_size = size; return std::malloc(size); }
static void __stdcall HookFree(void* p) { ++g_frees; g_freed = p; std::free(p); }
typedef void* (__stdcall* TPKAllocHook)(unsigned int size);
typedef void  (__stdcall* TPKFreeHook)(void* p);
static TPKAllocHook g_pfnPKAlloc = HookAlloc;
static TPKFreeHook  g_pfnPKFree = HookFree;
class CPKObject {
public:
    virtual ~CPKObject() { }
    static void* operator new(std::size_t size) { return g_pfnPKAlloc((unsigned int)size); }
    static void operator delete(void* p) { g_pfnPKFree(p); }
};
class CNode : public CPKObject { public: int m_a, m_b, m_c; };
int main()
{
    CPKObject* p = new CNode;
    int ok = g_allocs == 1 && g_size == sizeof(CNode);
    delete p;
    ok = ok && g_frees == 1 && g_freed == (void*)p;
    std::printf(ok ? "00c3c370_TEST PASS\n" : "00c3c370_TEST FAIL\n");
    return ok ? 0 : 1;
}
