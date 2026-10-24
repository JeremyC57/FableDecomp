#include <cstdio>
#include <cstring>
struct CCharString { const char* p; CCharString(const char* s, long n) : p(s) { } CCharString(const CCharString& o) : p(o.p) { } };
struct CActionEventShot { CCharString GetHook(); };
CCharString CActionEventShot::GetHook() { return CCharString("SHOT", -1); }
int main() { CActionEventShot o; bool ok = std::strcmp(o.GetHook().p, "SHOT") == 0; std::printf(ok ? "00692210_TEST PASS\n" : "00692210_TEST FAIL\n"); return ok ? 0 : 1; }
