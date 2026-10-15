#include <cstdio>
// ~CModeDragonFlying model: PEndAction releases its block (destroy + delete on the last reference) and clears both words.
static int g_destroyed = 0, g_deleted = 0;
struct SCountBlock { long refs; };
struct CCountedPointer {
    void* obj; SCountBlock* block;
    void Release()
    {
        if (block) {
            --block->refs;
            if (block->refs == 0) { ++g_destroyed; ++g_deleted; }
        }
        obj = 0;
        block = 0;
    }
};
int main()
{
    SCountBlock b1 = { 1 }, b2 = { 2 };
    int dummy = 0;
    CCountedPointer p1 = { &dummy, &b1 }, p2 = { &dummy, &b2 }, p3 = { 0, 0 };
    p1.Release(); p2.Release(); p3.Release();
    if (!(g_destroyed == 1 && g_deleted == 1 && b2.refs == 1 && p1.obj == 0 && p1.block == 0 && p2.block == 0)) { std::printf("00743000_TEST FAIL\n"); return 1; }
    std::printf("00743000_TEST PASS\n");
    return 0;
}