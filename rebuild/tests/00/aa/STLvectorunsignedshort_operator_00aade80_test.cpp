// Behaviour test for _STL::vector<unsigned short>::operator=(const vector&) @ 0x00AADE80
#include <cstdio>
#include <cstdlib>
#include <cstring>

struct CUShortVector {
    unsigned short* _M_start;
    unsigned short* _M_finish;
    unsigned short* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    unsigned int capacity() const { return (unsigned int)(_M_end_of_storage - _M_start); }
    static unsigned short* copy_trivial(const unsigned short* first, const unsigned short* last, unsigned short* result)
    {
        if (last == first) return result;
        std::memmove(result, first, (last - first) * sizeof(unsigned short));
        return result + (last - first);
    }
    CUShortVector& operator=(const CUShortVector& x)
    {
        if (&x != this) {
            const unsigned int xlen = x.size();
            if (xlen > capacity()) {
                unsigned short* tmp = xlen != 0 ? (unsigned short*)std::malloc(xlen * sizeof(unsigned short)) : 0;
                copy_trivial(x._M_start, x._M_finish, tmp);
                if (_M_start) std::free(_M_start);
                _M_start = tmp;
                _M_end_of_storage = _M_start + xlen;
            }
            else if (size() >= xlen)
                copy_trivial(x._M_start, x._M_finish, _M_start);
            else {
                copy_trivial(x._M_start, x._M_start + size(), _M_start);
                copy_trivial(x._M_start + size(), x._M_finish, _M_finish);
            }
            _M_finish = _M_start + xlen;
        }
        return *this;
    }
};

static CUShortVector make(unsigned int n, unsigned int cap, unsigned short base)
{
    CUShortVector v;
    v._M_start = (unsigned short*)std::malloc((cap ? cap : 1) * sizeof(unsigned short));
    v._M_finish = v._M_start + n;
    v._M_end_of_storage = v._M_start + cap;
    for (unsigned int i = 0; i < n; ++i) v._M_start[i] = (unsigned short)(base + i);
    return v;
}

static int same(const CUShortVector& a, const CUShortVector& b)
{
    if (a.size() != b.size()) return 0;
    for (unsigned int i = 0; i < a.size(); ++i) if (a._M_start[i] != b._M_start[i]) return 0;
    return 1;
}

int main()
{
    int ok = 1;
    CUShortVector src = make(5, 5, 100);
    CUShortVector grow = make(2, 2, 1);          // reallocate path
    grow = src;
    if (!same(grow, src) || grow.capacity() != 5) ok = 0;
    CUShortVector shrink = make(8, 8, 1);        // size() >= xlen path
    shrink = src;
    if (!same(shrink, src) || shrink.capacity() != 8) ok = 0;
    CUShortVector middle = make(3, 6, 1);        // fits capacity, longer than size()
    middle = src;
    if (!same(middle, src) || middle.capacity() != 6) ok = 0;
    CUShortVector self = make(4, 4, 9);
    self = self;
    if (self.size() != 4 || self._M_start[3] != 12) ok = 0;
    std::free(src._M_start); std::free(grow._M_start); std::free(shrink._M_start);
    std::free(middle._M_start); std::free(self._M_start);
    if (!ok) { std::printf("00aade80_TEST FAIL\n"); return 1; }
    std::printf("00aade80_TEST PASS\n");
    return 0;
}