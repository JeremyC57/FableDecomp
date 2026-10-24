// Behaviour test for 0x00A74A20 _STL::__uninitialized_fill_n over C2DLineF vectors: n deep copies of x, each with its
// own block, the returned end is first + n; n == 0 writes nothing.
#include <cstdio>
#include <cstdlib>
#include <new>
struct C2DLineF { float x0, y0, x1, y1; };
struct SLineVec {                                   // the STLport vector<C2DLineF> layout
    C2DLineF* start; C2DLineF* finish; C2DLineF* end;
    SLineVec(const SLineVec& x) : start(0), finish(0), end(0)
    {
        unsigned int n = (unsigned int)(x.finish - x.start);
        start = n != 0 ? (C2DLineF*)std::malloc(n * sizeof(C2DLineF)) : 0;
        finish = start;
        end = start + n;
        C2DLineF* cur = start;
        for (const C2DLineF* f = x.start; f != x.finish; ++f, ++cur) if (cur) new (cur) C2DLineF(*f);
        finish = cur;
    }
    SLineVec() : start(0), finish(0), end(0) { }
};
static bool SameLines(const SLineVec& a, const SLineVec& b)
{
    if (a.finish - a.start != b.finish - b.start || a.end - a.start != b.finish - b.start) return false;
    if (a.start == b.start && a.start != 0) return false;   // must own its own block
    for (int i = 0; i < a.finish - a.start; ++i)
        if (a.start[i].x0 != b.start[i].x0 || a.start[i].y1 != b.start[i].y1) return false;
    return true;
}
static SLineVec* FillN(SLineVec* first, unsigned int n, const SLineVec& x)
{
    SLineVec* cur = first;
    for (; n > 0; --n, ++cur)
        if (cur) new (cur) SLineVec(x);
    return cur;
}
int main()
{
    C2DLineF lines[3] = { { 1, 2, 3, 4 }, { 5, 6, 7, 8 }, { 9, 10, 11, 12 } };
    SLineVec src; src.start = lines; src.finish = lines + 3; src.end = lines + 3;
    SLineVec raw[4];
    SLineVec* e = FillN(raw, 3, src);
    if (e != raw + 3 || !SameLines(raw[0], src) || !SameLines(raw[2], src) || raw[0].start == raw[1].start) { std::printf("00a74a20_TEST FAIL\n"); return 1; }
    if (FillN(raw + 3, 0, src) != raw + 3 || raw[3].start != 0) { std::printf("00a74a20_TEST FAIL\n"); return 1; }
    SLineVec empty;
    FillN(raw + 3, 1, empty);
    if (raw[3].start != 0 || raw[3].finish != 0) { std::printf("00a74a20_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 3; ++i) std::free(raw[i].start);
    std::printf("00a74a20_TEST PASS\n");
    return 0;
}