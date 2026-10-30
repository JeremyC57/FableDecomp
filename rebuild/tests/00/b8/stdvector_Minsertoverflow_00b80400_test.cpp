// Behaviour test for STLport vector<CTVertexSeaBackground>::_M_insert_overflow (0x00B80400): a standalone copy of the growth
// logic over a 12-byte vertex; checks the new capacity, prefix/insert/suffix layout, the append form and the old block release.
#include <cstdio>
#include <cstdlib>
#include <new>
struct V { float X, Y, Z; };
static int g_fail = 0, g_frees = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed at line %d\n", __LINE__); } } while (0)
struct Vec {
    V* start; V* finish; V* eos;
    Vec() : start(0), finish(0), eos(0) {}
    unsigned int size() const { return (unsigned int)(finish - start); }
    void clear_() { if (start) { std::free(start); g_frees++; } }
    void insert_overflow(V* position, const V& x, unsigned int fill_len, bool atend)
    {
        const unsigned int old_size = size();
        const unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
        V* new_start = len ? (V*)std::malloc(len * sizeof(V)) : 0;
        V* new_finish = new_start;
        for (V* p = start; p != position; ++p, ++new_finish) new (new_finish) V(*p);
        if (fill_len == 1) { new (new_finish) V(x); ++new_finish; }
        else for (unsigned int n = fill_len; n > 0; --n, ++new_finish) new (new_finish) V(x);
        if (!atend) for (V* p = position; p != finish; ++p, ++new_finish) new (new_finish) V(*p);
        clear_();
        start = new_start; finish = new_finish; eos = new_start + len;
    }
};
static V mk(float a) { V v; v.X = a; v.Y = a + 0.5f; v.Z = -a; return v; }
int main()
{
    Vec v;
    v.insert_overflow(v.finish, mk(1), 1, true);             // empty -> capacity 1
    CHECK(v.size() == 1 && v.eos - v.start == 1 && v.start[0].X == 1 && v.start[0].Z == -1); CHECK(g_frees == 0);
    v.insert_overflow(v.finish, mk(2), 1, true);             // 1 -> 2
    CHECK(v.size() == 2 && v.eos - v.start == 2 && v.start[1].Y == 2.5f); CHECK(g_frees == 1);
    v.insert_overflow(v.start + 1, mk(9), 3, false);         // insert 3 in the middle: capacity 2 + max(2,3) = 5
    CHECK(v.size() == 5 && v.eos - v.start == 5);
    CHECK(v.start[0].X == 1 && v.start[1].X == 9 && v.start[2].X == 9 && v.start[3].X == 9 && v.start[4].X == 2);
    v.insert_overflow(v.start, mk(7), 1, false);             // front insert: 5 + 5 = 10
    CHECK(v.size() == 6 && v.eos - v.start == 10 && v.start[0].X == 7 && v.start[5].X == 2);
    CHECK(g_frees == 3);
    if (g_fail) { std::printf("00b80400_TEST FAIL\n"); return 1; }
    std::printf("00b80400_TEST PASS\n");
    return 0;
}