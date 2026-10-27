// behaviour test for 0x0054DE10: the action list runs only when the def flag is set, and the def reference is released
#include <cstdio>
static int g_runs = 0, g_destroyed = 0;
struct Def { long refs; bool flag; void Destroy() { ++g_destroyed; } };
struct Ptr { Def* d; explicit Ptr(Def* x) : d(x) { ++d->refs; } Ptr(const Ptr& o) : d(o.d) { ++d->refs; } ~Ptr() { if (d && --d->refs == 0) d->Destroy(); } };
static Ptr GetDef(Def* d) { return Ptr(d); }
static void Handler(Def* d) { if (GetDef(d).d->flag) ++g_runs; }
int main()
{
    Def on = { 1, true }, off = { 1, false };
    Handler(&on); Handler(&off);
    if (g_runs != 1 || on.refs != 1 || off.refs != 1 || g_destroyed != 0) { std::printf("0054de10_TEST FAIL\n"); return 1; }
    std::printf("0054de10_TEST PASS\n");
    return 0;
}