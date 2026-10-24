#include <cstdio>
#include <cstring>
struct CCharString { const char* p; CCharString(const char* s, long n) : p(s) { } CCharString(const CCharString& o) : p(o.p) { } };
struct CActionEventSound { CCharString GetHook(); };
CCharString CActionEventSound::GetHook() { return CCharString("SOUND", -1); }
int main() { CActionEventSound o; bool ok = std::strcmp(o.GetHook().p, "SOUND") == 0; std::printf(ok ? "006921d0_TEST PASS\n" : "006921d0_TEST FAIL\n"); return ok ? 0 : 1; }
