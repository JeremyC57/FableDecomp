// Behaviour test for 0x0064E0A1 NPlayerGui::CDrawStatUpdates ctor: no change seen, not initialised, six zeroed previous stats (standalone copy of the logic)
#include <cstdio>
#include <vector>
struct StatUpdates { long LastRenownChange, LastMoralityChange; bool Initialised; std::vector<long> PreviousStats;
    StatUpdates() : LastRenownChange(0), LastMoralityChange(0), Initialised(false) { PreviousStats.resize(6); } };

int main()
{
    StatUpdates s; if (s.LastRenownChange || s.LastMoralityChange || s.Initialised || s.PreviousStats.size() != 6) { std::printf("0064e0a1_TEST FAIL\n"); return 1; }
    for (unsigned i = 0; i < 6; ++i) if (s.PreviousStats[i]) { std::printf("0064e0a1_TEST FAIL\n"); return 1; }
    std::printf("0064e0a1_TEST PASS\n");
    return 0;
}