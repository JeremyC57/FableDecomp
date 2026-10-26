// Behaviour test for 0x007DC3F0 std::vector<CCard*>::operator= (STLport POD assignment).
#include <cstdio>
#include <cstdlib>
#include <cstring>

class CCard { public: int v; };

struct CardVec {
    CCard** _M_start;
    CCard** _M_finish;
    CCard** _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    unsigned int capacity() const { return (unsigned int)(_M_end_of_storage - _M_start); }
    static CCard** copy_trivial(CCard* const* first, CCard* const* last, CCard** result)
    {
        if (last == first) return result;
        std::memmove(result, first, (last - first) * sizeof(CCard*));
        return result + (last - first);
    }
    CardVec& assign(const CardVec& x)
    {
        if (&x != this) {
            const unsigned int xlen = x.size();
            if (xlen > capacity()) {
                CCard** tmp = xlen != 0 ? (CCard**)std::malloc(xlen * sizeof(CCard*)) : 0;
                copy_trivial(x._M_start, x._M_finish, tmp);
                if (_M_start) std::free(_M_start);
                _M_start = tmp;
                _M_end_of_storage = _M_start + xlen;
            } else if (size() >= xlen) {
                copy_trivial(x._M_start, x._M_finish, _M_start);
            } else {
                copy_trivial(x._M_start, x._M_start + size(), _M_start);
                copy_trivial(x._M_start + size(), x._M_finish, _M_finish);
            }
            _M_finish = _M_start + xlen;
        }
        return *this;
    }
};

static bool same(const CardVec& a, const CardVec& b)
{
    if (a.size() != b.size()) return false;
    for (unsigned int i = 0; i < a.size(); ++i)
        if (a._M_start[i] != b._M_start[i]) return false;
    return true;
}

static void make(CardVec& v, CCard* cards, int n, int cap)
{
    v._M_start = cap ? (CCard**)std::malloc(cap * sizeof(CCard*)) : 0;
    for (int i = 0; i < n; ++i) v._M_start[i] = &cards[i];
    v._M_finish = v._M_start + n;
    v._M_end_of_storage = v._M_start + cap;
}

int main()
{
    CCard cards[8];
    CardVec a, b, c, d;
    make(a, cards, 5, 5);
    make(b, cards + 3, 2, 3);      // grow path
    make(c, cards + 1, 6, 8);      // shrink path
    make(d, cards + 2, 3, 8);      // append path
    b.assign(a);
    c.assign(a);
    d.assign(a);
    if (!same(a, b) || !same(a, c) || !same(a, d) || b.capacity() != 5 || c.capacity() != 8) {
        std::printf("007dc3f0_TEST FAIL\n"); return 1;
    }
    if (&a.assign(a) != &a || a.size() != 5) { std::printf("007dc3f0_TEST FAIL\n"); return 1; }
    std::free(a._M_start); std::free(b._M_start); std::free(c._M_start); std::free(d._M_start);
    std::printf("007dc3f0_TEST PASS\n");
    return 0;
}