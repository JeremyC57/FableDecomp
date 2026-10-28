// Test: CV_AssassinAttacksScript ctor -- level-script base (Game, MasterScriptData), then the empty most-recent-kill name, five region records with empty names, and an empty kill-count vector.
#include <cstdio>
#include <cstring>
struct CStr { const char* s; CStr() : s("") { } };
struct CVec { int* first; int* last; int* end; CVec() : first(0), last(0), end(0) { } };
struct CLevel { void* Game; void* MasterScriptData; CLevel(void* data, void* game) : Game(game), MasterScriptData(data) { } };
struct CScript : CLevel { struct RegionData { CStr name; int count; int max; bool active; };
    int RegionIndex; CStr RegionOfMostRecentKill; RegionData RegionInfo[5]; CVec RegionKillCounts;
    CScript(void* data, void* game) : CLevel(data, game) { } };
int main()
{
    int data, game; CScript s(&data, &game);
    bool ok = s.Game == &game && s.MasterScriptData == &data && !std::strcmp(s.RegionOfMostRecentKill.s, "") && s.RegionKillCounts.first == 0 &&
              s.RegionKillCounts.last == 0 && s.RegionKillCounts.end == 0;
    for (int i = 0; i < 5; ++i) ok = ok && !std::strcmp(s.RegionInfo[i].name.s, "");
    std::printf(ok ? "00e376d0_TEST PASS\n" : "00e376d0_TEST FAIL\n");
    return ok ? 0 : 1;
}