// Behaviour test for 0x00777DB0 STLport basic_string<char>::reserve
#include <cstdio>
#include <cstdlib>
#include <cstring>

static int g_lengthErrors = 0;
struct CString {
    char* _M_start;
    char* _M_finish;
    char* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    void _M_throw_length_error() const { ++g_lengthErrors; }
    void reserve(unsigned int res)
    {
        if (res > 0xfffffffeu) { _M_throw_length_error(); return; }
        unsigned int sz = size();
        unsigned int n = (res < sz ? sz : res) + 1;
        char* ns = n != 0 ? (char*)std::malloc(n) : 0;
        char* nf = (_M_finish == _M_start) ? ns : (char*)std::memmove(ns, _M_start, _M_finish - _M_start) + (_M_finish - _M_start);
        *nf = 0;
        if (_M_start) std::free(_M_start);
        _M_start = ns; _M_finish = nf; _M_end_of_storage = ns + n;
    }
    void push_back(char c)
    {
        if (_M_finish + 1 == _M_end_of_storage) {
            unsigned int one = 1, sz = size();
            reserve(sz + (sz < one ? one : sz));
        }
        _M_finish[1] = 0;
        *_M_finish = c;
        ++_M_finish;
    }
    void Init() { _M_start = (char*)std::malloc(1); *_M_start = 0; _M_finish = _M_start; _M_end_of_storage = _M_start + 1; }
};
int main()
{
    CString s; s.Init();
    s.push_back('a'); s.push_back('b'); s.push_back('c');
    s.reserve(1);                                     // smaller than size: grows to size + 1
    if (s._M_end_of_storage - s._M_start != 4 || std::strcmp(s._M_start, "abc") != 0) { std::printf("00777db0_TEST FAIL\n"); return 1; }
    s.reserve(20);
    if (s._M_end_of_storage - s._M_start != 21 || s.size() != 3 || std::strcmp(s._M_start, "abc") != 0) { std::printf("00777db0_TEST FAIL\n"); return 1; }
    CString e = { 0, 0, 0 };
    e.reserve(0);                                     /* empty: no copy, terminator only */
    if (e._M_end_of_storage - e._M_start != 1 || e.size() != 0 || *e._M_start != 0 || g_lengthErrors != 0) { std::printf("00777db0_TEST FAIL\n"); return 1; }
    std::printf("00777db0_TEST PASS\n");
    return 0;
}