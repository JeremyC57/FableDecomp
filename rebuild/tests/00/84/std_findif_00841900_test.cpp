// Behaviour test for 0x00841900 std::__find_if<const char*, _Not_within_traits<char_traits<char> > >:
// the first character of [first, last) that is not in the set [set_first, set_last).
#include <cstdio>
#include <cstring>
struct CNotWithin {
    const char* _M_first;
    const char* _M_last;
    bool operator()(const char& x) const
    {
        const char* p = _M_first;
        for (; p != _M_last; ++p) if (*p == x) break;
        return p == _M_last;
    }
};
static const char* find_if_model(const char* first, const char* last, CNotWithin pred)
{
    int trip_count = (int)(last - first) >> 2;
    for (; trip_count > 0; --trip_count) {
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
    case 1:
        if (pred(*first)) return first;
    case 0:
    default:
        return last;
    }
}
int main()
{
    const char* set = " \t\r\n";
    CNotWithin p; p._M_first = set; p._M_last = set + 4;
    const char* cases[] = { "", " ", "x", "   \t\r\nHTTP/1.1", "\r\n\r\n", "  a  ", "\t\t\t\t\t\t\t\tz" };
    int want[] = { 0, 1, 0, 6, 4, 2, 8 };
    for (int i = 0; i < 7; ++i) {
        const char* s = cases[i]; int n = (int)std::strlen(s);
        const char* r = find_if_model(s, s + n, p);
        if (r - s != want[i]) { std::printf("00841900_TEST FAIL\n"); return 1; }
    }
    std::printf("00841900_TEST PASS\n");
    return 0;
}