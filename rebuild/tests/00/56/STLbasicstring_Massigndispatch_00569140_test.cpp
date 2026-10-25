// Behaviour test for 0x00569140: STLport basic_string::_M_assign_dispatch(char*, char*) (overwrite, erase tail / append rest)
#include <cstdio>
#include <cstring>
#include <cstdlib>

struct Str {
    char* start;
    char* finish;
    char* eos;
    void init(const char* s, unsigned int cap)
    {
        unsigned int n = (unsigned int)std::strlen(s);
        start = (char*)std::malloc(cap + 1);
        std::memcpy(start, s, n + 1);
        finish = start + n;
        eos = start + cap + 1;
    }
    void append(char* f, char* l)
    {
        unsigned int old = (unsigned int)(finish - start), n = (unsigned int)(l - f);
        char* s = (char*)std::malloc(old + n + 1);
        std::memcpy(s, start, old);
        std::memcpy(s + old, f, n);
        s[old + n] = 0;
        std::free(start);
        start = s; finish = s + old + n; eos = finish + 1;
    }
    void erase(char* first, char* last)
    {
        if (first != last) {
            unsigned int n = (unsigned int)(finish - last) + 1;
            if (n) std::memmove(first, last, n);
            finish = finish - (last - first);
        }
    }
    Str& assign(char* f, char* l)
    {
        char* cur = start;
        while (f != l && cur != finish) {
            *cur = *f;
            ++f;
            ++cur;
        }
        if (f == l)
            erase(cur, finish);
        else
            append(f, l);
        return *this;
    }
};

int main()
{
    char src1[] = "abc";
    Str s; s.init("hello world", 16);
    s.assign(src1, src1 + 3);
    if (std::strcmp(s.start, "abc") != 0 || s.finish - s.start != 3) { std::printf("00569140_TEST FAIL\n"); return 1; }
    char src2[] = "longer text here";
    s.assign(src2, src2 + 16);
    if (std::strcmp(s.start, "longer text here") != 0 || s.finish - s.start != 16) { std::printf("00569140_TEST FAIL\n"); return 1; }
    char src3[] = "xy";
    s.assign(src3, src3);
    if (s.finish != s.start || s.start[0] != 0) { std::printf("00569140_TEST FAIL\n"); return 1; }
    std::printf("00569140_TEST PASS\n");
    return 0;
}