// Test: CAIStateGroup_MinionReactToCrime::GetSomeoneElseGettingHelpMessage -- the newest type-0xA message, originator alive, NOT originated by this creature, carrying the given extra data
#include <cstdio>
class CMessageEvent { public: long Type; int Originator; bool OriginatorAlive; long ExtraData; };
static const CMessageEvent* Find(const CMessageEvent* ev, int count, int creature, long extraData)
{
    for (int i = count - 1; i >= 0; --i) {
        const CMessageEvent& e = ev[i];
        if (e.Type == 0xA && e.OriginatorAlive && !(e.Originator == creature) && e.ExtraData == extraData) return &e;
    }
    return 0;
}
int main()
{
    CMessageEvent ev[5] = { { 0xA, 2, true, 5 }, { 0xA, 1, true, 5 }, { 0xA, 3, false, 5 }, { 0xA, 4, true, 6 }, { 0xB, 2, true, 5 } };
    if (Find(ev, 5, 1, 5) != &ev[0] || Find(ev, 5, 1, 6) != &ev[3] || Find(ev, 5, 4, 6) != 0) { std::printf("00958770_TEST FAIL\n"); return 1; }
    std::printf("00958770_TEST PASS\n");
    return 0;
}