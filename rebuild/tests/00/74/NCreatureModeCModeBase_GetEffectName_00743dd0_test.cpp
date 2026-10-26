#include <cstdio>
#include <cstring>
class CCharString { public: const char* p; CCharString(const char* s, long n) : p(s) { } CCharString(const CCharString& o) : p(o.p) { } };
struct CModeBase { CCharString GetEffectName(); };
CCharString CModeBase::GetEffectName() { return CCharString("", -1); }
int main() { CModeBase o; bool ok = std::strcmp(o.GetEffectName().p, "") == 0; std::printf(ok ? "00743dd0_TEST PASS\n" : "00743dd0_TEST FAIL\n"); return ok ? 0 : 1; }
