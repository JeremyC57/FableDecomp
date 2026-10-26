// behaviour test for 0x005bc0fb std::vector<std::vector<std::pair<long, long> > >::_M_insert_overflow: the new capacity is size + max(size, fill_len); the head
// [start, pos) is copied, fill_len copies of x follow, then the tail [pos, finish) unless atend; the old block is released
#include <cstdio>
#include <cstdlib>
struct ElemC { int Words[3]; };                     // test-only element of the retail size 0xC
struct VecC {
    ElemC* Start;
    ElemC* Finish;
    ElemC* EndOfStorage;
};
static int gFrees = 0;
static void InsertOverflow(VecC& v, ElemC* pos, const ElemC& x, unsigned int fillLen, bool atEnd)
{
    unsigned int oldSize = (unsigned int)(v.Finish - v.Start);
    unsigned int len = oldSize + (oldSize < fillLen ? fillLen : oldSize);
    ElemC* newStart = len != 0 ? (ElemC*)std::malloc(len * sizeof(ElemC)) : 0;
    ElemC* newFinish = newStart;
    for (ElemC* p = v.Start; p != pos; ++p) *newFinish++ = *p;
    for (unsigned int k = 0; k < fillLen; ++k) *newFinish++ = x;
    if (!atEnd)
        for (ElemC* p = pos; p != v.Finish; ++p) *newFinish++ = *p;
    if (v.Start) { std::free(v.Start); ++gFrees; }
    v.Start = newStart; v.Finish = newFinish; v.EndOfStorage = newStart + len;
}
int main()
{
    int fails = 0;
    if ((unsigned int)sizeof(ElemC) != 0xCu) fails |= 1;
    VecC v = { 0, 0, 0 };
    v.Start = (ElemC*)std::malloc(3 * sizeof(ElemC));
    for (int k = 0; k < 3; ++k) for (int i = 0; i < 3; ++i) v.Start[k].Words[i] = k * 10 + i;
    v.Finish = v.EndOfStorage = v.Start + 3;
    ElemC x; for (int i = 0; i < 3; ++i) x.Words[i] = 99;
    InsertOverflow(v, v.Start + 1, x, 1, false);
    if (v.Finish - v.Start != 4 || v.EndOfStorage - v.Start != 6 || gFrees != 1) fails |= 2;
    if (v.Start[0].Words[0] != 0 || v.Start[1].Words[0] != 99 || v.Start[2].Words[0] != 10 || v.Start[3].Words[3 - 1] != 20 + 3 - 1) fails |= 4;
    InsertOverflow(v, v.Finish, x, 9, true);
    if (v.Finish - v.Start != 13 || v.EndOfStorage - v.Start != 13 || v.Start[12].Words[0] != 99 || gFrees != 2) fails |= 8;
    std::free(v.Start);
    if (fails) { std::printf("005bc0fb_TEST FAIL %d\n", fails); return 1; }
    std::printf("005bc0fb_TEST PASS\n");
    return 0;
}