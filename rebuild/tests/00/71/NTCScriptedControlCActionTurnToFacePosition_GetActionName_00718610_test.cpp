#include <cstdio>
#include <cstring>
class CCharString { public: const char* p; CCharString(const char* s, long n) : p(s) { } CCharString(const CCharString& o) : p(o.p) { } };
struct CActionTurnToFacePosition { CCharString GetActionName(); };
CCharString CActionTurnToFacePosition::GetActionName() { return CCharString("CActionTurnToFacePosition", -1); }
int main() { CActionTurnToFacePosition o; bool ok = std::strcmp(o.GetActionName().p, "CActionTurnToFacePosition") == 0; std::printf(ok ? "00718610_TEST PASS\n" : "00718610_TEST FAIL\n"); return ok ? 0 : 1; }
