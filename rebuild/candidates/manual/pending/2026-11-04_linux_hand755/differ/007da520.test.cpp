// Behaviour test for CTCTavernGameCardBlackjack::GetRequiredScore (0x007DA520), g++ model.
#include <cstdio>
#include <string>
#include <vector>
static std::string GetText(const char* id) { return std::string(id) + ": "; }
static std::string GFFloatToCharString(double value, long decimals) { char b[32]; std::snprintf(b, sizeof b, "%.*f", (int)decimals, value); return b; }
static long GetHandValue(const std::vector<int>& cards) { long v = 0; for (size_t i = 0; i < cards.size(); ++i) v += cards[i]; return v; }
static std::string GetRequiredScore(const std::vector<int>* currentPlayerCards, const std::vector<int>& handCards)
{
    if (currentPlayerCards)
        return GetText("RequiredScore") + GFFloatToCharString(GetHandValue(handCards), 0);
    return GetText("RequiredScore") + "0";
}
int main()
{
    std::vector<int> player, dealer; player.push_back(10); player.push_back(7); dealer.push_back(9); dealer.push_back(11);
    const std::vector<int>& hand = std::string("dealer") == "player" ? player : dealer;
    int ok = GetRequiredScore(&player, hand) == std::string("RequiredScore: ") + (hand == player ? "17" : "20");
    ok &= GetRequiredScore(0, hand) == "RequiredScore: 0";
    std::printf(ok ? "007da520_TEST PASS\n" : "007da520_TEST FAIL\n");
    return ok ? 0 : 1;
}
