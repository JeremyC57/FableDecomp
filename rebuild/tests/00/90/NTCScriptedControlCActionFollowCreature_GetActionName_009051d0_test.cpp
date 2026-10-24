#include <cstdio>
#include <cstring>
struct CCharString { const char* p; CCharString(const char* s, long n) : p(s) { } CCharString(const CCharString& o) : p(o.p) { } };
struct CActionFollowCreature { CCharString GetActionName(); };
CCharString CActionFollowCreature::GetActionName() { return CCharString("CActionFollowCreature", -1); }
int main() { CActionFollowCreature o; bool ok = std::strcmp(o.GetActionName().p, "CActionFollowCreature") == 0; std::printf(ok ? "009051d0_TEST PASS\n" : "009051d0_TEST FAIL\n"); return ok ? 0 : 1; }
