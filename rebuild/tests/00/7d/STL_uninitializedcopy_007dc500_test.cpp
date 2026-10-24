// Behaviour test for 0x007DC500 __uninitialized_copy<const vector<CCard*>*, vector<CCard*>*>: each hand is deep-copied
// (own storage of exactly the source size, same card pointers; empty hands get no storage).
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

struct CCard { int id; };
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
template <class I, class F> F __uninitialized_copy(I first, I last, F result, const __false_type&)
{
    F cur = result;
    for (; first != last; ++first, ++cur)
        new (&*cur) CardVector(*first);
    return cur;
}

int main()
{
    CCard c[3]; for (int i = 0; i < 3; ++i) c[i].id = i;
    CCard* cards[3] = { &c[0], &c[1], &c[2] };
    CardVector src[3];
    src[0]._M_start = cards; src[0]._M_finish = cards + 2; src[0]._M_end_of_storage = cards + 3;
    src[2]._M_start = cards + 2; src[2]._M_finish = cards + 3; src[2]._M_end_of_storage = cards + 3;
    CardVector* raw = (CardVector*)std::malloc(3 * sizeof(CardVector));
    const CardVector* first = src;
    CardVector* end = __uninitialized_copy(first, first + 3, raw, __false_type());
    bool ok = end == raw + 3
        && raw[0]._M_finish - raw[0]._M_start == 2 && raw[0]._M_start != cards && raw[0]._M_start[0] == &c[0] && raw[0]._M_start[1] == &c[1]
        && raw[0]._M_end_of_storage == raw[0]._M_finish
        && raw[1]._M_start == 0 && raw[1]._M_finish == 0 && raw[1]._M_end_of_storage == 0
        && raw[2]._M_finish - raw[2]._M_start == 1 && raw[2]._M_start[0] == &c[2];
    std::free(raw[0]._M_start); std::free(raw[2]._M_start); std::free(raw);
    if (!ok) { std::printf("007dc500_TEST FAIL\n"); return 1; }
    std::printf("007dc500_TEST PASS\n");
    return 0;
}