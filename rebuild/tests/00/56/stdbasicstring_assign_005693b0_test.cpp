// Behaviour test for 0x005693B0 std::basic_string::assign(const char*, const char*).
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace stl {
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct char_traits_char {
    static char* move(char* __s1, const char* __s2, unsigned int __n)
    {
        return (__n == 0 ? __s1 : (char*)std::memmove(__s1, __s2, __n * sizeof(char)));
    }
    static char* copy(char* __s1, const char* __s2, unsigned int __n)
    {
        return (__n == 0 ? __s1 : (char*)std::memcpy(__s1, __s2, __n * sizeof(char)));
    }
};
class basic_string {
public:
    typedef unsigned int size_type;
    typedef char* iterator;
    char* _M_start;
    char* _M_finish;
    char* _M_end_of_storage;
    explicit basic_string(const char* s)
    {
        unsigned int n = (unsigned int)std::strlen(s);
        _M_start = (char*)std::malloc(n + 1); std::memcpy(_M_start, s, n + 1);
        _M_finish = _M_start + n; _M_end_of_storage = _M_start + n + 1;
    }
    ~basic_string() { std::free(_M_start); }
    size_type size() const { return (size_type)(_M_finish - _M_start); }
    size_type capacity() const { return (size_type)(_M_end_of_storage - _M_start) - 1; }
    iterator begin() { return _M_start; }
    iterator end() { return _M_finish; }
    iterator erase(iterator __first, iterator __last)
    {
        if (__first != __last) {
            char_traits_char::move(__first, __last, (unsigned int)((_M_finish - __last) + 1));
            char* __new_finish = _M_finish - (__last - __first);
            _M_finish = __new_finish;
        }
        return __first;
    }
    // stand-in for the out-of-line forward-iterator append (0x00567D60)
    basic_string& append(const char* __first, const char* __last, const forward_iterator_tag&)
    {
        unsigned int n = (unsigned int)(__last - __first), old = size();
        if (n == 0) return *this;
        if (old + n > capacity()) {
            char* p = (char*)std::malloc(old + n + 1);
            std::memcpy(p, _M_start, old);
            std::free(_M_start);
            _M_start = p; _M_finish = p + old; _M_end_of_storage = p + old + n + 1;
        }
        std::memcpy(_M_finish, __first, n);
        _M_finish += n;
        *_M_finish = 0;
        return *this;
    }
    basic_string& append(const char* __first, const char* __last) { return append(__first, __last, forward_iterator_tag()); }
    basic_string& assign(const char* __f, const char* __l)
    {
        int __n = (int)(__l - __f);
        if ((size_type)__n <= size()) {
            char_traits_char::copy(_M_start, __f, __n);
            erase(begin() + __n, end());
        }
        else {
            char_traits_char::copy(_M_start, __f, size());
            append(__f + size(), __l);
        }
        return *this;
    }
};
}

static int Is(const stl::basic_string& s, const char* t)
{
    return s.size() == std::strlen(t) && std::strcmp(s._M_start, t) == 0;
}

int main()
{
    int ok = 1;
    stl::basic_string s("hello world");
    const char* a = "abc";
    if (&s.assign(a, a + 3) != &s || !Is(s, "abc")) ok = 0;            // shrink: overwrite + erase the tail
    const char* b = "a much longer text";
    s.assign(b, b + std::strlen(b));
    if (!Is(s, "a much longer text")) ok = 0;                         // grow: overwrite size() bytes + append
    s.assign(a, a);
    if (!Is(s, "")) ok = 0;                                           // assign empty
    const char* c = "xyz";
    s.assign(c, c + 3);
    if (!Is(s, "xyz")) ok = 0;
    if (!ok) { std::printf("005693b0_TEST FAIL\n"); return 1; }
    std::printf("005693b0_TEST PASS\n");
    return 0;
}