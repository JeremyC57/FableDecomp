// Behaviour test for 0x00836790: the member forwards its arguments unchanged to the owned object's virtual SetSpecificSamplePitch (slot 6)
// and returns what it returns.
#include <cstdio>
static int g_calls = 0; static double g_sum = 0;
struct Target {
    virtual ~Target() { }
    virtual int SetSpecificSamplePitch(unsigned long a, float b) { ++g_calls; g_sum = 0 + a + b; return 42; }
};
struct Owner {
    Target* p;
    int Forward(unsigned long a, float b) { return p->SetSpecificSamplePitch(a, b); }
};
int main()
{
    Target t; Owner o; o.p = &t;
    int r = o.Forward(11, 2.5f);
    double expect = 0 + 11 + 2.5;
    if (g_calls != 1 || r != 42 || g_sum < expect - 0.001 || g_sum > expect + 0.001) { std::printf("00836790_TEST FAIL\n"); return 1; }
    std::printf("00836790_TEST PASS\n");
    return 0;
}
