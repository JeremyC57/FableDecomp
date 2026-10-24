#include <cstdio>
#include <cstring>
struct CCharString { const char* p; CCharString(const char* s, long n) : p(s) { } CCharString(const CCharString& o) : p(o.p) { } };
struct CPSCSpline { CCharString GetGUIName(); };
CCharString CPSCSpline::GetGUIName() { return CCharString("Spline", -1); }
int main() { CPSCSpline o; bool ok = std::strcmp(o.GetGUIName().p, "Spline") == 0; std::printf(ok ? "00b1b470_TEST PASS\n" : "00b1b470_TEST FAIL\n"); return ok ? 0 : 1; }
