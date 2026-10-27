// behaviour test for 0x005F7E52: the backward copy assigns from the end so an overlapping shift right is safe
#include <cstdio>
struct E { long a, b; };
static E* CopyBackward(E* first, E* last, E* result) { for (int n = (int)(last - first); n > 0; --n) *--result = *--last; return result; }
int main()
{
    E v[5]; for (int i = 0; i < 5; ++i) { v[i].a = i; v[i].b = -i; }
    E* r = CopyBackward(v, v + 3, v + 4);
    E* e = CopyBackward(v, v, v + 2);
    if (r != v + 1 || e != v + 2 || v[1].a != 0 || v[2].a != 1 || v[3].b != -2 || v[4].a != 4) { std::printf("005f7e52_TEST FAIL\n"); return 1; }
    std::printf("005f7e52_TEST PASS\n");
    return 0;
}