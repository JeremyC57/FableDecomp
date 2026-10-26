#include <cstdio>
#include <cstring>
class CCharString { public: const char* p; CCharString(const char* s, long n) : p(s) { } CCharString(const CCharString& o) : p(o.p) { } };
struct CActionMoveTo { CCharString GetActionName(); };
CCharString CActionMoveTo::GetActionName() { return CCharString("CActionMoveTo", -1); }
int main() { CActionMoveTo o; bool ok = std::strcmp(o.GetActionName().p, "CActionMoveTo") == 0; std::printf(ok ? "007e7280_TEST PASS\n" : "007e7280_TEST FAIL\n"); return ok ? 0 : 1; }
