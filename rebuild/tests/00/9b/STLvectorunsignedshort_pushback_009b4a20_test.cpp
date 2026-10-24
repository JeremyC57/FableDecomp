// Behaviour test for 0x009B4A20 vector<unsigned short>::push_back: with spare capacity the element is constructed at
// _M_finish and _M_finish advances by one element (2 bytes); when full, _M_insert_overflow(pos, x, tag, 1, true) runs.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

struct __true_type { __true_type() {} };

static int g_overflowCalls = 0;
template <class _Tp> class vec {
public:
    _Tp* _M_start; _Tp* _M_finish; _Tp* _M_end_of_storage;
    void _M_insert_overflow(_Tp* pos, const _Tp& x, const __true_type&, unsigned int fill_len, bool atend)
    {
        ++g_overflowCalls;
        if (!atend || fill_len != 1 || pos != _M_finish) return;
        unsigned int n = (unsigned int)(_M_finish - _M_start);
        unsigned int cap = n ? n * 2 : 1;
        _Tp* p = (_Tp*)std::malloc(cap * sizeof(_Tp));
        if (n) std::memcpy(p, _M_start, n * sizeof(_Tp));
        p[n] = x;
        std::free(_M_start);
        _M_start = p; _M_finish = p + n + 1; _M_end_of_storage = p + cap;
    }
    void push_back(const _Tp& x)
    {
        if (_M_finish != _M_end_of_storage) { new (_M_finish) _Tp(x); ++_M_finish; }
        else _M_insert_overflow(_M_finish, x, __true_type(), 1UL, true);
    }
};

int main()
{
    vec<unsigned short> v; v._M_start = v._M_finish = v._M_end_of_storage = 0;
    for (unsigned short i = 0; i < 9; ++i) v.push_back((unsigned short)(0xFFF0 + i));
    if (v._M_finish - v._M_start != 9) { std::printf("009b4a20_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 9; ++i) if (v._M_start[i] != (unsigned short)(0xFFF0 + i)) { std::printf("009b4a20_TEST FAIL\n"); return 1; }
    // capacity growth 1,2,4,8,16: overflow at sizes 0,1,2,4,8 = 5 calls
    if (g_overflowCalls != 5) { std::printf("009b4a20_TEST FAIL\n"); return 1; }
    // in-place path advances by exactly 2 bytes
    char* before = (char*)v._M_finish;
    v.push_back(7);
    if ((char*)v._M_finish - before != 2 || g_overflowCalls != 5 || v._M_start[9] != 7) { std::printf("009b4a20_TEST FAIL\n"); return 1; }
    std::free(v._M_start);
    std::printf("009b4a20_TEST PASS\n");
    return 0;
}