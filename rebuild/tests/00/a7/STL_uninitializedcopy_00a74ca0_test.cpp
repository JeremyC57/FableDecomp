// Behaviour test for 0x00A74CA0 std::__uninitialized_copy over C2DLineF vectors: each source vector is deep-copied
// into raw storage (own block, same lines), the returned end is result + (last - first).
#include <cstdio>
#include <cstdlib>
#include <new>
class C2DLineF { public: float x0, y0, x1, y1; };
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
static SLineVec* UninitCopy(const SLineVec* first, const SLineVec* last, SLineVec* result)
{
    SLineVec* cur = result;
    for (; first != last; ++first, ++cur)
        if (cur) new (cur) SLineVec(*first);
    return cur;
}
int main()
{
    C2DLineF lines[3] = { { 1, 2, 3, 4 }, { 5, 6, 7, 8 }, { 9, 10, 11, 12 } };
    SLineVec src[2];
    src[0].start = lines; src[0].finish = lines + 3; src[0].end = lines + 3;
    src[1].start = lines + 1; src[1].finish = lines + 2; src[1].end = lines + 2;
    SLineVec raw[2];
    SLineVec* e = UninitCopy(src, src + 2, raw);
    if (e != raw + 2 || !SameLines(raw[0], src[0]) || !SameLines(raw[1], src[1]) || raw[1].start[0].x0 != 5) { std::printf("00a74ca0_TEST FAIL\n"); return 1; }
    if (UninitCopy(src, src, raw) != raw) { std::printf("00a74ca0_TEST FAIL\n"); return 1; }
    std::free(raw[0].start); std::free(raw[1].start);
    std::printf("00a74ca0_TEST PASS\n");
    return 0;
}