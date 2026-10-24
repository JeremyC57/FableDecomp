#include <cstdio>
#include <cstring>
struct CCharString { const char* p; CCharString(const char* s, long n) : p(s) { } CCharString(const CCharString& o) : p(o.p) { } };
struct CActionTalkToThing { CCharString GetActionName(); };
CCharString CActionTalkToThing::GetActionName() { return CCharString("CActionTalkToThing", -1); }
int main() { CActionTalkToThing o; bool ok = std::strcmp(o.GetActionName().p, "CActionTalkToThing") == 0; std::printf(ok ? "0071a4e0_TEST PASS\n" : "0071a4e0_TEST FAIL\n"); return ok ? 0 : 1; }
