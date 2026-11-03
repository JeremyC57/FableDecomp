// Behaviour test for 00c60ad0: CEAXListener's scalar deleting destructor -- destructor always, the library free only for delete
#include <cstdio>
#include <cstdlib>
static int g_dtor = 0, g_free = 0; static void* g_freed = 0;
class CCodaObjectBase {
public:
    virtual ~CCodaObjectBase() { }
    static void* operator new(std::size_t size) { return std::malloc(size); }
    static void operator delete(void* p) { ++g_free; g_freed = p; std::free(p); }
};
class CEAXListener : public CCodaObjectBase { public: int m_a, m_b; ~CEAXListener() { ++g_dtor; } };
int main()
{
    CEAXListener* p = new CEAXListener; void* block = p;
    delete p;
    int ok = g_dtor == 1 && g_free == 1 && g_freed == block;
    { CEAXListener local; local.m_a = 1; }
    ok = ok && g_dtor == 2 && g_free == 1;
    std::printf(ok ? "00c60ad0_TEST PASS\n" : "00c60ad0_TEST FAIL\n");
    return ok ? 0 : 1;
}
