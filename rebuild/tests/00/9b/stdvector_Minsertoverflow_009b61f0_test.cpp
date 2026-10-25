// behaviour test for 0x009B61F0 std::vector<pair<unsigned long, long>>::_M_insert_overflow (__false_type form): head copy, single construct or n-fold fill, tail copy unless atend, growth to size + max(size, n), old block freed
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stddef.h>
inline void* operator new(size_t, void* p, int) { return p; }

namespace stlp_test {
struct __true_type { };
struct __false_type { };
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair() : first(_T1()), second(_T2()) { }
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};
template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
static int gFrees = 0;
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p, 0) _T1(__val); }
template <class _InputIter, class _ForwardIter>
inline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur) _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur) _Construct(&*__cur, __x);
    return __cur;
}
template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)std::malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) { ++gFrees; std::free(__p); } }
};
template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public: _Value _M_data; };
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start; _Tp* _M_finish; _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
protected:
    void _M_clear() { this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start); }
    void _M_set(pointer __s, pointer __f, pointer __e) { this->_M_start = __s; this->_M_finish = __f; this->_M_end_of_storage._M_data = __e; }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len, bool __atend = false)
    {
        const size_type __old_size = size();
        const size_type __len = __old_size + (max)(__old_size, __fill_len);
        pointer __new_start = this->_M_end_of_storage.allocate(__len);
        pointer __new_finish = __new_start;
        __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
        if (__fill_len == 1) { _Construct(__new_finish, __x); ++__new_finish; }
        else __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
        if (!__atend)
            __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
        _M_clear();
        _M_set(__new_start, __new_finish, __new_start + __len);
    }
};
typedef pair<unsigned long, long> T;
}
typedef stlp_test::T T;
static T mk(unsigned int k)             // the first word carries the key (valid float bits for float members)
{
    T t;
    std::memset((void*)&t, 0, sizeof(T));
    unsigned int w = 0x3F800000u + k * 0x10000u;
    std::memcpy((void*)&t, &w, 4);
    return t;
}
static unsigned int key(const T& t) { unsigned int w; std::memcpy(&w, (const void*)&t, 4); return (w - 0x3F800000u) / 0x10000u; }
class CProbe : public stlp_test::vector<T> {
public:
    CProbe() { this->_M_start = 0; this->_M_finish = 0; this->_M_end_of_storage._M_data = 0; }
    ~CProbe() { std::free(this->_M_start); }
    void Overflow(unsigned int pos, const T& x, unsigned int n, bool atend)
    {
        _M_insert_overflow(this->_M_start + pos, x, stlp_test::__false_type(), n, atend);
    }
    unsigned int Cap() const { return (unsigned int)(this->_M_end_of_storage._M_data - this->_M_start); }
};
static int Check()
{
    CProbe v;
    v.Overflow(0, mk(1), 1, true);                         // empty: len = 0 + max(0, 1) = 1
    if (v.size() != 1 || v.Cap() != 1 || key(v._M_start[0]) != 1 || stlp_test::gFrees != 0) return 1;
    v.Overflow(1, mk(2), 1, true);                         // push_back growth: len = 2, old block freed
    if (v.size() != 2 || v.Cap() != 2 || key(v._M_start[1]) != 2 || stlp_test::gFrees != 1) return 2;
    v.Overflow(1, mk(3), 3, false);                        // middle fill of 3: len = 2 + max(2, 3) = 5
    unsigned int want[5] = { 1, 3, 3, 3, 2 };
    if (v.size() != 5 || v.Cap() != 5 || stlp_test::gFrees != 2) return 3;
    for (int i = 0; i < 5; ++i) if (key(v._M_start[i]) != want[i]) return 4;
    v.Overflow(4, mk(4), 1, false);                        // single middle insert: len = 10
    unsigned int want2[6] = { 1, 3, 3, 3, 4, 2 };
    if (v.size() != 6 || v.Cap() != 10) return 5;
    for (int i = 0; i < 6; ++i) if (key(v._M_start[i]) != want2[i]) return 6;
    v.Overflow(2, mk(5), 2, true);                         // atend: the tail is dropped
    if (v.size() != 4 || key(v._M_start[2]) != 5 || key(v._M_start[3]) != 5 || key(v._M_start[1]) != 3) return 7;
    return 0;
}
int main()
{
    int r = Check();
    if (r != 0) { std::printf("009b61f0_TEST FAIL %d\n", r); return 1; }
    std::printf("009b61f0_TEST PASS\n");
    return 0;
}