#include <cstdio>
#include <cstring>
struct CCharString { const char* p; CCharString(const char* s, long n) : p(s) { } CCharString(const CCharString& o) : p(o.p) { } };
struct CActionPlayCombatAnimation { CCharString GetActionName(); };
CCharString CActionPlayCombatAnimation::GetActionName() { return CCharString("CActionPlayCombatAnimation", -1); }
int main() { CActionPlayCombatAnimation o; bool ok = std::strcmp(o.GetActionName().p, "CActionPlayCombatAnimation") == 0; std::printf(ok ? "009035f0_TEST PASS\n" : "009035f0_TEST FAIL\n"); return ok ? 0 : 1; }
