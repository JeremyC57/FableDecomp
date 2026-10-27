// behaviour test for 0x005BAEC4: the copy holds the same elements in a separate block
#include <cstdio>
#include <vector>
struct E { long w[2]; };
int main()
{
    std::vector<E> a(3); a[0].w[0] = 1; a[2].w[2 - 1] = 7;
    std::vector<E> b(a);
    std::vector<E> e; std::vector<E> f(e);
    if (b.size() != 3 || b[0].w[0] != 1 || b[2].w[2 - 1] != 7 || &b[0] == &a[0] || !f.empty()) { std::printf("005baec4_TEST FAIL\n"); return 1; }
    std::printf("005baec4_TEST PASS\n");
    return 0;
}