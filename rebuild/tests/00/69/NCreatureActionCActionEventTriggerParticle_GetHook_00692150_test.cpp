#include <cstdio>
#include <cstring>
struct CCharString { const char* p; CCharString(const char* s, long n) : p(s) { } CCharString(const CCharString& o) : p(o.p) { } };
struct CActionEventTriggerParticle { CCharString GetHook(); };
CCharString CActionEventTriggerParticle::GetHook() { return CCharString("PARTICLE", -1); }
int main() { CActionEventTriggerParticle o; bool ok = std::strcmp(o.GetHook().p, "PARTICLE") == 0; std::printf(ok ? "00692150_TEST PASS\n" : "00692150_TEST FAIL\n"); return ok ? 0 : 1; }
