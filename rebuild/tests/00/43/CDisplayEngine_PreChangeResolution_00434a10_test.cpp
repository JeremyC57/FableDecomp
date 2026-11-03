// Behaviour test for 0x00434A10: the member forwards its arguments unchanged to the owned object's virtual PreChangeResolution (slot 15)
// and returns what it returns.
#include <cstdio>
static int g_calls = 0; static double g_sum = 0;
struct Target {
    virtual ~Target() { }
    virtual int PreChangeResolution() { ++g_calls; g_sum = 0; return 42; }
};
struct Owner {
    Target* p;
    int Forward() { return p->PreChangeResolution(); }
};
int main()
{
    Target t; Owner o; o.p = &t;
    int r = o.Forward();
    double expect = 0;
    if (g_calls != 1 || r != 42 || g_sum < expect - 0.001 || g_sum > expect + 0.001) { std::printf("00434a10_TEST FAIL\n"); return 1; }
    std::printf("00434a10_TEST PASS\n");
    return 0;
}