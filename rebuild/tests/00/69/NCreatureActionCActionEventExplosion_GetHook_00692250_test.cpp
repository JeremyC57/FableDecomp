#include <cstdio>
#include <cstring>
class CCharString { public: const char* p; CCharString(const char* s, long n) : p(s) { } CCharString(const CCharString& o) : p(o.p) { } };
struct CActionEventExplosion { CCharString GetHook(); };
CCharString CActionEventExplosion::GetHook() { return CCharString("EXPLOSION", -1); }
int main() { CActionEventExplosion o; bool ok = std::strcmp(o.GetHook().p, "EXPLOSION") == 0; std::printf(ok ? "00692250_TEST PASS\n" : "00692250_TEST FAIL\n"); return ok ? 0 : 1; }
