// Behaviour test for 0x00462B4E std::vector<CBrainBehaviour>::operator= (STLport assignment shape).
#include <cstdio>
#include <cstdlib>
#include <new>

class CBrainBehaviour { public: long Name; long Priority; long ScriptAIPriority; };

struct CBrainBehaviourVector {
    typedef CBrainBehaviour* pointer;
    typedef const CBrainBehaviour* const_pointer;
    pointer _M_start, _M_finish, _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    unsigned int capacity() const { return (unsigned int)(_M_end_of_storage - _M_start); }
    int allocations;
    pointer _M_allocate_and_copy(unsigned int n, const_pointer first, const_pointer last)
    {
        ++allocations;
        pointer p = (pointer)std::malloc(n * sizeof(CBrainBehaviour));
        for (unsigned int i = 0; first != last; ++first, ++i) p[i] = *first;
        return p;
    }
    void _M_clear() { std::free(_M_start); }
    CBrainBehaviourVector& operator=(const CBrainBehaviourVector& x)
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
                for (; f != x._M_finish; ++f, ++cur) new (cur) CBrainBehaviour(*f);
            }
            _M_finish = _M_start + xlen;
        }
        return *this;
    }
};

static void Make(CBrainBehaviourVector& v, unsigned int n, unsigned int cap, long base)
{
    v._M_start = (CBrainBehaviour*)std::malloc((cap ? cap : 1) * sizeof(CBrainBehaviour));
    v._M_finish = v._M_start + n;
    v._M_end_of_storage = v._M_start + cap;
    v.allocations = 0;
    for (unsigned int i = 0; i < n; ++i) { v._M_start[i].Name = base + i; v._M_start[i].Priority = base + 100 + i; v._M_start[i].ScriptAIPriority = (long)i; }
}

static bool Same(const CBrainBehaviourVector& a, const CBrainBehaviourVector& b)
{
    if (a.size() != b.size()) return false;
    for (unsigned int i = 0; i < a.size(); ++i)
        if (a._M_start[i].Name != b._M_start[i].Name || a._M_start[i].Priority != b._M_start[i].Priority
            || a._M_start[i].ScriptAIPriority != b._M_start[i].ScriptAIPriority) return false;
    return true;
}

int main()
{
    CBrainBehaviourVector src, grow, shrink, tail;
    Make(src, 4, 4, 10);
    Make(grow, 1, 2, 50);       // capacity 2 < 4: reallocate
    Make(shrink, 6, 6, 70);     // size 6 >= 4: copy over
    Make(tail, 2, 5, 90);       // size 2 < 4 <= capacity 5: copy 2, construct 2
    grow = src; shrink = src; tail = src;
    bool ok = Same(grow, src) && grow.allocations == 1 && grow.capacity() == 4
           && Same(shrink, src) && shrink.allocations == 0 && shrink.capacity() == 6
           && Same(tail, src) && tail.allocations == 0 && tail.capacity() == 5;
    CBrainBehaviourVector& self = (src = src);
    ok = ok && &self == &src && src.size() == 4;
    if (!ok) { std::printf("00462b4e_TEST FAIL\n"); return 1; }
    std::printf("00462b4e_TEST PASS\n");
    return 0;
}