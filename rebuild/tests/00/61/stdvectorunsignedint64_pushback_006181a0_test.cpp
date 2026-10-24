// Behaviour test for 0x006181A0 std::vector<unsigned __int64>::push_back(x): copy into the spare slot, else
// _M_insert_overflow(finish, x, __true_type(), 1, true) (0x00617370, modelled as a doubling grow that records its call).
#include <cstdio>
#include <cstdlib>
#include <cstddef>
#include <new>

#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif

namespace stl {
struct __true_type { __true_type() { } };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> struct allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
struct _STLP_alloc_proxy : public _MaybeReboundAlloc { _Value _M_data; };

static int g_overflows = 0;
template <class _Tp> class vector {
public:
    typedef _Tp* pointer;
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, allocator<_Tp> > _M_end_of_storage;
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __true_type&, size_t __fill_len, bool __atend = false)
    {
        ++g_overflows;
        if (__position != _M_finish || __fill_len != 1 || !__atend) return;
        size_t old = size_t(_M_finish - _M_start), len = old ? 2 * old : 1;
        pointer s = (pointer)std::malloc(len * sizeof(_Tp));
        for (size_t i = 0; i < old; ++i) s[i] = _M_start[i];
        s[old] = __x;
        std::free(_M_start);
        _M_start = s; _M_finish = s + old + 1; _M_end_of_storage._M_data = s + len;
    }
    void push_back(const _Tp& __x)
    {
        if (_M_finish != _M_end_of_storage._M_data) {
            _Construct(_M_finish, __x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, __x, __true_type(), 1UL, true);
    }
};
}

int main()
{
    int ok = 1;
    stl::vector<u64> v;
    v._M_start = v._M_finish = v._M_end_of_storage._M_data = 0;
    u64 vals[5];
    for (int i = 0; i < 5; ++i) {
        vals[i] = ((u64)(i + 1) << 33) | (u64)(i * 3);
        v.push_back(vals[i]);
    }
    // growth at sizes 0, 1, 2, 4 -> four overflow calls, the fifth (size 4 -> 5 needs room) is the fourth; capacity 8
    if (stl::g_overflows != 4 || v._M_finish - v._M_start != 5 || v._M_end_of_storage._M_data - v._M_start != 8) ok = 0;
    for (int i = 0; i < 5; ++i)
        if (v._M_start[i] != vals[i]) ok = 0;
    v.push_back(vals[0]);   // in place
    if (stl::g_overflows != 4 || v._M_start[5] != vals[0]) ok = 0;
    std::free(v._M_start);
    if (!ok) { std::printf("006181a0_TEST FAIL\n"); return 1; }
    std::printf("006181a0_TEST PASS\n");
    return 0;
}