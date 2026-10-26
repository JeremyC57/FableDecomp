// behaviour test for 0x00AE9C00 std::__copy_backward over pair<long, CCountedPointer<NParticleEngine::CParticleEmitter> >: last-first elements are assigned back to front (overlapping ranges shift right correctly), counted blocks are shared, the old blocks released, and the returned pointer is result - n
#include <cstdio>
struct Info { int RefCount; };
struct Entry { int Key; Info* PInfo; };
static void Assign(Entry& to, const Entry& from)
{
    to.Key = from.Key;
    if (to.PInfo != from.PInfo) { if (to.PInfo) --to.PInfo->RefCount; to.PInfo = from.PInfo; if (to.PInfo) ++to.PInfo->RefCount; }
}
static Entry* CopyBackward(Entry* first, Entry* last, Entry* result)   // the function under test
{
    for (int n = (int)(last - first); n > 0; --n) Assign(*--result, *--last);
    return result;
}
int main()
{
    int fail = 0;
    Info i1 = { 1 }, i2 = { 1 }, i3 = { 1 }, i4 = { 1 };
    Entry a[4] = { { 1, &i1 }, { 2, &i2 }, { 3, &i3 }, { 4, &i4 } };
    Entry* r = CopyBackward(a, a + 3, a + 4);             // shift the first three right by one
    if (r != a + 1) fail |= 1;
    if (a[1].Key != 1 || a[2].Key != 2 || a[3].Key != 3 || a[1].PInfo != &i1 || a[3].PInfo != &i3) fail |= 2;
    if (i1.RefCount != 2 || i2.RefCount != 1 || i3.RefCount != 1 || i4.RefCount != 0) fail |= 4;
    if (CopyBackward(a, a, a + 2) != a + 2) fail |= 8;
    if (fail) { std::printf("00ae9c00_TEST FAIL %d\n", fail); return 1; }
    std::printf("00ae9c00_TEST PASS\n");
    return 0;
}