// behaviour test for 0x0049ae70 std::__copy_backward<CTexture*,CTexture*,int>: assigns n = last - first 8-byte
// elements back to front (operator= per element) so the copy ends at result; returns the copy's new start
#include <cstdio>
static int gAssigns = 0;
struct Tex { int a, b; Tex& operator=(const Tex& o) { a = o.a; b = o.b; ++gAssigns; return *this; } };
static Tex* CopyBackward(Tex* first, Tex* last, Tex* result)
{
    for (int n = (int)(last - first); n > 0; --n) *--result = *--last;
    return result;
}
int main()
{
    int fails = 0;
    Tex t[6];
    for (int i = 0; i < 6; ++i) { t[i].a = i; t[i].b = i * 10; }
    Tex* r = CopyBackward(t, t + 3, t + 5);
    if (r != t + 2 || gAssigns != 3) fails |= 1;
    if (t[2].a != 0 || t[3].a != 1 || t[4].a != 2 || t[4].b != 20 || t[5].a != 5) fails |= 2;
    if (CopyBackward(t, t, t + 4) != t + 4 || gAssigns != 3) fails |= 4;
    if (fails) { std::printf("0049ae70_TEST FAIL %d\n", fails); return 1; }
    std::printf("0049ae70_TEST PASS\n");
    return 0;
}