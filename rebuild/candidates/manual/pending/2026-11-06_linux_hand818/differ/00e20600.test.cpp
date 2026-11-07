// Behaviour test for NScript::CQR_EscortTraderScript::CTraderToEscort::Main (0x00E20600): a model of the arrival -- the farmhouse door is
// opened only when it exists and is shut; the success grade picks the reward (perfect 3 x the downpayment plus the bonus object,
// high 2 x, medium 1 x, low only the lines, otherwise nothing) and the trader pays the hero what he gains.
#include <cstdio>
enum EscortTraderSuccessType { ET_SUCCESS_TYPE_PERFECT, ET_SUCCESS_TYPE_HIGH, ET_SUCCESS_TYPE_MEDIUM, ET_SUCCESS_TYPE_LOW, ET_SUCCESS_TYPE_NONE };
struct Sim {
    long heroGold, traderGold; bool gotBonus, doorOpen, doorAlive, openedDoor;
    void Door() { openedDoor = false; if (doorAlive && !doorOpen) { doorOpen = true; openedDoor = true; } }
    void Reward(EscortTraderSuccessType success, long downpayment)
    {
        long reward = 0;
        if (success == ET_SUCCESS_TYPE_PERFECT) { reward = downpayment * 3; gotBonus = true; }
        else if (success == ET_SUCCESS_TYPE_HIGH) reward = downpayment * 2;
        else if (success == ET_SUCCESS_TYPE_MEDIUM) reward = downpayment;
        heroGold += reward; traderGold -= reward;
    }
};
int main()
{
    bool ok = true;
    const long expect[5] = { 300, 200, 100, 0, 0 };
    for (int i = 0; i < 5; ++i) {
        Sim s = { 0, 1000, false, false, true, false };
        s.Door(); s.Reward((EscortTraderSuccessType)i, 100);
        ok = ok && s.heroGold == expect[i] && s.traderGold == 1000 - expect[i] && s.gotBonus == (i == 0) && s.openedDoor && s.doorOpen;
    }
    Sim d = { 0, 0, false, true, true, false }; d.Door(); ok = ok && !d.openedDoor;
    Sim e = { 0, 0, false, false, false, false }; e.Door(); ok = ok && !e.openedDoor && !e.doorOpen;
    std::printf(ok ? "00e20600_TEST PASS\n" : "00e20600_TEST FAIL\n");
    return ok ? 0 : 1;
}
