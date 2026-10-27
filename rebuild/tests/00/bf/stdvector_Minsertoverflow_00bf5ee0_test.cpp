// behaviour test for 0x00BF5EE0: growth keeps head, inserted copies and tail in order
#include <cstdio>
#include <vector>
struct E { long a[7]; };
int main()
{
    std::vector<E> v; E e0 = { { 1 } }, e1 = { { 2 } }, x = { { 9 } };
    v.push_back(e0); v.push_back(e1);
    v.insert(v.begin() + 1, 3, x);
    if (v.size() != 5 || v[0].a[0] != 1 || v[1].a[0] != 9 || v[3].a[0] != 9 || v[4].a[0] != 2) { std::printf("00bf5ee0_TEST FAIL\n"); return 1; }
    std::printf("00bf5ee0_TEST PASS\n");
    return 0;
}