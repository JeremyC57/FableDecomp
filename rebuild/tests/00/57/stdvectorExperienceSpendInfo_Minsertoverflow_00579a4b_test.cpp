// Behaviour test for 0x00579A4B vector<ExperienceSpendInfo>::_M_insert_overflow (12-byte records).
// Grows to old + max(old, fill_len), copies the prefix, inserts fill_len copies of x, copies the suffix unless
// atend, frees the old buffer and sets start/finish/end.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

struct E { int currentSpend; int initialSpend; int levelUpSpend; };
static E Make(int k) { E e; e.currentSpend = k; e.initialSpend = k * 10; e.levelUpSpend = -k; return e; }
static bool Same(const E& x, const E& y) { return std::memcmp(&x, &y, sizeof(E)) == 0; }
static int g_frees = 0;

struct Vec {
    E* start; E* finish; E* eos;
    unsigned int size() const { return (unsigned int)(finish - start); }
    void InsertOverflow(E* pos, const E& x, unsigned int fill_len, bool atend)
    {
        const unsigned int old_size = size();
        const unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
        E* new_start = (E*)std::malloc(len * sizeof(E));
        E* new_finish = new_start;
        for (E* p = start; p != pos; ++p, ++new_finish) new (new_finish) E(*p);
        if (fill_len == 1) { new (new_finish) E(x); ++new_finish; }
        else for (unsigned int n = fill_len; n > 0; --n, ++new_finish) new (new_finish) E(x);
        if (!atend)
            for (E* p = pos; p != finish; ++p, ++new_finish) new (new_finish) E(*p);
        if (start) { ++g_frees; std::free(start); }
        start = new_start; finish = new_finish; eos = new_start + len;
    }
};

int main()
{
    Vec v;
    v.start = (E*)std::malloc(3 * sizeof(E));
    for (int i = 0; i < 3; ++i) v.start[i] = Make(i + 1);
    v.finish = v.eos = v.start + 3;
    E x = Make(9);
    v.InsertOverflow(v.start + 1, x, 1, false);            // 1 9 2 3, capacity 6
    bool ok = v.size() == 4 && v.eos - v.start == 6 && Same(v.start[0], Make(1)) && Same(v.start[1], x) &&
              Same(v.start[2], Make(2)) && Same(v.start[3], Make(3)) && g_frees == 1;
    v.InsertOverflow(v.finish, Make(7), 5, true);          // append 5 sevens at the end, capacity 4 + 5
    ok = ok && v.size() == 9 && v.eos - v.start == 9 && Same(v.start[3], Make(3)) && Same(v.start[4], Make(7)) &&
         Same(v.start[8], Make(7)) && g_frees == 2;
    std::free(v.start);
    Vec e; e.start = e.finish = e.eos = 0;
    e.InsertOverflow(0, x, 1, true);                       // empty vector: capacity 1, no free
    ok = ok && e.size() == 1 && e.eos - e.start == 1 && Same(e.start[0], x) && g_frees == 2;
    std::free(e.start);
    std::printf(ok ? "00579a4b_TEST PASS\n" : "00579a4b_TEST FAIL\n");
    return ok ? 0 : 1;
}