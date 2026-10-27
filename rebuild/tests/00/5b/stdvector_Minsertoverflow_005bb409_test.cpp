// behaviour test for 0x005BB409: growth keeps head, inserted copies and tail in order
#include <cstdio>
#include <vector>
struct E { long a, b; };
int main()
{
    std::vector<E> v; E e0 = { 1, 1 }, e1 = { 2, 2 }, x = { 9, 8 };
    v.push_back(e0); v.push_back(e1);
    v.insert(v.begin() + 1, 3, x);
    if (v.size() != 5 || v[0].a != 1 || v[1].b != 8 || v[3].a != 9 || v[4].a != 2) { std::printf("005bb409_TEST FAIL\n"); return 1; }
    std::printf("005bb409_TEST PASS\n");
    return 0;
}