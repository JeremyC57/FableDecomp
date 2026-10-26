// behaviour test for 0x004941f2 std::vector<CSubtitleRenderer::CEmbeddedHighlight >::_M_insert_overflow: the new capacity is size + max(size, fill_len); the head
// [start, pos) is copied, fill_len copies of x follow, then the tail [pos, finish) unless atend; the old block is released
#include <cstdio>
#include <cstdlib>
struct Elem14 { int Words[5]; };                     // test-only element of the retail size 0x14
struct Vec14 {
    Elem14* Start;
    Elem14* Finish;
    Elem14* EndOfStorage;
};
static int gFrees = 0;
static void InsertOverflow(Vec14& v, Elem14* pos, const Elem14& x, unsigned int fillLen, bool atEnd)
{
    unsigned int oldSize = (unsigned int)(v.Finish - v.Start);
    unsigned int len = oldSize + (oldSize < fillLen ? fillLen : oldSize);
    Elem14* newStart = len != 0 ? (Elem14*)std::malloc(len * sizeof(Elem14)) : 0;
    Elem14* newFinish = newStart;
    for (Elem14* p = v.Start; p != pos; ++p) *newFinish++ = *p;
    for (unsigned int k = 0; k < fillLen; ++k) *newFinish++ = x;
    if (!atEnd)
        for (Elem14* p = pos; p != v.Finish; ++p) *newFinish++ = *p;
    if (v.Start) { std::free(v.Start); ++gFrees; }
    v.Start = newStart; v.Finish = newFinish; v.EndOfStorage = newStart + len;
}
int main()
{
    int fails = 0;
    if ((unsigned int)sizeof(Elem14) != 0x14u) fails |= 1;
    Vec14 v = { 0, 0, 0 };
    v.Start = (Elem14*)std::malloc(3 * sizeof(Elem14));
    for (int k = 0; k < 3; ++k) for (int i = 0; i < 5; ++i) v.Start[k].Words[i] = k * 10 + i;
    v.Finish = v.EndOfStorage = v.Start + 3;
    Elem14 x; for (int i = 0; i < 5; ++i) x.Words[i] = 99;
    InsertOverflow(v, v.Start + 1, x, 1, false);
    if (v.Finish - v.Start != 4 || v.EndOfStorage - v.Start != 6 || gFrees != 1) fails |= 2;
    if (v.Start[0].Words[0] != 0 || v.Start[1].Words[0] != 99 || v.Start[2].Words[0] != 10 || v.Start[3].Words[5 - 1] != 20 + 5 - 1) fails |= 4;
    InsertOverflow(v, v.Finish, x, 9, true);
    if (v.Finish - v.Start != 13 || v.EndOfStorage - v.Start != 13 || v.Start[12].Words[0] != 99 || gFrees != 2) fails |= 8;
    std::free(v.Start);
    if (fails) { std::printf("004941f2_TEST FAIL %d\n", fails); return 1; }
    std::printf("004941f2_TEST PASS\n");
    return 0;
}