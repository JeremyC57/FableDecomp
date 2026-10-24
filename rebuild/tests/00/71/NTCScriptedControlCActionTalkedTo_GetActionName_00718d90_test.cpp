#include <cstdio>
#include <cstring>
struct CCharString { const char* p; CCharString(const char* s, long n) : p(s) { } CCharString(const CCharString& o) : p(o.p) { } };
struct CActionTalkedTo { CCharString GetActionName(); };
CCharString CActionTalkedTo::GetActionName() { return CCharString("CActionTalkedTo", -1); }
int main() { CActionTalkedTo o; bool ok = std::strcmp(o.GetActionName().p, "CActionTalkedTo") == 0; std::printf(ok ? "00718d90_TEST PASS\n" : "00718d90_TEST FAIL\n"); return ok ? 0 : 1; }
