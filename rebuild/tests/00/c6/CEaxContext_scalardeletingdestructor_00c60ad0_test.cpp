// Behaviour test for 00c60ad0: CEaxContext's scalar deleting destructor -- destructor always, the library free only for delete
#include <cstdio>
#include <cstdlib>
static int g_dtor = 0, g_free = 0; static void* g_freed = 0;
class CPKObject {
public:
    virtual ~CPKObject() { }
    static void* operator new(std::size_t size) { return std::malloc(size); }
    static void operator delete(void* p) { ++g_free; g_freed = p; std::free(p); }
};
class CEaxContext : public CPKObject { public: int m_a, m_b; ~CEaxContext() { ++g_dtor; } };
int main()
{
    CEaxContext* p = new CEaxContext; void* block = p;
    delete p;
    int ok = g_dtor == 1 && g_free == 1 && g_freed == block;
    { CEaxContext local; local.m_a = 1; }
    ok = ok && g_dtor == 2 && g_free == 1;
    std::printf(ok ? "00c60ad0_TEST PASS\n" : "00c60ad0_TEST FAIL\n");
    return ok ? 0 : 1;
}
