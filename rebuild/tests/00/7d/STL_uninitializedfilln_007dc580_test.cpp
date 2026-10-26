// Behaviour test for 0x007DC580 __uninitialized_fill_n<vector<CCard*>*, unsigned int, vector<CCard*> >: n independent
// deep copies of one hand; n == 0 constructs nothing.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

class CCard { public: int id; };
struct CardVector {
    CCard** _M_start; CCard** _M_finish; CCard** _M_end_of_storage;
    CardVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    CardVector(const CardVector& x)
    {
        unsigned int n = (unsigned int)(x._M_finish - x._M_start);
        _M_start = n ? (CCard**)std::malloc(n * sizeof(CCard*)) : 0;
        _M_finish = _M_start; _M_end_of_storage = _M_start + n;
        if (x._M_finish != x._M_start) {
            std::memmove(_M_start, x._M_start, (char*)x._M_finish - (char*)x._M_start);
            _M_finish = _M_start + n;
        }
    }
};
struct __false_type { };
template <class F, class S, class T> F __uninitialized_fill_n(F first, S n, const T& x, const __false_type&)
{
    F cur = first;
    for (; n > 0; --n, ++cur)
        new (&*cur) T(x);
    return cur;
}

int main()
{
    CCard c[2]; c[0].id = 7; c[1].id = 8;
    CCard* cards[2] = { &c[0], &c[1] };
    CardVector hand; hand._M_start = cards; hand._M_finish = cards + 2; hand._M_end_of_storage = cards + 2;
    CardVector* raw = (CardVector*)std::malloc(3 * sizeof(CardVector));
    CardVector* end = __uninitialized_fill_n(raw, 3u, hand, __false_type());
    bool ok = end == raw + 3;
    for (int i = 0; i < 3 && ok; ++i)
        ok = raw[i]._M_finish - raw[i]._M_start == 2 && raw[i]._M_start[0] == &c[0] && raw[i]._M_start[1] == &c[1] && raw[i]._M_start != cards;
    ok = ok && raw[0]._M_start != raw[1]._M_start;
    ok = ok && __uninitialized_fill_n(raw, 0u, hand, __false_type()) == raw;
    for (int i = 0; i < 3; ++i) std::free(raw[i]._M_start);
    std::free(raw);
    if (!ok) { std::printf("007dc580_TEST FAIL\n"); return 1; }
    std::printf("007dc580_TEST PASS\n");
    return 0;
}