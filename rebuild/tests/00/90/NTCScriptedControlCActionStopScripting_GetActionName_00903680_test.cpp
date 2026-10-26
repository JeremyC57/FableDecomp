#include <cstdio>
#include <cstring>
class CCharString { public: const char* p; CCharString(const char* s, long n) : p(s) { } CCharString(const CCharString& o) : p(o.p) { } };
struct CActionStopScripting { CCharString GetActionName(); };
CCharString CActionStopScripting::GetActionName() { return CCharString("CActionStopScripting", -1); }
int main() { CActionStopScripting o; bool ok = std::strcmp(o.GetActionName().p, "CActionStopScripting") == 0; std::printf(ok ? "00903680_TEST PASS\n" : "00903680_TEST FAIL\n"); return ok ? 0 : 1; }
