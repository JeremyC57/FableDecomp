// Behaviour test for 0x007D24F0 vector<CShoveHaPennyRegionDef>::_M_fill_insert (in-place shift and fill paths).
#include <cstdio>
#include <cstdlib>
#include <new>

struct Region {
    float Start, Score;
    Region(float s) : Start(s), Score(-s) { }
    Region(const Region& o) : Start(o.Start), Score(o.Score) { }
    Region& operator=(const Region& o) { Start = o.Start; Score = o.Score; return *this; }
    virtual ~Region() { }
};

struct RegionVec {
    Region* _M_start;
    Region* _M_finish;
    Region* _M_end_of_storage;
    bool overflowed;
    static Region* ucopy(Region* first, Region* last, Region* result)
    {
        for (; first != last; ++first, ++result) new (result) Region(*first);
        return result;
    }
    void fill_insert(Region* position, unsigned int n, const Region& x)
    {
        if (n != 0) {
            if ((unsigned int)(_M_end_of_storage - _M_finish) >= n) {
                Region x_copy = x;
                const unsigned int elems_after = (unsigned int)(_M_finish - position);
                Region* old_finish = _M_finish;
                if (elems_after > n) {
                    ucopy(_M_finish - n, _M_finish, _M_finish);
                    _M_finish += n;
                    Region* last = old_finish - n; Region* result = old_finish;
                    for (int k = (int)(last - position); k > 0; --k) *--result = *--last;
                    for (Region* p = position; p != position + n; ++p) *p = x_copy;
                } else {
                    Region* cur = _M_finish;
                    for (unsigned int k = n - elems_after; k > 0; --k, ++cur) new (cur) Region(x_copy);
                    _M_finish += n - elems_after;
                    ucopy(position, old_finish, _M_finish);
                    _M_finish += elems_after;
                    for (Region* p = position; p != old_finish; ++p) *p = x_copy;
                }
            } else
                overflowed = true;
        }
    }
};

static bool expect(const RegionVec& v, const float* want, int n)
{
    if (v._M_finish - v._M_start != n) return false;
    for (int i = 0; i < n; ++i) if (v._M_start[i].Start != want[i]) return false;
    return true;
}

int main()
{
    RegionVec v;
    v.overflowed = false;
    v._M_start = (Region*)std::malloc(10 * sizeof(Region));
    for (int i = 0; i < 4; ++i) new (v._M_start + i) Region((float)i);
    v._M_finish = v._M_start + 4;
    v._M_end_of_storage = v._M_start + 10;
    Region x(9.0f);
    v.fill_insert(v._M_start + 1, 2, x);                         // elems_after 3 > 2
    const float w1[] = { 0, 9, 9, 1, 2, 3 };
    if (!expect(v, w1, 6)) { std::printf("007d24f0_TEST FAIL\n"); return 1; }
    v.fill_insert(v._M_start + 5, 3, x);                         // elems_after 1 <= 3
    const float w2[] = { 0, 9, 9, 1, 2, 9, 9, 9, 3 };
    if (!expect(v, w2, 9)) { std::printf("007d24f0_TEST FAIL\n"); return 1; }
    v.fill_insert(v._M_start, 0, x);
    v.fill_insert(v._M_start, 2, x);                             // no room: overflow path
    if (!v.overflowed || !expect(v, w2, 9)) { std::printf("007d24f0_TEST FAIL\n"); return 1; }
    for (Region* p = v._M_start; p != v._M_finish; ++p) p->~Region();
    std::free(v._M_start);
    std::printf("007d24f0_TEST PASS\n");
    return 0;
}