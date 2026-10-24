// Behaviour test: vector<SRecord20>::_M_insert_overflow -- grow the storage, copy the head, place one or n copies of the value, copy the tail unless at the end
#include <cstdio>
#include <cstdlib>
#include <new>

struct SRecord20 {
    long m_0, m_4, m_8, m_c, m_10;
};

static SRecord20 Rec(long v)
{
    SRecord20 r;
    r.m_0 = v; r.m_4 = v + 1; r.m_8 = v + 2; r.m_c = v + 3; r.m_10 = v + 4;
    return r;
}
static bool Same(const SRecord20& r, long v)
{
    return r.m_0 == v && r.m_4 == v + 1 && r.m_8 == v + 2 && r.m_c == v + 3 && r.m_10 == v + 4;
}

struct CVec {                                           // STLport vector<SRecord20> (start, finish, end of storage)
    SRecord20* _M_start;
    SRecord20* _M_finish;
    SRecord20* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    void _M_insert_overflow(SRecord20* position, const SRecord20& x, unsigned int fill_len, bool atend)
    {
        const unsigned int old_size = size();
        const unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
        SRecord20* new_start = len ? (SRecord20*)std::malloc(len * sizeof(SRecord20)) : 0;
        SRecord20* new_finish = new_start;
        for (SRecord20* p = _M_start; p != position; ++p, ++new_finish)
            new (new_finish) SRecord20(*p);
        if (fill_len == 1) {
            new (new_finish) SRecord20(x);
            ++new_finish;
        }
        else
            for (unsigned int n = fill_len; n > 0; --n, ++new_finish)
                new (new_finish) SRecord20(x);
        if (!atend)
            for (SRecord20* q = position; q != _M_finish; ++q, ++new_finish)
                new (new_finish) SRecord20(*q);
        if (_M_start)
            std::free(_M_start);
        _M_start = new_start;
        _M_finish = new_finish;
        _M_end_of_storage = new_start + len;
    }
};

int main()
{
    CVec v;
    v._M_start = v._M_finish = v._M_end_of_storage = 0;
    v._M_insert_overflow(v._M_finish, Rec(10), 1, true);              // empty: capacity 1
    if (v.size() != 1 || v._M_end_of_storage - v._M_start != 1 || !Same(v._M_start[0], 10))
        { std::printf("0081c270_TEST FAIL\n"); return 1; }
    v._M_insert_overflow(v._M_finish, Rec(20), 1, true);              // doubles to 2
    if (v.size() != 2 || v._M_end_of_storage - v._M_start != 2 || !Same(v._M_start[1], 20))
        { std::printf("0081c270_TEST FAIL\n"); return 1; }
    v._M_insert_overflow(v._M_start + 1, Rec(30), 3, false);          // three copies in the middle: 2 + max(2,3) = 5
    if (v.size() != 5 || v._M_end_of_storage - v._M_start != 5 || !Same(v._M_start[0], 10) || !Same(v._M_start[1], 30)
        || !Same(v._M_start[2], 30) || !Same(v._M_start[3], 30) || !Same(v._M_start[4], 20))
        { std::printf("0081c270_TEST FAIL\n"); return 1; }
    std::free(v._M_start);
    std::printf("0081c270_TEST PASS\n");
    return 0;
}