#include <cstdio>
#include <new>
// Behaviour model: copy three 12-byte records into raw storage and return the end.
struct R { unsigned long a, b, c; };
static R* ucopy(R* f, R* l, R* r) { for (; f != l; ++f, ++r) new ((void*)r) R(*f); return r; }
int main()
{
    R s[3] = { {1, 2, 3}, {4, 5, 6}, {7, 8, 9} };
    static unsigned long raw[9];
    R* d = (R*)raw;
    if (ucopy(s, s + 3, d) != d + 3 || d[2].c != 9 || d[1].a != 4 || ucopy(s, s, d) != d) { std::printf("008f67c0_TEST FAIL\n"); return 1; }
    std::printf("008f67c0_TEST PASS\n");
    return 0;
}