// behaviour test for 0x01228D80: the atexit destructor of a static vector<CCharString> (model: element dtors then free)
#include <cstdio>
#include <cstdlib>
static int g_dtors = 0, g_frees = 0;
class CCharString { public: void* PStringData; ~CCharString() { ++g_dtors; } };
struct CVec { CCharString* _M_start; CCharString* _M_finish; CCharString* _M_end_of_storage; };
static void Destroy(CVec& v)
{
    for (CCharString* p = v._M_start; p != v._M_finish; ++p) p->~CCharString();
    if (v._M_start) { std::free(v._M_start); ++g_frees; }
}
int main()
{
    CVec v; v._M_start = (CCharString*)std::malloc(3 * sizeof(CCharString)); v._M_finish = v._M_start + 3; v._M_end_of_storage = v._M_finish;
    Destroy(v);
    CVec e; e._M_start = e._M_finish = e._M_end_of_storage = 0;
    Destroy(e);
    if (g_dtors != 3 || g_frees != 1) { std::printf("01228d80_TEST FAIL\n"); return 1; }
    std::printf("01228d80_TEST PASS\n");
    return 0;
}