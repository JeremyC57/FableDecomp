#include <cstdio>
struct CPSCSpline { virtual void s0() { } virtual void s1() { } virtual void s2() { } virtual void s3() { } virtual void s4() { } virtual void s5() { } virtual void s6() { } virtual void s7() { } virtual void s8() { } virtual void s9() { } virtual void s10() { } virtual void s11() { } virtual void s12() { } virtual void s13() { } virtual void s14() { } virtual void s15() { } virtual void s16() { } virtual void s17() { } virtual void DatabaseCopy(CPSCSpline* from) { copiedFrom = from; } CPSCSpline* copiedFrom; CPSCSpline* DatabaseClone(); };
static CPSCSpline g_fresh;
static CPSCSpline* __fastcall Alloc(CPSCSpline* self) { return &g_fresh; }
CPSCSpline* CPSCSpline::DatabaseClone() { CPSCSpline* copy = Alloc(this); copy->DatabaseCopy(this); return copy; }
int main() { CPSCSpline a; a.copiedFrom = 0; g_fresh.copiedFrom = 0; CPSCSpline* c = a.DatabaseClone(); bool ok = c == &g_fresh && g_fresh.copiedFrom == &a && a.copiedFrom == 0; std::printf(ok ? "00b1b4b0_TEST PASS\n" : "00b1b4b0_TEST FAIL\n"); return ok ? 0 : 1; }
