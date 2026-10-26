#include <cstdio>
// Behaviour model of 0x00A03530: ~CScopedCritical leaves the pool's section only when locked and multi-threaded.
static int g_leaves = 0; static void* g_last = 0;
struct RTL_CRITICAL_SECTION_Fable { unsigned char _body[0x18]; };
static void FakeLeaveCriticalSection(RTL_CRITICAL_SECTION_Fable* s) { ++g_leaves; g_last = s; }
class CHandleBasedMemoryPool { public: bool MultiThreaded; RTL_CRITICAL_SECTION_Fable CriticalSection; };
struct CScopedCritical {
    CHandleBasedMemoryPool* Owner; bool Success;
    ~CScopedCritical() { if (Success && Owner->MultiThreaded) FakeLeaveCriticalSection(&Owner->CriticalSection); }
};
int main()
{
    CHandleBasedMemoryPool pool; pool.MultiThreaded = true;
    { CScopedCritical c; c.Owner = &pool; c.Success = false; }
    if (g_leaves != 0) { std::printf("00a03530_TEST FAIL\n"); return 1; }
    { CScopedCritical c; c.Owner = &pool; c.Success = true; }
    if (g_leaves != 1 || g_last != &pool.CriticalSection) { std::printf("00a03530_TEST FAIL\n"); return 1; }
    pool.MultiThreaded = false;
    { CScopedCritical c; c.Owner = &pool; c.Success = true; }
    if (g_leaves != 1) { std::printf("00a03530_TEST FAIL\n"); return 1; }
    std::printf("00a03530_TEST PASS\n");
    return 0;
}