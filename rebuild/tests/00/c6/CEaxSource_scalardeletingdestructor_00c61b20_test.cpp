// Behaviour test for 00c61b20: CEaxSource's scalar deleting destructor -- destructor always, the library free only for delete
#include <cstdio>
#include <cstdlib>
static int g_dtor = 0, g_free = 0; static void* g_freed = 0;
class CPKObject {
public:
    virtual ~CPKObject() { }
    static void* operator new(std::size_t size) { return std::malloc(size); }
    static void operator delete(void* p) { ++g_free; g_freed = p; std::free(p); }
};
class CEaxSource : public CPKObject { public: int m_a, m_b; ~CEaxSource() { ++g_dtor; } };
int main()
{
    CEaxSource* p = new CEaxSource; void* block = p;
    delete p;
    int ok = g_dtor == 1 && g_free == 1 && g_freed == block;
    { CEaxSource local; local.m_a = 1; }
    ok = ok && g_dtor == 2 && g_free == 1;
    std::printf(ok ? "00c61b20_TEST PASS\n" : "00c61b20_TEST FAIL\n");
    return ok ? 0 : 1;
}
