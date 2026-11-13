// Behaviour test for CTCTavernGameCardPairs::ShuffleCards 0x007D71E0: the pairing passes over a pack model (std::vector<Card*>),
// with the same erase / push_back / insert moves; checks that the first half ends with distinct values and the second half pairs them.
#include <cstdio>
#include <cstdlib>
#include <vector>
struct Card { long Value; };
static void Pair(std::vector<Card*>& Pack, long NumCards)
{
    for (long i = 0; i < NumCards / 2; ) {
        std::vector<Card*>::iterator it = Pack.begin();
        for (long n = 0; n < i; ++n) ++it;
        for (std::vector<Card*>::iterator p = Pack.begin(); p != it; ++p) {
            if ((*it)->Value == (*p)->Value) {
                Card* card = *it;
                Pack.erase(it);
                Pack.push_back(card);
                goto next;
            }
        }
        ++i;
    next:;
    }
    for (long j = NumCards / 2; j < NumCards; ++j) {
        std::vector<Card*>::iterator it = Pack.begin();
        for (long n = 0; n < j; ++n) ++it;
        Card* other = Pack[j - NumCards / 2];
        std::vector<Card*>::iterator p = it;
        while ((*p)->Value != other->Value)
            ++p;
        if (p != it) {
            Card* card = *p;
            Pack.erase(p);
            Pack.insert(it, card);
        }
    }
}
int main()
{
    Card cards[12];
    for (int seed = 1; seed < 40; ++seed) {
        std::srand(seed);
        std::vector<Card*> pack;
        for (int k = 0; k < 12; ++k) { cards[k].Value = k / 2; pack.push_back(&cards[k]); }
        for (int k = 11; k > 0; --k) { int r = std::rand() % (k + 1); Card* t = pack[k]; pack[k] = pack[r]; pack[r] = t; }
        Pair(pack, 12);
        if (pack.size() != 12) { std::printf("007d71e0_TEST FAIL\n"); return 1; }
        for (int a = 0; a < 6; ++a) {
            for (int b = 0; b < a; ++b)
                if (pack[a]->Value == pack[b]->Value) { std::printf("007d71e0_TEST FAIL\n"); return 1; }
            if (pack[a + 6]->Value != pack[a]->Value) { std::printf("007d71e0_TEST FAIL\n"); return 1; }
        }
    }
    std::printf("007d71e0_TEST PASS\n");
    return 0;
}