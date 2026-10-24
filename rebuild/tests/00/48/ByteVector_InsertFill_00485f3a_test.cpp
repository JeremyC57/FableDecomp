// Behaviour test for std::vector<unsigned char>::_M_fill_insert (0x00485F3A): inserting n copies of x before
// position, through the slide-tail branch (elems_after > n), the fill-past-end branch and the grow branch.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

namespace std {

struct __true_type { __true_type() {} };

void* __fastcall __copy_trivial(const void* first, const void* last, void* result)
{
    unsigned int n = (unsigned int)((const char*)last - (const char*)first);
    return (char*)memmove(result, first, n) + n;
}
void* __fastcall __copy_trivial_backward(const void* first, const void* last, void* result)
{
    int n = (int)((const char*)last - (const char*)first);
    if (n > 0) memmove((char*)result - n, first, n);
    return (char*)result - n;
}
void __fastcall fill(unsigned char* first, unsigned char* last, const unsigned char& value)
{
    memset(first, value, last - first);
}

template <class _Tp> class vector
{
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;

    template <class _Tag> __declspec(noinline) void _M_insert_overflow(_Tp* position, const _Tp& x, const _Tag&,
                                                                       unsigned int fill_len, bool atend = false)
    {
        // Same-TU stand-in for 0x00485B6C: grow, copy the head, fill, copy the tail (reads x, never the tag).
        unsigned int old_size = (unsigned int)(_M_finish - _M_start);
        unsigned int len = old_size + (old_size > fill_len ? old_size : fill_len);
        _Tp* new_start = (_Tp*)malloc(len);
        _Tp* new_finish = (_Tp*)__copy_trivial(_M_start, position, new_start);
        for (unsigned int i = 0; i < fill_len; ++i)
            *new_finish++ = x;
        if (!atend)
            new_finish = (_Tp*)__copy_trivial(position, _M_finish, new_finish);
        free(_M_start);
        _M_start = new_start;
        _M_finish = new_finish;
        _M_end_of_storage = new_start + len;
    }

    void _M_fill_insert(_Tp* position, unsigned int n, const _Tp& x);
};

template <class _Tp>
void vector<_Tp>::_M_fill_insert(_Tp* position, unsigned int n, const _Tp& x)
{
    if (n != 0) {
        if ((unsigned int)(_M_end_of_storage - _M_finish) >= n) {
            _Tp x_copy = x;
            const unsigned int elems_after = _M_finish - position;
            _Tp* old_finish = _M_finish;
            if (elems_after > n) {
                __copy_trivial(_M_finish - n, _M_finish, _M_finish);
                _M_finish += n;
                __copy_trivial_backward(position, old_finish - n, old_finish);
                fill(position, position + n, x_copy);
            }
            else {
                fill(_M_finish, _M_finish + (n - elems_after), x_copy);
                _M_finish += n - elems_after;
                __copy_trivial(position, old_finish, _M_finish);
                _M_finish += elems_after;
                fill(position, old_finish, x_copy);
            }
        }
        else
            _M_insert_overflow(position, x, __true_type(), n);
    }
}


}

static int Same(const std::vector<unsigned char>& v, const char* want)
{
    unsigned int n = (unsigned int)strlen(want);
    return (unsigned int)(v._M_finish - v._M_start) == n && memcmp(v._M_start, want, n) == 0;
}

int main()
{
    std::vector<unsigned char> v;
    unsigned char x;
    v._M_start = (unsigned char*)malloc(16);
    memcpy(v._M_start, "abcdef", 6);
    v._M_finish = v._M_start + 6;
    v._M_end_of_storage = v._M_start + 16;

    x = 'X'; v._M_fill_insert(v._M_start + 1, 2, x);          // after = 5 > 2
    if (!Same(v, "aXXbcdef")) { printf("FAIL slide\n"); return 1; }
    x = 'Y'; v._M_fill_insert(v._M_start + 6, 3, x);          // after = 2 <= 3
    if (!Same(v, "aXXbcdYYYef")) { printf("FAIL fill past end\n"); return 1; }
    x = 'Z'; v._M_fill_insert(v._M_start + 2, 0, x);          // n == 0: nothing
    if (!Same(v, "aXXbcdYYYef")) { printf("FAIL zero\n"); return 1; }
    x = 'W'; v._M_fill_insert(v._M_start + 11, 7, x);         // 5 free: grow
    if (!Same(v, "aXXbcdYYYefWWWWWWW") || v._M_end_of_storage - v._M_start < 18) { printf("FAIL grow\n"); return 1; }
    free(v._M_start);
    printf("BYTE_VECTOR_INSERT_00485f3a_TEST PASS\n");
    return 0;
}
