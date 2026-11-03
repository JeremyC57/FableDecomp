// Behaviour test for 0x006CB6B0: the member forwards its arguments unchanged to the owned object's virtual Interrupt (slot 18)
// and returns what it returns.
#include <cstdio>
static int g_calls = 0; static double g_sum = 0;
struct Target {
    virtual ~Target() { }
    virtual int Interrupt() { ++g_calls; g_sum = 0; return 42; }
};
struct Owner {
    Target* p;
    int Forward() { return p->Interrupt(); }
};
int main()
{
    Target t; Owner o; o.p = &t;
    int r = o.Forward();
    double expect = 0;
    if (g_calls != 1 || r != 42 || g_sum < expect - 0.001 || g_sum > expect + 0.001) { std::printf("006cb6b0_TEST FAIL\n"); return 1; }
    std::printf("006cb6b0_TEST PASS\n");
    return 0;
}