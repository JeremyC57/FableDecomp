// Behaviour test for CTCTavernGameCardBlackjack::GetCurrentScore (0x007DA400): the def text + the player's current hand value
// (aces counted 11 then dropped to 1 while bust, face-down cards skipped unless turning), "0" without a current hand, and the def
// released (count dropped, deleted at zero) before return.
#include <cstdio>
#include <string>
#include <vector>

struct Card { long Value; bool Turning; bool FaceDown; };
struct Def { long refs; int deleted; unsigned long CurrentScore; };
struct DefPtr { Def* p; DefPtr() : p(0) {} ~DefPtr() { if (p) { if (--p->refs == 0) p->deleted = 1; p = 0; } } };
static std::string GFFloatToCharString(double n, long precision) { char b[64]; std::sprintf(b, "%.*f", (int)precision, n); return b; }

struct Blackjack {
    Def* def;
    std::vector<Card*> DealersCards;
    std::vector<Card*>* CurrentPlayerCards;
    void GetDef(DefPtr* out) { out->p = def; ++def->refs; }
    std::string GetText(unsigned long id) { char b[32]; std::sprintf(b, "T%lu:", id); return b; }
    long GetHandValue(std::vector<Card*>* hand, bool include_face_down_cards)
    {
        long total = 0, num_aces = 0;
        for (std::vector<Card*>::iterator card = hand->begin(); card != hand->end(); ++card) {
            if (include_face_down_cards || (*card)->Turning || !(*card)->FaceDown) {
                if ((*card)->Value == 11) ++num_aces;
                total += (*card)->Value;
            }
        }
        while (total > 21 && num_aces > 0) { total -= 10; --num_aces; }
        return total;
    }
    std::string GetCurrentScore()
    {
        DefPtr pdef;
        GetDef(&pdef);
        if (CurrentPlayerCards)
            return GetText(pdef.p->CurrentScore) + GFFloatToCharString(GetHandValue(CurrentPlayerCards, false), 0);
        return GetText(pdef.p->CurrentScore) + "0";
    }
};

static int fail(int n) { std::printf("007da400_TEST FAIL %d\n", n); return 1; }

int main()
{
    Def d = { 1, 0, 7 };
    Card ace = { 11, false, false }, ace2 = { 11, false, false }, king = { 10, false, false }, hidden = { 9, false, true },
         turning = { 5, true, true };
    std::vector<Card*> hand; hand.push_back(&ace); hand.push_back(&king);
    Blackjack b; b.def = &d; b.CurrentPlayerCards = 0;
    if (b.GetCurrentScore() != "T7:0" || d.refs != 1 || d.deleted) return fail(1);
    b.CurrentPlayerCards = &hand;
    if (b.GetCurrentScore() != "T7:21") return fail(2);
    hand.push_back(&ace2);                      // 11+10+11 = 32 -> 22 -> 12
    if (b.GetCurrentScore() != "T7:12") return fail(3);
    hand.push_back(&hidden);                    // face down, not turning: skipped
    if (b.GetCurrentScore() != "T7:12") return fail(4);
    hand.push_back(&turning);                   // turning: counted
    if (b.GetCurrentScore() != "T7:17") return fail(5);
    d.refs = 0;                                 // the getter's reference is the last one: deleted on release
    b.GetCurrentScore();
    if (!d.deleted || d.refs != 0) return fail(6);
    std::printf("007da400_TEST PASS\n");
    return 0;
}
