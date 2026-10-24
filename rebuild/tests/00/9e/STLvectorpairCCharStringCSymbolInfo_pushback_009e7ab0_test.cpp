// Behaviour test for 0x009E7AB0 STLport vector::push_back (16-byte entries).
#include <cstdio>
#include <cstdlib>
#include <new>

struct SEntry { int key; int group; int entry; int val; };
static int gOverflows = 0;

struct vector {
    SEntry* _M_start; SEntry* _M_finish; SEntry* _M_end_of_storage;
    void _M_insert_overflow(SEntry* position, const SEntry& x, unsigned int fill_len, bool atend)
    {
        ++gOverflows;
        unsigned int n = (unsigned int)(_M_finish - _M_start), cap = n ? 2 * n : 1;
        SEntry* p = (SEntry*)std::malloc(cap * sizeof(SEntry));
        for (unsigned int i = 0; i < n; ++i) p[i] = _M_start[i];
        p[n] = x;
        std::free(_M_start);
        _M_start = p; _M_finish = p + n + fill_len; _M_end_of_storage = p + cap;
        (void)position; (void)atend;
    }
    void push_back(const SEntry& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) SEntry(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, 1, true);
    }
};

int main()
{
    vector v = { 0, 0, 0 };
    for (int i = 0; i < 5; ++i) { SEntry e = { i, 10 + i, 20 + i, 30 + i }; v.push_back(e); }
    bool ok = v._M_finish - v._M_start == 5 && gOverflows == 4 && v._M_end_of_storage - v._M_start == 8;
    for (int i = 0; i < 5; ++i) ok = ok && v._M_start[i].key == i && v._M_start[i].val == 30 + i;
    SEntry e = { 9, 9, 9, 9 };
    v.push_back(e);
    ok = ok && gOverflows == 4 && v._M_finish - v._M_start == 6 && v._M_start[5].group == 9;
    std::free(v._M_start);
    if (!ok) { std::printf("009e7ab0_TEST FAIL\n"); return 1; }
    std::printf("009e7ab0_TEST PASS\n");
    return 0;
}