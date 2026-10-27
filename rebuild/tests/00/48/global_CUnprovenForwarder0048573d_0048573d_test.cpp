// behaviour test for 0x0048573D (model: the forwarder calls the member on the one global object)
#include <cstdio>
static int g_calls = 0, g_arg = 0;
struct G { void M(int a) { ++g_calls; g_arg = a; } };
static G g_obj;
static void Forward(int a) { g_obj.M(a); }
int main()
{
    Forward(5);
    int ok = g_calls == 1 && g_arg == 5;
    if (!ok) { std::printf("0048573d_TEST FAIL\n"); return 1; }
    std::printf("0048573d_TEST PASS\n");
    return 0;
}