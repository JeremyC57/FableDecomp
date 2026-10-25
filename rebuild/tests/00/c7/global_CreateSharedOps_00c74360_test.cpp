// Behaviour test for 00c74360: CreateSharedOps -- picks the SIMD or x87 table by the CPU flag, refcount 1 on success
#include <cstdio>
#include <cstdlib>
static bool g_bUseSimd = false; static int g_allocs = 0; static bool g_fail = false;
class CDspOps {
public:
    CDspOps() { }
    static void* operator new(unsigned int size) { ++g_allocs; return g_fail ? 0 : std::malloc(size); }
    static void operator delete(void* p) { std::free(p); }
    virtual long GetKind() { return 1; }
};
class CDspOpsSimd : public CDspOps { public: CDspOpsSimd() { } virtual long GetKind() { return 2; } };
class CDspObject { public: static CDspOps* s_pOps; static long s_refCount; };
CDspOps* CDspObject::s_pOps = 0; long CDspObject::s_refCount = 0;
static bool CreateSharedOps()
{
    CDspOps* ops;
    if (g_bUseSimd) ops = new CDspOpsSimd; else ops = new CDspOps;
    CDspObject::s_pOps = ops;
    if (!ops) return false;
    CDspObject::s_refCount = 1;
    return true;
}
int main()
{
    int ok = CreateSharedOps() && CDspObject::s_pOps->GetKind() == 1 && CDspObject::s_refCount == 1;
    delete CDspObject::s_pOps; CDspObject::s_refCount = 7;
    g_bUseSimd = true; ok = ok && CreateSharedOps() && CDspObject::s_pOps->GetKind() == 2 && CDspObject::s_refCount == 1;
    delete CDspObject::s_pOps; CDspObject::s_refCount = 7;
    g_fail = true; ok = ok && !CreateSharedOps() && CDspObject::s_pOps == 0 && CDspObject::s_refCount == 7 && g_allocs == 3;
    std::printf(ok ? "00c74360_TEST PASS\n" : "00c74360_TEST FAIL\n");
    return ok ? 0 : 1;
}
