// Behaviour test for 0x005F9495 std::vector<EHeroAbility>::_M_insert_overflow(pos, x, __false_type, fill_len, atend):
// grow to old + max(old, fill_len), copy the head, place fill_len copies of x, copy the tail unless atend, free the old block.
#include <cstdio>
#include <cstdlib>
#include <cstddef>
#include <new>

enum EHeroAbility { ABILITY_NULL = 0, ABILITY_A = 3, ABILITY_B = 7, ABILITY_C = 11, ABILITY_X = 99,
                    EHeroAbility_Force32 = 0x7fffffff };

namespace stl {
struct __false_type { __false_type() { } };

EHeroAbility* __uninitialized_copy(const EHeroAbility* __first, const EHeroAbility* __last, EHeroAbility* __result,
                                   const __false_type&)
{
    EHeroAbility* __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        new (__cur) EHeroAbility(*__first);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        new (__cur) _Tp(__x);
    return __cur;
}

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
template <class _T1, class _T2> inline void _Copy_Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

static int g_frees = 0;
template <class _Tp> struct allocator {
    _Tp* allocate(size_t __n, const void* = 0) { return __n != 0 ? (_Tp*)std::malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, size_t) { if (__p) { ++g_frees; std::free(__p); } }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
struct _STLP_alloc_proxy : public _MaybeReboundAlloc {
    _Value _M_data;
};

template <class _Tp> class vector {
public:
    typedef _Tp* pointer;
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, allocator<_Tp> > _M_end_of_storage;
    size_t size() const { return size_t(_M_finish - _M_start); }
    void _M_clear() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        _M_start = __s;
        _M_finish = __f;
        _M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_t __fill_len,
                            bool __atend = false)
    {
        const size_t __old_size = size();
        const size_t __len = __old_size + (max)(__old_size, __fill_len);
        pointer __new_start = _M_end_of_storage.allocate(__len);
        pointer __new_finish = __new_start;
        __new_finish = __uninitialized_copy(_M_start, __position, __new_start, __false_type());
        if (__fill_len == 1) {
            _Copy_Construct(__new_finish, __x);
            ++__new_finish;
        }
        else
            __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
        if (!__atend)
            __new_finish = __uninitialized_copy(__position, _M_finish, __new_finish, __false_type());
        _M_clear();
        _M_set(__new_start, __new_finish, __new_start + __len);
    }
};
}

typedef stl::vector<EHeroAbility> V;

static void Make(V& v, int n)
{
    v._M_start = (EHeroAbility*)std::malloc(n * sizeof(EHeroAbility));
    v._M_finish = v._M_start + n;
    v._M_end_of_storage._M_data = v._M_finish;
    EHeroAbility vals[3] = { ABILITY_A, ABILITY_B, ABILITY_C };
    for (int i = 0; i < n; ++i) v._M_start[i] = vals[i % 3];
}

int main()
{
    int ok = 1;
    V v;
    // push_back shape: one element at the end of a full 3-vector -> capacity 6
    Make(v, 3);
    EHeroAbility x = ABILITY_X;
    v._M_insert_overflow(v._M_finish, x, stl::__false_type(), 1, true);
    if (v.size() != 4 || v._M_end_of_storage._M_data - v._M_start != 6 || v._M_start[0] != ABILITY_A ||
        v._M_start[2] != ABILITY_C || v._M_start[3] != ABILITY_X || stl::g_frees != 1) ok = 0;
    std::free(v._M_start);
    // fill insert of 5 in the middle of 3 -> capacity 3 + 5 = 8, tail copied after the fill
    Make(v, 3);
    v._M_insert_overflow(v._M_start + 1, x, stl::__false_type(), 5, false);
    if (v.size() != 8 || v._M_end_of_storage._M_data - v._M_start != 8 || v._M_start[0] != ABILITY_A ||
        v._M_start[1] != ABILITY_X || v._M_start[5] != ABILITY_X || v._M_start[6] != ABILITY_B ||
        v._M_start[7] != ABILITY_C || stl::g_frees != 2) ok = 0;
    std::free(v._M_start);
    // empty vector: nothing to free, one slot allocated
    v._M_start = v._M_finish = v._M_end_of_storage._M_data = 0;
    v._M_insert_overflow(v._M_finish, x, stl::__false_type(), 1, true);
    if (v.size() != 1 || v._M_start[0] != ABILITY_X || stl::g_frees != 2) ok = 0;
    std::free(v._M_start);
    if (!ok) { std::printf("005f9495_TEST FAIL\n"); return 1; }
    std::printf("005f9495_TEST PASS\n");
    return 0;
}