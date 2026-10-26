// behaviour test for 0x0057CB2A CTCHeroStats::GetStatsStatus: the renown level, then the points inside the level, are appended
// after whatever the vector already holds; the stats are left untouched
#include <cstdio>
#include <vector>
struct Stats {
    int RenownLevel; int RenownMaxLevel; int RenownPointsInLevel; int RenownTotal;
    void GetStatsStatus(std::vector<int>& status) const { status.push_back(RenownLevel); status.push_back(RenownPointsInLevel); }
};
int main()
{
    int fail = 0;
    Stats s; s.RenownLevel = 4; s.RenownMaxLevel = 9; s.RenownPointsInLevel = 123; s.RenownTotal = 777;
    std::vector<int> v; v.push_back(-1);
    s.GetStatsStatus(v);
    if (v.size() != 3u || v[0] != -1 || v[1] != 4 || v[2] != 123) fail |= 1;
    s.GetStatsStatus(v);
    if (v.size() != 5u || v[3] != 4 || v[4] != 123) fail |= 2;
    if (s.RenownMaxLevel != 9 || s.RenownTotal != 777) fail |= 4;
    if (fail) { std::printf("0057cb2a_TEST FAIL %d\n", fail); return 1; }
    std::printf("0057cb2a_TEST PASS\n");
    return 0;
}