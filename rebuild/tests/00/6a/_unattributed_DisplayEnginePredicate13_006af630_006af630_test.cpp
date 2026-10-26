#include <cstdio>
static bool g_flag = false;
class CDisplayEngine { public: virtual void v0() { } virtual void v1() { } virtual void v2() { } virtual void v3() { } virtual void v4() { } virtual void v5() { } virtual void v6() { } virtual void v7() { } virtual void v8() { } virtual bool IsDrawingEnabled() { return g_flag; } };
static CDisplayEngine g_engine;
struct CNode { CDisplayEngine* GetDisplayEngine() { return &g_engine; } bool DrawIsDrawable() { return GetDisplayEngine()->IsDrawingEnabled(); } };
int main() { CNode n; bool ok = !n.DrawIsDrawable(); g_flag = true; ok = ok && n.DrawIsDrawable(); std::printf(ok ? "006af630_TEST PASS\n" : "006af630_TEST FAIL\n"); return ok ? 0 : 1; }
