// behaviour test for 0x004EB494: inserting n copies keeps order (middle insert with spare capacity, and growth)
#include <cstdio>
#include <vector>
struct E { long a, b, c, d; };
int main()
{
    std::vector<E> v; v.reserve(10); E e = { 1, 2, 3, 4 }, x = { 7, 8, 9, 10 };
    for (int i = 0; i < 4; ++i) { e.a = i; v.push_back(e); }
    v.insert(v.begin() + 1, 2, x);
    int ok = v.size() == 6 && v[0].a == 0 && v[1].a == 7 && v[2].c == 9 && v[3].a == 1 && v[5].a == 3;
    v.insert(v.end(), 8, x); ok = ok && v.size() == 14 && v[13].b == 8;
    if (!ok) { std::printf("004eb494_TEST FAIL\n"); return 1; }
    std::printf("004eb494_TEST PASS\n");
    return 0;
}