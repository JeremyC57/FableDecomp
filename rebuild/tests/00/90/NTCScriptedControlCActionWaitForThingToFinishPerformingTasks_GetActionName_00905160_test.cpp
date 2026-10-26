#include <cstdio>
#include <cstring>
class CCharString { public: const char* p; CCharString(const char* s, long n) : p(s) { } CCharString(const CCharString& o) : p(o.p) { } };
struct CActionWaitForThingToFinishPerformingTasks { CCharString GetActionName(); };
CCharString CActionWaitForThingToFinishPerformingTasks::GetActionName() { return CCharString("CActionWaitForThingToFinishPerformingTasks", -1); }
int main() { CActionWaitForThingToFinishPerformingTasks o; bool ok = std::strcmp(o.GetActionName().p, "CActionWaitForThingToFinishPerformingTasks") == 0; std::printf(ok ? "00905160_TEST PASS\n" : "00905160_TEST FAIL\n"); return ok ? 0 : 1; }
