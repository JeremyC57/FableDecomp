#include <cstdio>
// Behaviour model: the flags are cleared and both reaction switches are turned off.
struct G { int opinion, deed; void Op(bool e) { opinion = e; } void Deed(bool e) { deed = e; } };
struct E { G* g; bool early, cham; long hint; void Init() { early = false; cham = false; hint = 0; g->Op(false); g->Deed(false); } };
int main()
{
    G g = { 1, 1 }; E e = { &g, true, true, 7 }; e.Init();
    if (e.early || e.cham || e.hint || g.opinion || g.deed) { std::printf("00f16250_TEST FAIL\n"); return 1; }
    std::printf("00f16250_TEST PASS\n");
    return 0;
}