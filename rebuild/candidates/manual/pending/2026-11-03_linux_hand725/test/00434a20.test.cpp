// Behaviour test for 0x00434A20: the member forwards its arguments unchanged to the owned object's virtual PostChangeResolution (slot 16)
// and returns what it returns.
#include <cstdio>
static int g_calls = 0; static double g_sum = 0;
struct Target {
    virtual ~Target() { }
    virtual int PostChangeResolution(unsigned long a, unsigned long b, unsigned long c) { ++g_calls; g_sum = 0 + a + b + c; return 42; }
};
struct Owner {
    Target* p;
    int Forward(unsigned long a, unsigned long b, unsigned long c) { return p->PostChangeResolution(a, b, c); }
};
int main()
{
    Target t; Owner o; o.p = &t;
    int r = o.Forward(640, 480, 32);
    double expect = 0 + 640 + 480 + 32;
    if (g_calls != 1 || r != 42 || g_sum < expect - 0.001 || g_sum > expect + 0.001) { std::printf("00434a20_TEST FAIL\n"); return 1; }
    std::printf("00434a20_TEST PASS\n");
    return 0;
}
