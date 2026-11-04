// Behaviour test for CTCTavernGameCardBlackjack::GetBet (0x007DA640), g++ model of the parked source.
#include <cstdio>
#include <string>
#include <vector>
struct Game { std::wstring betText; std::vector<long> bets; std::vector<int> hands; long index; long baseBet; };
static std::wstring IntToWide(long v) { wchar_t b[32]; std::swprintf(b, 32, L"%ld", v); return b; }
static std::wstring GetBet(const Game& g)
{
    std::wstring text = g.betText;
    if (g.bets.size() != 0 && g.index >= 0 && (unsigned long)g.index < g.hands.size())
        text += IntToWide(g.bets[g.index]);
    else
        text += IntToWide(g.baseBet);
    return text;
}
int main()
{
    Game g; g.betText = L"Bet: "; g.baseBet = 10; g.index = 1;
    int ok = GetBet(g) == L"Bet: 10";
    g.bets.push_back(25); g.bets.push_back(40); g.hands.push_back(0); g.hands.push_back(0);
    ok &= GetBet(g) == L"Bet: 40";
    g.index = -1; ok &= GetBet(g) == L"Bet: 10";
    g.index = 2; ok &= GetBet(g) == L"Bet: 10";
    g.index = 0; ok &= GetBet(g) == L"Bet: 25";
    std::printf(ok ? "007da640_TEST PASS\n" : "007da640_TEST FAIL\n");
    return ok ? 0 : 1;
}
