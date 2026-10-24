#include <cstdio>
#include <cstring>
struct CCharString { const char* p; CCharString(const char* s, long n) : p(s) { } CCharString(const CCharString& o) : p(o.p) { } };
struct CPSCOrbit { CCharString GetGUIName(); };
CCharString CPSCOrbit::GetGUIName() { return CCharString("Orbit", -1); }
int main() { CPSCOrbit o; bool ok = std::strcmp(o.GetGUIName().p, "Orbit") == 0; std::printf(ok ? "00b17910_TEST PASS\n" : "00b17910_TEST FAIL\n"); return ok ? 0 : 1; }
