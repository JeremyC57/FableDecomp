#include <cstdio>
#include <cstring>
class CCharString { public: const char* p; CCharString(const char* s, long n) : p(s) { } CCharString(const CCharString& o) : p(o.p) { } };
struct CActionSummonerLightningAttackTarget { CCharString GetActionName(); };
CCharString CActionSummonerLightningAttackTarget::GetActionName() { return CCharString("CActionSummonerLightningAttackTarget", -1); }
int main() { CActionSummonerLightningAttackTarget o; bool ok = std::strcmp(o.GetActionName().p, "CActionSummonerLightningAttackTarget") == 0; std::printf(ok ? "00718dd0_TEST PASS\n" : "00718dd0_TEST FAIL\n"); return ok ? 0 : 1; }
