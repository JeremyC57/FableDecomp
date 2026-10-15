// Behaviour test for CTCMoneyReceiver::CTCMoneyReceiver @ 0x004D49DD: base gets the owner, the amount defaults to 50.
#include <cstdio>
struct CThing { long id; };
struct CTCBase {
    CThing* ParentThing;
    CTCBase(CThing* owner) : ParentThing(owner) {}
    virtual ~CTCBase() {}
};
struct CTCMoneyReceiver : CTCBase {
    long AmountOfMoneyWanted;
    static long GetDefaultAmountOfMoneyWanted() { return 50; }
    CTCMoneyReceiver(CThing* owner) : CTCBase(owner) { AmountOfMoneyWanted = GetDefaultAmountOfMoneyWanted(); }
};
int main()
{
    CThing t; t.id = 3;
    CTCMoneyReceiver r(&t);
    if (r.ParentThing != &t || r.AmountOfMoneyWanted != 50) { std::printf("004d49dd_TEST FAIL\n"); return 1; }
    std::printf("004d49dd_TEST PASS\n");
    return 0;
}
