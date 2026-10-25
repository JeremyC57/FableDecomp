#include <cstdio>
#include <new>
#include <cstdlib>
static int g_dtor = 0, g_frees = 0;
void operator delete(void* p) throw() { ++g_frees; std::free(p); }
struct CInstance { ~CInstance() { ++g_dtor; } };
class CInstanceRegistry {
public:
    void DestroyInstance(CInstance* instance) { if (instance) { instance->~CInstance(); operator delete(instance); } }
};
int main()
{
    CInstanceRegistry r;
    r.DestroyInstance(0);
    bool ok = g_dtor == 0 && g_frees == 0;
    r.DestroyInstance(new (std::malloc(sizeof(CInstance))) CInstance);
    ok = ok && g_dtor == 1 && g_frees == 1;
    if (!ok) { std::printf("00c2baf0_TEST FAIL\n"); return 1; }
    std::printf("00c2baf0_TEST PASS\n");
    return 0;
}
