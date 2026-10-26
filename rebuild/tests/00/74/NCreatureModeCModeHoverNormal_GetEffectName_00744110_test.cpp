#include <cstdio>
#include <cstring>
class CCharString { public: const char* p; CCharString(const char* s, long n) : p(s) { } CCharString(const CCharString& o) : p(o.p) { } };
struct CModeHoverNormal { CCharString GetEffectName(); };
CCharString CModeHoverNormal::GetEffectName() { return CCharString("FLIGHT", -1); }
int main() { CModeHoverNormal o; bool ok = std::strcmp(o.GetEffectName().p, "FLIGHT") == 0; std::printf(ok ? "00744110_TEST PASS\n" : "00744110_TEST FAIL\n"); return ok ? 0 : 1; }
