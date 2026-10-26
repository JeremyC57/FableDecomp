#include <cstdio>
class CGameScriptInterface { public: long time; virtual long GetTimeOfDay() const { return time; } virtual ~CGameScriptInterface() { } };
struct CTH_Trigger_Bowerstone {
    CGameScriptInterface* PQuest; bool aborted; int abortChecks;
    bool IsAborted() { ++abortChecks; return aborted; }
    bool IsNightTime()
    {
        if (PQuest->GetTimeOfDay() > 1800 || PQuest->GetTimeOfDay() < 700) {
            if (!IsAborted()) return true;
        } else {
            IsAborted();
        }
        return false;
    }
};
int main()
{
    CGameScriptInterface q; CTH_Trigger_Bowerstone t; t.PQuest = &q; t.aborted = false; t.abortChecks = 0;
    q.time = 1200; if (t.IsNightTime() || t.abortChecks != 1) { std::printf("00ee13b0_TEST FAIL\n"); return 1; }
    q.time = 1801; if (!t.IsNightTime()) { std::printf("00ee13b0_TEST FAIL\n"); return 1; }
    q.time = 650; if (!t.IsNightTime()) { std::printf("00ee13b0_TEST FAIL\n"); return 1; }
    q.time = 700; if (t.IsNightTime()) { std::printf("00ee13b0_TEST FAIL\n"); return 1; }
    q.time = 1800; if (t.IsNightTime()) { std::printf("00ee13b0_TEST FAIL\n"); return 1; }
    t.aborted = true; q.time = 2300; if (t.IsNightTime() || t.abortChecks != 6) { std::printf("00ee13b0_TEST FAIL\n"); return 1; }
    std::printf("00ee13b0_TEST PASS\n");
    return 0;
}