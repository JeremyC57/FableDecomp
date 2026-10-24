// Behaviour test for 0x00777F80 STLport basic_string<char>::push_back
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
    const char* text = "weapon trail";
    for (const char* p = text; *p; ++p) s.push_back(*p);
    if (s.size() != 12 || std::strcmp(s._M_start, text) != 0) { std::printf("00777f80_TEST FAIL\n"); return 1; }
    if (s._M_end_of_storage - s._M_start != 17) { std::printf("00777f80_TEST FAIL\n"); return 1; }   /* blocks 2,3,5,9,17: size + max(size,1) + 1 */
    std::printf("00777f80_TEST PASS\n");
    return 0;
}