#include <cstdio>
#include <cstring>
struct CCharString { const char* p; CCharString(const char* s, long n) : p(s) { } CCharString(const CCharString& o) : p(o.p) { } };
struct CActionWait { CCharString GetActionName(); };
CCharString CActionWait::GetActionName() { return CCharString("CActionWait", -1); }
int main() { CActionWait o; bool ok = std::strcmp(o.GetActionName().p, "CActionWait") == 0; std::printf(ok ? "00903640_TEST PASS\n" : "00903640_TEST FAIL\n"); return ok ? 0 : 1; }
