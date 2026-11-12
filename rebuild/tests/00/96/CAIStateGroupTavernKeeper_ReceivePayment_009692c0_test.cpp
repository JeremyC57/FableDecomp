// Behaviour test for CAIStateGroup_TavernKeeper::ReceivePayment 0x009692C0: the same control flow over a scripted model.
#include <cstdio>
#include <cstring>
struct Model {
    // script
    bool walkOk; bool cancelled; int finishedAfter; int stopAfter; int deadAfter; long fps; long frameStep;
    // state
    long frame; int polls; int nothings; bool pushable; bool barFree; int removedOld; int removedNew; int added; int played;
    char log[64];
    void SetPushableByHero(bool b) { pushable = b; }
    bool WalkToPoint() { return walkOk; }
    bool GetPaymentCancelledMessage() { return cancelled; }
    bool GetPaymentFinishedMessage() { return finishedAfter >= 0 && polls++ >= finishedAfter; }
    void FreeBarPoint() { barFree = true; }
    void RemoveMessage(int which) { if (which == 0) ++removedOld; else ++removedNew; }
    int AddMessage(long type, unsigned long life) { if (type == 0x30 && life == 1000) ++added; return 1; }
    long GetFrame() { return frame; }
    void DoNothing() { ++nothings; frame += frameStep; }
    bool IsStopping() { return stopAfter >= 0 && nothings > stopAfter; }
    bool PayeeAlive() { return !(deadAfter >= 0 && nothings > deadAfter); }
    void Play(const char* name) { if (std::strcmp(name, "RECEIVE_MONEY_AT_BAR") == 0) ++played; }
    bool ReceivePayment()
    {
        SetPushableByHero(false);
        if (!WalkToPoint()) { FreeBarPoint(); SetPushableByHero(true); return false; }
        if (GetPaymentCancelledMessage()) { FreeBarPoint(); SetPushableByHero(true); return false; }
        RemoveMessage(0);
        int message = AddMessage(0x30, 1000);
        long endFrame = GetFrame() + fps * 20;
        while (!GetPaymentFinishedMessage()) {
            DoNothing();
            if (GetFrame() > endFrame || IsStopping() || !PayeeAlive()) {
                RemoveMessage(message);
                FreeBarPoint();
                SetPushableByHero(true);
                return false;
            }
        }
        Play("RECEIVE_MONEY_AT_BAR");
        FreeBarPoint();
        return true;
    }
};
static Model Make(bool walkOk, bool cancelled, int finishedAfter, int stopAfter, int deadAfter)
{
    Model m; std::memset(&m, 0, sizeof(m));
    m.walkOk = walkOk; m.cancelled = cancelled; m.finishedAfter = finishedAfter; m.stopAfter = stopAfter; m.deadAfter = deadAfter;
    m.fps = 30; m.frameStep = 1; m.frame = 100; m.pushable = true;
    return m;
}
int main()
{
    bool ok = true;
    { Model m = Make(false, false, 0, -1, -1);          // walk fails: nothing posted, pushable again
      ok = ok && !m.ReceivePayment() && m.barFree && m.pushable && m.removedOld == 0 && m.added == 0 && m.played == 0; }
    { Model m = Make(true, true, 0, -1, -1);            // payment cancelled before the request
      ok = ok && !m.ReceivePayment() && m.barFree && m.pushable && m.removedOld == 0 && m.added == 0; }
    { Model m = Make(true, false, 0, -1, -1);           // paid at once: no idle frame, animation, NOT pushable again
      ok = ok && m.ReceivePayment() && m.barFree && !m.pushable && m.removedOld == 1 && m.added == 1 && m.removedNew == 0
              && m.played == 1 && m.nothings == 0; }
    { Model m = Make(true, false, 3, -1, -1);           // paid after three idle frames
      ok = ok && m.ReceivePayment() && m.nothings == 3 && m.played == 1 && m.removedNew == 0; }
    { Model m = Make(true, false, -1, -1, -1);          // never paid: times out after fps*20 frames (the 601st is past the end)
      ok = ok && !m.ReceivePayment() && m.nothings == 601 && m.removedNew == 1 && m.barFree && m.pushable && m.played == 0; }
    { Model m = Make(true, false, -1, 1, -1);           // group stopping
      ok = ok && !m.ReceivePayment() && m.nothings == 2 && m.removedNew == 1 && m.pushable; }
    { Model m = Make(true, false, -1, -1, 0);           // payee dies
      ok = ok && !m.ReceivePayment() && m.nothings == 1 && m.removedNew == 1 && m.pushable && m.played == 0; }
    { Model m = Make(true, false, 600, -1, -1);         // paid on the last allowed frame (frame == end is not a timeout)
      ok = ok && m.ReceivePayment() && m.nothings == 600 && m.played == 1; }
    if (!ok) { std::printf("009692c0_TEST FAIL\n"); return 1; }
    std::printf("009692c0_TEST PASS\n");
    return 0;
}