#include <cstdio>
#include <cstring>
class CCharString { public: const char* p; CCharString(const char* s, long n) : p(s) { } CCharString(const CCharString& o) : p(o.p) { } };
struct CPSCAttractor { CCharString GetGUIName(); };
CCharString CPSCAttractor::GetGUIName() { return CCharString("Attractor", -1); }
int main() { CPSCAttractor o; bool ok = std::strcmp(o.GetGUIName().p, "Attractor") == 0; std::printf(ok ? "00b18720_TEST PASS\n" : "00b18720_TEST FAIL\n"); return ok ? 0 : 1; }
