// behaviour test for 0x00764640 std::__copy_backward over CCountedPointer<NShape::CShape>: last-first elements are assigned back to
// front (overlapping ranges shift right correctly), count blocks are shared, the old blocks released (deleted at zero), and the
// returned pointer is result - n
#include <cstdio>
struct Info { int RefCount; int Deleted; };
struct CP { void* Data; Info* PInfo; };
static void Assign(CP& to, const CP& from)
{
    if (to.PInfo != from.PInfo) {
        if (to.PInfo && --to.PInfo->RefCount == 0) to.PInfo->Deleted = 1;
        to.Data = from.Data; to.PInfo = from.PInfo;
        if (to.PInfo) ++to.PInfo->RefCount;
    }
}
static CP* CopyBackward(CP* first, CP* last, CP* result)   // the function under test
{
    for (int n = (int)(last - first); n > 0; --n) Assign(*--result, *--last);
    return result;
}
int main()
{
    int fail = 0;
    int o[4];
    Info i1 = { 1, 0 }, i2 = { 1, 0 }, i3 = { 1, 0 }, i4 = { 1, 0 };
    CP a[4] = { { &o[0], &i1 }, { &o[1], &i2 }, { &o[2], &i3 }, { &o[3], &i4 } };
    CP* r = CopyBackward(a, a + 3, a + 4);                 // shift the first three right by one
    if (r != a + 1) fail |= 1;
    if (a[1].Data != &o[0] || a[2].Data != &o[1] || a[3].Data != &o[2] || a[1].PInfo != &i1 || a[3].PInfo != &i3) fail |= 2;
    if (i1.RefCount != 2 || i2.RefCount != 1 || i3.RefCount != 1 || i4.RefCount != 0 || !i4.Deleted || i1.Deleted) fail |= 4;
    if (CopyBackward(a, a, a + 2) != a + 2) fail |= 8;
    CP same[2] = { { &o[0], &i1 }, { &o[1], &i1 } };       // one shared block: the assignment is skipped
    int before = i1.RefCount;
    CopyBackward(same, same + 1, same + 2);
    if (i1.RefCount != before || same[1].Data != &o[1]) fail |= 16;
    if (fail) { std::printf("00764640_TEST FAIL %d\n", fail); return 1; }
    std::printf("00764640_TEST PASS\n");
    return 0;
}