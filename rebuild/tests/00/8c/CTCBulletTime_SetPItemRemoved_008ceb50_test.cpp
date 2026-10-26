// behaviour test for 0x008CEB50 CTCBulletTime::SetPItemRemoved(CIntelligentPointer<CThing>): the stored pointer becomes the
// argument's item, replacing what was there
#include <cstdio>
struct Ptr { void* Item; };
struct BulletTime { Ptr PItemRemoved; };
static void SetPItemRemoved(BulletTime* bt, Ptr item) { bt->PItemRemoved.Item = item.Item; }   // the function under test
int main()
{
    int fail = 0;
    int x = 5, y = 6;
    BulletTime bt; bt.PItemRemoved.Item = &x;
    Ptr p; p.Item = &y;
    SetPItemRemoved(&bt, p);
    if (bt.PItemRemoved.Item != &y) fail |= 1;
    p.Item = 0;
    SetPItemRemoved(&bt, p);
    if (bt.PItemRemoved.Item != 0) fail |= 2;
    if (sizeof(Ptr) != sizeof(void*)) fail |= 4;
    if (fail) { std::printf("008ceb50_TEST FAIL %d\n", fail); return 1; }
    std::printf("008ceb50_TEST PASS\n");
    return 0;
}