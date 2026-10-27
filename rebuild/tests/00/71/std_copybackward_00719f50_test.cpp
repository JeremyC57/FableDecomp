// behaviour test for 0x00719F50: the backward copy assigns from the end so an overlapping shift right is safe
#include <cstdio>
struct E { unsigned long m[17]; };
static E* CopyBackward(E* first, E* last, E* result) { for (int n = (int)(last - first); n > 0; --n) *--result = *--last; return result; }
int main()
{
    E v[5]; for (int i = 0; i < 5; ++i) v[i].m[0] = i;
    E* r = CopyBackward(v, v + 3, v + 4);
    E* e = CopyBackward(v, v, v + 2);
    if (r != v + 1 || e != v + 2 || v[1].m[0] != 0 || v[2].m[0] != 1 || v[3].m[0] != 2 || v[4].m[0] != 4) { std::printf("00719f50_TEST FAIL\n"); return 1; }
    std::printf("00719f50_TEST PASS\n");
    return 0;
}