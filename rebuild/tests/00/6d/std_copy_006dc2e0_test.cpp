// Behaviour test for std::__copy<const long*, back_insert_iterator<vector<long>>> (0x006DC2E0): appends every element, growing through the overflow path when full.
#include <cstdio>
#include <cstdlib>
#include <cstring>
struct LongVector {
    int* start; int* finish; int* eos; int overflows;
    void insert_overflow(int* pos, const int& x)
    {
        int n = (int)(pos - start), cap = n ? 2 * n : 1;
        int* p = (int*)std::malloc(cap * sizeof(int));
        if (n) std::memcpy(p, start, n * sizeof(int));
        p[n] = x; std::free(start); start = p; finish = p + n + 1; eos = p + cap; ++overflows;
    }
    void push_back(const int& x) { if (finish != eos) { *finish = x; ++finish; } else insert_overflow(finish, x); }
};
struct BackInserter { LongVector* c; };
static BackInserter copy_back(const int* first, const int* last, BackInserter result)
{
    for (int n = (int)(last - first); n > 0; --n) { result.c->push_back(*first); ++first; }
    return result;
}
int main()
{
    LongVector v; v.start = v.finish = v.eos = 0; v.overflows = 0;
    int src[5] = { 4, -1, 8, 15, 16 };
    BackInserter bi; bi.c = &v;
    BackInserter r = copy_back(src, src + 5, bi);
    if (r.c != &v || v.finish - v.start != 5 || v.overflows != 4) { std::printf("006dc2e0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 5; ++i) if (v.start[i] != src[i]) { std::printf("006dc2e0_TEST FAIL\n"); return 1; }
    copy_back(src, src, bi);
    if (v.finish - v.start != 5) { std::printf("006dc2e0_TEST FAIL\n"); return 1; }
    std::free(v.start);
    std::printf("006dc2e0_TEST PASS\n");
    return 0;
}