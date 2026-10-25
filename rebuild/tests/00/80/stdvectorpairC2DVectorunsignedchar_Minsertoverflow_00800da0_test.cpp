// Behaviour test for 0x00800DA0 vector<pair<C2DVector,unsigned char> >::_M_insert_overflow (STLport growth).
#include <cstdio>
#include <cstdlib>

struct C2DVector { float X, Y; };
struct RoutePoint { C2DVector first; unsigned char second; };

struct RouteVec {
    RoutePoint* _M_start;
    RoutePoint* _M_finish;
    RoutePoint* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    void insert_overflow(RoutePoint* position, const RoutePoint& x, unsigned int fill_len, bool atend)
    {
        const unsigned int old_size = size();
        const unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
        RoutePoint* new_start = len != 0 ? (RoutePoint*)std::malloc(len * sizeof(RoutePoint)) : 0;
        RoutePoint* new_finish = new_start;
        for (RoutePoint* p = _M_start; p != position; ++p, ++new_finish) *new_finish = *p;
        if (fill_len == 1) { *new_finish = x; ++new_finish; }
        else for (unsigned int n = fill_len; n > 0; --n, ++new_finish) *new_finish = x;
        if (!atend)
            for (RoutePoint* p = position; p != _M_finish; ++p, ++new_finish) *new_finish = *p;
        if (_M_start) std::free(_M_start);
        _M_start = new_start;
        _M_finish = new_finish;
        _M_end_of_storage = new_start + len;
    }
};

static RoutePoint rp(float x, int b) { RoutePoint r; r.first.X = x; r.first.Y = -x; r.second = (unsigned char)b; return r; }

int main()
{
    RouteVec v;
    v._M_start = (RoutePoint*)std::malloc(3 * sizeof(RoutePoint));
    v._M_finish = v._M_start + 3;
    v._M_end_of_storage = v._M_finish;
    v._M_start[0] = rp(1, 1); v._M_start[1] = rp(2, 2); v._M_start[2] = rp(3, 3);
    v.insert_overflow(v._M_start + 1, rp(9, 9), 2, false);      // 1 9 9 2 3, capacity 6
    if (v.size() != 5 || v._M_end_of_storage - v._M_start != 6 || v._M_start[1].second != 9 || v._M_start[2].second != 9
        || v._M_start[3].second != 2 || v._M_start[4].first.X != 3.0f) { std::printf("00800da0_TEST FAIL\n"); return 1; }
    v._M_finish = v._M_end_of_storage;                            // pretend full (6 entries, last one garbage overwritten)
    v._M_start[5] = rp(5, 5);
    v.insert_overflow(v._M_finish, rp(7, 7), 1, true);            // push_back: 12 capacity
    if (v.size() != 7 || v._M_end_of_storage - v._M_start != 12 || v._M_start[6].second != 7 || v._M_start[5].second != 5) {
        std::printf("00800da0_TEST FAIL\n"); return 1;
    }
    v.insert_overflow(v._M_start, rp(4, 4), 9, false);           // 9 copies at the front: len = 7 + 9
    if (v.size() != 16 || v._M_end_of_storage - v._M_start != 16 || v._M_start[8].second != 4 || v._M_start[9].second != 1) {
        std::printf("00800da0_TEST FAIL\n"); return 1;
    }
    std::free(v._M_start);
    std::printf("00800da0_TEST PASS\n");
    return 0;
}