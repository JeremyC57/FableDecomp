// behaviour test for 0x00ABEDD0 C3DMeshFileGroupChunk's scalar deleting destructor (model: the array storage is freed, then the base runs)
#include <cstdio>
#include <cstdlib>
static int g_freed = 0, g_base = 0;
struct CBase { virtual ~CBase() { ++g_base; } };
struct CArr { void* start; CArr() : start(std::malloc(16)) { } ~CArr() { if (start) { std::free(start); ++g_freed; } } };
struct CChunk : CBase { CArr list; virtual ~CChunk() { } };
int main()
{
    CBase* p = new CChunk;
    delete p;
    int ok = g_freed == 1 && g_base == 1;
    if (!ok) { std::printf("00abedd0_TEST FAIL\n"); return 1; }
    std::printf("00abedd0_TEST PASS\n");
    return 0;
}