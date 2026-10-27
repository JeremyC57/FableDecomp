// behaviour test for 0x00467465: growth keeps head, inserted copies and tail in order
#include <cstdio>
#include <vector>
struct E { long a, b, c, d; };
int main()
{
    std::vector<E> v; E e = { 0, 0, 0, 0 }, x = { 9, 9, 9, 9 };
    for (int i = 0; i < 3; ++i) { e.a = i; v.push_back(e); }
    v.insert(v.begin() + 1, 5, x);
    int ok = v.size() == 8 && v[0].a == 0 && v[1].a == 9 && v[5].c == 9 && v[6].a == 1 && v[7].a == 2;
    if (!ok) { std::printf("00467465_TEST FAIL\n"); return 1; }
    std::printf("00467465_TEST PASS\n");
    return 0;
}