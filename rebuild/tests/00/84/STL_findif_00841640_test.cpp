// Behaviour test for 0x00841640: four-way unrolled find_if of a bound character (first match, else last)
#include <cstdio>
#include <cstring>

struct EqChar { char val; explicit EqChar(char c) : val(c) { } bool operator()(const char& c) const { return c == val; } };

static const char* FindIf(const char* first, const char* last, EqChar pred)
{
    int trip = (int)(last - first) >> 2;
    for (; trip > 0; --trip) {
        if (pred(*first)) return first;
        ++first;
        if (pred(*first)) return first;
        ++first;
        if (pred(*first)) return first;
        ++first;
        if (pred(*first)) return first;
        ++first;
    }
    switch (last - first) {
    case 3:
        if (pred(*first)) return first;
        ++first;
    case 2:
        if (pred(*first)) return first;
        ++first;
    case 1: if (pred(*first)) return first;
    case 0:
    default: return last;
    }
}

int main()
{
    const char* s = "abcdefghij";                           // 10 characters: two unrolled trips + a 2-tail
    const char* e = s + std::strlen(s);
    for (int i = 0; i < 10; ++i)
        if (FindIf(s, e, EqChar(s[i])) != s + i) { std::printf("00841640_TEST FAIL\n"); return 1; }
    if (FindIf(s, e, EqChar('z')) != e || FindIf(s, s, EqChar('a')) != s) { std::printf("00841640_TEST FAIL\n"); return 1; }
    for (int n = 0; n <= 10; ++n)                           // every tail length
        if (FindIf(s, s + n, EqChar('j')) != (n == 10 ? s + 9 : s + n)) { std::printf("00841640_TEST FAIL\n"); return 1; }
    const char* t = "xxaxa";
    if (FindIf(t, t + 5, EqChar('a')) != t + 2) { std::printf("00841640_TEST FAIL\n"); return 1; }
    std::printf("00841640_TEST PASS\n");
    return 0;
}