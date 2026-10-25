// Behaviour test for 0x008092D0 vector<NPlayerTargeting::CTargetPoint>::_M_insert_overflow (STLport growth, element destructor releases the pointer).
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_live = 0;

struct Region {
    float Start, Score;
    Region(float s) : Start(s), Score(-s) { ++g_live; }
    Region(const Region& o) : Start(o.Start), Score(o.Score) { ++g_live; }
    ~Region() { --g_live; }
};

struct RegionVec {
    Region* _M_start;
    Region* _M_finish;
    Region* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    static Region* ucopy(Region* first, Region* last, Region* result)
    {
        for (; first != last; ++first, ++result) new (result) Region(*first);
        return result;
    }
    void insert_overflow(Region* position, const Region& x, unsigned int fill_len, bool atend)
    {
        const unsigned int old_size = size();
        const unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
        Region* new_start = len != 0 ? (Region*)std::malloc(len * sizeof(Region)) : 0;
        Region* new_finish = ucopy(_M_start, position, new_start);
        if (fill_len == 1) { new (new_finish) Region(x); ++new_finish; }
        else for (unsigned int n = fill_len; n > 0; --n, ++new_finish) new (new_finish) Region(x);
        if (!atend) new_finish = ucopy(position, _M_finish, new_finish);
        for (Region* p = _M_start; p != _M_finish; ++p) p->~Region();
        if (_M_start) std::free(_M_start);
        _M_start = new_start; _M_finish = new_finish; _M_end_of_storage = new_start + len;
    }
};

int main()
{
    RegionVec v;
    v._M_start = (Region*)std::malloc(2 * sizeof(Region));
    new (v._M_start) Region(1.0f);
    new (v._M_start + 1) Region(2.0f);
    v._M_finish = v._M_end_of_storage = v._M_start + 2;
    {
        Region x(5.0f);
        v.insert_overflow(v._M_start + 1, x, 3, false);      // 1 5 5 5 2, capacity 5
        if (v.size() != 5 || v._M_end_of_storage - v._M_start != 5 || v._M_start[2].Start != 5.0f
            || v._M_start[4].Start != 2.0f || g_live != 6) { std::printf("008092d0_TEST FAIL\n"); return 1; }
        v.insert_overflow(v._M_finish, x, 1, true);           // push_back, capacity 10
        if (v.size() != 6 || v._M_end_of_storage - v._M_start != 10 || v._M_start[5].Score != -5.0f || g_live != 7) {
            std::printf("008092d0_TEST FAIL\n"); return 1;
        }
    }
    for (Region* p = v._M_start; p != v._M_finish; ++p) p->~Region();
    std::free(v._M_start);
    if (g_live != 0) { std::printf("008092d0_TEST FAIL\n"); return 1; }
    std::printf("008092d0_TEST PASS\n");
    return 0;
}