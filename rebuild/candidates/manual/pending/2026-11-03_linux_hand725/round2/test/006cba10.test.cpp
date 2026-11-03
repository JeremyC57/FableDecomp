// Behaviour test for 0x006CBA10: the member forwards its arguments unchanged to the owned object's virtual CalculateNavigableWidthOfLine (slot 19)
// and returns what it returns.
#include <cstdio>
static int g_calls = 0; static double g_sum = 0;
struct Target {
    virtual ~Target() { }
    virtual int CalculateNavigableWidthOfLine(int a, int b, int c, int d) { ++g_calls; g_sum = 0 + a + b + c + d; return 42; }
};
struct Owner {
    Target* p;
    int Forward(int a, int b, int c, int d) { return p->CalculateNavigableWidthOfLine(a, b, c, d); }
};
int main()
{
    Target t; Owner o; o.p = &t;
    int r = o.Forward(11, 22, 33, 44);
    double expect = 0 + 11 + 22 + 33 + 44;
    if (g_calls != 1 || r != 42 || g_sum < expect - 0.001 || g_sum > expect + 0.001) { std::printf("006cba10_TEST FAIL\n"); return 1; }
    std::printf("006cba10_TEST PASS\n");
    return 0;
}
