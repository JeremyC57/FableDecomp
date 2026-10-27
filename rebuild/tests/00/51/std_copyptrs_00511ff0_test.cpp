// behaviour test for 0x00511FF0: the copy assigns each 16-byte record in order and returns the end of the destination
#include <cstdio>
struct E { unsigned long w[4]; };
static E* Copy(const E* first, const E* last, E* result) { for (int n = (int)(last - first); n > 0; --n) { *result = *first; ++first; ++result; } return result; }
int main()
{
    E v[4]; for (int i = 0; i < 4; ++i) for (int j = 0; j < 4; ++j) v[i].w[j] = i * 10 + j;
    E* r = Copy(v + 1, v + 4, v);
    E* e = Copy(v, v, v + 3);
    if (r != v + 3 || e != v + 3 || v[0].w[0] != 10 || v[0].w[3] != 13 || v[2].w[1] != 31 || v[3].w[2] != 32) { std::printf("00511ff0_TEST FAIL\n"); return 1; }
    std::printf("00511ff0_TEST PASS\n");
    return 0;
}