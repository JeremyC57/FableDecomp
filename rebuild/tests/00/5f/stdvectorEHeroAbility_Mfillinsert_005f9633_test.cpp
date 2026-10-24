// Behaviour test for 0x005F9633 std::vector<EHeroAbility>::_M_fill_insert(pos, n, x): in-place when the spare capacity
// holds n (both the elems_after > n and the elems_after <= n arms), else _M_insert_overflow (0x005F9495, modelled).
#include <cstdio>
#include <cstdlib>
#include <new>

enum EHeroAbility { ABILITY_NULL = 0, ABILITY_A = 1, ABILITY_B = 2, ABILITY_C = 3, ABILITY_D = 4, ABILITY_X = 9,
                    EHeroAbility_Force32 = 0x7fffffff };

namespace stl {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

EHeroAbility* __uninitialized_copy(EHeroAbility* __first, EHeroAbility* __last, EHeroAbility* __result, const __false_type&)
{
    EHeroAbility* __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        new (__cur) EHeroAbility(*__first);
    return __cur;
}
EHeroAbility* __copy_backward(EHeroAbility* __first, EHeroAbility* __last, EHeroAbility* __result,
                              const random_access_iterator_tag&, int*)
{
    for (int __n = int(__last - __first); __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
EHeroAbility* uninitialized_fill_n(EHeroAbility* __first, unsigned int __n, const EHeroAbility& __x)
{
    EHeroAbility* __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        new (__cur) EHeroAbility(__x);
    return __cur;
}
inline EHeroAbility* __copy_backward_ptrs(EHeroAbility* __first, EHeroAbility* __last, EHeroAbility* __result,
                                          const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _ForwardIter, class _Tp> inline void fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for (; __first != __last; ++__first)
        *__first = __val;
}

template <class _Tp> class vector {
public:
    typedef _Tp* pointer;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;

    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, unsigned int __fill_len,
                            bool __atend = false)
    {
        unsigned int __old_size = (unsigned int)(_M_finish - _M_start);
        unsigned int __len = __old_size + (__old_size < __fill_len ? __fill_len : __old_size);
        pointer __new_start = (pointer)std::malloc(__len * sizeof(_Tp));
        pointer __new_finish = __uninitialized_copy(_M_start, __position, __new_start, __false_type());
        __new_finish = uninitialized_fill_n(__new_finish, __fill_len, __x);
        if (!__atend)
            __new_finish = __uninitialized_copy(__position, _M_finish, __new_finish, __false_type());
        std::free(_M_start);
        _M_start = __new_start;
        _M_finish = __new_finish;
        _M_end_of_storage = __new_start + __len;
    }
    void _M_fill_insert(pointer __position, unsigned int __n, const _Tp& __x)
    {
        if (__n != 0) {
            if ((unsigned int)(_M_end_of_storage - _M_finish) >= __n) {
                _Tp __x_copy = __x;
                const unsigned int __elems_after = (unsigned int)(_M_finish - __position);
                pointer __old_finish = _M_finish;
                if (__elems_after > __n) {
                    __uninitialized_copy(_M_finish - __n, _M_finish, _M_finish, __false_type());
                    _M_finish += __n;
                    __copy_backward_ptrs(__position, __old_finish - __n, __old_finish, __false_type());
                    fill(__position, __position + __n, __x_copy);
                }
                else {
                    uninitialized_fill_n(_M_finish, __n - __elems_after, __x_copy);
                    _M_finish += __n - __elems_after;
                    __uninitialized_copy(__position, __old_finish, _M_finish, __false_type());
                    _M_finish += __elems_after;
                    fill(__position, __old_finish, __x_copy);
                }
            }
            else
                _M_insert_overflow(__position, __x, __false_type(), __n);
        }
    }
};
}

typedef stl::vector<EHeroAbility> V;

static void Make(V& v, int n, int cap)
{
    v._M_start = (EHeroAbility*)std::malloc(cap * sizeof(EHeroAbility));
    v._M_finish = v._M_start + n;
    v._M_end_of_storage = v._M_start + cap;
    for (int i = 0; i < n; ++i) v._M_start[i] = (EHeroAbility)(i + 1);
}

static int Same(const V& v, const int* want, int n)
{
    if (v._M_finish - v._M_start != n) return 0;
    for (int i = 0; i < n; ++i)
        if (v._M_start[i] != (EHeroAbility)want[i]) return 0;
    return 1;
}

int main()
{
    int ok = 1;
    V v;
    EHeroAbility x = ABILITY_X;
    // elems_after (3) > n (2): tail moved up, hole filled
    Make(v, 4, 8);
    v._M_fill_insert(v._M_start + 1, 2, x);
    { int w[6] = { 1, 9, 9, 2, 3, 4 }; if (!Same(v, w, 6) || v._M_end_of_storage - v._M_start != 8) ok = 0; }
    std::free(v._M_start);
    // elems_after (1) <= n (3): the overflow part of the fill goes past the old end first
    Make(v, 4, 8);
    v._M_fill_insert(v._M_start + 3, 3, x);
    { int w[7] = { 1, 2, 3, 9, 9, 9, 4 }; if (!Same(v, w, 7)) ok = 0; }
    std::free(v._M_start);
    // no room: reallocation through _M_insert_overflow
    Make(v, 3, 3);
    v._M_fill_insert(v._M_start, 2, x);
    { int w[5] = { 9, 9, 1, 2, 3 }; if (!Same(v, w, 5) || v._M_end_of_storage - v._M_start != 6) ok = 0; }
    // n == 0: nothing happens
    EHeroAbility* before = v._M_start;
    v._M_fill_insert(v._M_start, 0, x);
    if (v._M_start != before || v._M_finish - v._M_start != 5) ok = 0;
    std::free(v._M_start);
    if (!ok) { std::printf("005f9633_TEST FAIL\n"); return 1; }
    std::printf("005f9633_TEST PASS\n");
    return 0;
}