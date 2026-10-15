// Behaviour test for 0x00B21450: index window clamped to [0, last].
#include <cstdio>
static void Window(long count, long index, long& first, long& last)
{
    long lastIndex = count - 1; first = index - 1 > 0 ? index - 1 : 0; last = index + 1 < lastIndex ? index + 1 : lastIndex;
}
int main()
{
    long f, l; bool ok = true;
    Window(10, 0, f, l); ok = ok && f == 0 && l == 1;
    Window(10, 5, f, l); ok = ok && f == 4 && l == 6;
    Window(10, 9, f, l); ok = ok && f == 8 && l == 9;
    Window(10, 1, f, l); ok = ok && f == 0 && l == 2;
    if (!ok) { std::printf("00b21450_TEST FAIL\n"); return 1; }
    std::printf("00b21450_TEST PASS\n");
    return 0;
}
