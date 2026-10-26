// Behaviour test for 0x004774ab: std::_Vector_base<CEngineLocalDetailObjectDef>::~_Vector_base frees the storage block
// once when there is one, and never frees a null start.
#include <cstdio>
static int g_frees; static void* g_last;
static void my_free(void* p) { ++g_frees; g_last = p; }
struct Base { void* _M_start; void* _M_finish; void* _M_end;
    ~Base() { if (_M_start != 0) { if (_M_start) my_free(_M_start); } } };
int main()
{
    char block[0x110];
    { Base b; b._M_start = block; b._M_finish = block + 0x88; b._M_end = block + 0x110; }
    bool ok = g_frees == 1 && g_last == block;
    { Base b; b._M_start = 0; b._M_finish = 0; b._M_end = 0; }
    ok = ok && g_frees == 1;
    if (!ok) { std::printf("004774ab_TEST FAIL\n"); return 1; }
    std::printf("004774ab_TEST PASS\n"); return 0;
}
