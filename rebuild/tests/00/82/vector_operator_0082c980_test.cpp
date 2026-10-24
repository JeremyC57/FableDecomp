// Behaviour test: vector<CMinimapData::MinimapTheme>::operator= -- reallocate when the source outgrows the capacity, else assign over the elements and copy-construct the tail
#include <cstdio>
#include <cstdlib>

enum MinimapTheme { THEME_0, THEME_1, THEME_2, THEME_3, THEME_4, THEME_5 };

struct CThemeVector {
    MinimapTheme* _M_start;
    MinimapTheme* _M_finish;
    MinimapTheme* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    unsigned int capacity() const { return (unsigned int)(_M_end_of_storage - _M_start); }
    MinimapTheme* _M_allocate_and_copy(unsigned int n, const MinimapTheme* first, const MinimapTheme* last)   // 0x0082C450
    {
        MinimapTheme* result = n ? (MinimapTheme*)std::malloc(n * sizeof(MinimapTheme)) : 0;
        MinimapTheme* cur = result;
        for (; first != last; ++first, ++cur)
            *cur = *first;
        return result;
    }
    CThemeVector& operator=(const CThemeVector& x)
    {
        if (&x != this) {
            const unsigned int xlen = x.size();
            if (xlen > capacity()) {
                MinimapTheme* tmp = _M_allocate_and_copy(xlen, x._M_start, x._M_finish);
                if (_M_start)
                    std::free(_M_start);
                _M_start = tmp;
                _M_end_of_storage = _M_start + xlen;
            } else if (size() >= xlen) {
                for (int n = (int)(x._M_finish - x._M_start), i = 0; n > 0; --n, ++i)
                    _M_start[i] = x._M_start[i];
            } else {
                for (int n = (int)size(), i = 0; n > 0; --n, ++i)
                    _M_start[i] = x._M_start[i];
                MinimapTheme* cur = _M_finish;
                for (const MinimapTheme* p = x._M_start + size(); p != x._M_finish; ++p, ++cur)
                    *cur = *p;                           // __uninitialized_copy 0x0082C420
            }
            _M_finish = _M_start + xlen;
        }
        return *this;
    }
};

static void Fill(CThemeVector& v, unsigned int cap, unsigned int n, int base)
{
    v._M_start = (MinimapTheme*)std::malloc(cap * sizeof(MinimapTheme));
    v._M_end_of_storage = v._M_start + cap;
    for (unsigned int i = 0; i < n; ++i)
        v._M_start[i] = MinimapTheme((base + i) % 6);
    v._M_finish = v._M_start + n;
}

int main()
{
    CThemeVector a, b, c, d;
    Fill(a, 2, 1, 0);
    Fill(b, 5, 4, 1);
    a = b;                                               // 4 > capacity 2: reallocated
    if (a.size() != 4 || a.capacity() != 4 || a._M_start[0] != THEME_1 || a._M_start[3] != THEME_4)
        { std::printf("0082c980_TEST FAIL\n"); return 1; }
    Fill(c, 6, 5, 2);
    c = b;                                               // shrink in place
    if (c.size() != 4 || c.capacity() != 6 || c._M_start[0] != THEME_1 || c._M_start[3] != THEME_4)
        { std::printf("0082c980_TEST FAIL\n"); return 1; }
    Fill(d, 6, 2, 3);
    d = b;                                               // grow within the capacity: assign 2, construct 2
    if (d.size() != 4 || d.capacity() != 6 || d._M_start[1] != THEME_2 || d._M_start[3] != THEME_4)
        { std::printf("0082c980_TEST FAIL\n"); return 1; }
    b = b;                                               // self-assignment: no change
    if (b.size() != 4 || b._M_start[0] != THEME_1)
        { std::printf("0082c980_TEST FAIL\n"); return 1; }
    std::free(a._M_start); std::free(b._M_start); std::free(c._M_start); std::free(d._M_start);
    std::printf("0082c980_TEST PASS\n");
    return 0;
}