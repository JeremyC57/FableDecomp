// Behaviour test for 0x00460EB5 std::vector<CMusicSetEntry>::operator= (STLport assignment shape).
#include <cstdio>
#include <cstdlib>
#include <new>

class CMusicSetEntry { public: long NormalMusicID; long DangerMusicID; long LoopCount; };

struct CMusicSetEntryVector {
    typedef CMusicSetEntry* pointer;
    typedef const CMusicSetEntry* const_pointer;
    pointer _M_start, _M_finish, _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    unsigned int capacity() const { return (unsigned int)(_M_end_of_storage - _M_start); }
    int allocations;
    pointer _M_allocate_and_copy(unsigned int n, const_pointer first, const_pointer last)
    {
        ++allocations;
        pointer p = (pointer)std::malloc(n * sizeof(CMusicSetEntry));
        for (unsigned int i = 0; first != last; ++first, ++i) p[i] = *first;
        return p;
    }
    void _M_clear() { std::free(_M_start); }
    CMusicSetEntryVector& operator=(const CMusicSetEntryVector& x)
    {
        if (&x != this) {
            const unsigned int xlen = x.size();
            if (xlen > capacity()) {
                pointer tmp = _M_allocate_and_copy(xlen, x._M_start, x._M_finish);
                _M_clear();
                _M_start = tmp;
                _M_end_of_storage = _M_start + xlen;
            } else if (size() >= xlen) {
                pointer d = _M_start;
                const_pointer f = x._M_start;
                for (int n = (int)(x._M_finish - x._M_start); n > 0; --n) { *d = *f; ++f; ++d; }
            } else {
                unsigned int s = size();
                for (unsigned int i = 0; i < s; ++i) _M_start[i] = x._M_start[i];
                const_pointer f = x._M_start + s;
                pointer cur = _M_finish;
                for (; f != x._M_finish; ++f, ++cur) new (cur) CMusicSetEntry(*f);
            }
            _M_finish = _M_start + xlen;
        }
        return *this;
    }
};

static void Make(CMusicSetEntryVector& v, unsigned int n, unsigned int cap, long base)
{
    v._M_start = (CMusicSetEntry*)std::malloc((cap ? cap : 1) * sizeof(CMusicSetEntry));
    v._M_finish = v._M_start + n;
    v._M_end_of_storage = v._M_start + cap;
    v.allocations = 0;
    for (unsigned int i = 0; i < n; ++i) { v._M_start[i].NormalMusicID = base + i; v._M_start[i].DangerMusicID = base + 100 + i; v._M_start[i].LoopCount = (long)i; }
}

static bool Same(const CMusicSetEntryVector& a, const CMusicSetEntryVector& b)
{
    if (a.size() != b.size()) return false;
    for (unsigned int i = 0; i < a.size(); ++i)
        if (a._M_start[i].NormalMusicID != b._M_start[i].NormalMusicID || a._M_start[i].DangerMusicID != b._M_start[i].DangerMusicID
            || a._M_start[i].LoopCount != b._M_start[i].LoopCount) return false;
    return true;
}

int main()
{
    CMusicSetEntryVector src, grow, shrink, tail;
    Make(src, 4, 4, 10);
    Make(grow, 1, 2, 50);       // capacity 2 < 4: reallocate
    Make(shrink, 6, 6, 70);     // size 6 >= 4: copy over
    Make(tail, 2, 5, 90);       // size 2 < 4 <= capacity 5: copy 2, construct 2
    grow = src; shrink = src; tail = src;
    bool ok = Same(grow, src) && grow.allocations == 1 && grow.capacity() == 4
           && Same(shrink, src) && shrink.allocations == 0 && shrink.capacity() == 6
           && Same(tail, src) && tail.allocations == 0 && tail.capacity() == 5;
    CMusicSetEntryVector& self = (src = src);
    ok = ok && &self == &src && src.size() == 4;
    if (!ok) { std::printf("00460eb5_TEST FAIL\n"); return 1; }
    std::printf("00460eb5_TEST PASS\n");
    return 0;
}