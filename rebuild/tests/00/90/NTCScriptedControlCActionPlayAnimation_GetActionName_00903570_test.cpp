#include <cstdio>
#include <cstring>
struct CCharString { const char* p; CCharString(const char* s, long n) : p(s) { } CCharString(const CCharString& o) : p(o.p) { } };
struct CActionPlayAnimation { CCharString GetActionName(); };
CCharString CActionPlayAnimation::GetActionName() { return CCharString("CActionPlayAnimation", -1); }
int main() { CActionPlayAnimation o; bool ok = std::strcmp(o.GetActionName().p, "CActionPlayAnimation") == 0; std::printf(ok ? "00903570_TEST PASS\n" : "00903570_TEST FAIL\n"); return ok ? 0 : 1; }
