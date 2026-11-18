// Behaviour test for CTCTavernGameCardBlackjack::GetBet (0x007DA640): the def's bet text followed by the bet on the current hand
// when there are bets and 0 <= CurrentPlayerCardsIndex < PlayersCards.size(), else the game's base bet; the def is released.
#include <cstdio>
#include <string>
#include <vector>

struct Def { long refs; int deleted; unsigned long Bet; };
struct DefPtr { Def* p; DefPtr() : p(0) {} ~DefPtr() { if (p) { if (--p->refs == 0) p->deleted = 1; p = 0; } } };
static std::string GFIntToWideString(long value) { char b[32]; std::sprintf(b, "%ld", value); return b; }

struct Blackjack {
    Def* def;
    long BaseBet;
    std::vector<std::vector<int> > PlayersCards;
    std::vector<long> Bets;
    long CurrentPlayerCardsIndex;
    void GetDef(DefPtr* out) { out->p = def; ++def->refs; }
    std::string GetText(unsigned long id) { char b[32]; std::sprintf(b, "B%lu:", id); return b; }
    std::string GetBet()
    {
        DefPtr pdef;
        GetDef(&pdef);
        std::string ret = GetText(pdef.p->Bet);
        if (Bets.size() != 0 && CurrentPlayerCardsIndex >= 0 && (unsigned long)CurrentPlayerCardsIndex < PlayersCards.size())
            ret += GFIntToWideString(Bets[CurrentPlayerCardsIndex]);
        else
            ret += GFIntToWideString(BaseBet);
        return ret;
    }
};

static int fail(int n) { std::printf("007da640_TEST FAIL %d\n", n); return 1; }

int main()
{
    Def d = { 1, 0, 3 };
    Blackjack b; b.def = &d; b.BaseBet = 25; b.CurrentPlayerCardsIndex = 0;
    if (b.GetBet() != "B3:25" || d.refs != 1) return fail(1);           // no bets
    b.Bets.push_back(40); b.Bets.push_back(80);
    if (b.GetBet() != "B3:25") return fail(2);                          // no hands yet: index not below size
    b.PlayersCards.resize(2);
    if (b.GetBet() != "B3:40") return fail(3);
    b.CurrentPlayerCardsIndex = 1;
    if (b.GetBet() != "B3:80") return fail(4);
    b.CurrentPlayerCardsIndex = 2;
    if (b.GetBet() != "B3:25") return fail(5);                          // past the hands
    b.CurrentPlayerCardsIndex = -1;
    if (b.GetBet() != "B3:25") return fail(6);                          // negative index
    Blackjack c; c.def = &d; c.BaseBet = 25; c.CurrentPlayerCardsIndex = 0; c.PlayersCards.resize(2);
    if (c.GetBet() != "B3:25") return fail(8);                          // hands but no bets: base bet (coordinator, audit e01)
    d.refs = 0; b.GetBet();
    if (!d.deleted) return fail(7);
    std::printf("007da640_TEST PASS\n");
    return 0;
}