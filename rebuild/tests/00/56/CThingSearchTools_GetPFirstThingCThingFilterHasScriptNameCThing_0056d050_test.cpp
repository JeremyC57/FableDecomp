// behaviour test for 0x0056D050: the first scan-visible match is returned (none -> false)
#include <cstdio>
struct T { unsigned char bits; int key; };
static bool Visible(unsigned char f) { unsigned char nf = ~f; return (f & 4) && (f & 8) && (nf & 1) && !(f & 0x20); }
static bool First(T* a, int n, int key, T** out) { for (int i = 0; i < n; ++i) if (Visible(a[i].bits) && a[i].key == key) { *out = &a[i]; return true; } return false; }
int main()
{
    T a[4] = { { 0x0d, 1 }, { 0x0c, 1 }, { 0x0c, 2 }, { 0x0c, 1 } }; T* r = 0;
    bool ok = First(a, 4, 1, &r) && r == &a[1] && !First(a, 4, 3, &r);
    if (!ok) { std::printf("0056d050_TEST FAIL\n"); return 1; }
    std::printf("0056d050_TEST PASS\n");
    return 0;
}