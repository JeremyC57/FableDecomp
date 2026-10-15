// Behaviour test for 0x00B214C0: parametric position to a control-point window.
#include <cstdio>
static void Window(long count, float pos, long& first, long& last)
{
    long lastIndex = count - 1; first = (long)(pos * (float)lastIndex); last = first + 1 < lastIndex ? first + 1 : lastIndex;
}
int main()
{
    long f, l; bool ok = true;
    Window(5, 0.0f, f, l); ok = ok && f == 0 && l == 1;
    Window(5, 0.6f, f, l); ok = ok && f == 2 && l == 3;
    Window(5, 1.0f, f, l); ok = ok && f == 4 && l == 4;
    if (!ok) { std::printf("00b214c0_TEST FAIL\n"); return 1; }
    std::printf("00b214c0_TEST PASS\n");
    return 0;
}
