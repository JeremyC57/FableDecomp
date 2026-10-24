// Behaviour test for STLport basic_string<char>::find(const char*, size_type) (0x00841790): first occurrence at or after
// pos, npos when absent or when pos + strlen(s) runs past the end; an empty pattern matches at pos.
#include <cstdio>
#include <cstring>
struct Str {
    const char* _M_start;
    const char* _M_finish;
    unsigned int size() const { return _M_finish - _M_start; }
};
static const char* search_range(const char* f1, const char* l1, const char* f2, const char* l2)
{
    if (f1 == l1 || f2 == l2) return f1;
    for (; f1 != l1; ++f1) {
        const char* a = f1; const char* b = f2;
        while (a != l1 && b != l2 && *a == *b) { ++a; ++b; }
        if (b == l2) return f1;
        if (a == l1) return l1;
    }
    return l1;
}
static unsigned int find(const Str& s, const char* p, unsigned int pos)
{
    unsigned int n = (unsigned int)std::strlen(p);
    if (pos + n > s.size()) return (unsigned int)-1;
    const char* r = search_range(s._M_start + pos, s._M_finish, p, p + n);
    return r != s._M_finish ? (unsigned int)(r - s._M_start) : (unsigned int)-1;
}
int main()
{
    const char* text = "http://host:8080/path";
    Str s = { text, text + std::strlen(text) };
    bool ok = find(s, "://", 0) == 4 && find(s, ":", 5) == 11 && find(s, "/", 7) == 16 && find(s, "zzz", 0) == (unsigned int)-1
        && find(s, "path", 18) == (unsigned int)-1 && find(s, "", 3) == 3 && find(s, "http", 0) == 0;
    std::printf(ok ? "00841790_TEST PASS\n" : "00841790_TEST FAIL\n");
    return ok ? 0 : 1;
}