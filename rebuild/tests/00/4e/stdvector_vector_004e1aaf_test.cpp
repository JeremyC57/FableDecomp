// Behaviour test for 0x004E1AAF std::vector<NPlayerTargeting::CTargetPoint>::~vector(): destroy [start, finish) through
// __destroy_aux (0x004DBE58), then free the block when there is one.
#include <cstdio>

static int g_rangeCalls = 0, g_rangeCount = 0, g_frees = 0;
static void* g_freed = 0;
struct CTargetPoint { int Words[7]; };          // 28 bytes
static void DestroyRange(CTargetPoint* first, CTargetPoint* last) { ++g_rangeCalls; g_rangeCount += (int)(last - first); }
static void FreeBlock(void* p) { ++g_frees; g_freed = p; }
struct TargetPointVector {
    CTargetPoint* _M_start;
    CTargetPoint* _M_finish;
    CTargetPoint* _M_end_of_storage;
    ~TargetPointVector() { DestroyRange(_M_start, _M_finish); if (_M_start) FreeBlock(_M_start); }
};

int main()
{
    static CTargetPoint block[3];
    { TargetPointVector v = { block, block + 2, block + 3 }; }
    bool ok = g_rangeCalls == 1 && g_rangeCount == 2 && g_frees == 1 && g_freed == block;
    { TargetPointVector e = { 0, 0, 0 }; }
    ok = ok && g_rangeCalls == 2 && g_rangeCount == 2 && g_frees == 1;
    std::printf(ok ? "AUTO_TINY_004e1aaf_TEST PASS\n" : "004e1aaf_TEST FAIL\n");   // the catalogue PassPattern
    return ok ? 0 : 1;
}
