#include <cstdio>
// Behaviour of CTCHeroStats::HasCurrentMarriage (0x0057AC4B): 1 when a marriage is in any state 1..4 (tested in order,
// stopping at the first hit), else 0.
static bool g_states[8]; static int g_queries = 0;
struct CTCHeroStats_Retail {
    bool HasMarriageInState(long state) const { ++g_queries; return g_states[state]; }
    long HasCurrentMarriage() const
    {
        return HasMarriageInState(1) || HasMarriageInState(2) || HasMarriageInState(3) || HasMarriageInState(4);
    }
};
int main()
{
    CTCHeroStats_Retail s;
    for (int i = 0; i < 8; ++i) g_states[i] = false;
    g_states[0] = true; g_states[5] = true;   // states outside 1..4 do not count
    if (s.HasCurrentMarriage() != 0 || g_queries != 4) { std::printf("0057ac4b_TEST FAIL\n"); return 1; }
    g_states[2] = true; g_queries = 0;
    if (s.HasCurrentMarriage() != 1 || g_queries != 2) { std::printf("0057ac4b_TEST FAIL\n"); return 1; }
    g_states[2] = false; g_states[4] = true; g_queries = 0;
    if (s.HasCurrentMarriage() != 1 || g_queries != 4) { std::printf("0057ac4b_TEST FAIL\n"); return 1; }
    std::printf("0057ac4b_TEST PASS\n");
    return 0;
}