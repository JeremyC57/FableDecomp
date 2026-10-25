// Behaviour test for std::basic_string<unsigned short>(const unsigned short* s, unsigned int n, const allocator&)
// (0x0099CA40): the string holds exactly the first n characters of s followed by a terminator, n = 0 gives an empty
// terminated string, and characters past n are not copied.
#include <cstdio>
#include <cstdlib>

namespace NS {
struct forward_iterator_tag { forward_iterator_tag() { } };
struct random_access_iterator_tag : public forward_iterator_tag { random_access_iterator_tag() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
};

template <class _Tp, class _Alloc> class _String_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    _String_base(const _Alloc&) : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~_String_base() { std::free(_M_start); }
};

template <class _CharT, class _Alloc = allocator<_CharT> >
class basic_string : public _String_base<_CharT, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Alloc allocator_type;
    allocator_type get_allocator() const { return allocator_type(); }
    basic_string(const _CharT* __s, size_type __n, const allocator_type& __a = allocator_type())
        : _String_base<_CharT, _Alloc>(__a)
    {
        _M_range_initialize(__s, __s + __n);
    }
    basic_string(const basic_string& __s) : _String_base<_CharT, _Alloc>(__s.get_allocator())
    {
        _M_range_initialize(__s._M_start, __s._M_finish);
    }
    // 0x0099C670 / 0x00567D00: allocate (l - f) + 1 characters, copy, terminate
    void _M_range_initialize(const _CharT* __f, const _CharT* __l, const forward_iterator_tag&)
    {
        int __n = (int)(__l - __f);
        this->_M_start = (_CharT*)std::malloc((__n + 1) * sizeof(_CharT));
        this->_M_end_of_storage = this->_M_start + __n + 1;
        for (int i = 0; i < __n; ++i)
            this->_M_start[i] = __f[i];
        this->_M_finish = this->_M_start + __n;
        *this->_M_finish = 0;
    }
    void _M_range_initialize(const _CharT* __f, const _CharT* __l) { _M_range_initialize(__f, __l, random_access_iterator_tag()); }
};
}

int main()
{
    static const unsigned short text[6] = { 'F', 'a', 'b', 'l', 'e', '!' };
    int fails = 0;
    {
        NS::basic_string<unsigned short> s(text, 5);
        if (s._M_finish - s._M_start != 5) ++fails;
        for (int i = 0; i < 5; ++i)
            if (s._M_start[i] != text[i]) ++fails;
        if (*s._M_finish != 0 || s._M_end_of_storage != s._M_finish + 1) ++fails;
        NS::basic_string<unsigned short> e(text, 0);
        if (e._M_finish != e._M_start || *e._M_start != 0) ++fails;
    }
    if (fails) { std::printf("0099ca40_TEST FAIL\n"); return 1; }
    std::printf("0099ca40_TEST PASS\n");
    return 0;
}