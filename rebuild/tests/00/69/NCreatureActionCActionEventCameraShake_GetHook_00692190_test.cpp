#include <cstdio>
#include <cstring>
struct CCharString { const char* p; CCharString(const char* s, long n) : p(s) { } CCharString(const CCharString& o) : p(o.p) { } };
struct CActionEventCameraShake { CCharString GetHook(); };
CCharString CActionEventCameraShake::GetHook() { return CCharString("RUMBLE", -1); }
int main() { CActionEventCameraShake o; bool ok = std::strcmp(o.GetHook().p, "RUMBLE") == 0; std::printf(ok ? "00692190_TEST PASS\n" : "00692190_TEST FAIL\n"); return ok ? 0 : 1; }
