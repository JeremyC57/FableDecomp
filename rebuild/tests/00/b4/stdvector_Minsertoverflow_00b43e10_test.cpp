// behaviour test for 0x00B43E10 std::vector<CUnprovenElement_00b43e10*>::_M_insert_overflow: middle insert of n copies (head, fill, tail), append-at-end without the tail copy, growth to size + max(size, n), old block freed
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stddef.h>
class CUnprovenElement_00b43e10;
namespace stlp_test {
struct __true_type { };
template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
static int gFrees = 0;
inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result
        : ((char*)std::memmove(__result, __first, ((const char*)__last - (const char*)__first))) + ((const char*)__last - (const char*)__first);
}
template <class _OutputIter, class _Size, class _Tp>
inline _OutputIter fill_n(_OutputIter __first, _Size __n, const _Tp& __value)
{
    for (; __n > 0; --__n, ++__first) *__first = __value;
    return __first;
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
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __true_type&, size_type __fill_len, bool __atend = false)
    {
        const size_type __old_size = size();
        const size_type __len = __old_size + (max)(__old_size, __fill_len);
        pointer __new_start = this->_M_end_of_storage.allocate(__len);
        pointer __new_finish = (pointer)__copy_trivial(this->_M_start, __position, __new_start);
        __new_finish = fill_n(__new_finish, __fill_len, __x);
        if (!__atend)
            __new_finish = (pointer)__copy_trivial(__position, this->_M_finish, __new_finish);
        _M_clear();
        _M_set(__new_start, __new_finish, __new_start + __len);
    }
};
}
typedef CUnprovenElement_00b43e10* T;
static T mk(unsigned int v) { return (T)(size_t)v; }
class CProbe : public stlp_test::vector<T> {
public:
    CProbe() { this->_M_start = 0; this->_M_finish = 0; this->_M_end_of_storage._M_data = 0; }
    ~CProbe() { std::free(this->_M_start); }
    void Overflow(unsigned int pos, T x, unsigned int n, bool atend)
    {
        _M_insert_overflow(this->_M_start + pos, x, stlp_test::__true_type(), n, atend);
    }
    unsigned int Cap() const { return (unsigned int)(this->_M_end_of_storage._M_data - this->_M_start); }
};
static int Check()
{
    CProbe v;
    v.Overflow(0, mk(0x10), 1, true);                      // empty: len = 0 + max(0, 1) = 1
    if (v.size() != 1 || v.Cap() != 1 || v._M_start[0] != mk(0x10) || stlp_test::gFrees != 0) return 1;
    v.Overflow(1, mk(0x20), 1, true);                      // push_back growth: len = 1 + 1 = 2, old block freed
    if (v.size() != 2 || v.Cap() != 2 || v._M_start[1] != mk(0x20) || stlp_test::gFrees != 1) return 2;
    v.Overflow(1, mk(0x30), 3, false);                     // middle insert of 3: len = 2 + max(2, 3) = 5
    unsigned int want[5] = { 0x10, 0x30, 0x30, 0x30, 0x20 };
    if (v.size() != 5 || v.Cap() != 5 || stlp_test::gFrees != 2) return 3;
    for (int i = 0; i < 5; ++i) if (v._M_start[i] != mk(want[i])) return 4;
    v.Overflow(0, mk(0x40), 0, false);                     // zero fill: len = 5 + 5 = 10, contents kept
    if (v.size() != 5 || v.Cap() != 10) return 5;
    for (int i = 0; i < 5; ++i) if (v._M_start[i] != mk(want[i])) return 6;
    v.Overflow(2, mk(0x50), 1, true);                      // atend: the tail is dropped (callers pass atend only at end())
    if (v.size() != 3 || v._M_start[2] != mk(0x50) || v._M_start[1] != mk(0x30)) return 7;
    return 0;
}
int main()
{
    int r = Check();
    if (r != 0) { std::printf("00b43e10_TEST FAIL %d\n", r); return 1; }
    std::printf("00b43e10_TEST PASS\n");
    return 0;
}