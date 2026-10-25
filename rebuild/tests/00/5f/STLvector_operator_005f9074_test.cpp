// Behaviour test for 005f9074 (_STL::vector<pair<long, EHeroAbility> >::operator=): self-assignment is a no-op; a longer source
// reallocates (allocate-and-copy, old block freed, capacity = source length); a shorter source is copied over the head; a
// source between size() and capacity() copies the head and copy-constructs the rest in place.
#include <cstdio>
#include <cstdlib>
#include <new>
enum EHeroAbility { HERO_ABILITY_NULL, HERO_ABILITY_99 = 99 };
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair() : first(), second() { }
    pair(const T1& a, const T2& b) : first(a), second(b) { }
    pair(const pair& o) : first(o.first), second(o.second) { }
};
typedef pair<long, EHeroAbility> T;
static int g_allocs = 0;
static int g_frees = 0;
struct __false_type { __false_type() { } };
template <class _Tp>
_Tp* __copy_ptrs(const _Tp* __first, const _Tp* __last, _Tp* __result, const __false_type&)
{
    for (long __n = (long)(__last - __first); __n > 0; --__n, ++__first, ++__result)
        *__result = *__first;
    return __result;
}
template <class _Tp>
_Tp* __uninitialized_copy(const _Tp* __first, const _Tp* __last, _Tp* __result, const __false_type&)
{
    for (; __first != __last; ++__first, ++__result)
        new (__result) _Tp(*__first);
    return __result;
}
template <class _Tp> void _Destroy(_Tp*, _Tp*) { }
struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    explicit vector(unsigned int cap) : _M_start((T*)std::malloc(sizeof(T) * (cap ? cap : 1))), _M_finish(_M_start), _M_end_of_storage(_M_start + cap) { ++g_allocs; }
    ~vector() { std::free(_M_start); ++g_frees; }
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    unsigned int capacity() const { return (unsigned int)(this->_M_end_of_storage - this->_M_start); }
    void push(long k) { new (this->_M_finish) T(k, (EHeroAbility)(k % 100)); ++this->_M_finish; }
    T* _M_allocate_and_copy(unsigned int __n, const T* __first, const T* __last)   // stands in for 0x005F8BD8
    {
        T* __result = (T*)std::malloc(sizeof(T) * __n);
        ++g_allocs;
        __uninitialized_copy(__first, __last, __result, __false_type());
        return __result;
    }
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        std::free(this->_M_start);                                         // the retail deallocate is free 0x00BFEA14
        ++g_frees;
    }
    vector& operator=(const vector& __x)
    {
        if (&__x != this) {
            const unsigned int __xlen = __x.size();
            if (__xlen > capacity()) {
                T* __tmp = _M_allocate_and_copy(__xlen, (const T*)__x._M_start + 0, (const T*)__x._M_finish + 0);
                _M_clear();
                this->_M_start = __tmp;
                this->_M_end_of_storage = this->_M_start + __xlen;
            }
            else if (size() >= __xlen) {
                T* __i = __copy_ptrs((const T*)__x._M_start + 0, (const T*)__x._M_finish + 0, this->_M_start, __false_type());
                _Destroy(__i, this->_M_finish);
            }
            else {
                __copy_ptrs((const T*)__x._M_start, (const T*)__x._M_start + size(), this->_M_start, __false_type());
                __uninitialized_copy((const T*)__x._M_start + size(), (const T*)__x._M_finish + 0, this->_M_finish, __false_type());
            }
            this->_M_finish = this->_M_start + __xlen;
        }
        return *this;
    }
};
static bool Equal(const vector& a, const vector& b)
{
    if (a.size() != b.size()) return false;
    for (unsigned int i = 0; i < a.size(); ++i)
        if (a._M_start[i].first != b._M_start[i].first || a._M_start[i].second != b._M_start[i].second) return false;
    return true;
}
#define FAIL() { std::printf("005f9074_TEST FAIL\n"); return 1; }
int main()
{
    {
        vector src(6);
        for (long k = 0; k < 6; ++k) src.push(100 + k);
        vector dst(2);
        dst.push(1);
        T* before = dst._M_start;
        dst = dst;                                                          // self-assignment
        if (dst._M_start != before || dst.size() != 1 || dst._M_start[0].first != 1) FAIL();
        dst = src;                                                          // longer than capacity: reallocate
        if (!Equal(dst, src) || dst.capacity() != 6 || dst._M_start == before || g_allocs != 3 || g_frees != 1) FAIL();
        vector shorter(3);
        for (long k = 0; k < 3; ++k) shorter.push(200 + k);
        T* kept = dst._M_start;
        dst = shorter;                                                      // shorter: copy over the head, keep the block
        if (!Equal(dst, shorter) || dst._M_start != kept || dst.capacity() != 6) FAIL();
        vector middle(5);
        for (long k = 0; k < 5; ++k) middle.push(300 + k);
        dst = middle;                                                       // between size() and capacity(): copy + construct
        if (!Equal(dst, middle) || dst._M_start != kept || dst.capacity() != 6 || g_allocs != 5 || g_frees != 1) FAIL();
        if (dst._M_start[4].second != (EHeroAbility)(304 % 100)) FAIL();
    }
    if (g_allocs != g_frees) FAIL();
    std::printf("005f9074_TEST PASS\n");
    return 0;
}