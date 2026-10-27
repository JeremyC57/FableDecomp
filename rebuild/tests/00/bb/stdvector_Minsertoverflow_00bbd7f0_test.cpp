// behaviour test for 0x00BBD7F0: growth keeps head, inserted copies and tail in order
#include <cstdio>
#include <vector>
struct E { long a; bool b; };
int main()
{
    std::vector<E> v; E e0 = { 1, true }, e1 = { 2, false }, x = { 9, true };
    v.push_back(e0); v.push_back(e1);
    v.insert(v.begin() + 1, 2, x);
    if (v.size() != 4 || v[0].a != 1 || v[1].a != 9 || v[2].b != true || v[3].a != 2) { std::printf("00bbd7f0_TEST FAIL\n"); return 1; }
    std::printf("00bbd7f0_TEST PASS\n");
    return 0;
}