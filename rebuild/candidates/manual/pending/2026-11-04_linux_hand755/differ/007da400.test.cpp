// Behaviour test for CTCTavernGameCardBlackjack::GetCurrentScore (0x007DA400), g++ model.
#include <cstdio>
#include <string>
#include <vector>
static std::string GetText(const char* id) { return std::string(id) + ": "; }
static std::string GFFloatToCharString(double value, long decimals) { char b[32]; std::snprintf(b, sizeof b, "%.*f", (int)decimals, value); return b; }
static long GetHandValue(const std::vector<int>& cards) { long v = 0; for (size_t i = 0; i < cards.size(); ++i) v += cards[i]; return v; }
static std::string GetCurrentScore(const std::vector<int>* currentPlayerCards, const std::vector<int>& handCards)
{
    if (currentPlayerCards)
        return GetText("CurrentScore") + GFFloatToCharString(GetHandValue(handCards), 0);
    return GetText("CurrentScore") + "0";
}
int main()
{
    std::vector<int> player, dealer; player.push_back(10); player.push_back(7); dealer.push_back(9); dealer.push_back(11);
    const std::vector<int>& hand = std::string("player") == "player" ? player : dealer;
    int ok = GetCurrentScore(&player, hand) == std::string("CurrentScore: ") + (hand == player ? "17" : "20");
    ok &= GetCurrentScore(0, hand) == "CurrentScore: 0";
    std::printf(ok ? "007da400_TEST PASS\n" : "007da400_TEST FAIL\n");
    return ok ? 0 : 1;
}
