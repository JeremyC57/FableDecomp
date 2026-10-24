// Behaviour test for 0x0056A2D0 std::vector<basic_string>::push_back: in-place copies while capacity lasts, growth
// through _M_insert_overflow (stand-in copy of the 0x00569E30 model) when full.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

namespace stl {
struct __false_type { __false_type() { } };
template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
class basic_string {
public:
    char* _M_start;
    char* _M_finish;
    char* _M_end_of_storage;
    void _M_range_initialize(const char* __f, const char* __l)
    {
        unsigned int n = (unsigned int)(__l - __f);
        _M_start = (char*)std::malloc(n + 1);
        std::memcpy(_M_start, __f, n);
        _M_finish = _M_start + n; *_M_finish = 0; _M_end_of_storage = _M_finish + 1;
    }
    explicit basic_string(const char* s) { _M_range_initialize(s, s + std::strlen(s)); }
    basic_string(const basic_string& __s) { _M_range_initialize(__s._M_start, __s._M_finish); }
    ~basic_string() { std::free(_M_start); }
};
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _T1, class _T2> inline void _Copy_Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _InputIter, class _ForwardIter>
_ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _Tp> class vector {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    size_type size() const { return size_type(_M_finish - _M_start); }
    _Tp* allocate(size_type n) { return n != 0 ? (_Tp*)std::malloc(n * sizeof(_Tp)) : 0; }
    void _M_clear()
    {
        for (_Tp* p = _M_start; p != _M_finish; ++p) p->~_Tp();
        std::free(_M_start);
    }
    void _M_set(pointer __s, pointer __f, pointer __e) { _M_start = __s; _M_finish = __f; _M_end_of_storage = __e; }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len, bool __atend = false)
    {
        const size_type __old_size = size();
        const size_type __len = __old_size + (max)(__old_size, __fill_len);
        pointer __new_start = allocate(__len);
        pointer __new_finish = __new_start;
        __new_finish = __uninitialized_copy(_M_start, __position, __new_start, __false_type());
        if (__fill_len == 1) {
            _Copy_Construct(__new_finish, __x);
            ++__new_finish;
        } else
            __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
        if (!__atend)
            __new_finish = __uninitialized_copy(__position, _M_finish, __new_finish, __false_type());
        _M_clear();
        _M_set(__new_start, __new_finish, __new_start + __len);
    }
    void push_back(const _Tp& __x)
    {
        if (_M_finish != _M_end_of_storage) {
            _Copy_Construct(_M_finish, __x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, __x, __false_type(), 1UL, true);
    }
};
}

static int Is(const stl::basic_string& s, const char* t) { return std::strcmp(s._M_start, t) == 0; }

int main()
{
    int ok = 1;
    stl::vector<stl::basic_string> v;
    stl::basic_string a("one"), b("two");
    char buf[8];
    for (int i = 0; i < 9; ++i) {
        buf[0] = (char)('a' + i); buf[1] = 0;
        stl::basic_string s(buf);
        stl::basic_string* before = v._M_end_of_storage;
        stl::basic_string* finish = v._M_finish;
        v.push_back(s);
        if (before != finish && v._M_end_of_storage != before) ok = 0;       // no regrowth while there is room
        if (!Is(v._M_finish[-1], buf) || v._M_finish[-1]._M_start == s._M_start) ok = 0;
    }
    if (v.size() != 9 || v._M_end_of_storage - v._M_start != 16) ok = 0;   // 1, 2, 4, 8, 16
    for (int i = 0; ok && i < 9; ++i) { buf[0] = (char)('a' + i); buf[1] = 0; if (!Is(v._M_start[i], buf)) ok = 0; }
    v._M_clear();
    if (!ok) { std::printf("0056a2d0_TEST FAIL\n"); return 1; }
    std::printf("0056a2d0_TEST PASS\n");
    return 0;
}