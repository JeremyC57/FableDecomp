#include <cstdio>
// Model of CGameScriptInterface::StopProgressDisplay: NProgressDisplay::StopProgress(true, false).
static int g_calls; static bool g_a, g_b;
namespace NProgressDisplay { void StopProgress(bool a, bool b) { ++g_calls; g_a = a; g_b = b; } }
struct Interface { void StopProgressDisplay() const { NProgressDisplay::StopProgress(true, false); } };
int main()
{
    Interface gsi; gsi.StopProgressDisplay();
    if (g_calls != 1 || !g_a || g_b) { std::printf("0088e970_TEST FAIL\n"); return 1; }
    std::printf("0088e970_TEST PASS\n"); return 0;
}