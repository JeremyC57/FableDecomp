// Behaviour test for 0x006CB990: the member forwards its arguments unchanged to the owned object's virtual IsNewDestinationGoingToCauseRestart (slot 11)
// and returns what it returns.
#include <cstdio>
static int g_calls = 0; static double g_sum = 0;
struct Target {
    virtual ~Target() { }
    virtual int IsNewDestinationGoingToCauseRestart(int a, float b) { ++g_calls; g_sum = 0 + a + b; return 42; }
};
struct Owner {
    Target* p;
    int Forward(int a, float b) { return p->IsNewDestinationGoingToCauseRestart(a, b); }
};
int main()
{
    Target t; Owner o; o.p = &t;
    int r = o.Forward(11, 2.5f);
    double expect = 0 + 11 + 2.5;
    if (g_calls != 1 || r != 42 || g_sum < expect - 0.001 || g_sum > expect + 0.001) { std::printf("006cb990_TEST FAIL\n"); return 1; }
    std::printf("006cb990_TEST PASS\n");
    return 0;
}